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
`0x4733d34-0x4733d4c` checks an optional origin at request+1840/flag+1920,
now proven to carry Blink `ResourceRequestHead::TopFrameOrigin()` independently
of trusted IsolationInfo (see [dedicated provenance audit](renderer_extra_origin_evidence.md)).
The original network member's literal C++ spelling remains stripped.
`0x4733d80-0x4733e38` checks
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

## Native network throttle: redirects, workers, synchronization, and count path

The full native throttle vtable is **0xdd27ff0**. Slots0/1 are destructors
0x4733b18/0x4733b1c; slot2 DetachFromCurrentSequence is no-op0xfbc0; slot3
WillStartRequest is0x4733b34; slot4 logging-name returnsnull0x614600;
slots5/6 WillRedirectRequest/WillProcessResponse are no-op0xfbc0; slot7
response logging-name returnsnull; slots8/9/10 BeforeWillProcessResponse,
BeforeWillRedirectRequest, WillOnCompleteWithError are all no-op0xfbc0.
This order matches `third_party/blink/public/common/loader/url_loader_throttle.h`.
Therefore native Arc evaluates initial request admission only; this throttle
adds no redirect re-evaluation or response/completion cancellation behavior.
At0x4733b58 it explicitly sets *defer=false. On DISALLOW it calls
Delegate::CancelWithError(**-90**, empty string_view),0x4733fb8-0x4733fd8.
Empty reason literal0xd0061f4 and length0 are passed. Stock Chromium156 names
BLOCKED_BY_CLIENT as-20 (`net/base/net_error_list.h:91`); -90 is Arc's observed
custom numeric error and cannot be replaced with-20 if count parity depends
on it.

**Provider Clone:** native wrapper vtable0xda82e18 has Clone0x43ab5fc,
CreateThrottles0x43ab41c, SetOnline0x43ab684. Clone calls underlyingprovider
Clone at0x43ab610-0x43ab61c, allocates another24-byte wrapper, preserves the
SAME agent/provider-interface pointer (oldwrapper+8 -> newwrapper+8) and stores
cloned underlyingprovider at+16 (0x43ab638-0x43ab65c). SetOnline delegates to
underlyingprovider. No new agent or renderer main-thread call occurs in Clone.
The source provider API explicitly allows CreateThrottles on background fetch,
dedicated/shared/service-worker threads, and Clone on a parent worker for nested
workers (`third_party/blink/public/platform/url_loader_throttle_provider.h:30-84`).

**State crossing threads:** each native throttle constructor0x4733814 retains
shared RulesetDealer from provider-interface virtual+24 (0x4733850-0x4733868),
then acquires a ref to the excluded-origin snapshot from virtual+16
(0x4733888-0x473389c). Renderer secondaryinterface addresspoint0xda828f8
maps those getters to0x43a6988 (dealer) and0x43a68e8 (origins). The dealer
getter uses atomic refcount; excluded getter locks physical full-agent+312,
refs originvector at physical+376, unlocks, returns. In secondaryinterface
coordinates(agent+16), these are+296/+360. Directgetter0x43a6878 uses the
same physical+312/+376. Throttle owns these refs, avoiding main-thread-only
maps or RenderFrame lookups on worker request admission.

The ruleset is NOT frozen as a raw matcher pointer in the constructor.
WillStartRequest loads retained dealer from throttle+16, locks **dealer+16**
via0x35a37f8 at0x4733f38-0x4733f40, reads currentmatcher atdealer+8, performs
both activation queries and GetLoadPolicyForResourceLoad under thatlock, and
unlocks via0x35a3764 at0x4733fdc-0x4733fe0. Renderer rule-update0x43a6d3c
locks the SAME dealer+16 before replacing matcher atdealer+8. Thus native has
refcounted shared dealer lifetime plus serialized matcher access, and an
immutable excluded snapshot per throttle. It does not reach a raw process-state
singleton from worker threads. Current IndexedRulesetMatcher API is const for
queries, but constituent UrlPatternIndexMatcher has mutable lazy rules_count_;
a blanket unverified thread-safety claim should be avoided even for const APIs.
FirstPartyOrigin also has mutable per-instance host cache and should remain
per-request (`components/subresource_filter/core/common/first_party_origin.h`).

**Blocked count:** native0x438f2d0 is WebContentsObserver::ResourceLoadComplete
(the matching source API is `content/public/browser/web_contents_observer.h:539`).
It reads resource_load_info.net_error atargumentx4+440, increments by1 only
forexact-90, and ignores render_frame_host, request_id, original_url, and
resource destination. There is no observer-level mainframe or prefetch test.
Count is not incremented directly by renderer cancellation callback, so requests
that never reach this WebContents completion path do not gain a count merely
because a throttle rejected them. Source flow is RenderFrameHostImpl::
ResourceLoadComplete (`render_frame_host_impl.cc:11543`) -> delegate ->
WebContentsImpl::ResourceLoadComplete (`web_contents_impl.cc:8939`) -> observers.
Main-resource callbacks can be deferred until commit to identify request ID;
worker/prefetch attribution must follow actual Chromium completion routing,
not a synthetic direct counter call. PrimaryPageChanged reset/equality guard
remain as documented above.

**Historical correction: extra-origin provenance is now recovered.** Native
ResourceRequest has an independent final std::optional<url::Origin> at +1840,
engaged byte +1920. Constructor `0x12da3d4..0x12da3d8` zeros it; copy assignment
`0x15327dc..0x153281c` copies origin and engagement. The earlier description of
`0x1f84c50..0x1f84ca0` as a "native Mojo Read" was wrong. That range belongs to
in-process `blink::PopulateResourceRequest` (`0x1f83a94`), copying Blink request
head `TopFrameOrigin()` at +208 into this network optional. Independent getter
`0x419a380`, setter `0x419a388`, and FrameFetchContext producer correspondence
confirm it. See [renderer_extra_origin_evidence.md](renderer_extra_origin_evidence.md)
for the full independent proof. The literal stripped network member name and
Mojo field spelling remain unrecovered; its producer semantics are established.
Current stock network ResourceRequest lacks this added member. It remains
distinct from ordinary isolated_world_origin, SiteForCookies, and separately
checked trusted_params.isolation_info.top_frame_origin.

## Exact native net error -90 label

Native **ErrorToShortString0x21bfac0** maps **-90 to
ERR_BLOCKED_BY_CONTENT_BLOCKER**, not ERR_BLOCKED_BY_CLIENT. This is established
by the numeric dispatch, not the presence of a nearby string alone:

At0x21bfaf4, `index = error + 508`;0x21bfb08-0x21bfb1c dispatches a
uint16 table at**0xcaa98f8** relative to codebase**0x21bfb20**, scaling entriesby4.
Forerror-90,index418, entry**0xcaa9c3c** contains**0x018e**, yielding target
**0x21c0158**. That block loads literal**0xd3e1d27**,
`BLOCKED_BY_CONTENT_BLOCKER`, at0x21c0158-0x21c015c, then branches commonprefix
assembly0x21bfb84 (`ERR_`, literal0xd3e328f). As a cross-check,error-20,index488,
entry0xcaa9cc8=0x0266 yields0x21c04b8 and literal0xd3e1bc2 BLOCKED_BY_CLIENT.
Therefore a minimal shared net-error addition can faithfully name
`NET_ERROR(BLOCKED_BY_CONTENT_BLOCKER, -90)`; the original arbitrarynumeric
cancellation and count discriminator already agree with this exactlabel.

## Color-scheme public NSInteger is decoded from its low 32 bits

Fresh bounded native inspection establishes a correction to the broad statement
that every public NSInteger value other than 0/1 traps: both backends inspect
only the argument's low 32 bits. The Objective-C wrappers preserve all 64 bits,
so this is backend truncation, not a narrower Objective-C ABI encoding.

Direct `applyColorScheme:` wrapper starts at `0x4412d9c`: `0x4412dac` saves
full argument x2 into x19; `0x4412dcc` copies x19 to x1 and `0x4412dd0` calls
backend `0x43794f0`. There, `0x4379500` saves full x1, but the discriminator is
`cbz w19` at `0x4379534` followed by `cmp w19, #1` at `0x4379538`. The former
selects public Light/native internal 1; the latter selects public Dark/native
internal 0. Other **low-word** values reach the trap branch at `0x437953c`.

Profile `registerColorScheme:forHostPattern:` wrapper at `0x4436cb4` likewise
saves full x2 into x20 (`0x4436ccc`), forwards x20 to x1 (`0x4436d0c`) and calls
`0x42e3140` (`0x4436d10`). That backend saves full x1 into x20 (`0x42e3168`)
but tests `cbz w20` at `0x42e3254` and `cmp w20, #1` at `0x42e3258` before
writing internal enum/presence at host config +80/+84.

