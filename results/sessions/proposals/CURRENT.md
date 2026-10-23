# Sessions proposal choices

Updated 2026-10-23. All proposals are **UNAPPLIED**. Current owned source is `e973b371`; current core WebContents is `47534304`, internal header `70495f85`, native hook header `a6bb3a03`. Full pins are in the linked proofs. The sessions source passes actual and official syntax checks and post-promotion review.

## Direct current-base option

[Current live-base proof](current-six-live-base-e973/README.md): both original six-patch alternatives apply without core11, accounting or private core headers. Twelve strict checks and twelve temporary applications pass; both owned syntax checks exit0 with empty diagnostics. Fresh independent review passes. This establishes applicability and owned syntax independence, not native compilation, linkage or behavior.

Choose one column and apply its rows in order:

| Step | Source only | Source plus startup policy |
|---|---|---|
| 1 native source | [original source hooks](source-policy-20261023T1217Z/arccore-sessions-native-source-hooks.patch) | [original source/policy hooks](source-policy-20261023T1217Z/arccore-sessions-native-source-policy.patch) |
| 2 owned source | [source registration](source-policy-20261023T1217Z/arccore-sessions-source-registration.patch) | [source/policy registration](source-policy-20261023T1217Z/arccore-sessions-source-policy-registration.patch) |
| 3 native WINDOW | [factory](factory-20261023T1221Z/arccore-sessions-factory-native.patch) | [factory after policy](factory-20261023T1221Z/arccore-sessions-factory-native-after-source-policy.patch) |
| 4 owned WINDOW | [factory](factory-20261023T1221Z/arccore-sessions-factory-owned.patch) | [factory after policy](factory-20261023T1221Z/arccore-sessions-factory-owned-after-source-policy.patch) |
| 5 native TAB | [TAB factory](tab-factory-20261023T1232Z/arccore-sessions-tab-factory-native.patch) | same patch |
| 6 owned TAB | [TAB factory](tab-factory-20261023T1232Z/arccore-sessions-tab-factory-owned.patch) | same patch |

The proposed factory scope is normal ROOT WINDOW plus ungrouped ROOT TAB. Grouped TAB, ROOT GROUP, partial/app-window routes remain unendorsed. Lead decisions remain: shared/native adoption, weak/cancellation safety adaptations, canonical-browser identity versus original distinct shells, and startup policy. The policy alternative additionally needs the original-order policy snapshot after isProduction/before loadProfiles and configure call before ChromeMain/profile initialization; these six patches do not supply that shared app call.

[Extensions' combined check](../../../logs/wp/extensions-profile-observer-sessions-six-live-composition/README.md) verifies both original alternatives commute with E4223 across13 slots, with full reversals to the live bases. No combined compiler or runtime ran. After selection, its affected header consumers need focused checks and a fresh runner manifest; the old E-only runner rejects combined headers.

## Option after exact core11

[Current core11 + six proof](current-six-compat-e973/README.md) uses core's after-progress generic `ac1b3d7e` and observer `b222ff87`, and preserves the exact16-file core checkpoint. Use its after-core11 first-native variants `fd6068db` / `fc43a59b`; the remaining five sessions patches retain their pinned bytes. Both17-patch routes pass34 checks/applications and two owned syntax checks. Core adapter prerequisites remain HOLD. Do not mix the two first-native baselines.

## Held accounting and acceptance

The historical seventh accounting patch `3a0cd2ca` is omitted from both current proofs. Its old body context changed in e973, and its CloseState-only lifetime does not provide the selected-registration or real logical-owner contract. The current precise adapter local covers the sessions callback/decrement; a future outer core counter needs its own selected-owner lease. Both live and absent-registration native tasks retain the functional logical peer; absence queues an uncounted no-op. Setter wiring, logical lookup/retention and qualified discard rebind remain missing. See [native evidence](../native-evidence/discard-registration-accounting.md).

Ordinary core close is still deferred. Accepted close/reopen and restart fixtures stop at their synchronous-close assertion; canonical restart launch2 is unreached. No proposal here grants close activation, shared access or acceptance. Historical seven-patch proofs remain preserved with their original source pins. Full status and exact diff headers: [sessions report](../../../logs/wp/sessions.report.md).
