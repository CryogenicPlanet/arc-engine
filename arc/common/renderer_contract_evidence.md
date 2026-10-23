# Renderer contract evidence (2026-10-23)

Read-only inspection of the arm64 slice of
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
Addresses are unslid ArcCore 1.170.0 VAs. Reproduce bounded instructions with
`python3 arc/common/renderer_contract_evidence.py ADDRESS:SIZE`.
The helper reads the fat Mach-O slice, translates segment VAs, uses Xcode's
libLTO disassembler, and changes no binary or process state. Its pointer decoder
matches the DYLD_CHAINED_PTR_64 representation used by this image. Interpret
unresolved import/bind pointers separately.

## Extraction completion: exactly one object, synchronous empty branches

`-[ArcBrowserContextPageContentExtraction getExtractedContentForWebContents:completion:]`
at `0x43f9a98` captures the block and sets native continuation `0x43f9e60`, then
calls `0x42f0254`. Reviewed body:
`research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43f9a98.c`.

The continuation proves `void (^)(ArcExtractedPageContent* nullable)`, with no
NSError argument. The native result optional's engagement byte is +472:

```asm
0x43f9e80 ldrb w8, [x1, #472]
0x43f9e84 cbnz w8, #24
0x43f9e88 ldr x0, [x20]       ; retained completion block
0x43f9e8c ldr x8, [x0, #16]  ; block invoke
0x43f9e90 mov x1, #0
0x43f9e94 blr x8             ; completion(nil)
```

Success allocates class reference `0xe28c240 -> 0xe37d3d8`
(`ArcExtractedPageContent`, independently in the ObjC metadata), initializes it
with selector reference `0xe290ae8 -> 0xd759110`, whose string is
`initWithExtractedPageContent:`, and calls the same block with that object:

```asm
0x43f9efc adrp x8, #166293504
0x43f9f00 ldr x1, [x8, #2792] ; initWithExtractedPageContent:
0x43f9f04 mov x2, sp
0x43f9f08 bl 0x80045ec        ; objc_msgSend
0x43f9f0c mov x19, x0
0x43f9f10 ldr x0, [x20]
0x43f9f14 ldr x8, [x0, #16]
0x43f9f18 mov x1, x19
0x43f9f1c blr x8
```

`0x42f0254` directly invokes that native continuation for unavailable profile
(`self+40 == 0`) or unavailable underlying contents (`contents == 0` or
`contents+192 == 0`). These branches contain no task post or queue hop:

```asm
0x42f03cc strb wzr, [sp]
0x42f03d0 strb wzr, [sp, #472]
0x42f03d4 ldr x8, [x19, #16]
0x42f03dc mov x1, sp
0x42f03e0 mov x0, x19
0x42f03e4 blr x8
0x42f042c strb wzr, [sp, #480]
0x42f0430 strb wzr, [sp, #952]
0x42f0434 ldr x8, [x19, #16]
0x42f043c add x1, sp, #480
0x42f0440 mov x0, x19
0x42f0444 blr x8
```

Thus those empty callbacks occur synchronously on the caller's thread. The
public ObjC wrapper itself traps for nil ArcWebContents/disengaged wrapper;
do not mistake the backend's null-native handling for a proven nil-public-arg
contract. The backend captures frame lifetime/native identity, sets continuation
`0x42f102c`, and calls `0x41ba8a0`; that resolves native frame-associated state
through `0x4ebd98`, `0x805e0`, then `0x41ba32c`. Producer options, completion
metadata capture, and exact proto-to-Arc enum mappings are recovered below.

## Adblock list strings are FilterList contents

Both APIs accept arrays of strings. `setBlockLists:` enumerates through
`0x43d763c`, `setExcludedDomains:` through `0x43ea1c8`. Each directly calls the
NSString-to-std::string helper `0x1b356b8` (`0x43d7698`, `0x43ea214`). Their
24-byte vector elements are strings, not dictionaries or DTOs.

`setBlockLists:` -> `0x4332374` -> `0x42bd840` posts background work with source
location string `SetBlockLists` at `0xd37e3a8`, source path
`../../arc/browser/browser_application_impl.cc` at `0xd37dc17`, line 1784.
The work invokes converter `0x47318dc`, then eventually publishes through
`0x42c0538 -> 0x42970d0`. It is not synchronously compiled in the setter.

Each string is **raw FilterList text**, not a path or download URL:

- `0x4731f10` constructs an input string stream (`0x2daf798`) directly from
  each vector string. RTTI `0xdd289c0 -> 0xc4ae48c` is
  `NSt4__Cr19basic_istringstreamIcNS_11char_traitsIcEENS_9allocatorIcEEEE`.
- `0x4731f94-0x4731fa0` calls getline (`0x3629a44`) with delimiter 10.
- `0x4732008-0x4732010` scans backwards for `$` (36), excludes regex-tail `/`
  at `0x47320ac-0x47320b4`, then splits options on comma (literal at
  `0xd0093b0`). `0x4732164-0x473217c` recognizes the nine-byte string
  `badfilter`. This is additional Arc preprocessing, beyond plain Chromium
  parsing.
- The second pass constructs the same string stream (`0x4732594`), selects
  input format **1** (`0x4732598`), creates the rule input stream at
  `0x4420880`, then calls transfer/conversion at `0x4420cd8`.
- Debug string `0xd72981c` is `Convert blocklist`.

Source-backed comparison: local
`chromium/src/components/subresource_filter/tools/ruleset_converter/ruleset_format.h`
defines 1 as `RulesetFormat::kFilterList`; `rule_stream.cc:79` defines
`FilterListRuleInputStream` using getline + `RuleParser::Parse`; the same file's
`RuleInputStream::Create` and `TransferRules` provide the existing reusable
parser/converter. This is strong candidate identification from call shape,
format value, constructor RTTI, and parser behavior, not a claim that every Arc
customization equals upstream Chromium.

Excluded domains: `0x42bde04 -> 0x4296d5c` gets global state and
`0x42971f0` replaces its optional string vector at +48/+56/+64, sets engagement
byte +72, then notifies existing registered agents through `0x4297370`.
That clones the string vector and calls `0x4730480`. Domain matching grammar
(suffix, exact, wildcard) has not been recovered here, but the setter's actual
preprocessing is now concrete:

```asm
0x47308a4 ldrsb x8, [x27, #23] ; input std::string
0x47308c4 add x22, x20, #8     ; output length = input length + 8
0x47307b0 mov x8, #29800
0x47307b4 movk x8, #28788, lsl #16
0x47307b8 movk x8, #14963, lsl #32
0x47307bc movk x8, #12079, lsl #48
0x47307c0 str x8, [x0]        ; little-endian bytes: https://
0x47307e0 mov x0, x22         ; destination immediately after https://
0x47307e4 mov x1, x21         ; original domain bytes
0x47307e8 mov x2, x20         ; original domain length
0x47307ec bl 0xc4a5a48        ; copy
0x4730814 bl 0xc28450         ; GURL constructor
0x4730818 add x8, sp, #24     ; 80-byte aggregate result
0x473081c add x0, sp, #128    ; GURL
0x4730820 bl 0x4b7f9c         ; candidate url::Origin::Create
```

`0xc28450` has the GURL layout: 24-byte string, validity byte +24, parsed
component pairs starting +32, nested URL storage +112. `0x4b7f9c` reads that
layout, splits scheme/host/port, and returns an 80-byte origin-like aggregate.
This is strong evidence for origin normalization, not a raw suffix matcher.
The origin-class function name remains a source-backed inference. The converted
vector replaces refcounted storage at agent +112 (`0x47304f0-f4`). The method
posts `OnExcludedDomainsChanged` from source line 108; callback `0x4730fa0`
iterates already registered agents and sends the vector through `0x441fb58`.
That serializer's message ID is `0x258a997e` (`0x441fba0-a4`), and receiver
dispatch passes the decoded 80-byte vector to implementation virtual slot +56
at `0x44202c4-d4`. Exact equality/suffix behavior at that implementation,
mainframe-versus-frame matching, and count-reset behavior remain unproven.

Badfilter normalization is bounded at `0x47320a0-0x473239c`: locate the final
`$`, split options at commas, compare option key before `=` against the
case-sensitive nine-byte `badfilter`, build a prefix from bytes preceding `$`,
append `$` plus a joined option string, then parse the resulting FilterList
rule. The exact predicate used when rebuilding options and the final normalized
rule comparison have not been fully identified. Do not claim raw-line removal
alone is equivalent.

## Effects persistence and update semantics

Reviewed bodies already available:
`research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/42e1d80.c`
and `42e3140.c`.

- Browser-side host patterns are retained verbatim as keys in a sorted vector
  (`0x42e29c0`: lexicographic binary search/insert, raw copies). Registration
  does not parse a URLPattern or regex at this boundary; the renderer exact-host
  matching implementation is recovered below.
- Config holds independent stylesheet and script vectors. Entries retain
  identifier + text. Existing identifier replacement occurs within the
  selected host pattern and mode; absent identifier appends (0x42e1d80).
- Every registration posts `NotifyBoostConfigChanged` from
  `../../arc/browser/browser_context/effects_impl.cc:45`; captured weak state
  calls `0x42e47c4`, which iterates existing registered agents and sends cloned
  configuration through `0x4428568`. Thus updates propagate to already
  registered renderer agents; renderer-side application timing is separate.
- Color-scheme registration accepts only API values 0 and 1 in examined
  backend `0x42e3140`, maps 0 -> native 1 and 1 -> native 0, and marks the
  optional present. This reversal is explicit in the body; names/light-dark
  meanings are not established here.
- Binder registration `0x43a809c` publishes both `arc.mojom.EffectsAgent` and
  `arc.mojom.AdBlockerAgent`; corresponding targets are `0x43a8a50` and
  `0x43a482c`. These targets were unresolved LAB references in prior research.

Renderer exact-host matching, CSS priority, JS world, document-start timing,
and existing-document configuration handling are recovered below. Visual filter
hook details are audited separately by the renderer visual-contract worker.

## Renderer exclusion implementation

The actual 400-byte renderer agent primary vtable is `0xda82898`. Its slot +56
points to `0x43a6e54`, exactly matching message `0x258a997e` dispatch. This
implementation clones the decoded origin vector into refcounted storage +376
(`0x43a6f20-f24`), clears its cached map at +208/+216/+224
(`0x43a6f58-f78`), invokes invalidation at `0x78225ac`, and enumerates current
frames through RenderFrame::ForEach (`0x22322ec`) to reapply cosmetic CSS.
Blocked-resource count reset occurs separately on PrimaryPageChanged, below.

Filter evaluator `0x43a51f4` receives a document origin as its second argument.
At `0x43a5260-268` it calls `0x47336f0(exclusions, document_origin)`. This helper:

1. Returns true unconditionally when the origin scheme equals the 16-byte
   literal `chrome-extension` (`0x4733730-0x4733784`).
2. Otherwise loops 80-byte origin entries and calls
   `0x16bce58(excluded_origin, document_origin, 1)` (`0x47337a4-b0`).
3. That function extracts **only each origin's host field at +32**, and
   tailcalls string comparison helper `0x14f6940` (`0x16bce74-0x16bce9c`).
4. The string helper rejects empty hosts, accepts exact host equality, and
   otherwise compares nonempty domain-and-registry strings after registry
   parsing (`0x14f6940-0x14f6b00`).

The exact source-backed identification is
`net::registry_controlled_domains::SameDomainOrHost(url::Origin, url::Origin,
INCLUDE_PRIVATE_REGISTRIES)`:
`chromium/src/net/base/registry_controlled_domains/registry_controlled_domain.cc:482-500`
and `:536-539`; enum value 1 is `INCLUDE_PRIVATE_REGISTRIES` in the adjacent
header. This establishes **same registrable domain or exact host**, ignoring
scheme and port, including sibling subdomains, while respecting private PSL
boundaries. The setter's https prefix canonicalizes host parsing; it does not
restrict bypass to HTTPS.

## Recovered extraction proto-to-Arc enum converter

Native success continuation `0x42f102c` calls recursive converter `0x42f0524`
at `0x42f1578`. Its input is a protobuf ContentNode, with content_attributes
pointer at +40 and child repeated-pointer field at +24. The converter reads
ContentAttributes attribute_type at +68, subtracts 1, and indexes 27-byte table
`0xc9d67dc`; out-of-range types map to Arc container (1).