Consequently `0x100000000` and `-0x100000000` alias Light, while
`0x100000001` aliases Dark; these conclusions follow directly from AArch64
W-register semantics. No execution of invalid values was attempted. A faithful
checked helper must switch on `static_cast<uint32_t>(value)` and trap only when
that low word is neither 0 nor 1. A full-int64 switch incorrectly rejects these
native aliases. Ordinary public enum 0/1 mappings are unchanged.

Reproduction ranges: `0x4412d9c:0x3c`, `0x4379500:0x50`,
`0x4436cb4:0x60`, `0x42e3140:0x30`, `0x42e324c:0x20`.
The preserved instruction log is
`logs/wp/renderer-effects-proposal/native-scheme-lowword-evidence.log`.

## Exact blocked-count equality guard and independent wrapper synchronization

The native counter setter0x438eb84 starts with:

```
438eba4 ldr w8, [x0, #1484]   // current native count
438eba8 cmp w8, w1            // requested count
438ebac b.eq 438ee18          // direct epilogue
438ebb0 str w1, [x0, #1484]   // reached only on change
```

Target0x438ee18 restores registers and returns at0x438ee34. Thus equalvalue
suppresses both storage and ALL following observer notification machinery.
PrimaryPageChanged thunk0x438f2fc subtracts its observer-subobject offset168,
setsw1=0, and tailcallssetter0x438eb84 at0x438f304 unconditionally; alreadyzero
causes the guard toreturn without a count-change notification. It does not
produce an additional ObjCsetBlockedResourceCount through that change path.

The separate bulk wrapper synchronization code does unconditionally copy the
native count when it reaches that property:0x440edbc loadsnative+1484 into w2,
0x440edc4 loads selectorref0xe290df8 -> literal0xd75de54
`setBlockedResourceCount:`, and0x440edcc callsobjc_msgSend. This permits other
initialization/full-sync flows to dispatch ObjC setters independently. The
native counter equalityguard proves suppression for a redundant counterreset;
it does not prove that every other full-sync callback is absent during a page
change or unrelated DevTools/download event.

## Native TopBandColorTracker: lifecycle, candidates, and emitted state

These are read-only findings from the pristine ArcCore image. They describe the
custom Blink producer separately from the browser surface-sampling fallback in
`RENDERER_VISUAL_GAPS.md`. In particular, a periodic view screenshot is not an
equivalent implementation of this producer.

### Ownership and scheduling

`LocalFrame` constructor `0x2716a30` creates the tracker at `0x2717f50`, storing
the compressed managed pointer in frame +396. The creation path requires a main
frame (`0x2717e88..0x2717ea4`, FrameTree parent null) and runtime byte
`0xe369ced & 1` (`0x2717ea8..0x2717eb0`). Its name and default are now proven
by the parallel runtime-feature tables below. The GC descriptor at `0xe25c8c0` names
`arc::TopBandColorTracker`; frame trace `0x2a943f0..0x2a94418` strongly traces
the tracker. Tracker trace `0x46848f0` traces its frame and selected style, and
the embedded timer's weak receiver.

`LocalFrameView::RunPostLifecycleSteps` (`0x1afbbc0`) calls tracker
`FireDetection` (`0x46849d0`) at `0x1afc5fc`. The embedded timer is constructed
at tracker +8 using frame task type 23 (`kInternalDefault`). At
`0x4684a38..0x4684a54`, its one-shot delay is exactly 100,000 microseconds.
This is a retry cadence, not a direct periodic screenshot callback: timer target
`ScheduleDetection` (`0x46847c0`) tests tracker byte +135, re-fetches the frame's
current view at frame +584, and calls `LocalFrameView::ScheduleAnimation`
(`0xb44440`) with reason 0, zero delay, and urgent=false. It does not detect or
emit directly. Its source location is `ScheduleDetection`, custom file
`../../arc/blink/core/top_band_color_tracker.cc`, line 252; timer start records
`FireDetection`, line 225. The finalizer (`0x45f6634`) destroys the timer through
`0x871728`, which cancels its pending task.

`FireDetection` coalesces detections: when the timer is inactive it starts that
100ms timer, clears +135, and attempts detection. While the timer is active it
marks +135=true and ordinarily returns; an active pending presentation candidate
(+192) permits an immediate recheck (`0x4684b3c..0x4684c04`). The timer's later
animation request therefore ensures another post-lifecycle pass if work was
coalesced. It is not a permanent repeating timer.

There are additional exact triggers:

- `Document::OnLargestContentfulPaintUpdated` (`0x23d6c9c`) calls
  `0x46876a0` at `0x23d6cdc`. It sets tracker +133=true, clears the selected
  node/style/time cache (+260/+264/+272 and +300), then requests an animation.
  Its sole direct caller `0x23d698c` belongs to
  `WindowPerformance::OnLargestContentfulPaintUpdated` (`0x23d6818`), matching
  upstream LCP object construction, observer notification, Document callback,
  and subsequent image-element handling. Thus +133 records that LCP was seen.
- `LocalFrameView::MaybeStopDeferringCommitsWithoutContentfulPaint`
  (`0x18f4550`) sets tracker +136=true once at `0x18f4600` and requests an
  animation. This custom block requires a main frame, finished parsing (native
  parsing state 2), and an existing first-paint timestamp. Its direct callers
  are `PaintTiming::SetFirstPaint` (call `0x1284838`) and
  `Document::FinishedParsing` (call `0x18f3808`), covering either event order.
  It precedes upstream feature gating and FCP checks. It is parsing-plus-first-
  paint readiness, not an FCP or LCP marker.

Source correspondence is visible in current Blink `window_performance.cc:1748`,
`document.cc:8022` and `:8096`, `paint_timing.cc:681`, and
`local_frame_view.cc:5264`. These source names are recovered by native control
flow correspondence, rather than exported native symbols.

### Candidate analysis that is directly established

Detection body `0x4684c40` obtains the frame's Document/LayoutView, a positive
reference width W, and a root background color. **W is now source-mapped:**
`0xa9d440` is `PaintLayerScrollableArea::VisibleContentRect` and argument0 is
`kExcludeScrollbars`. It returns gfx::Rect with x0=packed x/y and x1=packed
width/height; W is its low32 width. Native body inlines LayoutContentRect:
LayoutBox size minus borders and conditional scrollbars, clamp negative sizes,
floor scroll position, and round size. The producer's final chain is
LayoutView -> FragmentDataList(+52) -> RareData(+8) -> PaintLayer(+0) ->
PaintLayerScrollableArea(+40), matching LayoutBoxModelObject::GetScrollableArea.
Thus W is layout viewport visible-content width excluding scrollbars, rather
than widget/screen width. Source: `paint_layer_scrollable_area.cc:677..701`,
`layout_box_model_object.cc:372`, `fragment_data.h:206..240`, `paint_layer.h:385`,
and enum `scrollable_area.h:88`.

Feature default proof: native `ArcMacCoalescedResizing` base::Feature object is
`0xe304a40`, whose decoded name pointer is `0xd41f79f`; raw +8 default-state
word at `0xe304a48` is **0, disabled by default**. Native FeatureList evaluator
`0x14cc0` loads feature +8 at `0x14d2c`, compares with1 at `0x14d38`, and returns
equality at `0x14d3c`. This independently establishes the enum meaning rather
than borrowing a guessed default. FeatureList command-line/field-trial overrides
may enable it at runtime; pristine storage alone does not establish a launched
app's effective state. Current `arc/api/arc_app_flags.mm:16` declares this feature
enabled by default, a contradiction reported to the lead/app owner for review;
this audit made no production edit.

The color property object is
exactly `background-color`: producer uses `0xe12e7c0`; its vtable
`0xdc04278` points to name function `0xaa4ccd0`, which returns string
`0xd2a2415`, `background-color`. Root color is accepted only with alpha >=1
(`0x4684e30..0x4684e50`). Default color bytes at `0xcdb88e8` are a transparent
Blink color; they must not be replaced by an invented white default.

The detection loop performs four samples, i=0..3. Coordinates constructed at
`0x46850c8..0x46850ec` are x=i*floor(W/4), y=1 CSS pixel, encoded as 1/64px
LayoutUnits. There is scroll-dependent point adjustment at
`0x46850f0..0x4685220`; both hit-test construction (`0xcdff50`) and execution
(`0xce07c0`) are visible. It walks the returned hit-object list and element
ancestors, rather than reading a single root CSS color.

Both hit tests use flags7206 (`0x1c26`): ReadOnly, Active, IgnoreClipping,
IgnorePointerEventsNone, ListBased, and PenetratingList. They do not set
AllowChildFrameContent. `0xce07c0` matches LayoutView::HitTest(location,result),
and `0xa79cc00` is the lazy ListBasedTestResult() accessor. IgnoreClipping is
material because translated document-top coordinates can lie outside the
currently visible scrolled area.

