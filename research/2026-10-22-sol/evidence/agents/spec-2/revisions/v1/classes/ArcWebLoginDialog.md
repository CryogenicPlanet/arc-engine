# ArcWebLoginDialog

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 11 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37cf28`, class_ro `0xe2a9d80`; instanceStart 8, instanceSize 136 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 128 | `_dialog` | `{WebLoginDialog="is_proxy_login"B"is_url_potentially_trustworthy"B"authority"{basic_string<char16_t, std::char_traits<char16_t>, s…` | 0xe2a9d18 |

The inline dialog value contains proxy/trust flags at +8/+9, authority UTF-16 at +0x10, and three callable objects. init copies string/callable state (0x441d2c4); setters assign that state (0x441d5cc); destruction uses 0x4346764 (0x441d734). isProxyLogin is a byte load, recovered from raw instructions despite a bad Ghidra signature (0x40b1230).

## Lifecycle, callbacks and relations

accept:with: converts nil username/password to empty native strings and invokes the callable whose entry is at +0x38, using callable storage at +0x28 (0x441d470). cancel invokes the +0x48 callable through +0x58; close separately invokes +0x68 through +0x78 (0x441d5b4, 0x441d464). No idempotence or automatic close-after-accept behavior is established. authority only converts an existing string; it does not compute trust (0x441d440).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Authentication UI can use a host callback, but three separate Arc callables and displayed security flags need matching lifetime and ordering. CEF authentication parity has not been checked here.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebLoginDialog.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithWebLoginDialog: | `@24@0:8r^v16` | 0x441d2c4 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x007ab764, 0x007ab624 | IMP 0x441d2c4; record 0xd77a9b8; dyld L2295; otool L29310 | medium / available / local_observation |
| base | - isProxyLogin | `B16@0:8` | 0x40b1230 | Raw 39402000/d65f03c0 loads byte at self+8; metadata specifies BOOL despite bad Ghidra signature/shared IMP. | local state / indirect operation; see body | IMP 0x40b1230; record 0xd77a9c4; dyld L2296; otool L15622 | medium / available_signature_artifact / bounded_review |
| base | - isURLPotentiallyTrustworthy | `B16@0:8` | 0x43ecb04 | Reads 8-bit register value from self+0x9 (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x43ecb04; record 0xd77a9d0; dyld L2297; otool L23765 | high / available / local_verified |
| base | - authority | `@16@0:8` | 0x441d440 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x27, 0x10, 0x18. | 0x01d9fb90 | IMP 0x441d440; record 0xd77a9dc; dyld L2298; otool L29319 | medium / available / local_observation |
| base | - close | `v16@0:8` | 0x441d464 | Invokes separate close callable at self+0x68 through entry at self+0x78; Ghidra erroneously truncates self in signature. | local state / indirect operation; see body | IMP 0x441d464; record 0xd77a9e8; dyld L2299; otool L29322 | medium / available_signature_artifact / bounded_review |
| base | - accept:with: | `v32@0:8@16@24` | 0x441d470 | Converts nil username/password to empty native byte strings, invokes stored callable through self+0x38 with callable storage self+0x28, then releases temporary strings. | 0x01b356b8 | IMP 0x441d470; record 0xd77a9f4; dyld L2300; otool L29325 | medium / available / bounded_review |
| base | - cancel | `v16@0:8` | 0x441d5b4 | Invokes stored cancel callable at self+0x48 through entry at self+0x58. | local state / indirect operation; see body | IMP 0x441d5b4; record 0xd77aa00; dyld L2301; otool L29328 | medium / available / bounded_review |
| base | - dialog | `{WebLoginDialog=BB{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{function<void (const std::string &, const std::string &)>={__policy_func<void (const std::string &, const std::string &)>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}}16@0:8` | 0x441d5c0 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x8. | 0x043467d8 | IMP 0x441d5c0; record 0xd77aa0c; dyld L2302; otool L29331 | medium / available / local_observation |
| base | - setDialog: | `v144@0:8{WebLoginDialog=BB{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{function<void (const std::string &, const std::string &)>={__policy_func<void (const std::string &, const std::string &)>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}}16` | 0x441d5cc | Assigns/replaces local native or ObjC state; self offsets 0x8, 0x10, 0x27, 0x20, 0x18, 0x40, 0x38, 0x28. Calls 0x007ab764, 0x007ab624. | 0x007ab764, 0x007ab624 | IMP 0x441d5cc; record 0xd77aa18; dyld L2303; otool L29334 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x441d734 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x8. | 0x04346764 | IMP 0x441d734; record 0xd77aa24; dyld L2304; otool L29337 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x441d73c | Initializes native storage/engagement fields at self offsets 0x8, 0x18, 0x20, 0x10, 0x38, 0x40, 0x58, 0x60; returns self. | local state / indirect operation; see body | IMP 0x441d73c; record 0xd77aa30; dyld L2305; otool L29340 | medium / available / local_observation |

## Remaining consequential questions

Trace callable producers to Chrome login handlers; determine one-shot/reentrancy, accept/cancel/close ordering, trust calculation and app presentation thread. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
