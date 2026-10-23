# PiP session comparison feasibility, canonical media-pip/current-run1

Read-only finding: these captures support a **bounded logical PiP episode mapping** through public owner/lifecycle edges. They do not expose an immutable native PiP session identifier or recover native pointer equality. This is a concrete basis for the lead-authorized same-session policy, not permission to equate every controller of this class.

Lead message 143529, captured in `logs/wp/webextras-lead-policy-143529.json`, keeps our PiP controller canonical. `arc/DIFF-ALLOWLIST.md:13` permits comparison by the underlying same WebContents / PiP session. The designated omnibox integrator owns comparator implementation. This note changes no policy, source, recorder, comparator, transcript, index or runtime state.

All sequence numbers below are original raw `seq` values, not file line numbers or renumbered events. Gold is `results/golden/arc-1.170-media-pip/launch-1/record.jsonl`; ours is `results/webextras/media-pip-current-run1/launch-1/record.jsonl`.

| Public evidence | Gold | Ours |
|---|---|---|
| Sole browser-created fixture WebContents | 3042: Browser89 → WC118, active=true | 3216: Browser76 → WC102, active=true |
| Owner's media session | 3067/3068: WC118 → MS128 | 3249/3252: WC102 → MS112 |
| Media-session delegate | 3069/3070: MS128 → D126 | 3253/3256: MS112 → D110 |
| Controller-created callback on that delegate | 4156/4195: D126, [WC118, C148] | 4284/4323: D110, [WC102, C132] |
| Show callback on that delegate/owner | 4228–4313: D126, [WC118] | 4358–4455: D110, [WC102] |
| Nested owner → media session | 4269/4270: WC118 → MS128 | 4399/4400: WC102 → MS112 |
| Nested media-session getter | 4271/4274: MS128 → **C150** | 4401/4404: MS112 → **C132** |
| Hide callback on that delegate/owner | 4626/4665: D126, [WC118] | 4766/4805: D110, [WC102] |

Here C denotes `ArcWebContentsMediaSessionPiPController`, MS `ArcWebContentsMediaSession`, and D `ADK.WebContentChromium`. Each transcript contains exactly one controller-created, Show and Hide call; neither contains `webContentsPiPControllerDestroyed:`. The latter selector exists in frozen callback metadata. Hide is not evidence of destruction.

An independent full raw stack scan matched call/return `(thread, depth, from, dir, kind, cls, sel, self)` with zero errors in both captures. Both getter calls have the actual Show frame as their immediate parent; their returns match their receivers/signatures. The only unreturned frame is the process-long `ArcBrowserApplication.run` (gold47 / ours27). No return was invented. Thus the association uses dynamic callback containment and explicit receiver edges, not row proximity or `<ptr>` initializer arguments.

Cross-run owner correspondence is also public: each run has one browser/WebContents creation role and one app-observed URL owner for the fixture `/a` (ports 54613 and 49979, handled by the existing local-port policy). Three paired WC.sessionId → ArcSessionID.identifier reads yield the same positive integer within each run: gold 407265285 at 3232/3236/3290, ours 367162046 at 3420/3424/3486. These unequal raw integers are subject to the existing per-run bijection; integer equality or first appearance alone is not a cross-run identity proof. The creation role, unique scenario URL, owner/session graph and lifecycle edges jointly support pairing this one fixture owner and its one observed PiP episode.

## Narrow comparator requirements for a later proposal

A typed prepass can associate the controller-created argument and the returned controller of the same owner's media-session getter **inside this balanced Show callback**, using the proven delegate and active creation episode. Under the lead policy, gold C148/C150 and ours C132 then represent the corresponding logical episode. Reuse existing caller admission and normalization; inspect raw receiver/ancestry proof before admission without changing raw records. Exact metadata methods have object returns (`mediaSession`, `pipController`, `videoView`); never interpret arbitrary strings merely because they look like object references.

For this bounded route, missing/malformed/conflicting owner or getter edges, ambiguous corresponding owners, another controller creation/replacement, an intervening Hide/destroy, or a getter outside the proved Show scope must leave the ordinary comparison different/unsupported. Do not extend the alias after Hide, through destruction/recreation, to another WebContents, to arbitrary controller payloads, or to an unobserved future session. Supporting those cases needs additional public lifecycle evidence and review. This restriction is a conservative implementation scope, not a claim that Hide destroys native PiP.

