# Independent correction evidence

Every engine address in this document and the bounded original-body transcripts is an **unslid VA in ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48**, copied image `/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64`, SHA-256 `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd`. App addresses, where mentioned, belong separately to Arc 1.168.0 arm64. Chromium source pin: `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`; CEF pin: `426920c33ef22551e0b8f48cf343d623ac25642d`. No CEF source or replacement runtime was tested for this correction.

[Instructions](instructions.tsv) were freshly decoded using the installed LLVM 20 C API with bounded LC_SEGMENT_64 file translation. Each row identifies its image, VA, file offset and bytes. [Binary check](binary-check.json) records selected-range hashes and 436 byte/file-offset comparisons against the ABI report's context/download-delegate observations. Seven bounded windows contain 967 instruction words; three words in the native menu helper remain undecoded by this LLVM configuration and are retained as such. No function-span totals are used as coverage. Branch targets and directly visible stores support the corrections; indirect cleanup calls do not establish complete callee semantics.

The original body text was reread through `/home/exedev/work/read-function.py`, with limits of 18,000 bytes per body (10,000 for the additional BoostIDE boundary set). [Finding bodies](rechecked-original-bodies.txt), [nearby bodies](nearby-original-bodies.txt), and [BoostIDE boundaries](boostide-boundary-bodies.txt) retain exact index names, shard paths, offsets and lengths before each body. None of these selected reads is failed or truncated. New transcripts are separate from the unchanged copied `bodies/`, `evidence/` and `sources/` snapshots. All selected transcript bodies are compared with the read-only index during revision validation.

## Findings 1-2: context and history nil paths

At `0x43ed9c0`, context setter saves delegate x2 in x19; `0x43ed9e0` invokes the getter and `0x43ed9e4` saves returned control pointer x1 in x22. `0x43ed9e8` branches on nil to `0x43eda98`, bypassing the nonnil existing-bridge weak store at `0x43eda20`. The nil target checks/decrements x22's shared count, conditionally invokes control-block cleanup and converges on release/return at `0x43eda78`. The stored wrapper field is not visibly zeroed and no registration setter is called on that path. This agrees with the rechecked original body; it is temporary shared-handle cleanup, insufficient for a stored-clear, detachment or queued-callback contract.

History follows the same distinction: `0x40439e0` saves x2; `0x4043a00` calls the getter; `0x4043a04` saves returned x1. `0x4043a08` branches on nil to `0x4043a94`, bypassing the weak store at `0x4043a40`. Nil checks/decrements the returned control pointer, conditionally destroys/releases it and returns through `0x4043a70`. The original body confines `setVisitObserverBridge:` and `0x4332f08` registration to nonnil creation. Exact native registration ownership, nil detachment and in-flight callbacks remain unknown.

The independent ABI cross-report observation is directly corroborated: downloads `0x4042030` branches on nil to cleanup at `0x40420bc`, bypassing the weak store call at `0x4042068`. This is a useful analogous observation, not a reason to generalize all delegate setters.

## Finding 3: native content hash versus pointer equality

`0x43ed260` sends the `search_engine_template_url` accessor, checks optional engagement, calls `0x4340180` with native pointer and releases its temporary shared owner. There is no preference-key lookup in that body. `0x4340180` copies string representations at native +0x3b8/+0x3d0, calls `0x18f3c` on each using twice the recovered length, and arithmetically combines the two results. String field names and exact Chromium symbols are not assigned. `0x43ecf8c` separately compares native pointer values after receiver/class/optional checks. Hashing and equality must not both be described as pointer identity. The nearby equality matrix was made explicit as well as the hash matrix and lifecycle prose.

## Finding 4: BoostIDE payload strings

`0x8371aa8` constructs both event arguments with `0xc87c40` before the `cr.webUIListenerCallback` call. `0xc87c40` allocates or uses short storage, moves input bytes, writes a terminator and sets tag 4. Its fresh raw instruction window corroborates the copy/tag construction. Chromium `base/values.h:746-754` declares string constructors and `base/values.cc:182-190` constructs the string variant; these corroborate a Value-family string construction without proving a binary symbol or ABI identity.

