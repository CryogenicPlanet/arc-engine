# ROOT TAB live-group destination probe — public fixture feasibility

Status: HELD / currently unsupported by a proven public fixture path. Read-only scout; no implementation, extension fixture edits, builds, apps, queue, Chirp, credentials or private native ABI were used.

## Conclusion

A single native ArcBrowser containing two real native-group members A and B would suffice. Close only A individually, keep B and a sentinel alive, and application restore of A's actual historical ROOT TAB ID enters the native nil-context path. A surviving native group in B's browser can redirect restoration there, without creating a browser. Two browsers are not inherently required.

However, the permitted current public surface cannot establish that initial native TabStripModel group. App/extension group DTOs and positive extension group IDs are not a native-group oracle. No runtime probe should be scheduled from this plan until a genuine native-group producer and observable verification route is established.

## What is proven and where

- Local MV3 loading through public CDP Extensions.loadUnpacked, a loopback content script and chrome.tabs.query are covered by accepted extensions-page-actions. tests/scenarios/extensions-page-actions.ts and tests/scenarios/REPORT.md:10.
- Existing fixture permissions include tabs and tabGroups: tests/scenarios/extension-fixture/manifest.json:1. Its background groups request calls chrome.tabGroups.query({}): background.js:7.
- Existing extensions candidate does not establish group functionality: tests/scenarios/REPORT.md:281 says tabGroups.query never completes. Candidate record results/golden/arc-1.170-extensions.candidate/launch-1/record.jsonl:4025 shows engine->app browser:getAllTabGroupsWithCompletion:. This is candidate evidence, not a canonical group golden.
- Generic C156 TabsGroupFunction would call TabListInterface::AddTabsToGroup and create real native groups: chrome/browser/extensions/api/tabs/tabs_api.cc:3368–3512, especially3503–3509.
- Hosted Arc replaces that API: arc/browser/extensions_groups.mm:772 declares HostedTabsGroupFunction; its create route calls the app's browser:createTabGroupWithSessionIds:properties:completion: at875–878, with session IDs. Registration at944–952 replaces tabs.group, tabs.ungroup and tabGroups get/query/update/move. It does not call native AddTabsToGroup on that route.
- Reported chrome.tabs groupId is likewise insufficient: chrome/browser/extensions/extension_tab_util.cc:385–395 first reads native membership, then the Arc hook overwrites it with the app-owned mapping. tabs.query({groupId}) has the analogous override in tabs_api.cc:2026–2046.
- Original native group read/create adapters also route to app delegates; arc/browser/extensions_group_readback.md:57–79 records original GetAll/numeric lookup. Its native enqueue-false evidence at112–120 gives create failure No browser delegate, rather than a native-group creation fallback. Removing/replacing the delegate therefore is not a supported escape to generic Chromium grouping.
- ArcBrowserDelegate public protocol exposes app group requests, not a native group setter: research/2026-10-22-sol/evidence/agents/protocols/initial/protocol-specs/ArcBrowserDelegate.md:40–46.
- Exact public metadata arc/tools/arccore-1.170.0-objc.tsv exposes ArcTabGroupResult/Info DTO properties, ArcSessionTab.groupId readonly (2832/2842), ArcSessionWindow.tabGroups readonly (1167/1175), and ArcSessionTabGroup getters. It supplies no public ArcBrowser/ArcWebContents native-group mutation selector. Session snapshot constructors/native handle properties use unavailable C++ ABI and cannot be fabricated as a fixture.
- A genuine foreign ArcSessionTab snapshot could carry native group data into restoreForeignTab, but no account-free local producer of such a genuine snapshot is established in this baseline. Do not invent a SessionTab/shared_ptr handle or infer native group membership from an app DTO.
- Public CDP Target/Page/Browser surfaces examined do not expose a native tab-group creation command. Evaluating private WebUI Mojo, disabling the host API override, invoking private native Chrome entrypoints, or creating an unobserved unhosted browser would introduce a different/unproven surface and is not an accepted alternative here.

The statement is absence of a proven permitted fixture path, not a claim that every conceivable internal Arc route is unreachable. An app-delegate group operation might create an Arc folder, but success would not establish the native group needed by restoration. No app-folder mutation was attempted.