**Scroll conversion is now source-mapped:** `0xa506808` is
`LocalFrameView::DocumentToFrame(PhysicalOffset)`, subtracting the layout
viewport's scroll offset after rounding to LayoutUnits; missing viewport
returns the point unchanged (`local_frame_view.cc:3839..3847`). Native sibling
`0xa50691c` performs the inverse add, matching FrameToDocument. This is not
ViewportToFrame, which instead traverses VisualViewport/root-frame mapping.

Native stack+128 records vertical scroll offset >0. If false, all four points
use DocumentToFrame. If true, a preliminary raw-point hit test walks candidate
LayoutObject and containing blocks (helper `0x52a1e8` is
LayoutObject::ContainingBlock(nullptr)). A nonzero sticky position offset
(`0x2b4180`, LayoutBoxModelObject::StickyPositionOffset) or fixed-position object
selects raw points for the whole pass; otherwise the whole pass converts through
DocumentToFrame. Fixed is exact: ComputedStyle Data position bits54..56 equal
enum1, EPosition::kFixed. Sticky positioned-state alone does not suffice: its
offset must be nonzero. This corrects earlier fixed-only/Node-walk descriptions.

Candidate helper `0x4686a2c` requires an element with both style and layout
object, visible visibility, display other than none, and positive opacity. The
style field mapping is supported by exact generated `ComputedStyleBase::Data`
bit correspondence: Data begins at native +40, display occupies bits26..31
and is rejected for enum22 (`EDisplay::kNone`), visibility occupies bits141..142
and is required to equal enum1 (`EVisibility::kVisible`). The generated
`computed_style_base_constants.h` and `computed_style_base.h` preserve those
enum values/bit positions. Opacity is the float at StyleSVGData +60, reached
through style +24. The rectangle
helper `0x119c340` gathers absolute quads and returns their enclosing integer
rectangle. Style and size checks require height >=11 and width >=floor(W/3).
After those prerequisites, **left<1 OR right>=W admits immediately**. Strictly
interior candidates require `ArcMacCoalescedResizing` enabled (feature object
`0xe304a40`, name `0xd41f79f`) and all three quarter points in the horizontal
half-open interval `[left,right)`. The feature is disabled by native default.

This corrects the prior inverted interpretation, which confused adjacent return
labels. Fresh raw-bit decoding of word `0x54fffd6b` at `0x4686b08` gives signed
imm19=-21, displacement=-84, target **`0x4686ab4`**, which returns the original
element pointer. Word `0x34fffcc0` at `0x4686b18` gives signed imm19=-26,
displacement=-104, target **`0x4686ab0`**, which zeroes the pointer before the
common return. At `0x4686b04`, `ccmp left,#1,#8,lt` compares left against1 only
when right<W; otherwise fallback NZCV8 has N=1/V=0 and forces LT=true. Thus its
following B.LT **admits**, while feature-false CBZ **rejects** the strict interior.
The enabled path counts all three quarter points (`0x4686b1c..0x4686b50`) and
preserves the pointer only when count>2. Caller CBZ at `0x4685484` advances to
the next hit (`0x4685438`), and preliminary CBZ at `0x46851c0` similarly advances
to `0x468517c`; nonzero is admission in both callers. Raw proof is in
`logs/wp/renderer-topband-next-controls/native-helper-and-callers.log` and the
fresh annotated `logs/wp/renderer-top-band-proposal/geometry-native-annotated.log`.
The previous proposal88 and its inverted fixtures are historical HELD inputs;
compilation of those fixtures did not validate the predicate. The rectangle helper matches
`LayoutObject::AbsoluteBoundingBoxRect(MapCoordinatesFlags)` (current source
`layout_object.cc:2108`), using enclosing bounds of absolute quads.

The local hash map has 48-byte records keyed by a compressed node pointer,
with Blink Color at +8, sample count at +28, browser-sampling flag at +32, and
sampling token at +40 (`0x4686cb8`). Successful node samples increment that
record's count (`0x4685094..0x46850a0`, `0x46864a0..0x46864a8`). A separate
root-background sample count lives at stack +372. Final choice scans records
for the greatest positive count (`0x4685b70..0x4685bb8`). The winning node's
color/request and token are selected at `0x4685908..0x4685950`; an opaque root
background overrides that winner only when its count is strictly greater
(`0x468597c..0x46859b0`). Equal counts retain the node winner. Hash iteration
order breaks ties between equal node counts, because selection uses strict >.

If no color or sampling candidate is selected, `0x4685a5c` calls
`LocalFrameView::DocumentBackgroundColor` (`0x1288750`). Native control flow
matches the current source at `local_frame_view.cc:2054`: base background,
fullscreen/backdrop source, forced-color handling, then `Color::Blend`.
With no pending candidate, its opaque result is withheld while readiness +136
is false (`0x4685a84..0x4685a90`). Once readiness is true, that fallback
sets optional color presence and ChosenState +40=true
(`0x4685a94..0x4685a9c -> 0x46859c0`), causing presentation deferral.
Nonopaque DocumentBackgroundColor yields an absent chosen color. Pending-state
branches additionally cancel/reconcile earlier candidates; they must remain
separate from this no-pending readiness path.

This producer also examines image content. In addition to CSS background-color
composition (`0x1288a1c`), paths draw a style image into a bitmap, cap bitmap
dimensions at 800 (`0x4685d70..0x4685d80`), and invoke shared bitmap analyzer
`0x40c05c0(bitmap, 8, 24)` at `0x4685fd0`. Another image path invokes the same
analyzer at `0x4686644`. These are image-specific analysis paths; they do not
establish a policy of screenshotting the entire rendered view. The replaced-
image type tag33 is source-mapped: the same native tag comparison at
`0x23d6994..0x23d69a0` inside verified
`WindowPerformance::OnLargestContentfulPaintUpdated` corresponds exactly to
upstream `DynamicTo<HTMLImageElement>(element)`, followed by LCP-image and lazy-
loading handling. It is HTMLImageElement, not HTMLCanvasElement. Exact drawing
and source-type decisions are closed in the dedicated image section below.

Independent native type metadata makes the replaced-element cases exact:

| Native type byte | Class | Vtable | Wrapper getter | Descriptor |
| --- | --- | --- | --- | --- |
| 10 | HTMLCanvasElement | `0xdb7e118` | `0x4602388` | `0xe153c50` |
| 33 | HTMLImageElement | `0xdb8c310` | `0x460cbe8` | `0xe154df0` |
| 89 | HTMLVideoElement | `0xdb9cf70` | `0x4610ca0` | `0xe1584c0` |

In each case getter is vtable +24, descriptor +24 points to the exact class-name
literal, and byte at vtable -17 supplies the listed type. Producer explicitly
skips Canvas and Video at `0x4685494..0x46854a0` (and in its earlier list pass at
`0x4685368..0x4685370`). HTMLImageElement trace `0x175acd4` first traces element
+136 at `0x175acec`, matching `image_loader_` in current `html_image_element.cc`.
Thus element +136 -> loader +12 is its CachedImage()/GetContent() path, not
canvas raster extraction. The direct image-bitmap helper `0xab198c0(image, 1)`
strongly matches `Image::AsSkBitmapForCurrentFrame(kRespectImageOrientation)`;
current `image.cc:396` obtains PaintImageForCurrentFrame, handles bitmap-image
orientation/scaling, obtains a software SkImage, and returns its legacy bitmap.

LCP is also an admission gate for complex candidates. At
`0x46855a4..0x46855bc`, partial background alpha (strictly between 0 and 1),
opacity below1, or the image-path flag takes the complex path. If LCP-seen +133
is false, `0x46855c8 -> 0x46858e4` exits detection through cleanup at
`0x4685a1c`, reconciling a pending presentation-background state when necessary.
It does not request a new browser surface sample on that path. LCP notification
later clears the candidate cache and schedules another detection as above.

The YouTube exception is a class-token test, not a tag-name test. At
`0x46854bc..0x4685504`, candidate Element +72 is ElementData, whose +12 holds
SpaceSplitString ClassNames (matching current `element_data.h:145`); it scans
the token vector for atomic string `ytd-masthead` (`0xd5d3290`). If found,
`0x4685874..0x468587c` obtains the Document URL's host. It compares exact
`youtube.com` (`0xd034ab4`) via StringView equality `0x24a140`, then suffix
`.youtube.com` (`0xd030a2a`) via `0x1bb9300`. Either match branches back to
`0x4685438`, skipping this candidate. Non-YouTube hosts or candidates without
that class continue through ordinary background analysis.

### Closed image admission, rasterization, and capture policy