Exact table bytes:
`[0,1,2,4,5,11,12,17,7,14,16,1,1,1,1,1,1,1,1,3,15,6,13,18,8,9,10]`.
These match the local optimization_guide proto enum, not Blink Mojo's enum:

| Proto ContentAttributeType | Arc type |
|---|---:|
| ROOT=1 | 0 |
| CONTAINER=2 | 1 |
| IFRAME=3 | 2 |
| TEXT=20 | 3 |
| PARAGRAPH=4 | 4 |
| HEADING=5 | 5 |
| ANCHOR=22 | 6 |
| IMAGE=9 | 7 |
| SVG_ROOT=25 | 8 |
| CANVAS=26 | 9 |
| VIDEO=27 | 10 |
| ORDERED_LIST=6 | 11 |
| UNORDERED_LIST=7 | 12 |
| LIST_ITEM=23 | 13 |
| TABLE=10 | 14 |
| TABLE_ROW=21 | 15 |
| TABLE_CELL=11 | 16 |
| FORM=8 | 17 |
| FORM_CONTROL=24 | 18 |

Input attribute types 12-19 and values outside 1-27 map to Arc container (1).
Current Chromium dialog types 28/29 therefore also map to container when
reproducing this recovered converter.

Text conversion (`0x42f0700-0x42f0794`) handles proto oneof tag 10. TextStyle
has_emphasis goes straight to native ContentNode+64, text_color straight to+72.
Proto text size 0 (default) -> Arc2; proto1..4 map through table
`0xd741de4 = [0,1,3,4]`. Out-of-range proto size -> Arc2.

Other oneof field mappings are explicit in the converter:
tag14 anchor URL -> native+80; tag11 image alt ->+112;
tag9 iframe's local-frame-data origin ->+144; tag19 form name/action ->+320/+352;
tag8 table name ->+384; tag20 form-control data ->optional at+176. Child nodes
are converted recursively at `0x42f0674`, in input order, with 416-byte native
ContentNode elements. This matches
`chromium/src/components/optimization_guide/proto/features/common_quality_data.proto`
ContentNode/ContentAttributes/TextStyle/ContentAttributeType, and provides a
direct route from `GetAIPageContent(...).proto.root_node()` to Arc's DTO tree.

Form-control proto enum values 2..33 map through `0xc9d67f8`:
`[8,12,16,17,18,19,22,24,25,27,30,31,0,33,1,2,3,5,6,7,9,10,11,13,14,15,20,21,23,26,28,29]`.
Outside that range, including unspecified/contenteditable, maps to Arc0.
Form data checked/required scalar bytes are copied directly; is_redacted is
`form_control_data.redaction_decision() == 2`. SelectOption value/text strings
preserve presence (nil for absent), while is_selected/is_disabled are copied
directly (`0x42f0a78-84`, `0x42f0b10-7c`). imageAlt is image_caption (tag11),
not image URL. iframeOrigin follows iframe frame_data/security_origin/value
presence checks; it does not independently normalize that string.

URL/title/time are captured on successful native completion, after lifetime
checks: current committed entry URL at `0x42f12f8-0x42f133c`, current title
getter at `0x42f1370`, current wall clock at `0x42f1444`, then Unix-seconds
conversion at `0x42f1540-48`. They are not snapshots from request start.

## Recovered Boost application lifecycle, host grammar, CSS origin and JS world

`--calls-to 0x43a8e40` finds exactly one direct call, at `0x43a7fe4`, inside
function `0x43a7fb4`. The original renderer client vtable `0xda829d0` slot45
(+0x168) points there. The local macOS ContentRendererClient declaration order
(two destructor slots, excluding Android-only HandleNavigation) identifies
slot45 as **RunScriptsAtDocumentStart**. Slots46/47 are the adjacent document
end/idle hooks and are not this Arc override. See
`chromium/src/content/public/renderer/content_renderer_client.h:348-359`.

Its frame argument is RenderFrameImpl: frame_ at +80, scheduling group at +88,
initialized_ at +96, is_main_frame_ at +97, matching local
`content/renderer/render_frame_impl.h:1201-1212`. Application `0x43a8e40`
requires +97 to be true (`0x43a9014-18`): **profile Boosts apply only to the main
frame in this path**. This replaces the earlier tentative readiness-flag label.

The function derives the current frame GURL and uses host-piece begin/length
(GURL +56/+60, stack+200/+204). It performs lexicographic lower_bound followed
by exact length and memcmp equality against configuration keys at
`0x43a905c-0x43a9110` and `0x43a9200-08`. There is no wildcard, suffix or regex
matching in this recovered host application route.

EffectsAgent configuration receiver `0x43a93a0` only swaps the config pointer
at +16 and destroys the prior configuration. It does not reapply to existing
frames. The sole recovered application path is the document-start hook.

CSS entries use `0x43a9608`: WebDocument::InsertStyleSheet target `0xad13e8c`
gets `key=nullptr`, `WebCssOrigin::kUser` (1),
`BackForwardCacheAware::kAllow` (0) at `0x43a98c8-e0`. Returned insertion keys
are cached by identifier. Replacement removes the old inserted sheet through
`0xad140cc` with CSS origin1 before reinserting. This matches local
`third_party/blink/renderer/core/exported/web_document.cc:282-307` and
`public/web/web_css_origin.h`.

JS entries use `0x43a9e48`: `0xa546eec` gets **isolated world ID6** and
BackForwardCacheAware::kAllow (0) at `0x43a9fc0-d0`. Its body checks world>0
and the embedder limit and dispatches the isolated script world. It matches
`WebLocalFrameImpl::ExecuteScriptInIsolatedWorld`, local
`third_party/blink/renderer/core/frame/web_local_frame_impl.cc:1072-1094`.


## Extraction request option defaults (native constructor)

The APC request constructor `0x3127d5c` allocates its 88-byte
AIPageContentOptions and stores it at request+64. It reads FeatureParam
`0xe1e0aa0`: owner `0xe36f028` (AnnotatedPageContentExtraction), name
`mode` (`0xd01f3fc`), default `default` (`0xd0a2692`). An exact ten-byte
comparison with `actionable` at `0x31280b0-0x31280e0` yields mode1 only on
that value; otherwise mode0. Thus stock defaults use kDefault. At
`0x3127e68-0x3127e80`, FeatureParam `0xe1e0a40` has the same owner,
name `on_critical_path` (`0xd48a5bc`), bool default0, copied to options+4.
`include_inner_text` (`0xe1e0a80`, default0) is copied to request+72 at
`0x3127e94-0x3127eac`. Options are feature-controlled at request creation;
there is no unconditional actionable/on-critical-path override in this route.

