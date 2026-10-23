# Final-owner qualification of the conditional PiP retirement proof

2026-10-23; bounded offline arm64 audit. This new addendum qualifies the frozen sessions PiP step3 and phase-qualification note; no existing evidence or manifest is rewritten. No production/shared changes, app, runtime probe, attach, build, board or credential action.

The earlier synchronous PiP ordering applies **when the Media peer acquires a live logical owner**. It must not be read as forcing a PiP app callback on genuine final shared-owner disposal. Existing same-Impl/counter conclusions for the qualified replacement route remain unchanged; this adds a missing prerequisite to the PiP callout chain rather than a counter reset or new replacement phase.

## Exact prerequisite and ordering

In original `43639e8`, `43639fc` loads the Media peer's logical weak control from `+0x28`; absent control branches at `4363a00` to return `4363ab0`. `4363a0c` reads the shared-owners word at control `+8`; `4363a10..14` tests the expired `-1` sentinel and branches to the same return. `4363a18..30` CAS-acquires a strong owner only while that count remains live. The raw logical pointer at Media `+0x20` is then checked at `4363a34..38`.

Only after these gates does `4363a5c` clear the cached PiP-controller peer and `4363a84` invoke the synchronous dispatcher. Therefore an expired final shared owner prevents both this cached-peer clear and this app invocation, even if the enclosing retirement helper reaches the virtual PiPDestroyed entry. A retained peer/raw Impl/native userdata route does not make that logical weak pair lockable.

Final Impl destructor `4371b84` reads its managed native binding `+0x130` at `4371ba4` and, when present, calls common DeInitialize `4372b6c` at `4371bb4`. UI-manager anchor clear `4372070` (`Impl+0x370`) and guard poison `4372080` (flag at stored `Impl+0x368`, offset4) are later. This timing does **not** imply the logical shared owner remains alive: logical strong expiry and destructor-invalidated dispatcher guards are different lifetime boundaries. Control disposal `43a04dc` calls the destructor directly on UI at `43a0504` or posts the raw Impl at `43a055c` to `43a05ac`, whose `43a05b8` calls the destructor; that task is not a new strong logical owner.

For ordinary qualified discard/rebind with a successfully locked live root, the previous PiP ordering still holds: cached PiP peer clear precedes the app callback, while the old native binding/Media receiver/physical observation/root userdata have not yet reached their later clears in the recovered local sequence. This does not validate arbitrary getters or reentry. No expected-old/generation revalidation is added or recovered here.

## Limits and implication

Do not force an extra PiP app callback, resurrect a final root, or poison all dispatcher guards at strong-owner expiry to approximate this source. Conversely, do not suppress accepted live-owner Media work merely because eventual final disposal is pending. This qualification concerns the inspected Media PiPDestroyed producer during common cleanup, not every possible PiP endpoint or every teardown callout.

Final cleanup is **not universally silent**: the separate DevTools retirement audit recovers raw-client destructor `435d09c` calling `435c74c` at `435d0bc` before its private flag poison `435d0d0`; stored notifications have their own copied-delivery conditions. No all-teardown/no-other-callout claim, runtime interleaving, implementation approval or parity acceptance follows from this addendum.

## Provenance and independent review

Original binary: `/Users/cryogenicplanet/general/arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`, full SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`. Fresh small-range extraction uses `/tmp/arccore-sessions-persistence.py` with Capstone from `/tmp/arc-ui-capstone`; exact commands/ranges and instructions are in the adjacent new disassembly file, and all input/output pins are in the new manifest. The core final-phase raw/seal corroborate the shared-disposal classification and complete destructor ordering; no executable target was launched.

Fresh read-only reviewer `/root/sessions_recon/full_probe_review` independently confirmed the logical weak-lock gate, final-owner versus later UI-guard distinction, and need to retain separate DevTools/raw-close qualifications. Reviewer changed no files. Every preexisting file in this evidence directory is hashed before and after and preserved unchanged.
