# Post-adoption v6 restart audit

**PASS for the unchanged supplemental v6 fixture. Canonical acceptance remains false.** This audit launches nothing and makes no independent parent-reaping claim.

The same fresh disposable profile completes two launches, PID **14683 → 15413**, inside one cache-v10/profile-v10 isolation transaction. Both ready and shutdown sidecars match their PID, nonce, request ID, ours156 bundle and saved state. Both original wrappers become uninitialized after exactly one genuine main-thread close callback each.

| Tab | Source live ID | Saved historical ID / restored callback ID | New live ID |
|---|---|---|---|
| tab1 | 1285773345 | 1285773347 | 1285773403 |
| tab2 | 1285773346 | 1285773348 | 1285773404 |

After the new PID, both persisted IDs are eligible. Restoring tab1 consumes only tab1; tab2 remains eligible until its own restore. Each callback runs once on main after the restore action unwinds, names the historical ID rather than original/new live ID, and delivers an initialized wrapper at its exact B URL. Both restored tabs coexist, retain exactly A/B/C and titles with B selected (index1), and the fresh sentinel remains initialized. No unmatched restore event appears.

The before/after-first-quit sidecars retain identical opaque files: SNSS v5, 6487 bytes, SHA `127b7d11d333443c525258fd25f592c034d9759f7b39f822310fe06fa40eba9a`; SNSS v3, 6355 bytes, SHA `7b55804567608c595ab6ab684a124a04c72e1ff1aa2dd3f6977bbac28ac45408`. Both are under the same disposable Agent Chromium Data/Default profile. The probe neither decrypts nor identifies fixture content in those files; new-process native eligibility, callbacks and histories provide the logical persistence evidence. One outer isolation pair and unchanged fixture launch path preserve the fresh profile across both launches; per-launch root inode values are not serialized.

Both shutdowns record matching AppKit nonce/PID acknowledgments and browser absence, at **265 ms / 276 ms**, with no signal fallback or cleanup error. The runner records exactly one original cache restoration and one profile restoration, no refusal, restored root identities/absence and exact-agent quiet. This relies on the pinned helper's device/inode check plus receipts; it is not an independent OS exit-status capture. Both scoped app logs have no fatal/crash marker; unrelated sandbox/stub diagnostics remain visible.

The fixture `.ts`, `.mm`, `.dylib` and storage snapshot still match all four frozen original v6 hashes. Saved input maps agree: **46** frozen local tool/fixture pins, **178** entry source/build inputs, **180** before/after inputs (only the two generated API .inc files added after generation), **226** total runtime pins. Source178 and all saved source hashes remain exact across the runtime. This is not a complete native/toolchain freeze.

Framework records are unchanged before/after: built `7536f5e29fc879472956e96dc7042947ff6862a4e62c6a69ef922b1541207ddb`; packaged `dc446842d5f84dc02ab01da5528d7e85cff51629a8a14db06fa447e62a2f0060`. This audit compares the saved records, without rehashing live 650MB binaries. Build/package/codesign commands and fixture child record exit0; status complete/pass, cleanupVerified and inventory-stable are true, with no deferred signal.

The recorded individual close callbacks are asynchronous (`insideAction:false`), matching this existing supplemental route. They do not fix the canonical inline/synchronous-close failure. No standard recorder transcript or golden diff is produced here. Ungrouped ROOT TAB destination creation, open/full-window startup parity, cancellation/reentry, foreign DTOs and discard/logical ownership are not covered. Exact file hashes and all checks are in review.json.