## Ad-block debug callback, URL argument, and disabled-feature gate

ObjC IMP `0x43f5194` converts its first argument with `0x43abdf8`, which
uses NSURL-to-GURL bridge `0x312f86c`, then copies valid GURL spec via
`0x7cf864`; invalid conversion yields an empty string. Continuation
`0x43f8ed0` converts one native std::string to NSString and invokes the block
with exactly one object argument: `void (^)(NSString*)`, no NSError.

Backend `0x42d6edc` returns these strings directly on the calling thread:

- native context+136 null: `BrowserContext is headless` (`0xd37e99c`);
- feature11 false via `0x20cf42c(11)`: `Native ad blocker feature disabled`
  (`0xd37e9b7`);
- the service's renderer agent+152 null in `0x47314c4`:
  `No agent is bound to profile` (`0xd72978d`).

Otherwise `0x47314c4` sends the URL-spec string over the agent Mojo
proxy `0x441f82c`; renderer receiver `0x43a70c0` creates a GURL and origin
from that same debug URL, invokes `0x43a51f4`, and formats its debug result.
This debug URL origin is independent of frame-origin provenance in the actual
page application path.

Feature11 **defaults false**: initializer-table entry `0xdff3520` uses
`0x44d8a48`, which emits bool type1 and value0. Native feature getter
`0x20cf42c` returns that stored bool. Feature initialization
`0x44d8788-0x44d87a8` conditionally overrides feature11 to true when an
application-global enum equals1. Explicit `setFeatureEnabled:11 enabled:YES`
therefore removes the disabled gate for an otherwise-default probe host.


## Boost identifier order and unregister scope

Registration `0x42e1d80` finds/replaces an existing identifier within the
selected host/mode vector in place (`0x42e4104`), otherwise appends
(`0x42e5a3c`). This preserves insertion order, including on replacement.
Renderer application iterates the CSS vector in stride56 at
`0x43a9208-0x43a9248`, then the script vector at
`0x43a924c-0x43a928c`, each in stored order.

UnregisterStylesheet/Script bridge to `0x42e2240` with mode0/1 respectively.
The backend walks sorted host entries (stride32), then selected vector
(stride56), and removes **the first matching identifier only**. It shifts
following entries left preserving order. On that first match it posts
NotifyBoostConfigChanged and returns; it does not continue to another host.
No match simply returns without notification. Nil identifier maps to the
empty identifier string in the ObjC bridge.

Recovered cleanup has a narrow original quirk. After removal, instructions
`0x42e2384-0x42e238c` load config from host_entry+24 and test **config+8**
(CSS-vector size). If nonzero, notify/return. If zero,
`0x42e2348-0x42e234c` tests **config+72 bit0** (color-filter presence).
If present, notify/return; otherwise `0x42e2358` erases the whole host entry
via `0x42e2fe8`, then notifies/returns. These instructions do not separately
check the remaining script-vector count or color-scheme presence. Thus
unregister can erase a host entry that still has scripts/scheme if those two
checked fields are empty. This is binary evidence, not an intended API rule.

## Direct stylesheet insertion uses the same CSS helper

Browser direct insert endpoint `0x4379020` sends Mojo request hash
`0x3190b3f3` through `0x4429e2c`. Renderer frame-agent method
`0x43aa76c` (vtable entry `0xda82d38`; adjacent remove at `0xda82d40`)
resolves its frame and tailcalls **the same helper `0x43a9608`** at
`0x43aa7a0`. Therefore direct insertion shares the recovered
WebCssOrigin::kUser/BFCacheAllow and identifier replacement behavior with
Boost CSS. It does not use author-origin insertion defaults.


## Bound ad-block debug payload

Renderer `0x43a70c0` constructs a GURL and origin from the supplied debug
URL string, then passes the origin/GURL and a CSS out-string to `0x43a51f4`
at `0x43a714c`. Formatting at `0x43a7234-0x43a747c` is exactly:

```text
/* Result: <STATE> */
/ * CSS: */
<CSS>
```

There is no added trailing newline after the CSS string. The unusual
slash-space-star second-line literal is original (`0xd38329f`). State values
are 0 Success, 1 `Cached: ` plus result.string, 2 Empty, 3 NoScheme,
4 Excluded, 5 InternalScheme, 6 NoBlockList. Strings reside at
`0xd38328f`, `0xd38329b`, `0xd38329f`, `0xd3832f6-0xd383320`,
`0xd00f003` (Empty), and `0xd0312c4` (Success). Missing service ruleset
(agent+112 null) produces state6 at `0x43a5218-0x43a52a4` and empty CSS,
so its exact payload is `/* Result: NoBlockList */\n/ * CSS: */\n`.


## Blocked-resource counter reset

Native WebContents field+1484 is the source for the ObjC
blockedResourceCount property: sync `0x440edbc-0x440edcc` loads that int and
calls setBlockedResourceCount. Setter `0x438eb84` writes+1484 and notifies
observers; constructor zeroes it at `0x4371a94`.

Reset thunk `0x438f2fc` subtracts168 from its WebContentsObserver subobject,
sets w1=0, and branches to the setter. It occupies vtable
`0xda81b40` slot5 (`0xda81b68`), matching two destructor slots followed by
RenderFrameCreated, RenderFrameDeleted, RenderFrameHostChanged, then
**PrimaryPageChanged** in local content/public/browser/web_contents_observer.h.
Thus the native count resets at primary-page change, including page activation,
rather than exclusion updates or provisional navigation start. Direct reset
entry `0x438f2f4` is also present in the primary class vtable.


The setter starts with an equality guard at `0x438eba4-0x438ebb0`:
current+1484 is compared with new w1 and equal values return without writing
or notifying. Resetting an already-zero counter therefore does not issue
another ObjC property setter through the observer path.
Increment observer `0x438f2d0` reads resource_info+440 and increments only
if that error int is **-90**, using `0x438f2e0-0x438f2f0`. This differs from
current upstream ERR_BLOCKED_BY_CLIENT=-20; -90 appears Arc-specific.


