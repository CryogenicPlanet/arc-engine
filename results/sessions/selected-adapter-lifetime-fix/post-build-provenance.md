# Webextras native10: post-promotion build provenance limits

**Exact sessions-object provenance is absent from this peer run's archived evidence.** Its build occurs after the reported e973 promotion, but the log does not prove `arc_sessions_restore.o` was recompiled, nor record its source digest at compilation.

- `logs/wp/webextras.native.run10.engine-build.log:1–3` contains only preflight16:05:57, build attempt16:06:00 and BUILD DONE16:06:46 UTC. There is no object/compiler action row or `arc_sessions_restore` reference. This establishes a completed whole build after the reported16:02:~09 promotion, not a sessions compile receipt.
- Peer source snapshots captured16:05:57.616,16:06:54.541 and16:07:29.777 each pin **15 webextras/probe-owned files only**. None contains `arc/api/arc_sessions_restore.mm` or `arc/wp/sessions.gni`. The journal likewise provides no sessions source pin. These snapshots must not be expanded retrospectively into an all-Arc/native freeze.
- Package log identifies the built ArcCore framework and successful packaged app. `results/webextras/native-run10/ours156/binary-identity.json` captured16:07:10.442 pins the launched framework to **28beee762b3858ea197a87736aca21b5cbbfa01ef897ca6f0e747d91568eee5c**, while explicitly reporting `sourceSnapshot: unknown` and launch-file identity provenance. That is an exact packaged binary identity, not an e973 correspondence seal.
- The parent's independent source watch reportedly kept e973 unchanged after promotion. Combined with these later build times, inclusion is a reasonable **inference**, not exact peer-recorded sessions-object/source provenance. This audit does not inspect Ninja journals/object metadata or reconstruct a missing whole-source freeze.

Run summary has complete=false/parity=false; the helper fixture is EXIT1/inconclusive. Neither a completed build nor that binary launch establishes execution, behavior or acceptance of discard/finalize/selected-adapter lifetime code. No unrelated helper debugging was performed. No app/build/attach/source mutation or board access occurred.

Evidence SHA-256 pins:

- `logs/wp/webextras.native.run10.engine-build.log`: `7b5b3ee570e11a3c3634edb73441a5f3dd54a208d839cf103e2b3dc7b3dff403`
- `logs/wp/webextras.native.run10.package.log`: `1a304afdc8d1b0487ba4758ce360f5e5b8ce8cf9eefbbd7c81b3b83b65c93bf3`
- `logs/wp/webextras.native.run10.source-before.json`: `650cdc1d26a0e81c52fad5af41cf70045b73768fc056dcbcdf0a7dbb18f4f5e3`
- `logs/wp/webextras.native.run10.source-after-build.json`: `060bf4cdc33c04793849009bf1896c7b2abf1f2ab97f07382db9b67b19dd1124`
- `logs/wp/webextras.native.run10.source-after-run.json`: `26890637b05d6c4505159fde3ffdf212d9469074e1be0137f4d7a8fa345552c8`
- `results/webextras/native-run10/ours156/binary-identity.json`: `33d7dd2e58e4130f970657613ec71dbf6a64ee1c84e2cb5ec4ad90f40c9ea391`
- `results/webextras/native-run10/summary.json`: `0360a95e7f20fa008884a0441298f6fb5e6a584187cd7ef8b9a335ed9c42d35f`
