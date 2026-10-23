# Independent protocol evidence review and synthesis corrections

A fresh GPT-6.1-Sol/high reviewer recomputed the completed protocol audit's retained tables, inspected its parser/validator logic, sampled instruction and body evidence, and checked cached objc4 source. No network, app execution, builds or complete binary reparse occurred. The review supports the bounded inventory after the qualification below; it does not certify every callback's transitive behavior.

## Required termination qualification

The historical `agents/protocols/initial/report.md:38` overgeneralizes termination callback retention beyond the caller's temporary storage. The retained ArcCore **1.170.0 arm64**, unslid `0x42b2430` body (`bodies/focus-0x42b2430.c:18–62`) stores callable state at native application offsets `+0x150..+0x168` only on the first initialized request when `DAT_0e49cf50 == 0`. The repeated-request branch can log and returns `2` without storing the new callable. The first path can also continue synchronously into `0x42b258c`; storage does not prove survival beyond return on every first request.

Use this qualified contract in synthesis: **The ObjC wrapper retains temporary block storage; native code copies and stores the callable in application state only on the first initialized termination request. Repeated requests return `2` without storing the new callable. Immediate status does not establish eventual callback delivery.** Public enum names, complete downstream lifetime and cardinality remain unresolved. The narrower `protocol-specs/ArcBrowserApplicationDelegate.md:19` is compatible with this correction. Original completed artifacts remain unchanged.

## Supported counts and sampled behavior

- The audit reconciles 26 guide protocols, 235 physical entries, 200 unique `(protocol, kind, selector)` identities and 198 selector names. Five getter identities are DevTools command/event values; 195 are candidate callbacks or app services. Of those, 189 have static selector-send evidence. The combined table has 195 distinct send VAs across 192 selectors, supporting 194 guide identities. None is a runtime callback count or complete receiver proof.
- There are 270 app implementation joins across 18 explicit adopter classes. Thirteen raw type differences erase to equal annotation-free encodings. This is not full ABI or execution equivalence.
- Parsed descriptor lists contain 385 app protocols, including 26 Arc names, and 69 descriptors per engine with zero Arc names. This is absence in the parsed lists, not proof that callbacks are unused or no other metadata exists.
- Extended-type pairing uses the required-instance/class and optional-instance/class prefix order plus each physical entry's index. Cached objc4 source supports the rule, without establishing Arc's shipped runtime revision.
- Permission adapter `0x4446dcc` has `mov w8,#3; cmp x1,#3; csel x1,x1,x8,lo`: unsigned values 0–2 pass through, other representations become 3. Quiet-UI reason normalization keeps 1–5 and maps other inputs to 0. Enum names remain unknown.
- Dialog metadata supports zero-argument alert completion, BOOL confirm/before-unload, NSString text completion and uint64 permission response. Retaining a block does not establish cancellation, cardinality or presentation sequence.

Use `evidence/engine-all-static-sends.tsv` for the combined evidence. The focus-only `updateDevToolsToolbox:` send at `0x4447edc` is absent from the narrower table named by some original provenance. The generated alert declaration's `void (^)()` can be read more clearly as canonical `void (^)(void)` for its established zero-argument shape; original raw encodings are retained.

## Compared fields across the two engine versions

The parent and a fresh reviewer independently compared the retained protocol-audit implementation and layout tables. `compare-metadata.py` reproduces `parent-metadata-comparison.json`: both versions have 1,964 unique class/category/kind/selector keys with equal raw encodings, and 146 Arc-prefixed instance-layout records covering 351 ivars with equal retained fields after excluding image/address anchors. Compared layout fields include flags, superclass, instance start/size, ivar names/types/numeric offsets/sizes/alignment and strong/weak maps. No unresolved-offset sentinel is hidden by the equality.

State only: **Across the parsed Arc-prefixed surface, all 1,964 method declaration identities/raw encodings and all retained instance-layout metadata for 146 classes match between 1.168.0 and 1.170.0.** This is stability of the compared fields, not every property, protocol conformance, metaclass layout, native C++ ABI, implementation, runtime layout or behavioral equivalence. Neither retained Arc-prefixed layout set contains `_mouseInside`; its historical attribution to `ArcWebContents` in ENGINE.md is not reproduced, and no alternate containing class was verified here. The separate parent raw-entry check corroborates the six previously unknown older selector/type/IMP associations.