FillLayer traversal and image-kind bits are source-mapped. StyleBackgroundData
begins with FillLayer; its cached HasImage bit is native +52 bit14, computed by
`0x1a1040` = FillLayer::ComputeCachedProperties. FillLayer +0 is Next wrapper,
+4 StyleImage; StyleImage +8 bit0 is IsImageResource, bit2 IsGeneratedImage.
Pending/image-set/crossfade kinds without either of those bits are not admitted
by this image test. Background-repeat X/Y are words +40/+44. The +48 bits23..24
read at `0x468581c` are **BackgroundYOrigin**, not background-size; independent
native CSS background-position conversion uses the same field/mask at
`0xa2a3714..0xa2a3724`. Current FillLayer field order and BackgroundEdgeOrigin
enum agree (Top0, Right1, Bottom2, Left3).

Background-image significance test (`0x46857dc..0x468584c`) walks image layers:

- For NoRepeat Y, Bottom origin is ignored, as is Top origin with PositionY
  exactly Percent100. Other admitted positions make the background significant.
- For repeating Y, generated images are significant. Fetched images ordinarily
  are too, except NoRepeat X additionally checks the intrinsic-width threshold.
  A fetched image with intrinsic width times native float device-pixel ratio
  below half the LayoutBox physical content-box width is ignored; otherwise it
  is significant. Missing relevant box/resource falls back to significant.

Threshold APIs are grounded: `0xb637f0` is
ImageResourceContent::IntrinsicSize(kRespectImageOrientation), and `0x404240` is
LayoutBox::PhysicalContractedBoxRect(kContractToContentEdge, style,
PhysicalFragments(), StitchedSize()), i.e. PhysicalContentBoxRect. Native takes
its width from low32(x1), converted from LayoutUnits. The float ratio at
`0x4685764..0x468576c` is Page +1044 times LocalFrame +616. Independent native
V8 devicePixelRatio getter `0x3d8b6f4`, value path `0x3d8b82c..0x3d8b848`,
multiplies these same two fields as doubles. Native setter
`0xad3a058..0xad3a088` source-maps Page +1044 to InspectorDeviceScaleFactorOverride;
SetZoomFactors store `0x16d31ac` source-maps frame +616 to LayoutZoomFactor.
The producer deliberately multiplies as floats instead.

Admitted fetched images and HTMLImageElement CachedImage paths require content
status >1, then GetImage() (null/errors become NullImage) and
AsSkBitmapForCurrentFrame(kRespectImageOrientation). These bitmaps are **not**
clamped to800. Generated images alone allocate a target bitmap whose width and
height are min(candidate absolute bounding-box dimension,800), checked positive
and converted to target_size floats (`0x4685d70..0x4685da8`). Native image-info
constant `0xd734ad0` is null color-space, BGRA8888(6), premultiplied alpha(2),
with target width/height; rowbytes is checked width*4.

Generated raster path (`0x4686050..0x46861a0`) allocates SkBitmap pixels using
`0x1265cfc` (tryAllocPixels), clears transparent via `0x1d79700`, obtains
StyleImage::GetImage(layout_object, element, computed_style, target_size) at
virtual +112, constructs cc::SkiaPaintCanvas over the bitmap (`0x12742e8`), then
calls Image::Draw at virtual +256. Source and destination are both
RectF(0,0,target_width,target_height). Native cc::PaintFlags initialization is
the default opaque-black color, width0, miter4, SrcOver, no filters, default HDR
headroom -1; ImageDrawOptions are default orientation-respecting, clamp-source,
synchronous-decode, no dark-mode filter. The raw constant at `0xcae96e8` is
black with alpha1 and the bitfield/default writes are at `0x4686128..0x4686178`.

The common bitmap analyzer is called on each fetched/generated layer in order.
A successful opaque result is accepted immediately; a nonuniform/empty layer
allows trying the next layer, whereas a returned translucent color exits to the
capture-admission path. This code does **not** paint all CSS background layers
into one composite before analyzing them. HTMLImageElement has a single cached
image path with the same opaque-result requirement. Accepted analyzed colors
are cached at tracker +280..300 with the node/style/time keys described below.

Capture suppression is independent of excluding Canvas/Video from color
candidates. Before color traversal, native scans each hit element and its
ParentOrShadowHostElement chain (`0x1a6d4`, matching node.cc:2108). A Canvas or
Video with visible style, display!=none, opacity>0, width>=floor(W/3), and
height>=11 sets local screenshotAllowed=false (`0x468529c..0x468530c` ->
`0x46864b0`). This prepass does not apply the color helper's edge/interior
coalescing admission tests.

When bitmap analysis fails: if screenshotAllowed is false, skip that complex
candidate. Otherwise issue a surface-sampling candidate if unscrolled, or if
the existing sampling token still matches the recent node/style cache. When
vertically scrolled without compatible recent cache, native instead sets the
pass's reuse_cached_color flag (`0x4685710..0x4685714`) and continues examining
candidates. That flag becomes ChosenState +41. Thus it preserves an earlier
browser color instead of automatically sampling the current view for a
document-top coordinate. Compatible recent analyzed-color cache (+300) can
bypass bitmap analysis entirely at `0x46855e0..0x46855f4 -> 0x468642c`.

The resize-feature view-transition guard is also closed: `0x1b09738` is
ViewTransitionUtils::ForEachTransition(document), inlining Supplement's snapshot
of document transition and element-scoped transitions. Callback `0x46879d4`
tests state +24 and considers every state except16/17/18 active. These are
exactly Finished/Aborted/TimedOut in current ViewTransition::State; equivalent
to !transition.IsDone(). With ArcMacCoalescedResizing enabled, any active
transition sets retry +135 and returns without selecting/emitting
(`0x4684d64..0x4684da4`). Source: view_transition_utils.cc:212,
view_transition_supplement.cc:485, view_transition.h:370..400.

### Sampling token correction and recent-candidate cache

**This supersedes earlier "frame token" wording for ChosenState +32.** The
64-bit field is proven to be an opaque sampling/cache token; it is not proven
to be a Blink LocalFrameToken or any frame identity. An image-candidate path
records the node identifier (helper `0x480500`), current style, and current
monotonic time in tracker +260/+264/+272. Helper `0x4686b58` recognizes matching
node/style within 1,500,000 microseconds. For a non-IMG whose ComputedStyle
pointer differs from the saved pointer, native compares BackgroundLayers()
(FillLayer equality `0x47acc0`), Filter() and BackdropFilter() (FilterOperations
equality `0x19a19bc`), then Opacity() exactly. It does not require equality of
all ComputedStyle fields or layout dimensions. Current generated StyleMiscData
+8 reaches StyleMisc2Data's first FilterOperations field; +0 reaches
StyleMisc1Data's first BackdropFilter field, matching native +4 -> +8/+0 chains.
The native type-tag33 path returns
compatible after node/time equality, without the further style comparisons
(`0x4686bc8..0x4686c04`); type33 is HTMLImageElement as established above.
If that cached candidate remains
compatible, detection reuses tracker counter +304; otherwise it increments that
counter at `0x4686414..0x468641c`. It writes the counter into candidate +40 at
`0x4686424`, which becomes ChosenState +32 at `0x4685948`. This token lets the
browser reuse the result of an earlier surface sample for the same candidate.

### ChosenState, presentation wait, equality, and crossfade

The native ChosenState layout recovered from emitter `0x46874c0` is:

| Offset | Meaning |
| --- | --- |
| 0..19 | Full Blink Color |
| 20 | Optional color presence |
| 24 | Request browser surface sampling |
| 32 | Opaque sampling/cache token, as corrected above |
| 40 | DocumentBackgroundColor fallback waits for presentation |
| 41 | Reuse browser cached optional color if available |

State selection helper `0x4687028` keeps a pending state at tracker +200,
presence +248, active callback +192, and generation +256. An exact duplicate
of the active pending candidate returns immediately. A changed pending
candidate invalidates the old one and increments generation. A candidate that
differs from the emitted state is deferred only when candidate +24 or +40 is
true and the frame has a Page (`0x4687180..0x46871ac`). It stores the pending
state, captures an incremented generation, and invokes ChromeClient virtual
+1048 (`0x46872cc..0x46872dc`). This signature and forwarding strongly match
`ChromeClient::NotifyPresentationTime(LocalFrame&, ReportTimeCallback)`;
the native virtual's name is not independently symbol-verified. Callback
`0x468743c` ignores a stale generation, otherwise consumes the pending state
and emits. Its presentation-details argument is ignored. Other candidates emit
immediately (`0x4687310..0x4687324`). There is no numeric delay/threshold in this
presentation-wait helper; the 100ms scheduling above is separate.