## Real network throttle exclusion provenance

The network URLLoaderThrottle starts at `0x4733b34`, proven by its blocked
cancellation at `0x4733fb8-0x4733fd8`: delegate virtual CancelWithError gets
w1=-90. This is distinct from renderer cosmetic evaluator `0x43a51f4`.

At `0x4733b6c-0x4733b8c`, it chooses url::Origin from
ResourceRequest.request_initiator (origin at request+240, optional flag+320)
when present; otherwise creates an origin from request.url (GURL at+24).
The chosen origin is tested against exclusions at `0x4733c4c-0x4733c58`.
Ordinary subresource filtering therefore starts with its initiator origin,
not automatically the outermost document origin.

There are additional bypass origins, so initiator-only is incomplete:
`0x4733d34-0x4733d4c` checks an optional origin at request+1840/flag+1920
(field name not yet identified). `0x4733d80-0x4733e38` checks
ResourceRequest.trusted_params (+952, optional flag+1256), takes its
IsolationInfo refcounted Data pointer, tests top_frame_origin flag Data+88,
copies origin Data+8, then calls the same exclusions matcher at `0x4733e38`.
The Data layout matches net/base/isolation_info.h: refcount4, RequestType4,
then optional top_frame_origin at+8. Thus a trusted top-frame origin exclusion
also bypasses network blocking, even for a different initiator origin.

All these bypasses use the previously recovered SameDomainOrHost with private
registries included, plus unconditional chrome-extension scheme exemption.


## Badfilter matching uses canonical parsed URL-rule protobufs

The rebuilt rule with
badfilter removed is fed to RuleInputStream::Create(kFilterList) at
`0x4732350-0x4732368`; FetchNextRule must return RULE_TYPE_URL=2
(`0x4732374-0x4732384`), then GetUrlRule is obtained at `0x47325e4-0x47325f4`.
The rule is serialized into an empty string at `0x473203c-0x4732044` and
inserted into the disabled-rule hash set (`0x4732064-0x4732078`).

The serialization helper `0x19df250` matches protobuf
MessageLite::AppendPartialToString: virtual ByteSizeLong at+24, resize
old_size+size, serializer virtual+40; see local
third_party/protobuf/src/google/protobuf/message_lite.cc:601-616.
Normal parsed UrlRules are serialized identically at
`0x4732548-0x4732550`, hashed/compared against that set, and only unmatched
rules are passed to AddUrlRule (`0x473251c -> 0x905b3c8`).

Therefore disabled-rule comparison is **parsed UrlRule protobuf equality**,
with the upstream parser's option normalization, not raw source string
equality. This applies across input lists. Do not hand-invent option sorting
or whitespace normalization; parse and compare serialized UrlRule values.


## Network skip conditions and element types

The predicate callback stored at throttle+32 is bound to `0x43a3b0c` by
both browser (`0x42ac5b8`, mode flag1) and renderer (`0x43ab4dc`, mode flag0)
constructors. `0x99ed2c8` is its RepeatingCallback::Run wrapper, not a regex.
The predicate exact-matches schemes chrome-extension, devtools, chrome,
view-source. Literal pointers are `0xc4ae8e5`, `0xc4ae8ab`, `0xc4ae903`,
`0xc75e958`. It is applied to the chosen origin scheme at `0x4733d00-2c`
and target request.url scheme at `0x4733edc-0x4733f30`.

Helper `0x19517c4` exempts filesystem/about/blob/data target URL schemes
(literals `0xc6c2212`, `0xc6d6050`, `0xc6d5f28`, `0xc6ca4b0`).
There is no local/nonunique-host or HTTPS-only exemption in this main routine.
The constructor mode flag+40 skips third-party Document destination3 only
(`0x4733bd8-0x4733bfc`); it does not bypass script loads.
`0x1328298` is FirstPartyOrigin::IsThirdParty, matching local implementation.

Request+704 is network RequestDestination. Table `0xcf27094` maps values1..22
as `[512,1,128,16,1024,128,128,4,1,16,1,1,2,1,1,8,512,512,16384,1,1,128]`.
Script13 therefore maps to ElementType2, image8 to4, style16 to8. Other values
use ElementTypeOther1. Native IndexedRulesetMatcher::GetLoadPolicyForResourceLoad
is `0x1327f68`; the throttle cancels only result3 (DISALLOW), not ALLOW1 or
EXPLICITLY_ALLOW0.

## Cosmetic debug CSS assembly and cache

Evaluator cache-hit `0x43a64d4-0x43a655c` copies cached CSS from map node+40
into the CSS out-string and returns state1 with the **full request host**
string initially built at `0x43a5324-0x43a5378` (sp152). Thus a debug cache
result says `Cached: <full-host>` and includes that same CSS.

No selectors returns state2 Empty at `0x43a5b94`. Selector assembly
`0x43a5a90-0x43a5bd4` iterates unordered_set nodes, comma-separates selectors,
and ends each group with exact `{display:none!important;}` followed by newline
(literal `0xd3832dc`). Groups split before accumulated selector count would
exceed4096, counting embedded commas+1 for each selector. Final group also ends
with a newline. Selector iteration order follows the native unordered_set,
rather than source-list order.


## Cosmetic state precedence, publication timing, and stylesheet key

Fresh evaluator ordering is ruleset presence (NoBlockList6), exclusions4,
cache lookup1, absent parsed URL scheme3, internal scheme5, then selector
assembly Empty2/Success0. Exclusion matcher also handles chrome-extension, so
that chosen-origin scheme returns Excluded before generic InternalScheme.
The later internal scheme test additionally handles
filesystem/about/blob/data and chrome-extension/devtools/chrome/view-source.
Success cache write at `0x43a5df4-0x43a5e0c` is gated by nonzero selector
count; **Empty results are not cached**.

The exact cosmetics application key is **blocklist** (9-byte literal
`0xd27d76c`). After evaluator `0x43a4ef4`, result state0/1 inserts CSS using
shared helper `0x43a9608` at `0x43a501c`; any state>1 removes it through
`0x43a9b48` (`0x43a4f58`/`0x43a4fc0`). This shares user-origin CSS and the
same identifier insertion-key cache as public direct insertion and Boosts;
there is no separate namespace avoiding an identifier collision.

