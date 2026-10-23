# Allowed boundary differences (ours vs Arc's engine)

Each entry: what differs in `tests/recorder/diff.py` output, and why it is not a behavior difference for the
app. Only the lead adds entries; packages request them in their reports.

| Difference | Reason |
|---|---|
| Engine-internal constructors taking a native pointer (`initWith…:` with `"<ptr>"` args) are absent | Arc's engine builds wrappers from its C++ objects; ours builds them its own way. The diff already reports these as `internal`. |
| `-[ArcBrowser context]` (and other getters returning ArcBrowserContext) returns the same object each call; Arc's returns a fresh wrapper | We keep one wrapper per profile (ARCHITECTURE.md, Identity). `isEqual:`/`hash` compare the profile ID, so app comparisons are unchanged, and per-profile service objects stay consistent. Compare these returns by `-id`, not by object number. |
| Positive native SessionID scalars differ between runs | Only retained `->engine -[ArcSessionID identifier]` returns and argument index 1 of `engine->app -[ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:]` calls share a first-seen bijection per transcript, after the existing caller/selected-fixture admission filter. Chromium SessionID is signed int32: only exact JSON integers 1 through 2147483647 become aliases. Repeated/distinct identity across both fields, all counts/order, 0/-1 and other invalid values/types, callback returns, and unrelated fields remain compared. This does not normalize extension group IDs or tab/window IDs in other APIs. |