Tracker byte +132 is now fully identified: engagement of the optional saved
last-emitted ChosenState at +144..185. Constructor `0x46848d4` zeros it;
changed-state emitter `0x4687520` sets it true; stabilizer `0x468713c` reads it
only to gate equality against that saved state. It is not an additional runtime
feature flag. Observed mode writes in the complete component are initial0,
prerequisite-failure3 (`0x4684b14`, `0x4684f1c`), and submitted-choice1
(`0x46859c4`); no mode2 or reset-to0 write was found. Preserve these numeric
states without inventing a source enum name.

ChosenState +40 is also identified: the DocumentBackgroundColor fallback's
wait-for-presentation flag. Normal selected color/surface-sampling paths set it
false at `0x46859bc`; the opaque, readiness-approved background path alone sets
it true at `0x4685a94`. In that fallback tail: nonopaque background submits nil;
opaque background with no active pending callback and readiness false returns
unchanged (`0x4685a90 -> 0x4685a1c`). If an active pending surface-sampling
request exists, it is canceled, generation advances, retry +135 becomes true,
and the function returns. Otherwise readiness permits submitting the background;
without readiness an active pending state is invalidated. This preserves the
native difference between an absent-color emission and no emission.

Emitter equality (`0x46874dc..0x468759c`, `0x4687658..0x4687698`) compares
optional presence, full Blink Color equality (color space, four missing-channel
bits, and four float components), and fields +24/+32/+40/+41. It does not compare
only quantized ARGB. Changed states are saved, then crossfade is computed:

```cpp
bool crossfade = false;
if (tracker.byte133 /* LCP has been seen */) {
  crossfade = tracker.mode128 == 3 ? false : tracker.byte134;
  tracker.byte134 = true;
}
```

Consequently the first changed emission after byte134 is clear never crossfades.
Mode3 suppresses crossfade even after that first emission. Mode1 is assigned
when a color or surface-sampling candidate is chosen (`0x46859bc..0x46859c4`);
mode3 is assigned when the document/layout prerequisites fail and an empty state
is submitted (`0x4684f0c..0x4684f40`). These observed assignments do not recover
the observed numeric state transitions described above. The event meanings of
byte133/136 and the saved-state engagement +132 are recovered above.

Present colors convert using `0xfa1340` (matching Color::toSkColor4f), then
clamp/round into ARGB32 (`0x46875c4..0x468760c`); absent color remains absent.
Mojo serializer `0x418c740` sends message `0x597900bf` with payload size32:
nullable uint32-color structure at +8, flags at +16 (bit0 surface sample, bit1
reuse cached color, bit2 crossfade), token at +24. Browser dispatcher
`0x1e3ad9c..0x1e3ae18` preserves these arguments through `0x4150da4` into
`0x40c0094`. Browser optional-ARGB suppression at `0x40c0124` is independent of
the renderer's richer Blink Color equality.

Lifecycle hooks, width, scroll conversion, bitmap kind/admission/rasterization,
observed numeric mode transitions, state transport and cache rules are now
source-mapped above. No whole-view screenshot or root-color-only substitute
preserves this producer.

### Tracker runtime gate: exact name, default, and bounded relay limit

The creation byte is exactly RuntimeEnabledFeatures::TopBandColorSamplingEnabled:

- Native base feature descriptor `0xe2c32b8` points to string `0xd0eb641`,
  `TopBandColorSampling` (a suffix of the coalesced `kTopBandColorSampling`
  literal). Its +8 default-state word is0, disabled by default.
- UpdateStatusFromBaseFeatures (`0xddbfc0`) uses parallel base-feature table
  `0xe13e870`, uint16 flag-index table `0xcdcd83c`, and policy table
  `0xcdce084`. Feature entry934 at `0xe1405a0` points to that descriptor.
- Flag index entry934 at `0xcdcdf88` is669. Native feature_states_ base is
  `0xe369a50`; base+669 is exactly `0xe369ced`.
- Loop `0xddc020..0xddc028` loads/evaluates the base feature; `0xddc004` loads
  its flag index and `0xddc010` writes the bool through indexed STRB. This
  explains why no ordinary direct setter xref exists. Startup caller is
  `0xddb14c`, followed by generic command-line Blink feature handling.
- Entry934 policy byte0 means copy when the base feature is enabled or is
  explicitly overridden. Disabled, unoverridden default leaves pristine
  runtime bool0 unchanged. This matches current generated
  runtime_enabled_features.cc:8799..8808. Generic SetFeatureEnabledFromString
  can also address the name in native name pool `0xcdd5705`.

**Corrected public enum mapping and proven startup relay:** TopBand is public
Arc feature **31**, not36. The earlier table reading paired an enum with the
next record's name. The actual name table uses24-byte records with pointer+0,
length+8 and enum+16; lookup `0x44da780` and consumer `0x44d8970` establish that
schema. Record `0xdff38d0` names `kEnableTopBandColorSampling` and stores enum31.
Enum36 instead names `kEnableTabLifecycleSnapshot`; HEIF is43. Initializer
`0x44d92b8` belongs to enum31 and uses JSON key `enableTopBandColorSampling`
with false default. There is no proven native name/config mismatch.

Renderer child-command-line hook `0x42abb3c` admits only process type renderer,
then `0x42abd14/0x42abd18` read runtime feature31. True loads descriptor
`0xe2c32b8`'s `TopBandColorSampling` name and appends
`--enable-blink-features=TopBandColorSampling` through `0x1b2e164` at
`0x42abd38..0x42abd4c`. Thus **startup relay from public31 is proved**; native
public getter/setter preserve31 without remapping it to36. Prior controls that
toggled36 tested Snapshot, not TopBand, and cannot reject this relay.

Live propagation to an existing renderer remains unproved. Bounded singleton
subscription/callback audits found adblock11 and memory20..23 handling, not a
TopBand live update, but do not exclude every indirect consumer. Do not invent
a live relay. Full raw provenance and independent clean review are in
`renderer_arc36_relay_evidence.md` (historical filename) and
`logs/wp/renderer-arc36-relay/independent-static-review.txt`. The current private
proposal still lacks this proven public31 startup hook; shared-hook work is
separate from the geometry/timer correction.

### Minimum faithful port boundary and remaining uncertainty

The producer algorithm is now concrete enough for a source port in a Blink GC
tracker, keeping full Blink Color and the pending/presentation/cache state
inside Blink. The minimal public callback payload is optional ARGB32,
request_surface_sample, uint64 sampling_token, reuse_cached_color, and crossfade.
The presentation-only +40 flag stays internal; it is not sent by native Mojo.

Shared changes required for a proposal:

1. Add tracker implementation/header to Blink Core's build and restore
   main-LocalFrame ownership/Trace, feature-gated construction, and the exact
   three lifecycle locations documented above. Register the disabled-default
   TopBandColorSampling runtime/base feature. Share one disabled-default
   ArcMacCoalescedResizing base::Feature definition with its public app getter;
   do not create a second same-name Feature object.
2. Add a small WebLocalFrameClient notification and forward it from
   RenderFrameImpl to renderer observers (or an equally narrow embedder
   callback). Existing Arc RenderFrameAgent can consume that observer callback;
   it avoids adding Arc browser or Objective-C dependencies to Blink.
3. Owned Arc transport extends RendererHost in renderer.mojom with those five
   native-equivalent fields. RenderFrameAgent forwards; the browser bridge owns
   sampling/cache/scroll-deferral per main/root RFH document and delivers to
   the WebContents delegate. Preserve
   the separately recovered 8-DIP browser fallback from RENDERER_VISUAL_GAPS.md,
   its optional equality, token, five-point distance policy, and scroll events.

This is a proposal boundary, not permission to edit shared files. Remaining
external work includes the now-proven Arc feature31 startup relay's
shared hook; live-update behavior remains unestablished.
The stripped native source enum labels are unnecessary: observed mode values
0/1/3 and transitions are exact. Full arbitrary integer overflow/NaN states
outside Blink's validated geometry/color invariants have not been probed; use
the existing Blink LayoutUnit/Color APIs rather than alternate arithmetic.
No remaining image-mode branch requires replacing native logic with a screenshot
approximation; the detailed admission/raster/cache mapping above is available
for implementation and review against the bounded transcript.

The bounded native instruction transcript is preserved in
`logs/wp/renderer-topband-producer.native.log`; it is generated by the reusable
`renderer_contract_evidence.py` helper against the pristine local framework.

### Browser fallback: RFH ownership, capture coalescing, and replay