Both ruleset receiver `0x43a6c2c-0x43a6c88` and exclusions receiver
`0x43a6f58-0x43a6fb4` clear their CSS cache, construct a RenderFrameVisitor
(vtable `0xda82928`, Visit `0x43a7778`), and call
**content::RenderFrame::ForEach** (`0x22322ec`). The Visit method calls
`0x43a4bdc` on every current frame and returns true. Document-start hook
`0x43a7fb4` also applies cosmetics, after Boost application. There is no
main-frame restriction in cosmetics `0x43a4bdc`, unlike Boosts.

Consequently cosmetics update immediately in existing frames on ruleset or
exclusion publication, and at document start for new documents. This timing
uses native frame enumeration, not a DOM mutation observer or timer.


Native generic-rule disabling is not unconditional. At
`0x4733f68-0x4733f7c`, the throttle tests matched activation type1 DOCUMENT
and bypasses the request on a match. At `0x4733f80-0x4733f94`, it tests
type8 GENERICBLOCK through helper `0x2c4485c` and copies that boolean to w4.
The load-policy call at `0x4733f98-0x4733fac` then receives matcher, URL,
FirstPartyOrigin, mapped ElementType, **that activation-derived bool**,
and null out_rule. Helper matches current IndexedRulesetMatcher activation
query (indexed_ruleset.cc:177-186). A raw `$script` blocking rule alone does
not set GENERICBLOCK; that requires a matching deactivation/allowlist rule.


## Renderer network throttle provider is unconditional

Renderer client factory `0x43a8008` calls upstream provider factory
`0x21a80b4`, then wraps it with Arc provider vtable `0xda82e18`, holding
ad-block agent's provider interface (agent+16, renderer client+96).
CreateThrottles method `0x43ab41c` calls the wrapped provider and
unconditionally appends Arc throttle `0x4733814` with mode0. There is no
feature11 test in either function. Browser provider path `0x42ac508`
separately gates on feature11 at `0x42ac548-0x42ac550` and creates mode1.
A hypothesis that renderer provider creation itself is disabled by a separate
child-process feature11 table is therefore unsupported by these functions.


## HTML-to-Markdown selectors: AI marshaller omission, not ObjC DTO behavior

Original runtime evidence
`results/ai-service-probe/trace156/injected/events.json` conversion.convert
returns selectorsApplied=[] despite a fixture C ABI result with two selectors.
The omission is in native C-to-Mojo marshaller **0x42857b8**.

On success it allocates and zeroes the entire128-byte Mojo result at
`0x42858fc-0x4285918`. C ABI struct (including outer union+8) reads are:
body ptr+8/length+16, title ptr+24/length+32, references ptr+40/length+48.
It never reads selectorsApplied ptr+56 or length+64, leaving destination
selector vector+72/+80/+88 zero. At `0x4285bcc-0x4285bdc`, it skips directly
to percentage at C+72 -> destination+96, then metaTags C+80/length+88 ->
destination vector+104/+112/+120. Thus native service conversion intentionally
reproduces an observable omission independent of provided selector values.

The subsequent native-to-ObjC factory **0x43de6ec** correctly copies its
native result via `0x42a5f10`, constructs selector NSArray capacity from
sp128 at `0x43de8b0`, loops native std::strings (stride24) and adds each at
`0x43de8c0-0x43de91c`, then passes that NSArray as x5 to the public initializer
at `0x43dea58-0x43dea60`.

The public ObjC initializer **0x43dd5d4** preserves caller arrays correctly:
selectors argument x5 is saved in x22 at `0x43dd5fc`, sent copy at
`0x43dd6b8-0x43dd6c0`, and stored self+32 at `0x43dd6c8`.
Getter `0x41615f8` returns self+32. Therefore the parity correction belongs
only in the AI C ABI conversion callback, leaving direct DTO initialization
as a normal NSArray copy. Body/title/reference/metaTag/percentage mapping is
unaffected by this specific omission.


Badfilter preprocessing also uses TRIM_WHITESPACE and SPLIT_WANT_NONEMPTY:
`0x47320d4-0x47320f0` passes separator length1, w4=1, w5=1 to
SplitStringPiece (`0x1723788`), matching base/strings/string_split.h enums.
Thus a padded option such as `script, badfilter` is recognized before parsing,
and empty comma options are dropped before rejoining retained options.

Live stock readiness evidence:
`results/renderer-adblock-enabled-rules-original-run2/trace156/adblock-observations.json`
records initial NoBlockList through30 half-second readiness polls, then later
Success/CSS/blocking after exclusions were cleared. The exclusions receiver
has no ruleset-pointer write, so this does not establish an empty-exclusions
activation requirement. It is consistent with delayed initial ruleset
publication/receive; use debug-state readiness, not a synthetic exclusion gate.

## Compiled blocklist publication: exclusions do not flush it

The later combined probe `results/renderer-combined-run2/trace156/observations.json`
stayed NoBlockList through120 half-second polls; elapsed time alone is not a
reliable readiness contract. The entire native publication chain is now bounded:

- `0x42bd840` first checks feature11 (`0x42bd858-0x42bd860`); false drops the
  request. True posts the raw string-vector to the singleton sequenced pool
  runner, invoking `0x42bda30`. Its task traits at **0xc9d66c4** are
  `80 00 01 00 01 00 00 00`: explicitly priority0/BEST_EFFORT, MayBlock.
  Constructor `0x107d9c0`, specifically `0x107da10-0x107da30`, masks byte0
  with0x7f and stores the resulting priority0. This is subject to pool best-effort
  fences, rather than a timer in the ad-block compiler.
- Worker `0x42bda30` calls compiler `0x47318dc` at `0x42bda48`; after completion
  `0x42bda4c-0x42bdad4` posts the resulting pair to UI runner `0x49fafc`
  using BrowserTaskTraits at0xc9d66cc (type0, priority2). The UI callback is
  **0x42c0538**, which invokes singleton manager `0x4296d5c` then **0x42970d0**.
- `0x42970d0` immediately stores the pair at manager+24/+32 with engaged flag+40,
  then, if feature11 true, iterates registered profiles+80/+88 and invokes
  **0x42977cc** (`0x42971ac-0x42971c8`). That invokes cosmetic setter
  **0x472f990** and URL setter **0x472ff20**.
