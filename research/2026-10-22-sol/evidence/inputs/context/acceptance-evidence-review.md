# Engine swap evidence and promotion gates

An independent GPT-6.1-Sol reviewer inspected the saved JSON, Markdown, test scripts and all four screenshots for `results/engine-swap/20261023T0501/`. The review did not launch apps or rerun tests. The inspected artifacts support narrow smoke-test parity between two running ArcMod agent sessions reporting Chromium 155.0.8059.40 and 156.0.8078.48. They do not establish normal-profile migration safety or compatibility with a replacement engine built from source.

The source and results were introduced by engine-repo commit `d5d7efe`. Paths below are relative to `/Users/cryogenicplanet/general/arc-engine` unless noted otherwise.

| Evidence | What it establishes | Boundary |
| --- | --- | --- |
| `tests/engine-swap/suite.ts:142` compares serialized values | Equality between captured results | No pass/fail assertions; identical errors, false values and missing APIs can be equal. Runner exit alone is not acceptance. |
| `suite.ts:60` negotiates `requestMediaKeySystemAccess` | Widevine key-system negotiation in those sessions | No MediaKeys, license exchange, decryption, protected playback, seek or renewal was exercised. |
| `suite.ts:61`, results JSON | Both WebRTC loopback/data-channel probes timed out | ICE/candidate/connection states and candidate-add failures were not saved. The mDNS/macOS permission explanation is a hypothesis; cause is unresolved. |
| Both `swap155-pdf.png` and `swap156-pdf.png` | The simple one-page fixture rendered in Chromium's PDF viewer | The original DOM/embed probe was inconclusive. Search, copy, forms, links, print, viewer download and larger PDFs were not checked. |
| `tests/engine-swap/ext-test.ts:29`, fixture content script | The report says one unpacked extension loaded via privileged CDP and set its DOM marker | No saved extension output in this run directory; script logs rather than asserts. No normal installation, persistence, background messaging, storage, popup UI or native-host coverage. |
| `suite.ts:50` through `:58` | Same-page storage/cookie access, IndexedDB write transaction, service-worker registration, key generation, GPU API availability and advertised codec support | Not restart persistence, IndexedDB readback, service-worker activation/offline behavior, GPU rendering correctness or decoding of those advertised codecs. |
| `suite.ts:59` | Tiny generated VP9 video playback | Does not generalize to representative files, audio, protected media or sustained playback. |
| `suite.ts:62` and `:63` | Notification permission enum and clipboard method presence | No actual prompt or clipboard operation. |
| `suite.ts:126` | A download filename existed | File content/hash was not validated. |

The retained directory contains Markdown, JSON and four PNGs. It does not retain launch commands/ports, binary hashes, signing/entitlement identity, isolated-profile provenance, test revision at execution, durations, raw run logs or crash-query output. The report's no-errors/no-crashes assertion cannot be independently audited from this directory. The recorded Chromium versions come from user-agent data rather than loaded-image hash attribution.

Native dialogs, permission prompts, Little Arc, the shortcut matrix and a copied real profile are explicitly outside this run's scope. Existing ArcMod runtime evidence in `../arc/notes/runtime-status.md` applies to its recorded build. It also records incomplete shortcut/state coverage and unresolved automatic startup restoration; those baseline limitations must be preserved when evaluating an engine swap.

## Promotion requirements

1. Repeat a comparison with recorded app/engine hashes, signing identity, launch flags, test revision, isolated-profile origin, raw outputs and logs. Expected outcomes must be assertions. Retain a working rollback bundle and an untouched profile snapshot.
2. Use a disposable copy of a real ArcMod profile to verify sign-ins/cookies, history, passwords/keychain, extension state, storage/service-worker data, Spaces/pins/tabs and settings through multiple clean restarts. Inspect migration/data-loss errors and use a separately restored snapshot for rollback; do not assume downgrade safety.
3. Run native command-bar/URL-copy, close/restore, navigation, Settings, profiles/Spaces, tabs/pins/splits, external routing, Little Arc, dialogs, printing and download UI checks on the signed candidate. Record baseline failures separately.
4. Verify actual protected-media licensing and playback, representative codec files/streams, audio, seeking, fullscreen and sustained playback.
5. Instrument WebRTC ICE/TURN/permission states; verify loopback and a real peer call/data channel, camera/microphone, screen capture, denial/retry and device switching.
6. Exercise representative extensions through normal installation and restart, including blocking, popup/action, background messaging/storage and required password-manager/native-host flows.
7. Check larger/searchable/form PDFs, print/save, functional clipboard operations and prompts, HTTPS/certificates/cross-origin behavior, GPU rendering and a sustained mixed-workload session with retained crash/log evidence.

These are proposed validation gates. The review did not perform them.