This completes the earlier high-level browser fallback description. The browser
sampler is **per root/main RFH document**, not one unkeyed WebContents cache.
Get-or-create `0x4150da4` looks up its user-data key using `0x91af7c`, allocates
144 bytes, constructs with that RFH (`0x4150df4 -> 0x40bfbc0`), and installs into
the RFH's embedded user-data store at +3784 when optional engagement +4296 is
set. Lookup `0x91af7c` tests that same engagement and searches the map at +3792.
This is the source-equivalent shape of DocumentAssociatedData (which inherits
SupportsUserData) and `content::internal::Get/SetDocumentUserData`, rather than
WebContentsUserData or raw whole-RFH lifetime storage. Current source:
content/public/browser/document_user_data.cc:11..24 and
renderer_host/document_associated_data.h:45. The upstream DocumentUserData
lifetime preserves BFCache, but clears at same-RFH cross-document non-BFCache
commit as well as RFH deletion (document_user_data.h:40..45). The stripped native
class name is not separately available; the optional/user-data access path is
exact. Thunk
`0x4150e34` walks RFH parent +376 to the root before obtaining the sampler.
The sampler keeps its RFH at +8. Its constructor registers an input observer
subobject at sampler +16 with the current RenderWidgetHost observer list +584
and retains that widget at +112 (`0x40bfc8c..0x40bfd00`). Destruction removes the
observer and invalidates weak capture callbacks. Delegate delivery remains
through the associated WebContents bridge; tokens/cache follow that document
association. This explicitly refines earlier generic "per-RFH" wording.

| Sampler offset | Meaning |
| --- | --- |
| 24, 32 | Cached sampling token, token presence |
| 40, 44 | Cached optional ARGB value, color presence |
| 48, 56 | In-flight capture token, capture active |
| 64 | Current capture invalidated by scroll begin |
| 72, 80, 88 | Latest pending token, crossfade, request presence |
| 96, 100, 104 | Last-emitted state exists, ARGB value, color presence |
| 112, 120 | Observed widget, scrolling |
| 128, 136 | Weak-callback flag/pointer pair |

Entry `0x40c0094` receives optional color, request-surface-sample, token,
reuse-cached-color, and crossfade. A direct-color request clears pending +88;
if reuse is requested and cached optional color +44 is present, it substitutes
that cached color. An absent cached color does not replace the direct input.
It emits through optional-ARGB equality helper `0x40c0124`. This direct path
does not cancel an already active capture: its completion clears active and
discards when it finds no pending request.

