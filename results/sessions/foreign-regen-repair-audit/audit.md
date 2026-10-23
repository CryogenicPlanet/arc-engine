# Foreign patch regeneration repair: provenance audit

No new foreign-source blocker established. Eight foreign patch outputs changed bytewise from malformed v4 entry preimages, but every current output is byte-exact the accepted core11 canonical receipt. Canonical stock-HEAD to current-source reconstruction succeeds for all28 foreign source sections. The malformed entry payloads also match those current sources: representation changed, not foreign source behavior.

| Foreign patch | V4 entry SHA256 | Current / accepted core11 SHA256 |
|---|---|---|
| 0001-sdk-26.0-compat-isDeviceConfiguredForPasskeys.patch | `0dc0de36d7348f8865f3bdd0fae39bdc36745010bb601ab693aed0549672e617` | `ea0621842c64ef8d8bb8268eb91fc7e4b9cdf4981735ecc5f575b2a6ae24b20b` |
| 0002-arccore-framework-layout.patch | `9e26cf5ba25689aa193c248c8f3b5ef4583f4c1b4b8b6ea071acafb5ed966989` | `90ebff3e9e0129690a6ab66c650ded9e2e73b3007bfd110c25966aa2084f2f79` |
| 0003-arc-host-app-mode.patch | `cb07fd9591813aa149897bc232dcfe8a9d47f15b7c37f6894dbb60330d0f37d7` | `2c016d9311e5ce75bbbdf0dae73e7f85025106875fc435e91ff65d5514ac9326` |
| 0005-arcmod-renderer-agent.patch | `bd05791f1aac3b4b946da37cd1ddf9c9a04b1d67d11d04653d5a21767b2ca82d` | `5f4acaebedbdd82fa66bdf2b0829e7e7e63d2701f87d636a2119b4252eddd63e` |
| 0006-arcmod-host-tolerance.patch | `166535f0ba94c9ca522b9bb44f2c7862be9ab337e9df44bd6c11632b64a616ae` | `1362de94c484429a0e654e6832e473728325e3a06109c90430f9fa17a2111827` |
| 0010-app.patch | `ce55565a7194416cdafd3e0c1f542ad432309b57d2e9c28d548e7c1328de71b9` | `0d875af65c2d9ff66921684fe0361cb2ce562c5771b4f0d889ff3953a76b24b2` |
| 0010-core.patch | `61c91e68db3ab3c49fd3bb1ff010bc5efe28d9be33cd96e970a612216d26f02b` | `6dcb1ab6ba7cc9f80f1df088ac2fbf68031aad94da9b91ebad0bc362aff8ef4c` |
| 0010-extensions.patch | `bff8175b1506d72c9dfdd2628b6b7ad3c2f4e04133631f60718570ba9e403b3e` | `6104e1223dea5178439c07e8d85c43ddc8635716a20f6e15bd66dc377a2c3dfa` |

27 of28 entry sections incorrectly describe files already present in Chromium HEAD as `/dev/null` new-file additions. The remaining new-file section is genuinely absent in stock. Current patches reconstruct the exact live file bytes from stock HEAD; no private or real Git index was written. Git reads used GIT_OPTIONAL_LOCKS=0.

Chronology is bounded by saved byte-verified snapshots:

- V1 entry17:36:45.728120Z: all eight canonical; aborted without writes.
- V3 entry17:44:16.193689Z: all eight canonical; aborted without writes.
- V4 entry17:47:44.116814Z: first inspected malformed preimage set.
- V4 regen17:47:59→17:48:05 restores canonical eight hashes.

The responsible writer and exact intervening mutation time are unknown. No attribution is made. V3 failure text was not printed.

Source seals:69 broad Arc entry paths remain equal except authorized sessions e973→178091. All48 selected core11 source paths remain equal except the authorized shared arcmod_hooks.h union. Removing only the approved insertion strings yields exact entry/core11 hash e0fde96081267d16179b35f804c46a35cbf1491ade90fc7b6240457682cb4602. Separate E/GCD/history receipts corroborate current extension6104, app0d875 and history1362 outputs; older GCD0003 is superseded by the newer core11 receipt.

Shared0004 nowac9a3340… includes the approved sessions header addition over canonical core11dd5e7073…; own0010-sessions is newly generated d2153493…. They are separate from the eight foreign output repairs.

A prior statement that all foreign outputs were byte-identical to v4 entry was false. The correction is to acknowledge canonical repair and the preserved source seals, rather than roll back accepted canonical outputs to malformed full-file additions. Nothing in this audit requires interrupting runtime for a foreign-source mutation. Parent alone owns transaction/rollback decisions.

Scope: provenance and reconstruction only; no build/query/app/process/lock/Chirp actions or production writes. Only this audit directory was written. No runtime or source-only acceptance claim. Exact raw paths/hashes and per-source reconstruction results are in audit.json.
