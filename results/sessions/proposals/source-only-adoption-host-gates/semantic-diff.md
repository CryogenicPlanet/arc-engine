# Semantic scope

The additive patch changes only `components/sessions/core/tab_restore_service_helper.cc` after the source-only six-patch chain. It includes generic `base/command_line.h`; it adds no Chrome-layer include or hook to components.

`RestoreEntryById`: the new helper weak token is obtained only when `--arcmod-host-app` is present. Post-RestoreTab, post-ShowBrowserWindow, post-factory and final helper-liveness returns are host-only. Null-context cancellation and root-entry identity validation are host-only. The cancellation return after notification is explicitly host-only.

`RestoreTab`: the weak token, source/root lookup and early `*live_tab = nullptr` publication occur only for host. The new post-factory weak/null/root-identity returns and final weak-liveness return are host-only. `LiveTab* restored_tab` returns to its original declaration; both existing native branches assign it before its existing uses. No nonhost root lookup, new weak-token acquisition, early publication, null return or cancellation is introduced.

With the host flag absent, the original native operation ordering and branch bodies are retained. Added inactive local state and the flag read remain; no cancellation or safety guard changes the nonhost route. With the host flag present, the existing six-patch safety/cancellation policy is retained. This does not assert original-engine cancellation parity.

The source-only factory hook in `BrowserLiveTabContext::Create` already runs only for host and a nonempty factory hook. An empty/declined callback immediately follows Chrome's existing return contract. Only an accepted host callback obtains the new browser weak token and can produce its guarded null return. The source marker in `AddRestoredTabImpl` already runs only for a source record, host flag and nonempty hook; the default scope is inert. These files are verified but unchanged by this additive patch.

No source-policy/bootstrap configuration, owned sessions implementation, accounting, logical root, canonical identity, tests or production files change. The helper's previously added WeakPtrFactory field remains, but these two new weak-token acquisitions are host-only. This is integration gating, not a public Close or runtime acceptance result.