Apply correspondence only to exact proven object-typed comparison slots. Keep all calls, returns, receiver proof, nesting, order, argument values, callback sequence and counts. No broad class/object-number mask, fake pointer identity, constructor `<ptr>` equality, scalar tolerance, or event deletion is warranted. Negative controls should change the owner/delegate, break pairing/ancestry, insert replacement/Hide/destroy before the getter, and use an unrelated/reference-shaped string; each must reject aliasing. Valid aliasing must still detect getter payload, callback argument, count and sequence changes.

## Remaining implementation versus policy work

Exact staged comparator `24437bed…` has no PiP-specific mapping. Its generic per-class first appearance in arguments/returns assigns gold C148→#1 and C150→#2, but ours C132→#1 throughout; the existing report's `pipController` return mismatch is therefore explained by this canonical-versus-fresh wrapper distinction. `download_identity.py` proves download/source-WebContents aliases only; it supplies no PiP-session mapping. A reviewed typed comparator increment is needed for this identity difference. Allocating a fresh engine wrapper solely to copy these object numbers would conflict with the current canonical policy.

This finding establishes neither whole-scenario parity nor a general lifecycle guarantee. Both captures have videoView twice and sourceElementBounds once inside Show, returning their respective same PiPNSView and the same `{{144, 453}, {128, 128}}` geometry. Keep the actual remaining differences: total engine→app callbacks **172/170**, PiP feature-state callbacks **92/72**, PiP source-title callbacks **2/1**. PiP playback callbacks are 6/6 and media-position callbacks 4/4. Internal PiP constructors are 2/1 and already excluded by caller policy, not by a new identity rule. The `ArcMediaPosition.currentTime` difference (0.322324 / 0.422503) and missing canonical timing anchors remain as described in `webextras_timed_comparison_requirements.md`; a session alias does not repair or normalize time. Source getter/cache timing is separate from immutable position-value math.

The stored current-run1 report is historical output from its own comparator invocation, not a new execution of 24437 or an acceptance claim. Its first two lines remain:

```text
methods differing: 50 (missing 8, extra 1, returns 12, callback args 4, counts 25; internal-only 0, ours-internal 1)
callback sequence: {"golden": 172, "ours": 170, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

## Input identity

SHA-256 pins below were rechecked after this note was written. The staged recorder files are under `logs/wp/webextras-boundary-arguments-callback-correction/staged/tests/recorder/`. No comparator was executed, and an independent read-only factchecker confirmed the owner/episode graph and raw pairing/counts.

| Input | SHA-256 |
|---|---|
| Gold record | `672f405866b7904b0ae0b996de12ef6496c0121b495450fede4a92acdb1d954f` |
| Ours current-run1 record | `5760c41eef96ce0bfd05477cc86477fda80f687fc20a66e3952c207472696ff1` |
| Stored current-run1 diff-launch-1.txt | `8143b1921bff97363a459561817b6a5adb18418984628af2ec8e2990de69c77c` |
| Staged diff.py | `24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf` |
| Staged download_identity.py | `5318e64f44d9f7549227ab893185c083a34dd835b01be0ba5552d0940631ccea` |
| Staged app_callbacks.inc | `52b3687bd7f574acb34f8c3c5fcf6750e0fc044c9bf159a6971531a952830748` |
| arc/tools/arccore-1.170.0-objc.tsv | `9ba3d4750f15e80ca6836966af5ec50523ccfe5e0ed3c22faa31f8b347492cf5` |
| Lead policy capture | `345d9a5ade98c35d5e11828d3dd8e5ce7c024979f9534d66ff78c0274ac6338b` |
| arc/DIFF-ALLOWLIST.md | `a232b6fb3e130669ff2b2ccdbed0ae6ae76cc6aade40be6818b4819e09c3c4cf` |
| arc/ARCHITECTURE.md | `b50a871a73b33c32481937976642fc10510e90ab9e59dd26a2c434c90aa0ee15` |
| arc/wp/COMMON.md | `eefc2d44a17e53acbbab195da8120bcd002074ec51d4577c65d14fa000b7324c` |
| arc/wp/webextras.md | `52dd960ebb4d1c03802b7660df2e4e149f29aa8cd3fb99f6ffa5b43a126e49a1` |

Request to lead/designated comparator owner: use this graph for a reviewed narrow same-episode comparison, or explicitly require stronger identity evidence. The raw captures cannot establish native-pointer equality. No architecture/spec contradiction was found for keeping our wrapper canonical; reopen/replacement identity and complete media-pip parity remain uncovered.