- Cosmetic setter stores its shared region at service+16 and posts weak-pointer
  callback **0x4730a08** through service+192/+200. This callback iterates bound
  agents and sends cosmetic Mojo **0x441f6b4** when feature11 and service+24
  region validity are true (`0x4730ac0-0x4730b20`); otherwise resets with0x441f7a0.
  The source-location name at0xd729733 is `NotifyCosmeticFilterRulesetChanged`.
- URL setter stores its shared region at service+64, then **0x4730320** replaces
  weak-pointer flag+176 and posts callback **0x4730bb4**. That sends URL Mojo
  **0x441f9e0** to agents and updates the service's local agent through0x43a6d3c.
  Its location name0xd729756 is `NotifyURLBlockListChanged`.
- Exclusions backend **0x42bde04** gets the same singleton and invokes
  **0x42971f0**, storing vector at manager+48/+56/+64 with engaged flag+72.
  It iterates profiles and calls **0x4297370**, which invokes **0x4730480**.
  This only replaces service+112 origin-vector and posts callback **0x4730fa0**
  through service+192/+200. The callback sends **0x441fb58** with origin-vector
  (`0x4730fec-0x4731004`). Location0xd729770 is `NotifyExcludedDomainsChanged`.
  Neither browser excluded setter nor its posted callback invokes the compiled
  pair setter, region setters, or cosmetic/URL broadcasters. Renderer exclusion
  receiver0x43a6e54 likewise only swaps origins+376, clears CSS cache, and reapplies.

Thus no recovered exclusion path flushes a pending ruleset. The strong-run
transition coinciding with exclusions-cleared does not prove causal activation.
Useful runtime checkpoints to distinguish starvation from lost distribution are:
worker-entry0x42bda30, compiler-return0x42bda4c, UI-completion0x42c0538,
cosmetic-set0x472f990, cosmetic-broadcast0x4730a08, URL-broadcast0x4730bb4,
and exclusion-broadcast0x4730fa0. No instrumentation was performed in this audit.
The local Chromium156 source explicitly holds a ScopedBestEffortExecutionFence
until its PostAfterStartupTask callback resets it
(`content/browser/browser_main_loop.cc:1011-1017`). This is a concrete possible
queue gate, not proof that the original failing run had that fence active.

## Debug chooses the earliest bound renderer agent, not the local browser agent

The profile-local browser agent hypothesis is disproved by the exact call path:
`0x42d6edc` checks context+136 and feature11, gets keyed profile service through
`0x4296bcc` at0x42d6f74, and calls **0x47314c4** at0x42d6fa0. That service
method tests **service+152, the bound-agent map size**, at0x47314dc-0x47314e0.
If zero it synchronously completes `No agent is bound to profile`. Otherwise
it loads **service+136, the ordered map's leftmost node**, and that node's
**+56 Mojo proxy**, then tail-calls **0x441f82c** at0x4731560. This serializes
an actual Mojo debug request (message hash0x4067115f) and response callback.
It never reads the service's local browser agent at+120, nor searches frames
by the supplied URL.

Registration **0x4731028**, called at0x42ac400, assigns monotonic keys from
service+128: reads counter and increments at0x4731160-0x473116c, inserts old
counter at map-node+32 (`0x4731258`), saves remote state+40/version+48/proxy+56,
and increments map-size+152 at0x4731290-0x4731298. The constructor initializes
that counter to1 (`0x472f55c-0x472f560`). The selected debug destination is
therefore the earliest surviving registration, not lowest OS process ID.
The registered remote's disconnect callback0x4731720 removes that key from
the map (`0x47317ec-0x473185c`); subsequent debug uses the next leftmost node.
The map entry owns its remote state and proxy; the absence condition is an
empty registry, not absence of the browser-local evaluator.

Both debug and actual cosmetic application use the **same evaluator/cache in
one renderer agent**: debug0x43a70c0 parses the supplied string into GURL and
Origin then calls0x43a51f4 at0x43a714c; actual frame cosmetics0x43a4bdc calls
it at0x43a4ef4. No separate debug cache appears in either path. Cosmetic and
origin receivers clear each renderer agent's cache before applying to that
process's frames. A first registered process with no matching frame can have
an empty host cache while a later process has already applied CSS; querying
the first process then yields Success. This is a concrete explanation for
the probe pattern, but identifying which runtime process held each role
requires process/agent identity checkpoints.

The completed native-checkpoint run
`results/renderer-native-checkpoints-run2/trace156/native-checkpoints.retry1.jsonl`
shows compilation finished in14ms with all publication callbacks observed;
`adblock-observations.json` shows initial readiness within708ms and network
blocking/CSS persisting while feature11 was disabled. Those observations
supersede queue-starvation as the explanation for that successful run.
After exclusions-cleared, badfilter, and feature re-enable, debug Success can
reflect the first agent's cleared cache rather than the active page's cache.
Do not replace this with a browser-process evaluator merely to reproduce
those strings: the native destination is a bound renderer Mojo remote.

## Bounded original runtime checkpoints (run2)

`results/renderer-native-checkpoints-run2/trace156` is a complete original-only
probe with exact-PID LLDB observation. The helper pinned pristine image SHA256,
UUID, module path and each original instruction before installing eight bounded
auto-continuing checkpoints. Observation completed with 25 hits; app exited via
Browser.close; debugger exited successfully. No native values or security
settings were changed. The initial helper preflight needed Python 3.9 hashing
and signal-handler compatibility corrections before attachment.

First request gate observed feature11 true at Unix1792750924.649995. Worker
entry followed at .693096, compiler return at .707720, UI completion at .772483,
cosmetic setter at .818953, cosmetic broadcast at .868342 and URL broadcast at
.883816. First debug readiness was 708ms. Three complete compilation/publication
chains were observed, for initial lists, badfilter lists, and clear.

This instrumented run proves the healthy publication chain and genuine native
blocking. It does not establish why the separate uninstrumented combined run
stayed NoBlockList for 60 seconds; debugger attachment can perturb scheduling.
Do not present startup best-effort fencing as a proven cause of that older run.

