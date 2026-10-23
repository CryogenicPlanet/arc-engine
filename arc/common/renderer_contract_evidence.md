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
contract. Success producer/thread/DOM schema remains unresolved. The backend
captures frame lifetime/native identity, sets continuation `0x42f102c`, and
calls `0x41ba8a0`; that resolves native frame-associated state through
`0x4ebd98`, `0x805e0`, then `0x41ba32c`. No guess at extraction node enum is
supported by this inspection.

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
  does not parse a URLPattern or regex at this boundary. This does not prove
  renderer matching grammar.
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

Not yet recovered: renderer host wildcard/case/subdomain matching, stylesheet
frame/world/injection priority, initial JS injection timing, existing-document
script rerun behavior, HSLA color space. These require a direct stock-engine
probe or a deeper renderer implementation audit; registration bridge evidence
alone cannot justify invented behavior.

## Recovered renderer exclusion implementation (supersedes the earlier gap)

The actual 400-byte renderer agent primary vtable is `0xda82898`. Its slot +56
points to `0x43a6e54`, exactly matching message `0x258a997e` dispatch. This
implementation clones the decoded origin vector into refcounted storage +376
(`0x43a6f20-f24`), clears its cached map at +208/+216/+224
(`0x43a6f58-f78`), invokes invalidation at `0x78225ac`, and notifies observers
through `0x22322ec`. Clearing this cache is proven; resetting Arc's observable
blocked-resource count is not yet proven.

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
