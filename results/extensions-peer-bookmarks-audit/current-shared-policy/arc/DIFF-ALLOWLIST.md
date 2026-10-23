# Allowed boundary differences (ours vs Arc's engine)

Each entry: what differs in `tests/recorder/diff.py` output, and why it is not a behavior difference for the
app. Only the lead adds entries; packages request them in their reports.

| Difference | Reason |
|---|---|
| Engine-internal constructors taking a native pointer (`initWith…:` with `"<ptr>"` args) are absent | Arc's engine builds wrappers from its C++ objects; ours builds them its own way. The diff already reports these as `internal`. |
| `-[ArcBrowser context]` (and other getters returning ArcBrowserContext) returns the same object each call; Arc's returns a fresh wrapper | We keep one wrapper per profile (ARCHITECTURE.md, Identity). `isEqual:`/`hash` compare the profile ID, so app comparisons are unchanged, and per-profile service objects stay consistent. Compare these returns by `-id`, not by object number. |