Native runtime feature toggle evidence is decisive: after setFeature11(false),
script remains blocked, count increments 1->2, cosmetic display stays none,
while browser debug returns Native ad blocker feature disabled. Reenable keeps
blocking and increments count to3. Thus renderer network/cosmetics must not be
gated by browser feature11; the public list request/debug/browser throttle gates
remain. Reenable produced an internal exclusion broadcast at1792750929.412144
within the public enable call interval1792750929.341..493; disable did not.
Explicit domain/host-port exclusions both bypassed filtering; clear exclusions
restored it. badfilter allowed script without resetting count; clearing lists
returned NoBlockList and restored visible cosmetic content.

## Feature11 gates registration/publication, not an installed renderer filter

**New renderer binding while false:** service registration0x4731028 tests
feature11 at0x473104c-0x4731054; false branches directly to0x4731148. This
skips all initial messages: cosmetic0x441f6b4 (0x47310b4), URL0x441f9e0
(0x4731120), and exclusions0x441fb58 (0x4731144). It sends no reset message
on that path. It still registers/owns the remote and assigns a map key.
Existing service regions+16/+64 are not destroyed or modified by registration.
A fresh renderer therefore retains its constructor defaults while false.

**New profile while false:** manager profile-registration0x4297504 saves
feature11 inw21, inserts profile into its registry, then0x429758c branches
to return0x4297600 when false. It skips compiled-pair inheritance0x42977cc
and excluded-origin inheritance0x4297370. Fresh service constructor0x472f3e4
has invalid cosmetic/URL regions (validity fields+24/+72 zero) and an empty
excluded-origin vector at+112.

**Broadcast methods themselves are gated:** cosmetic callback0x4730a08
requires a valid service region+24 and feature11true at0x4730ac0-0x4730ad0;
otherwise sends reset0x441f7a0. URL callback0x4730bb4 setsw20 tofalse when
service+72 is invalid, otherwise loads feature11 at0x4730c40-0x4730c48.
At0x4730d04, false selects reset0x441facc; the browser-local URL agent also
receives an empty region (0x4730df0-0x4730e14). Thus an independently queued
publication reaching its callback after disable can reset agents. This is
different from the feature-toggle observer doing a publication.

**Toggle observer asymmetry:** manager0x42978ac reacts only tofeature11.
Onfalse it destroys only its global compiled pair+24/+32 and clears engagement
flag+40 (0x42978dc-0x42978fc). It then iterates profiles, but flag+40 is nowfalse
so skips compiled setters at0x4297964-0x4297980, and currentfeaturefalse skips
exclusions at0x4297984. It consequently leaves each existing profile service's
shared regions and existing renderer state intact. Onre-enable, global pair is
still absent, so no compiled setters run; retained global exclusions republish
through0x4297990. A renderer newly binding afterre-enable receives retained
regions from its existing profile service, if any; a profile created whilefalse
has no inherited rules until a later successful blocklist publication. These
outcomes follow the branches; they are not a blanket enabled flag inrenderer.

### Re-enable, in-flight completion, and explicitly empty lists

At0x42978ac, enable=true leaves manager's pair untouched and, for each profile,
checks global engagement+40 at0x4297964. If engaged,0x4297980 calls0x42977cc,
which republishes that GLOBAL pair through cosmetic0x472f990 and URL0x472ff20.
Then enable=true calls0x4297370 for exclusions at0x4297990. If pair absent,
there is **no fallback reading a profile's retained regions**: only exclusions
publish. Thus an agent bound whilefalse remains without rules after enabling
if the manager's pair is absent, even if older agents in the same profile kept
old rules. A newly bound agent afterenable separately inherits retained profile
regions through0x4731028, regardless of manager engagement.

An accepted request's compiler can finish after disable. Completion0x42970d0
loads currentfeature into w20 at0x42970e8-0x42970f0, but ALWAYS stores the
compiled pair and sets engagement+40 (0x4297148-0x4297164); false only skips
broadcast at0x4297168. A laterenable therefore republishes that now-engaged
pair to all profiles. Calling SetBlockLists whilealreadyfalse isdifferent:
0x42bd860 rejects the request before queueing it.

An explicit SetBlockLists([]) is **engaged empty**, not absent. There is no
empty-input earlyreturn in0x42bd840. With noinput, compiler's URL accepted
flag remains0 and selects null at0x4731c9c; cosmetic accepted flag starts0 at
0x4731dc0, is set1 only for an accepted rule at0x4731e40, and selects null at
0x4732694-0x473269c. Pair{null,null} is written at0x47326f0. Completion stores
this pair with engagement1 and publishes resets whileenabled. Enabling with
such an engaged empty pair reapplies emptyregions; enabling with absentpair
(afterdisable and no subsequent completion) sends onlyexclusions.

## Bound-agent ordering is assigned during browser RenderProcessWillLaunch

The sole direct caller of RegisterAgent0x4731028 is **0x42ac400** inside
**0x42abee4, ArcContentBrowserClient::RenderProcessWillLaunch**. Its primary
vtable entry is0xda7aba8 (slot16 from address point0xda7ab28), matching the
ContentBrowserClient declaration order including IsDragAllowedByPolicy and
the two destructor slots. It first calls upstream Chrome implementation
**0x2adfdb0** at0x42abf00; that implementation's operations match
ChromeContentBrowserClient::RenderProcessWillLaunch (profile fromhost+416,
WebRtcLoggingController attachment, AudioDebugRecordingsHandler userdata,
RendererUpdater). Local source counterpart is
`chrome/browser/chrome_content_browser_client.cc:1867`.

The Arc body gets the profile's AdBlockerService through0x4296bcc at0x42abf10,
constructs a generic receiver named **arc.mojom.AdBlockerAgent** (literal
0xd37db87, used0x42ac3a0), sends it through browser BindReceiver0x28316f4 at
**0x42ac3c8**, then directly invokes0x4731028 at**0x42ac400**. There is no
renderer-originated registration dispatch, Ping callback, or acknowledgment
between the receiver send and monotonic sequence assignment0x4731164.
The service registry entry is installed synchronously in that browser call.

The local content source calls RenderProcessWillLaunch from RPH initialization
(`content/browser/renderer_host/render_process_host_impl.cc:2018`); API comment
explicitly says it notifies before the render process is created. Therefore
when A's browser bind attempt precedes B's, A receives the earlier native key
even if B becomes responsive or acknowledges a probe before A. A replacement
that assigns registry ordering only on Ping acknowledgment can reverse native
debug-agent selection. Liveness checks may separately gate use, but an
acknowledgment is not the native ordering event.
