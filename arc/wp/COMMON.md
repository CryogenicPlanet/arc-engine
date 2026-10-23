# Brief for every ArcCore work package (read fully before starting)

You are implementing part of ArcCore: Arc browser's engine interface (146 ObjC classes) on top of Chromium 156,
so Arc's unmodified app (1.168.0) runs on an engine we build from Chromium source and **behaves 1-1 with Arc's
own engine at the app↔engine boundary**. Working directory: `/Users/cryogenicplanet/general/arc-engine`.

Read first, in order:
1. `arc/ARCHITECTURE.md`: binding design decisions and rules. Follow them exactly.
2. `NEXT.md`, section "Building our engine": where things stand.
3. `specs/README.md` and `specs/<Class>.md` for your classes: reviewed static research of Arc's engine (what each
   method does, which Chromium code it calls; confidence labels are per writer, don't over-trust them).
4. `arc/tools/arccore-1.170.0-objc.tsv`: exact runtime metadata of Arc's engine (C class, M method with type
   encoding, R property attributes, I ivars). Your selectors/encodings must match it exactly.
5. Golden transcripts: `results/golden/arc-1.170-*/launch-*/record.jsonl` (every boundary call on Arc's own engine,
   with arguments and return values). This is the ground truth for behavior.
6. Existing code: `arc/api/*.mm`, `arc/api/arc_internal.h`, `patches/*.patch` (Chromium changes), Chromium source
   under `chromium/src` (read freely; never edit it directly, see Patches).

Tools (from the working directory):
- `arc/tools/check-file.sh arc/api/<file>.mm`: syntax/type check of one file with the real build flags. Fast, no
  lock. The file must be listed in your `arc/wp/<package>.gni` and gn regenerated once (any build does that).
- `arc/tools/diff-run.sh [scenario]`: (takes the shared build/run lock; may wait) regenerates the stub table,
  builds, packages our engine into Arc's app, runs the scenario with the boundary recorder and diffs against the
  golden. Results: `results/ours/<scenario>/diff-launch-N.txt` and `.json`. Builds take ~1-2 min, a run ~1-2 min.
- `./try-engine.sh 30` (only inside `arc/tools/locked.sh`): quick launch, prints unimplemented engine calls in
  order and the newest crash.
- App logs: `../arc/build/ArcModAgent-ours156-run.log` (lines "ArcCore unimplemented: ..." are stubs being hit).

Rules:
- Edit only the files your package owns (listed in your brief) plus your `arc/wp/<package>.gni`. Shared files
  (`arc_internal.h`, `ARCHITECTURE.md`, other packages' files, `patches/`, `tests/` unless you own it) are
  read-only for you: if you need a change there, describe it in your report under "Requests to lead".
- Keep the tree compiling. Run `check-file.sh` on every file you touched before taking the lock. If the build
  fails in a file you don't own, wait a few minutes and retry; mention it in your report if it persists.
- One primary `@implementation` per class in the whole tree. Add surface to someone else's class only as a
  category in your own file.
- Never touch `/Applications/Arc.app`, `~/Library/Application Support/Arc`, the real ArcMod profile/data, or any
  VM. Agent apps/data named `ArcModAgent-*` are disposable. No git commits or pushes (the lead commits).
- Chromium style and constraints: ObjC++ with ARC, `-Werror`, `base::NoDestructor` for statics, no exit-time
  destructors, no unsafe libc buffer calls (`strcmp`, `memcpy` on raw buffers: use `std::string_view`, spans),
  raw pointers to Chromium objects in long-lived places go through `base::WeakPtr` or `raw_ptr`.
- Behavior must match the golden transcripts (return values, callback order/arguments). Where Arc's engine depends
  on The Browser Company's servers or accounts, keep the call shape and return what the golden shows for ArcMod
  (account-free, services blocked). Don't add features, don't remove features.
- Work in small verified steps: implement a class or method group, check-file, diff-run, read the diff, fix.

Finish with a report (your final message) containing:
1. What you implemented (classes/methods), file by file.
2. Diff status per scenario (paste the first two lines of each diff-launch-N.txt) and which differences remain
   in your area, with the reason.
3. Requests to lead (shared-file changes, patches, design questions).
4. Anything you found that contradicts ARCHITECTURE.md or the specs.

## Persistence (overnight; the lead answers on chirp `projects/arc`, Rahul is offline)

- Don't end your session for transient blockers: another package's compile error, waiting for the build lock,
  a missing golden scenario, a pending lead answer. Post the request on chirp `projects/arc` addressed to
  `@claude` (exact file:line, what you need), then keep working on other parts of your package, or wait
  (`sleep 300`) and retry. Lead answers appear on the same board; check it before asking again.
- If a golden scenario you need doesn't exist yet, ask the scenarios worker on the board and meanwhile verify
  against launch-slice and any `results/golden/arc-1.170-*` that exists (`.candidate` dirs are not final).
- End only when every part of your package that existing goldens exercise is diff-clean, and your report lists
  what remains uncovered and why. If you are restarted, start from your previous report
  `logs/wp/<package>.report.md` and the current state of your files.
- Builds share one tree: keep your files compiling at all times (check-file before every save you leave in
  place); if a build fails in a file you own, fixing it is your top priority.
