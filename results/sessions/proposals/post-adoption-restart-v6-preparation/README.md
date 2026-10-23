# Prepared replay: unchanged v6 closed-tab restart after source-only adoption

**PRIVATE preparation only. Parent alone queues and reaps the later locked run.** No source/test/fixture changes, executor call, build, app, query, index, runtime or Chirp action was performed. The new orchestrator has 45 physical lines and reuses the reviewed runtime-v1 helpers without calling their main function.

Orchestrator `/tmp/arccore-sessions-post-adoption-restart-v1.py` SHA256: `983659c6d3f98d8b0f570e2b467c7be378daeb14ff672710c229ec69b5b33892`. Frozen 46-input local tool/fixture/import/package map `pins.json` SHA256: `eba2728164f6a425bead22705078653ba2d28a5f0dd745b1e7f86b09925ca2bb`.

This replays unchanged `/tmp/arccore-sessions-tab-restart-v6.ts` (`c6504e3a...`), `.mm` (`26f3fcf4...`), `.dylib` (`27c95a13...`) and storage snapshot (`c07dbd0f...`), OURS156 only. Existing v6 gates demand two genuine individually closed tabs, opaque positive historical IDs, saved SNSS presence, a clean quit and new browser PID, both persisted-ID restores, asynchronous main callbacks, consumed eligibility, two distinct new live IDs, exact A/B/C with B selected, and surviving sentinels. The fixture is unchanged: its supplemental asynchronous-close route does not replace or relax the failed canonical inline-close contract.

Before import, the orchestrator verifies the explicit prequeue pin-map SHA and every frozen input; the runtime helper is exactly pinned `0df88d88...`. It then sets the helper LOCK/OUT/PINS/STATUS globals, requires source178 and the five exact source-only native file payload hashes, and verifies the two hook blocks individually. Additional foreign declarations around those blocks are retained because no source/header is written. Later edits to the five guarded files fail closed for renewed review.

Under the outer lock it freezes every owned Arc .mm/.h/.cc/.gni/.gn/.mojom input, MANIFEST and every manifest-native path/maintained patch. Existing `.inc` generator outputs are excluded at entry, then frozen immediately after the standard generator and before build. Key-set and content comparisons detect added/deleted/changed inputs. These scoped source pins are rechecked around standard generator/build/package/codesign and after runtime. They are not a complete Chromium/system-toolchain freeze.

The only scenario is existing v6 through runtime-v1's cache-v10 → profile-v10 isolation, port9464. No canonical or native-original reruns. The helper isolates child sessions, drains each started child and defers signals; final wait_safe holds the lock until the original cache/profile device+inode or absence returns and exact-agent helpers are quiet. Fixture0/pass/contractPass, two distinct PIDs, exact history, both shutdown acknowledgments/exits and no fallback signal are required. Fatal markers fail the run. Framework records must remain identical before/after. Any failure returns nonzero after safe cleanup; no source rollback is performed because this runner writes no source.

Frozen inputs include local TypeScript import graphs, relevant package-builder sources/resources, mod source inputs read only, generator metadata, selected build configuration, stock app binary/plist and Bun/Python/codesign binaries. Optional xcode.env or sdk-compat-min absence is frozen too. This does not freeze all system libraries or the whole stock app resource tree.

Parent invocation from the repository root, with a fresh output directory:

```sh
arc/tools/locked.sh python3 /tmp/arccore-sessions-post-adoption-restart-v1.py \
  --out /Users/cryogenicplanet/general/arc-engine/results/sessions/post-adoption-restart-v6-parent-v1 \
  --pins /Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/post-adoption-restart-v6-preparation/pins.json \
  --pins-sha256 eba2728164f6a425bead22705078653ba2d28a5f0dd745b1e7f86b09925ca2bb
```

Still not established by a later supplemental pass: canonical close/restart acceptance, ungrouped ROOT TAB destination creation, cancellation/reentry, full open-window startup parity, foreign DTO metadata or discard/logical ownership. Current canonical fixtures stop at their first inline-close failure before this persisted-ID branch.