A surface request overwrites the latest pending token/crossfade. It returns
while scrolling or a capture is active (`0x40c00e8..0x40c00f8`). Otherwise, an
equal cached token consumes pending and emits its cached optional result
(including a cached nil); a miss starts `0x40c0230`. No list/queue of all tokens
is retained. Capture obtains the RFH's current view, returns without clearing
pending or emitting nil when there is no view or view width <1
(`0x40c0254..0x40c0290`; GetViewBounds' packed return low32 of x1 is width).
Independent of return-ABI naming, `0x40c029c` uses that tested w1 as capture
width and literal8 as capture height. Earlier scout "height <1" wording is
explicitly withdrawn. Capture
then requests `(0,0,width,8)` DIP with an empty destination size. It captures
display DSF, stores active token/true, and clears the invalidated byte.
The asynchronous closure captures only weak sampler, token, and DSF
(`0x40c0374..0x40c037c`); crossfade is read from latest pending +80.

Completion `0x40c03f0` first requires active +56 and token +48 equal to its
captured token (`0x40c0400..0x40c0410`). It clears active and consumes the
invalidation flag. A scroll-invalidated capture, a currently scrolling sampler,
or no pending request prevents its bitmap from being analyzed. When idle with
a pending request, it either replays an equal cached token or starts a new
capture for the latest pending token. A different pending token also discards
the completed bitmap and follows that same cache-or-recapture branch
(`0x40c0454..0x40c0498`). Only a valid capture whose token still matches the
latest pending request consumes that request, analyzes, caches the optional
result/token, and emits with its pending crossfade (`0x40c04bc..0x40c0554`).
After delegate notification it checks scrolling and active again, then services
any newly pending request (`0x40c0558..0x40c0568`), preserving reentrant calls.

The copy-result adapter `0x40c0e64` converts failure (tag1) to an empty SkBitmap;
success (tag0) copies the bitmap. Empty dimensions make analyzer `0x40c05c0`
return absent color. Thus a valid latest failed copy caches/emits nil subject
to equality; missing view/invalid width causes no emission and retains pending.
The analyzer signature takes **integer** height and horizontal margin. Browser
rounding happens outside it at `0x40c04c0..0x40c051c`: height is
`max(1, round_away_from_zero(8*DSF))`, margin is
`round_away_from_zero(24*DSF)`, with native saturated numeric conversion.
Analyzer then uses `min(height,bitmap.height)-1` for the second row and
`max(bitmap.width-margin,0)` for horizontal span. The renderer image path calls
the same analyzer directly with integers 8 and 24, without browser DSF scaling.

Input observer `0x40c0ce4` (adjusting thunk `0x40c0d50`) handles only event11
GestureScrollBegin and event12 GestureScrollEnd. Begin sets scrolling true and
copies capture-active into invalidated. End clears scrolling; if active or no
pending request it returns. Otherwise it consumes/replays a matching cached
token using pending crossfade, or starts a capture for pending token. It does
not poll or add a timeout (`0x40c0cf8..0x40c0d4c`).

Refresh `0x40c0bc4` first requires active RFH, no parent, primary FrameTree.
Its only direct caller `0x438b824` lies in a committed-navigation observer
`0x438b6ec`, whose native state checks admit8/11. This state mapping is
independently pinned by consecutive NavigationHandle vtable entries
`0xd8e1298 -> 0x414f174` (loads +216 and returns state8 or11) and
`0xd8e12a0 -> 0x414f188` (loads +216 and returns state11). These match
NavigationRequest::HasCommitted and IsErrorPage in navigation_request.cc:
10445..10451, and their consecutive public NavigationHandle virtuals. Thus
8=DID_COMMIT and11=DID_COMMIT_ERROR_PAGE, rather than enum-value coincidence.
The refresh
services pending requests only when not scrolling or capturing: matching cache
replays, otherwise it tries capture. Then, if last-emitted engagement +96 is
set, it directly republishes that optional color with crossfade=false
(`0x40c0c74..0x40c0cb8`). This last replay bypasses equality suppression.
It must not be reduced to a new periodic capture or an equality-filtered setter.
The native entry/call condition is established; the stripped observer method's
exact C++ name is not independently recovered.

### Browser call arguments, notification ordering, and delegate replacement

Native surface-copy invocation `0x40c0394..0x40c03a4` passes source rect,
empty destination size, zero in x3, and callback in x4. Against the current
RenderWidgetHostView signature this is `base::TimeDelta()` (zero timeout), not
an invented positive timeout. DSF is from the view's cached ScreenInfos:
`0x40c02a4` copies view +272 through `0x1f5fc10`; its helper `0x1f5fd18`
copies current_display_id, the loop at `0x40c02c4` matches ScreenInfo +600
against it, and `0x40c02d0` reads that entry's float at +0. Current
`view->GetDeviceScaleFactor()` uses that source. A nearest-display query would
be a different policy and is unnecessary.

Notification helper `0x40c0124` has a deliberate ordering:

1. Compare/suppress equal optional ARGB32 (`0x40c0140..0x40c015c`,
   `0x40c01c8`). Crossfade does not participate in equality.
2. Store engagement +96, color +100, and color-presence +104
   (`0x40c0164..0x40c016c`).
3. Require RFH active (pending lifecycle update +2576 clear and lifecycle
   state +4360 equals3), then root/main-frame parent +376 null and primary
   FrameTree (root identity and type0), `0x40c0174..0x40c01b8`.
4. Resolve the Arc bridge/delegate and notify (`0x40c01d4..0x40c020c`).

Consequently a failed lifecycle/frame/delegate guard still updates last-result
state. Do not move the guard before equality/storage. Its source-equivalent
delivery predicate is `rfh->IsActive() && rfh->IsInPrimaryMainFrame()`.

Delegate setter registration `0x4345e80 -> 0x4376364` only replaces the native
weak slot +632/+640. It does not read the sampler, reset notification equality,
replay a cached color, or request sampling. ObjC setter `0x44417ac` with a
nonnil delegate allocates a 48-byte shared wrapper: weak self and weak delegate
are in payload +8/+16; its payload bridge vtable `0xda89008` points at
`0x4445afc`. The setter retains the shared wrapper at ObjC +216/+224 via
`0x44445ac`, then installs its native weak reference. The nil branch
`0x444196c..0x44419ac` clears the native weak slot only; it does not clear the
ObjC retained-wrapper property. Both payload members remain weak.

Delivery `0x4395190` locks the **current** native weak slot at
`0x4395544..0x4395578`; bridge callback `0x4445afc` loads weak delegate/self
with objc_loadWeakRetained at `0x4445b20/0x4445b2c`. The resolved objects remain
retained for that callback. A capture does not retain the delegate that existed
when it began. After replacement, any later eligible notification resolves the
new delegate; nil unregister prevents later delivery. Replacement itself does
not clear equality, so an equal pending result can still produce no callback.
An already-entered callback may finish using its retained receiver. The symbol
mapping for objc_loadWeakRetained is native indirect symbol
`0x84edd34 -> 0xc4a5b14`; objc_initWeak and objc_copyWeak are independently
mapped at `0xad9331c -> 0xc4a5ae4` and `0xad9239c -> 0xc4a6d80`. Shared-wrapper
last-strong destruction clears weak fields, and last-weak destruction frees
the wrapper (`0xda88eb8` virtual +16 -> `0x4042a20`, weak delete `0x39acb40`).

### One renderer sample: traversal, composition, and exactly one vote

This refines any earlier shorthand about accepting the first eligible hit.
The native main color loop walks HitTestResult's insertion-ordered node list;
`0x4685438..0x4685458` follows its next index. `0x468547c` calls eligibility
helper `0x4686a2c` on **that node itself**. This helper performs the element,
style/layout, visibility, opacity, geometry and optional quarter-point checks
documented above. It does not walk ancestors. Separate preliminary passes do
walk containing blocks for coordinate policy and ParentOrShadowHostElement for
Canvas/Video screenshot admission; those must not be added to the color loop.

The following is bounded pseudocode for its control flow (Color::Blend is the
existing Blink API; `backdrop.Blend(front)` preserves the foreground):

```cpp
if (hit_list_is_empty) continue_to_next_sample(); // 0x4685278..0x468527c
bool screenshot_allowed = canvas_video_prepass(hit_list);
optional<Color> accumulated_front;
Element* first_partial = nullptr;
for (Node* hit : hit_list) {
  Element* e = eligible_element(hit);
  if (!e || is_canvas_or_video(e) || youtube_exception(e)) continue;
  Color background = e->background_color;
  bool partial = background.Alpha() > 0 && background.Alpha() < 1;
  bool image = significant_image(e);
  bool complex = partial || e->opacity < 1 || image;
  if (!complex) {
    if (background.Alpha() < 1) continue; // transparent contributes nothing
    Color color = accumulated_front ? background.Blend(*accumulated_front)
                                   : background;
    vote_node(first_partial ? first_partial : e, color);
    continue_to_next_sample();
  }
  if (!lcp_seen) abort_detection_pass_and_reconcile_pending_background();
  if (partial && e->opacity >= 1 && !image) {
    accumulated_front = accumulated_front
        ? background.Blend(*accumulated_front) : background;
    if (!first_partial) first_partial = e;
    continue;
  }
  // Other complex branches use the recovered recent-node/style cache,
  // image raster/analyzer, and surface-capture admission rules above.
  // Accepted analyzed color or capture request is keyed by e itself.
  // Each accepted branch casts one vote and advances to the next sample.
  // Rejected/unavailable branches continue to later hits; a scrolled
  // uncached candidate may set pass-wide reuse_cached_color before continuing.
}
if (root_background.Alpha() >= 1) {
  if (accumulated_front)
    vote_node(first_partial, root_background.Blend(*accumulated_front));
  else
    ++root_background_votes;
}
```

Opaque plain acceptance branches `0x4685428..0x4685434 -> 0x46864b8` select
first_partial or current element as key, blend if needed, increment its record
and advance via `0x46850a0..0x46850b0`. Partial plain background after LCP
branches `0x46855cc..0x46855dc -> 0x46853e0`, accumulates/sets the first key
at `0x46853e8..0x4685420`, then continues. Fully transparent, otherwise plain
background branches `0x46855bc -> 0x4685428 -> 0x4685438`; it neither establishes
the first key nor adds a vote. Color helper `0x1288a1c` matches current
Color::Blend (color.cc:1220), including the transparent/opaque short-circuits.

Accepted analyzed-image/cached-image color writes a current-element record at
`0x4686454..0x46864a8`; accepted browser surface sampling writes its current
element record at `0x4686360..0x4686424`, then increments once and advances.
These branches do not blend the partial-front accumulator into the accepted
image value. Existing map records do not automatically skip this traversal or
validation: the accepted branch performs the put/find, updates, and increments.

List exhaustion reaches `0x4685020` through `0x468538c`, `0x46853a0`, or
`0x4685454`. A nonopaque root background casts no vote. An opaque root with no
accumulated partial background increments the separate root count
(`0x4684ffc..0x4685018`). With an accumulator, it inserts a node record keyed
by first_partial and blends root behind it (`0x4685034..0x46850a0`). A truly
empty result set instead skips directly to the next sample and casts no root
vote. Therefore each sample contributes at most one vote, although it may visit
and composite several eligible hits first.

The pre-LCP complex branch is an **entire-pass early return**, not a skip of
that hit/sample. `0x46855c8 -> 0x46858e4` submits no chosen state. It only
cancels an existing active/present pending state when that pending state's
wait-for-background-presentation field +40 (tracker +240) is true
(`0x46858ec..0x4685904 -> 0x4685ad4`). It does not cancel a pending browser
surface-sampling request merely because its +24 field is true, and it does not
set retry +135 in this path.

### Shared bitmap analyzer: exact pairs, pixel reads, and rounding

Independent bounded disassembly corroborates the writer's primitive:
`0x40c0888..0x40c0908` checks all ten unordered pairs of the five top-row
pixels. It additionally checks **only** top[0] against second[0]
(`0x40c0914..0x40c0974`) and top[4] against second[4]
(`0x40c0978..0x40c09dc`), not the three intermediate vertical pairs and not
all pairs in the second row. Distance uses signed integer channel-byte
differences, converts each to float and divides by255.f before squaring;
the sum order is B²+(R²+G²), then sqrt. Alpha does not participate in distance.
The maximum is updated by comparison/selection. Maximum exactly zero returns
the first top-row ARGB unchanged even if other pixels' alpha differs;
maximum >0.5 returns nil; otherwise it sums the five top-row ARGB channels
as floats, divides each by5.f, and rounds each away from zero with saturated
byte conversion (`0x40c0a88..0x40c0bbc`). Do not replace the maximum-zero
shortcut with an unconditional channel mean.

Pixel access is `SkPixmap::getColor(int,int)`, not raw premultiplied bytes:
analyzer calls `0x4c843c0` with bitmap+8 at `0x40c0718/0x40c0838`.
Its native color-type4 RGBA arm at `0x4c847b0` swizzles R/B; type6 BGRA arm
at `0x4c847d4` reads the word directly. Both test alphaType==2 (premul) at
`0x4c847e8`; that case tail-calls `0x4063b18`, which applies the alpha-indexed
scale table and `(scale*component+0x800000)>>24` for unpremultiplication.
This matches SkUnPreMultiply::PMColorToColor and current SkPixmap.cpp:192..265.
Use the existing SkPixmap API to preserve other color-type conversion cases.

### Root vote background and foreground IMG admission

The root-vote background uses the **LayoutView's ComputedStyle**, not a direct
documentElement/body lookup. `0x4684ca4` obtains LayoutView from Document +2100;
its FragmentDataList/Style layout and subsequent viewport calls independently
establish that object type (current document.h:865 exposes GetLayoutView).
`0x4684df8` loads its style at LayoutView +32 and calls `0x63b5c0`, matching
ComputedStyle::HasBackground: resolve background color alpha, return true for
nontransparent color, account for compositor background-color animation, then
HasBackgroundImage (current computed_style.cc:2547). Only when true does
`0x4684e14..0x4684e30` load that same style and call
`VisitedDependentColor(GetCSSPropertyBackgroundColor(), nullptr)`. Only alpha
>=1 replaces the transparent root-vote default (`0x4684e34..0x4684e50`).
This is separate from the later readiness fallback that invokes
LocalFrameView::DocumentBackgroundColor at `0x4685a5c`; do not conflate them or
add a new root/body background-propagation policy.

HTMLImageElement (native type33) sets the complex-image/significance boolean
true unconditionally: type comparison `0x4685550 -> 0x46855a0` stores w23=1.
It bypasses the CSS FillLayer significance scan used by other elements. Thus a
foreground IMG takes the complex path even with opaque plain CSS background,
opacity1, and no CSS background image. Canvas/Video remain explicitly skipped
in the main candidate loop, as documented above.

### Committed-navigation refresh: established local order and boundary

Refresh caller `0x438b6ec` is an ArcWebContents secondary observer interface:
thunk `0x438b950` subtracts168 from its receiver and tail-branches there;
native vtable data reference is `0xda81be8`. Its committed-state admission
(NavigationRequest state8/11), primary-root checks and sampler refresh at
`0x438b824` are documented above. Within this function, an optional native
state/UI update `0x4374a1c` occurs at `0x438b790` **before** the sampler refresh;
native user-data record synchronization calls `0x575e00` and `0x1b92100` at
`0x438b874/0x438b888` occur **after** it. The function contains no direct
navigation observer bridge dispatch around that refresh.

Objective-C navigation observer registration is a separate route:
ArcWebContentsNavigationController addObserver `0x4416eac` registers through
`0x4333598 -> 0x42f2480`. Its bridge vtable `0xda882d0` has provisional
`0x44193c0`, commit `0x44195b4`, finish `0x44197a8`, failure `0x441999c`, and
provisional-failure `0x4419b90` endpoints. This separation alone does **not**
establish whether the top-band refresh callback occurs before or after an app's
navigation commit/failure notification. A shared-core hook should not be called
native-order-equivalent merely because it is placed adjacent to a method named
DidFinishNavigation; relative observer registration/traversal must be grounded
separately or left an explicit proposal limitation.

The bounded controller endpoint inspection confirms a distinct list:
`0x42f2480` accesses controller +136, performs dedup/lookup using `0x42f67a0`,
and when absent increments controller +176 and inserts the shared observer pair
through `0x42f6ae0`. Controller registration/traversal timing relative to the
ArcWebContents +168 observer was not recovered by this bounded follow-up.
No before/after navigation-delegate ordering is claimed.

### Candidate-cache mutation occurs only on accepted image/capture branches

The recent-candidate helper `0x4686b58` is read-only: it obtains current
monotonic time, rejects age >1,500,000us (`0x4686b70..0x4686b9c`), then tests
node/style compatibility as documented above. It does not refresh age or style.

| Accepted path | Persistent mutations |
| --- | --- |
| Compatible cached analyzed color (`0x46855e8/0x46855f4 -> 0x468642c`) | Reads Color +280..296, then votes at +6454; **no** node/style/time/token refresh |
| Fresh opaque analyzed bitmap (`0x46862dc..0x4686358`) | Writes node +260, style +264, current time +272, Color +280..296 and analyzed presence +300=true; leaves token counter +304 unchanged |
| Compatible accepted surface-capture candidate (`0x468639c..0x46863c0 -> 0x4686420`) | Reuses nonzero token +304; no node/style/time refresh |
| New accepted surface-capture candidate (`0x46863c4..0x4686424`) | Clears analyzed presence +300, writes node/style/time +260/+264/+272, increments token +304, stores token in vote record +40 |

Surface acceptance begins at `0x4686360`; its vote record requests sampling via
record +32. Token reuse requires both nonzero +304 and helper compatibility.
The bitmap failure itself is not an acceptance. If screenshot admission is
false, `0x46856d8..0x46856e4` continues the hit loop with no persistent cache
write. If scrolled and no compatible recent token permits capture,
`0x46856fc..0x4685718` only sets the pass-wide reuse_cached_color flag and
continues. These rejected paths preserve the **previous** candidate cache,
including any previously engaged analyzed color. Do not clear/update the cache
merely because a different node or failed bitmap was examined. Likewise, a
successful cache reuse does not extend the original 1.5-second retention age.

### FireDetection active-timer branch and missing-view behavior

`0x46849d0` branches on timer activity. With an active timer, it sets retry +135
true at `0x4684b48`; if pending-presentation active +192 is false or mode +128
is3 it returns (`0x4684b50/0x4684b5c`). Otherwise it re-fetches the frame's
current view and returns when null (`0x4684b74..0x4684b78` target `0x4684b00`).
With a view but no `view->GetScrollableArea()`, it instead sets mode3 and
submits an empty ChosenState through `0x4684b10..0x4684b34`, including while
the timer is active. Fresh conditional-target decoding distinguishes those
destinations: failures at `0x4684b94`, `0x4684bb4`, `0x4684bc0`,
`0x4684bd0`, `0x4684be0` and `0x4684bf0` all target `0x4684b10`;
only the missing-view check at `0x4684b78` targets the quiet return. Raw full
function proof: `logs/wp/renderer-topband-bounds-correction-review/adjacent-bridges.raw.log`.
This corrects prior prose and proposal88's active-timer/no-area quiet return.
Valid state tail-
calls Detect at `0x4684c04`. That branch neither rearms the timer nor clears
retry, including when immediate detection is permitted. Thus every eligible
FireDetection while the timer is active and a pending presentation exists can
detect immediately, while retry stays true.

With an inactive timer, it arms100ms at `0x4684a40..0x4684a54`, then clears retry
at `0x4684a58`. Mode3 returns. With a ScrollableArea it detects
(`0x4684afc`); no ScrollableArea sets mode3 and submits an empty ChosenState via
`0x4684b10..0x4684b34`. This inactive branch has no explicit null-view check
before dereferencing view at `0x4684a80`; it presumes the lifecycle caller has a
live view. Do not describe that presumed-live case as a native missing-view nil
emission. The explicit active-branch missing-view check returns without emit.

Timer callback `0x46847c0` only reads retry and current view. If retry is true
and view exists, it schedules animation through `0xb44440` at `0x4684824`.
It does not clear retry, perform detection, or submit a state itself.

### ScrollableArea prerequisite and preliminary scroll-policy hit provenance

The FireDetection prerequisite is exactly **`view->GetScrollableArea() !=
nullptr`**, not rendering throttle, pending layout, or view +576 equals zero.
Native +576 is a compressed Member<RootFrameViewport>. Inactive branch
`0x4684a84` CBNZ jumps **to** `0x4684aec`, and active `0x4684b80` jumps **to**
`0x4684be8`; both decode/test that pointer and proceed to Detect when nonnull.
When absent, the inline fallback traverses the frame/window/document/LayoutView
and its PaintLayerScrollableArea. This matches LocalFrameView::GetScrollableArea
at local_frame_view.cc:4697..4706: return viewport_scrollable_area_ if present,
otherwise LayoutViewport(). Its Frame -> LocalDOMWindow -> Document chain is
an implementation of that fallback, not a separate readiness policy.
Native `0x4684e54..0x4684e80` independently loads the same +576 RootFrameViewport
and calls GetScrollOffset via virtual +328; positive Y establishes scrolled
state. Thus scroll uses the frame ScrollableArea, including root viewport
semantics, rather than unconditionally using only LayoutViewport.

The raw preliminary hit for scrolled coordinate policy does **not** use
HitTestResult::InnerNode or InnerNodeOrImageMapImage. After its HitTest
`0x4685154`, `0x468515c` explicitly obtains ListBasedTestResult (`0xa79cc00`).
The next-index loop `0x468516c..0x46851ac` visits insertion-order hits and calls
the same eligible-element helper `0x4686a2c` at `0x46851bc` with reference W.
Rejected entries continue; the **first eligible element** stops that search.
It loads that element's GetLayoutObject from node +48 at `0x46851c4`. This
layout object then follows the already documented ContainingBlock chain,
checking active sticky offset/fixed positioning (`0x4685bbc..0x4685c28`).
No eligible element, no applicable layout ancestor, or reaching LayoutView
selects DocumentToFrame adjustment. Do not replace this search with InnerNode,
the raw first hit, or the separate main color-voting traversal.

Scrolled surface-capture admission additionally requires **nonzero token
+304 and compatibility** to bypass the scroll restriction: `0x46856e8` loads
the counter; zero branches to `0x4685858`, which tests scrolled state before
allowing capture. Nonzero checks compatibility at `0x46856fc`; only true goes
directly to capture acceptance `0x4686360`. A compatible node with token0 is
therefore insufficient to admit a fresh capture while scrolled. Rejected
scrolled paths set reuse_cached_color and continue, as documented above.

The raw-versus-adjusted policy is selected **once per entire detection pass**,
despite preliminary-HitTest instructions being physically laid out inside the
sample-loop block. Persistent stack word sp+120 is policy_decided, initialized
false by `0x4684e98` (`stp wzr,w8,[sp,#120]`, second word is quarter-width).
Stack word sp+108 is use_raw, initialized false at `0x4684e8c`. For a scrolled
sample, `0x46850fc..0x4685100` tests policy_decided; false runs the preliminary
HitTest. Its sticky/fixed success returns through `0x4685c54 -> 0x468510c`,
sets use_raw=true, and reaches `0x468520c`, which sets policy_decided=true.
No qualifying layout object/reaching LayoutView instead uses
`0x46851e8..0x4685200` to transform that point and set use_raw=false, then also
sets policy_decided at +520c.

Subsequent samples advance via `0x46850b8..0x46850f0` without resetting either
flag. Once decided, +5104 reads use_raw: true follows +510c/+520c without
transforming the new raw point; false follows +51e8 to transform the new point.
Only initialization +4e98 and assignment +520c write policy_decided in the
complete Detect body. Therefore do not perform a new raw preliminary hit at
each sample. The first x=0 hit can admit an edge element; geometry intuition
does not change the persistent control flow. That native control flow is exact;
no alternative per-sample policy is inferred from geometry intuition.