## Native destination evidence, once a genuine group exists

Original evidence /tmp/arccore-sessions-native-context-evidence.md:
- Application restore passes nil context and NEW_FOREGROUND_TAB at42bf0f8–104.
- Native RestoreTab skips old browser_id destination lookup for nonzero disposition at6c86ee8.
- Native live-group lookup6c86f04–20 may choose existing B within the profile; B skips creation at6c86f24–2c.
- On bounded successful redirection, the app manager discards a nonempty return without created(B), finished(B), or finished(nil); its pending callback remains active. Existing B insertion stays ordinary asynchronous didCreate. No eventual completion behavior after unrelated browser creation/cancellation is established.

Current source comparison: components/sessions/core/tab_restore_service_helper.cc:1538–1552 (disposition/group destination choice) and chrome/browser/ui/browser_live_tab_context.cc:595+ (native FindContextWithGroup). The new owned ungrouped TAB factory proposal correctly excludes group-present roots.

## Conditional safe probe plan — NOT IMPLEMENTED/AUTHORIZED TO RUN HERE

Only proceed after an independently verified public native-group producer exists.

1. Under the parent-owned lock, launch only disposable original trace156 with exact bundle/executable/PID/birth/readiness/nonce guards and isolated profile. Keep one loopback nonce server. No AX, activation, global input, private C++ calls or recorder selector replacement.
2. Use a single captured actual ArcBrowser. Create A and B local history fixtures and an unrelated sentinel in that browser. The genuine producer must establish A and B in the SAME actual native group and provide a native membership/liveness oracle independent of overridden extension groupId/app DTOs. If this cannot be established, stop before any restore request.
3. Close only A through genuine ArcWebContents.close while B and sentinel remain alive. Do not close the group or browser. Capture exactly one main-thread UIDelegate webContents:didCloseWithSessionID: for A's exact pre-close wrapper, forwarding the original once; retain the actual positive historical ID and pre-close live ID via NSInteger getters. Archive raw timing. Require old A uninitialized, B/sentinel still initialized, no browser close, and known ID eligible. Never use -1, a guessed ID, ROOT GROUP ID, or synthetic Session DTO.
4. Invoke restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback: exactly ONCE with captured public context and A's historical TAB ID. Blocks record raw identities/events without creating a window or installing another browser delegate. Observe B's existing public browser delegate and URL/history. The expected static route is one ordinary asynchronous didCreate on B, not a full didRestore callback.
5. Verify returned tab URL/history, B browser public identity/context and surviving native group through the independent oracle. Observe created/finished counts for a defined bounded interval, e.g.5seconds after the insertion. A zero count proves only no callback within that interval; it does not prove eventual noncompletion or success of a completion contract.
6. Do not submit another application restore or create an unrelated browser while the native pending request remains active. Keep callbacks/test state alive until actual process exit. Never synthesize finished(B)/nil or clear native pending state as a workaround.
7. Mark observation and cleanup phases separately. Main-GCD NSApp terminate after response acknowledgment is cleanup, not a restore callback. Termination could indirectly create an empty replacement window and consume native pending state; record any callbacks during cleanup separately, never attribute them to the earlier observation window. Require bounded actual owned-PID exit, with exact PID/executable/birth fallback cleanup owned by parent. Do not await finished to trigger cleanup or activate/close arbitrary windows.

## Uncovered/unsupported requirements

- Genuine native group creation and a trustworthy public native membership oracle are missing.
- Hosted extension tabGroups query is historically unresolved and cannot be silently treated as passing.
- App-owned group mappings may conflict with Chrome native membership; forwarding their delegates exactly once does not make those concepts identical.
- Foreign-session group creation needs a real producer snapshot absent from this baseline.
- Existing-group B pending callback lifetime after the bounded observation window and shutdown remains unresolved.
- ROOT GROUP nil-context behavior is never invoked. Static fault matching or crashes are not an acceptance workaround.

Recommendation: retain existing-group B as statically characterized, runtime-uncovered behavior. Do not add a fixture extension solely to manufacture app group success, and do not broaden the current root-TAB factory token to grouped entries based on that success.
