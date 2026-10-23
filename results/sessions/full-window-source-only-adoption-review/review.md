# Full WINDOW after source-only adoption

**PASS for the supplemental normal-WINDOW contract; canonical sessions acceptance remains FAIL.** The adopted run fixes the former zero-restored-tabs/precreated-wrong-window failure. This audit is read only and launched nothing.

| Evidence | Original run3 | Prior ours run2 | Adopted ours v4 |
|---|---|---|---|
| Full contract/history | PASS | FAIL, zero restore callbacks | PASS |
| Source-matched restored callbacks | 2 | 0 | 2 |
| Exact A/B/C history, B selected, both tabs | Yes | Unreached | Yes |
| Finished callback after restore action unwinds | Yes | No | Yes |
| Created/finished native identity | Same | Same wrong destination | Same actual restored destination |
| Created/finished ObjC pointer | Different | Same | Same |
| Child-close callback IDs | Both -1 | Not recorded | Not recorded |

The new fixture maps two uniquely identified URL wrappers to one actual `Arc.BrowserWindow` / `Arc.MainWindowController`, confirms exactly two observed owned tabs and calls `[window performClose:nil]`. Its global normal-window close callback reports historical root ID **2130008821**, distinct from source tab IDs **2130008818/2130008819**; old browser is uninitialized and the close callback has unwound before restoration. This supports the genuine whole-window producer route, rather than two individual tab closes; the native Entry.type is not independently serialized.

Created runs once synchronously on main. Two restored callbacks report the two exact source IDs in order, each with an initialized new live wrapper at its own B URL (new live IDs **2130008825/2130008826**). Both restored callbacks and finished run on main after the restore action and close callback have unwound, in created → restored → restored → finished order. Created and finished are initialized with identical native browser session ID **2130008824**, UUID **CF8D5F07-7518-4DB1-9D98-B7BD9F6A6BB4** and context **Default/0**. The surviving sentinel remains initialized and can see the restoration at created time, while the restoring browser cannot restore again. Both tabs retain exactly A/B/C, selected index1, titles and B URLs.

The frozen full-v5 fixture does **not** capture child-close callbacks. Therefore this run cannot verify that its child-close IDs are -1; original-run3 did directly record both -1. It also does not establish ObjC pointer parity: original created/finished wrappers differ, ours are the same canonical object, despite matching native identity. Original global closed-window callback occurred outside perform-close action depth; ours occurs inside, as in prior ours-run2. Neither difference is silently treated as resolved.

Cleanup records matching AppKit nonce/PID acknowledgment, owned browser absence, no signal fallback, and a 315 ms shutdown interval (well below the fixture's 6-second transport-fallback threshold). The runner records exactly one cache restoration and one profile restoration receipt, no refusal, child0, restored original roots and exact-agent quiet. Its pinned code compares entry/exit device+inode or absence; those inode values themselves are not persisted in status, so this is verified runner/receipt evidence rather than an independent inode reconstruction. The full app log has no scoped fatal/crash markers; its sandbox warning and unrelated stub diagnostics remain visible. No OS exit status or complete crash census is claimed.

The runtime archived source is sessions **178091e65d07409a00eeeb86101a7234afd44f00a3597512c2b1be0095118978**. Thirteen relevant fixture/runtime/wrapper/seven-source archives were independently rehashed against expected pins. Built framework record **7536f5e29fc879472956e96dc7042947ff6862a4e62c6a69ef922b1541207ddb** and signed packaged record **dc446842d5f84dc02ab01da5528d7e85cff51629a8a14db06fa447e62a2f0060** are unchanged before/after runtime. This is scoped evidence, not a full native build-input freeze. Exact evidence paths/hashes, checks and comparisons are in review.json.

Canonical public API remains harness1 / **10 pass, 1 fail, 1 inconclusive**, and restart remains harness1 / **13 pass, 1 fail, 1 inconclusive**. Both coverage accepted flags are false; the unchanged first inline/synchronous public-close failure prevents their restoration and second-launch portions. The supplemental pass is explicitly not canonical acceptance.

Still uncovered: ungrouped ROOT TAB destination factory, grouped/split/partial/app/popup restores, created-callback cancellation/deletion/reentry and safety-unwind parity, actual restart persistence, foreign DTO metadata, discard replacement and logical ownership/accounting. The child-close -1 result requires separate current recording; no additional probe was prepared or launched.
