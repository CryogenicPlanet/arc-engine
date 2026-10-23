# Arc ResourceRequest extra-origin provenance

Read-only audit, 2026-10-23. Image:
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`,
arm64 slice, unslid virtual addresses. No app launches, instrumentation, builds,
shared-source edits, or binary changes were used.

## Result and correction to the earlier audit

The native network `ResourceRequest` optional Origin at **+1840**, engaged byte
**+1920**, is populated from **Blink `ResourceRequestHead::TopFrameOrigin()`**.
It is a separately carried top-frame origin, not `isolated_world_origin`,
`SiteForCookies`, or `trusted_params.isolation_info.top_frame_origin`.

**The earlier label "native Mojo Read 0x1f84c50-0x1f84ca0" is incorrect.**
That range belongs to native **`blink::PopulateResourceRequest` at 0x1f83a94**:
an in-process conversion from Blink's request head/body to a network request.
Both the converter's complete field sequence and an independent getter/setter
and producer path establish the source field. This audit did not identify the
original final member's literal C++ spelling or a corresponding Mojo field name.
Calling our new field `top_frame_origin` would be our descriptive source name,
not a claim to have recovered stripped native identifiers.

## 1. Identify the conversion independently of the extra field

Native `0x1f83a94` has three arguments: x0 is the source Blink request head,
x1 its request body, and x2 the destination network request. The prologue retains
those as x25, x26, and x19 respectively (`0x1f83ab4-0x1f83abc`). Its conversions
match the current source function with the same signature at
`third_party/blink/renderer/platform/loader/fetch/url_loader/request_conversion.cc:260`:

| Native source head | Native destination network request | Evidence |
|---|---|---|
| HttpMethod at +248 | method at +0 | `0x1f83ac8-0x1f83ae4`, Blink string Latin1 conversion |
| Url at source base | url at +24 | `0x1f83aec-0x1f83afc`, KURL-to-GURL conversion |
| SiteForCookies at +120 | site_for_cookies at +144 | `0x1f83b1c-0x1f83b24` |
| RequestorOrigin pointer at +216 | request_initiator at +240, engaged +320 | `0x1f83c20-0x1f83d3c`, including the `ToString() == "null"` branch |
| **TopFrameOrigin pointer at +208** | **extra optional Origin at +1840, engaged +1920** | **`0x1f83d48-0x1f83dbc`, `0x1f84c50-0x1f84cc0`** |
| navigation_redirect_chain data +224, size +236 | redirect vector at +416 | `0x1f83dec-0x1f83eb0` |
| IsolatedWorldOrigin pointer at +240 | isolated_world_origin at +328, engaged +408 | `0x1f83eb4-0x1f83f38` |

The method/URL/cookies sequence and the distinctive requestor-origin null-string
handling exclude an interpretation as a Mojo reader. Current source has that
same sequence but lacks the added network top-frame-origin assignment.

The source field order is independently visible in current
`third_party/blink/renderer/platform/loader/fetch/resource_request.h:649-659`:
KURL, timeout, SiteForCookies, `top_frame_origin_`, `requestor_origin_`, redirect
vector, `isolated_world_origin_`, then method. The extra conversion reads the
intervening +208 pointer, not either known neighboring Origin pointer.

### Direct callers identify the allocation/conversion route

Native **0x17170c0** implements the shape of `CreateNetworkRequest` in current
`third_party/blink/renderer/platform/loader/fetch/resource_loader.cc:196-207`:

```asm
0x17170d8 mov x20, x1                  ; request body
0x17170dc mov x22, x0                  ; source request head
0x17170e0 mov w0, #1928                ; sizeof native network request
0x17170e4 bl 0x8790                    ; allocation
0x17170e8 mov x19, x0
0x17170ec bl 0x12da220                 ; known ResourceRequest constructor
...
0x171710c add x1, sp, #8               ; moved ResourceRequestBody
0x1717110 mov x0, x22                  ; Blink head
0x1717114 mov x2, x19                  ; allocated network request
0x1717118 bl 0x1f83a94                 ; PopulateResourceRequest
```

The same function preserves/restores the form-body reference as the source
helper does. Its second conversion branch calls `0x1f83a94` at `0x17171ac`.
Callers of this allocation helper are `0x1716cfc` and `0x340b6c0`; each passes
the Blink request head at its Resource+728 and request body at its Loader+72.

## 2. Independent TopFrameOrigin getter, setter, and producer

The native request-head accessors are adjacent:

```asm
0x419a378 add x0, x0, #120       ; SiteForCookies() -> const reference
0x419a37c ret
0x419a380 ldr x0, [x0, #208]     ; TopFrameOrigin() -> SecurityOrigin pointer
0x419a384 ret
0x419a388 mov x8, x0            ; SetTopFrameOrigin(scoped_refptr<...>)
0x419a38c ldr x0, [x0, #208]    ; previous refptr
0x419a390 str x1, [x8, #208]    ; replacement refptr
0x419a394 cbnz x0, ...          ; release previous refptr
...
```

These match the named methods in current
`third_party/blink/renderer/platform/loader/fetch/resource_request.cc:290-306`.
The next native accessor at `0x419a3c0` returns head+256, consistent with the
HTTP header map following the method slot.

The TopFrameOrigin getter has an independent direct caller at **0xb05a38**:

```asm
0xb057d4 ldr w8, [x19, #336]    ; request context
0xb057d8 cmp w8, #25
0xb057dc b.eq 0xb05a34
...
0xb05a34 mov x0, x19
0xb05a38 bl 0x419a380           ; request.TopFrameOrigin()
0xb05a3c cbz x0, ...            ; requires absent origin in this branch
0xb05a40 brk #0
```

This matches **`FrameFetchContext::PopulateResourceRequestBeforeCacheAccess`**
at current `third_party/blink/renderer/core/loader/frame_fetch_context.cc:974-1001`,
particularly its service-worker-main-resource exception:

```cpp
if (request.GetRequestContext() == RequestContextType::SERVICE_WORKER) {
  CHECK(!request.TopFrameOrigin());
} else {
  request.SetTopFrameOrigin(GetTopFrameOrigin());
}
```

`SERVICE_WORKER` is value25 in the current enum at
`third_party/blink/public/mojom/fetch/fetch_api_request.mojom:27-65`.
The normal native branch retains the origin obtained from frame/document/window
state and replaces **head+208** at `0xb05878-0xb05898`. Thus this is an actual
producer path for the slot, independently of the downstream network converter.

A separate native fetch-context route at `0xa85c338-0xa85c36c` invokes its
top-origin supplier `0xa85b880` and tailcalls setter **0x419a388** with the result.
The supplier receives an optional WebSecurityOrigin from its Web context, matching
current `WorkerFetchContext::GetTopFrameOrigin` and
`PopulateResourceRequestBeforeCacheAccess` at
`third_party/blink/renderer/core/loader/worker_fetch_context.cc:64-78,299-303`.
The worker function naming is a source-backed identification; the setter's
slot and actual call are directly proven. Current source documents that shared
and service workers have no top-frame origin. Do not synthesize one for them.

## 3. Actual extra-field assignment and consumer

Within the converter, the ordinary requestor-origin assignment finishes first.
Then the independent top-frame pointer is loaded and converted:

```asm
0x1f83d48 ldr x22, [x25, #208]  ; Blink head.TopFrameOrigin()
0x1f83d4c cbz x22, 0x1f83ce4   ; absent -> disengaged optional
0x1f83d54 sub x8, x29, #176     ; return storage
0x1f83d58 mov x0, x22
0x1f83d5c bl 0x71896c           ; SecurityOrigin::ToUrlOrigin()
...
0x1f83dac add x9, x19, #1840    ; destination extra optional
0x1f83db0 ldrb w10, [x19, #1920]
```

`0x71896c` is the same converter used for the known requestor and isolated-world
Origin fields. Its implementation unwraps an opaque precursor at source+88,
converts protocol/host strings, and handles the source nonce, matching current
`SecurityOrigin::ToUrlOrigin` at
`third_party/blink/renderer/platform/weborigin/security_origin.cc:296-312`.
Preserve that conversion, including opaque-origin semantics.

The previously quoted `0x1f84c50-0x1f84ca0` is the inlined optional move branch:
it stores the Origin's first bytes at destination+1840, moves the string members
into +1848/+1872, stores the optional nonce and finally sets +1920 to1.
The absent-source branch explicitly disengages the destination optional.

Native adblock throttle **0x4733d34-0x4733d4c** checks +1920, passes +1840
to the recovered exclusion matcher, and bypasses blocking on a match. The
`trusted_params.isolation_info.top_frame_origin` bypass is a later, independent
path at `0x4733d80-0x4733e38`. Neither substitutes for the other.

## 4. Minimal shared plumbing proposal (no implementation in this audit)

The current network request has no field carrying this Blink value. A faithful
minimal addition would use a new optional Origin, with an explicitly chosen
descriptive name such as `top_frame_origin`:

1. **`services/network/public/cpp/resource_request.h`**, `network::ResourceRequest`:
   add `std::optional<url::Origin> top_frame_origin;`, document that it comes from
   Blink request-head TopFrameOrigin, may be absent, and is distinct from trusted
   isolation information. Keep its use confined to the hosted Arc feature.
2. **`third_party/blink/renderer/platform/loader/fetch/url_loader/request_conversion.cc`**,
   `PopulateResourceRequest`: set it from `src.TopFrameOrigin()->ToUrlOrigin()`
   when the pointer is present, otherwise `std::nullopt`. Assign the absent case
   too so reuse cannot preserve an earlier request's origin. Do not reconstruct
   it from request initiator, URL, cookies, or frame getters at throttle time.
3. **`services/network/public/cpp/resource_request.cc`**:
   extend `ResourceRequest::EqualsForTesting` with optional-origin equality.
   Existing copy assignment, move constructor, and move assignment are defaulted;
   copy constructor delegates to copy assignment (`:316-325`), so the new member
   is automatically copied/moved. No custom copy operation is necessary.
4. To preserve the field when the request crosses IPC, add a nullable
   `url.mojom.Origin? top_frame_origin` to **`services/network/public/mojom/url_request.mojom`**,
   preferably appended for existing field/version stability. Extend the
   **`url_request_mojom_traits.h`** `StructTraits<URLRequestDataView, ResourceRequest>`
   getter and **`url_request_mojom_traits.cc`** `Read` with `ReadTopFrameOrigin`.
   Generated files are regenerated, not hand-edited. A precise original Mojo
   schema/ordinal is not recovered here; this is our transport for the proven
   value, not a binary protocol compatibility claim.
5. Hosted Arc URLLoaderThrottle uses this exact optional as the additional
   SameDomainOrHost exclusion origin, independently of initiator and trusted
   top-frame checks. Absence must do nothing.

Meaningful verification for the shared patch:

- `services/network/public/cpp/url_request_mojom_traits_unittest.cc` existing
  ResourceRequest roundtrip/copy test: include a top-frame origin deliberately
  different from initiator and isolated-world origin; assert equality survives
  copy and serialization. Cover absent and opaque Origin values too.
- A converter test should construct a Blink request with a distinct
  `SetTopFrameOrigin`, call `PopulateResourceRequest`, and verify exactly that
  converted Origin. Repeat with an absent source using a previously populated
  destination to verify clearing.
- Hosted integration: initiator domain A, carried top-frame domain B, request
  domain C; excluding B must bypass the blocker even when A/C are not excluded.
  An absent carried origin must not invent B. Keep the separate trusted-top
  origin test so one route cannot accidentally mask the other.

## Reproduction

All bounded instructions can be re-read with the existing helper:

```sh
python3 arc/common/renderer_contract_evidence.py 0x1f83a94:0x100 0x1f83c20:0x1b0 0x1f83eb4:0x90 0x1f84c50:0x74
python3 arc/common/renderer_contract_evidence.py 0x17170c0:0x110 --calls-to 0x1f83a94 --calls-to 0x17170c0
python3 arc/common/renderer_contract_evidence.py 0x419a370:0x58 0xb057d4:0xcc 0xb05a34:0x18 0xa85c338:0x38
python3 arc/common/renderer_contract_evidence.py 0x4733d34:0x20 0x4733d80:0xbc
```

Limits: literal native C++ field spelling, original Mojo schema/ordinal,
additional native producers beyond the identified frame/worker routes, and any
browser-initiated population policy remain unrecovered. The proven renderer
producer requires no guessed source substitution.