The response bodies `0x43d35c8`/`0x43d38cc` first search pending IDs, construct ID/payload via the same helper, adapt via `0x21beb38`, and call `0x68122a8`/`0x6812404`. The latter construct Boolean 1/0 and send `cr.webUIResponse`. The tag-4 arm in `0x21beb38` forwards the string pointer/length rather than parsing it. Wrapper sends at `0x4414488`/`0x44145b0`/`0x44146d8` convert nil to empty native strings and gate on the engaged optional/live handler. Pinned `content/public/browser/web_ui_message_handler.h:76-103` corroborates response/listener API families. No reviewed body parses the outgoing payload as JSON. Receiving JavaScript, downstream serialization details and end-to-end JS-allowed/pending-ID behavior remain limits. All three send descriptions/questions and adjacent class prose were corrected.

## Finding 5: observer vector assignment

The preserved encoding names `vector<shared_ptr<ArcBrowserContextPreferencesObserverBridge>>`. `0x4418ce4` locates destination at self+0x28 and skips identical source/destination. Its branches reuse capacity or release old elements/allocation and allocate new storage, then copy 16-byte elements with control-count increments and release replaced/trailing elements. The Ghidra name identifies another sharing class; metadata identifies this method. This is vector storage assignment, not one delegate slot or proof of service registration/synchronization.

The analogous context `setBookmarkObservers:` description already distinguishes vector assignment/capacity/self-assignment. Other vector-bearing declarations were inspected for the same wording error; no additional correction was needed.

## Finding 6: configuration stack-argument recovery

The declared encoding remains exactly `@72@0:8i16@20@28Q36B44Q48B56B60@64`. Recovered C uses `param_9._1_1_` although param_9 is bool and `CONCAT44(...,param_10)` for a pointer although param_10 is bool. This is now `available_prototype_malformed`, with a specific recovery flag.

The fresh instruction window resolves the storage correspondence more narrowly than the C: prologue subtracts 112 from SP and sets x29=SP+96, so x29+16 is entry SP. `0x43eca88`/`0x43eca8c` load bytes from entry-SP+1/+0; `0x43ecab4`/`0x43ecab8` store them at self+9/+10 in the corresponding order (entry+0 -> self+9, entry+1 -> self+10). `0x43eca48` loads x21 from x29+24, entry-SP+8; `0x43ecabc`-`0x43ecac4` passes that pointer to the object-store call for self+0x30. Register-saved values populate the earlier ivars. Thus final field stores have raw corroboration; the recovered C expressions remain invalid as a callable prototype. Startup consumption, policy meaning, argument validity and runtime compatibility are not established.

## Finding 7: lifecycle VA transcription

The two correct IMPs are `0x43f1968` (add bookmark observer) and `0x43f21a8` (remove). Their complete bounded exported bodies were independently reread. Existing metadata/matrix entries are already correct; revision validation rechecks every method-record IMP from the copied binary. Only the two lifecycle typos and related method evidence annotations change; invalid `0x443f1968`/`0x443f21a8` are never adopted as image addresses.

## Finding 8: native menu delegate clear versus wrapper ownership

The nil path at `0x4440110`-`0x4440138` obtains/releases the shared-handle temporary. At `0x4440140` the native pointer is loaded from wrapper+0x38, x1/x2 are zeroed, and `0x444014c` calls `0x432bf9c`. The helper's nil branch at `0x432c06c` loads the prior native control pointer and `0x432c070` stores x1 and zero at native+0x20/+0x28. This directly supports native slot clearing. No wrapper shared-slot clear/setter is visible on the nil path. Lifetime/reuse of the wrapper-held bridge and suppression of pending callbacks remain unknown. The three undecoded helper words affect cleanup interpretation, not these decoded stores; no full cleanup/ABI guarantee is asserted.

Nearby nil descriptions were checked against `0x44011d8` (side panel) and `0x4414268` (BoostIDE): both original bodies explicitly zero their wrapper pointer/control fields on nil, respectively self+0x18/+0x20 and self+8/+0x10. Those narrower wrapper-clear statements remain justified and were retained without adding cancellation claims.

## Finding 9: pinned source line

At the Chromium pin, `components/sessions/core/session_types.h:185` is `SessionID window_id`; line 203 is `int selected_tab_index`. The class source-family citation now uses 185. Numeric correspondence with CEF IDs remains unproved. New [source hashes](primary-sources.tsv) and [exact excerpts](source-excerpts.txt) retain provenance; copied source/evidence manifests remain unchanged.
