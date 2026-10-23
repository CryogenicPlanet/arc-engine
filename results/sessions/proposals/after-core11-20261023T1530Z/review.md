# Fresh after-core11 variant review

**Bounded PASS: no concrete composition or command bug found.** This review reconstructed headers in memory and read hashes/stages/commands/logs; it did not apply patches, invoke compilers, build, run apps/probes, modify production/shared/test files, or access board/credentials. No native-TU/link/runtime, adoption/grant, public Close, functional-peer or acceptance result is established.

## Semantic union and exact preserved inputs

- Variant source-only SHA is `fd6068db59ea60e2de4c02e999e6d37f7b4ddd66568c2a1d6453b010e9e192db`; source-policy is `fc43a59b8d6fabdba47f2fdf611e505a233fc32aad7afff7a4e87bf9dd6e539f`. Current24 baseline states, all22 composition patch files and three input manifests match recorded hashes. Original frozen source variants also still match their old hashes; no old proposal is replaced.
- Every non-header file patch section is **byte-identical** to its original. The inserted session namespace body is byte-identical to the original added body, including the policy hook in the policy alternative. The only original include additions remain `<cstdint>` and `base/functional/callback_helpers.h`.
- Applying each new header diff **in memory** to exact core11 header `27e5d1f5…` equals core11 plus only those two includes and the original session body directly after `namespace arcmod`. Expected first headers match the supplied files and hashes: source-only `b2be6c7faa76f459e905a46018fa28ec72e8baadca2594febcccdb60b28021ec`; policy `2ed7909f76cb898dd1e98d9852f3cf9be15a27db1e4ed770d00fcce89a7398c6`.
- The frozen matching factory header patch applied in memory to each first header exactly equals the final staged header. Reversing that factory header patch and then the new source header patch restores **core11 byte for byte**. No core declaration was altered or dropped. Every other core11 file matches its expected final hash, including the two BeforeUnload replacements and noncreating accounting adapter `65003d80…`.

## Recorded complete chains and syntax scope

Both core11→sessions7 routes contain18 successful check/apply pairs: **36 checks and36 applications, all0**. The remaining factory/TAB/accounting patch files are unchanged; their root-TAB/group restrictions, exact-root/weak-helper guards, one-shot factory handling and accounting wrapper remain present. Current a647 setter and QueueRestorationCallback regions are independently byte-identical in both final stages: publish-before-enumeration, no incoming-count callout, entries nonempty guard, current-wrapper binding check and no old native-WC weak capture. The core quit guard remains present. Final owned files exactly match the prior standalone full sessions7 hashes `7a370597…` / `0aa0f890…`.

Both recorded actual-Siso owned syntax checks return0 with empty logs. Their argv reconstruct exactly from the saved original compiler command: staged include first, staged owned source, `-Werror` and ARC retained, object/dependency output switches removed, `-fsyntax-only` plus private crash directory added. This checks only owned `arc_sessions_restore.mm` with complete staged headers; it does not compile native/core translation units or resolve linking/behavior.

The original1520 four-route conflict evidence remains unchanged and valid **for the unchanged original patches**. This pass applies only to these explicitly separate after-core11 variants and tested core-first order; no sessions-first after-core11 route is implied. All previously held semantic/lifecycle/cancellation limits remain.

[review.json](review.json) records exact reconstruction/hash/guard/command checks. Independent fresh read-only second review by `/root/sessions_recon/full_probe_review` also finds no concrete bug within this scope.
