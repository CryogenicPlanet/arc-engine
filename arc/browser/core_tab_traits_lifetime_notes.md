# Core tab traits lifetime audit — 2026-10-23

Read-only source audit; only this /tmp note written. No production/native/shared changes, builds, apps or board actions.

## Conclusion

Existing core kInserted is early enough to bind before core's own ArcSessions/app notifications, and public TabInterface callbacks are sufficient for subsequent discard/transfer/delete handling. It is NOT the original controller construction boundary: several synchronous callback surfaces run between native TabAlertController construction and core's receipt. The inspected insertion functions contain no direct RunLoop/RunUntilIdle call, but arbitrary registered callbacks execute earlier. Consequently exact no-yield/no-peer-event equivalence across that interval is unproven. Do not promote kInserted to a source-proven equal observer-start boundary solely because the ordinary insertion stack is synchronous.

A bounded integration can choose first-insertion binding with this limitation explicit. For exact constructor lifetime, the minimum missing seam is a narrow hosted callback at native TabAlertController construction, after tab/controller identity exists and before constructor subscriptions/callouts, rather than a new broad WebContentsObserver method. Subsequent rebind/reset can use public TabInterface callbacks; no extra native hook is needed solely for those normal lifecycle events. Creation in public ArcWebContents init is too early unless an additional source-proven tab/controller-epoch gate prevents pre-tab observation.

## Exact insertion path

1. `TabStripModel::InsertWebContentsAt`, `chrome/browser/ui/tabs/tab_strip_model.cc:350–357`, constructs a NEW `tabs::TabModel` before entering InsertDetachedTabAt.
2. `tab_model.cc:52–72` creates `TabLookupFromWebContents` at57, attaches tab helpers61, constructs TabFeatures62 and calls Init70. `tab_features.cc:743–744` constructs TabAlertController within that Init. Other feature construction follows it (746 onward).
3. `InsertDetachedTabAt:360–381` calls `tab->OnAddedToModel(this)`366. `tab_model.cc:79–94` can notify foreground subscriptions88 and calls its visibility handler93.
4. `InsertTabAtImpl:3895–3949` calls delegate WillAddWebContents3909 (current Chrome delegate164–167 attaches helpers; idempotent helper adoption guard in ui/tab_helpers.cc247–251), foreground/background callback path3923, and blocked state3946.
5. `InsertTabAtIndexImpl:4900–4949` may emit focus/collection/selection effects before tab DidInsert4941. `TabModel::DidInsert`, tab_model.cc193–195, directly notifies its callback list. It then calls OnChange4949.
6. `TabStripModel::OnChange:697–708` first CompleteModelUpdateTransaction699 (dispatches pending collection notifications5242–5246) and OnActiveTabChanged700, then iterates ALL observers702–704 in order. Core is one of those observers, without an exclusive first-observer guarantee. Other observers may run first.
7. Owned `ArcBrowserTabsObserver::OnTabStripModelChanged`, `arc/api/arc_browser.mm:160`, can create traits state at kInserted entry before its wrapper/app callouts. Current body makes wrappers in append190–198, later calls didInsert219–227 and NotifyTabChange230. `didInsertWebContents` calls ArcSessionsDidInsertWebContents at502. Put any eager C++ binding before append/wrapper materialization, not after those callouts.

Core observer is installed in ArcBrowser init atarc_browser.mm426, after native Browser construction and feature observers. A newly wrapped browser may already contain tabs, so a kInserted-only installation must also reconcile existing tabs while explicitly acknowledging any already elapsed peer events. There is no replay in original or current peer observer registration.

The callback surfaces above prove non-exclusive ordering; they do NOT prove a real dropped peer event in existing goldens. No runtime receipt probe was performed. Ordinary Mojo peer events require UI dispatch, but a no-nested-loop guarantee through all these callbacks has not been established. This is a bounded correctness uncertainty, not a measured acceptance failure.

## Public lifecycle APIs after binding

Store the TabInterface epoch as well as weak WC identity, and hold CallbackListSubscription objects. Public APIs in `components/tabs/public/tab_interface.h`:

- `GetWeakPtr()`127; comments108–127 explicitly recommend `RegisterWillDetach()` to clear a cached tab and paired associated state before destruction.
- `RegisterWillDiscardContents(callback)`177–183, signature `void(TabInterface*,WebContents* old,WebContents* new)`.
- `RegisterWillDetach(callback)`224–236, signature `void(TabInterface*,DetachReason)`, reasons kDelete and kInsertIntoOtherWindow.
- `RegisterDidInsert(callback)`238–245, signature `void(TabInterface*)`. Its documentation warns the initial event is seen only when registered early enough. Registering it from kInserted cannot recover the current DidInsert that already ran4941.

The `TabModel` implementations simply add callbacks (RegisterWillDetach268–270, DidInsert273–275); no initial replay. WillDetach187–190 and DidInsert193–195 notify synchronously. Discard subscribers run in registration order, explicitly documented in `tab_features.cc:1021–1025`. The native controller's base registration predates a later core binding.

### Discard / native contents replacement

`tab_model.cc:444–460`: WillDiscardContents Notify447 passes old/new while `tab.GetContents()` still equals OLD; only448 removes old lookup,451–453 moves ownership/updates current WC,459 creates new lookup. The core callback MUST use passed new_contents directly. A newWC->MaybeGetFromContents lookup at this callback is premature. Do not snapshot controller/history from that new WC yet.

Current TabAlertController survives discard and its callback `tab_alert_controller.cc:310–323` rebinds WCO/VR/recent-audio subscriptions, preserving active_alerts_. Original custom ICE peers/18 are reset in that same tab discard lifecycle (previous audit). Core should call RebindForDiscard(new_contents), clearing only own peers/cached13, and migrate service binding/native marker consistently with the parent-owned replacement design. kReplaced observer comes later (`tab_strip_model.cc:400` then410–416), after TabModel::DiscardContents returns; using only kReplaced is late for exact reset timing.

### Transfer preserving the tab epoch

`DetachTabAtForInsertion`, tab_strip_model.cc421–426, moves the SAME TabModel with reason kInsertIntoOtherWindow. InsertDetachedTabAt360–381 reinserts that object. `TabModel::OnRemovedFromModel:96–116` preserves contents and features, clearing owning model and its own visibility observation only. Group/split collection transfer also reuses TabModels (639–650,673–685). Do NOT clear ICE state or create a second peer observer on same-TabInterface reinsertion. Keep discard/detach subscriptions and cache.

### Removing the TabModel while retaining the same WC

`DetachWebContentsAtForInsertion:430–434` uses kDelete, then TabModel::DestroyAndTakeWebContents (`tab_model.cc:464–468`) returns the surviving native WC and destroys its TabModel. `RemoveTabFromIndexImpl:4973–4974` destroys TabFeatures/controller before actual collection removal4981. A future InsertWebContentsAt350–357 creates a NEW TabModel/controller for that same WC. Therefore a WC-userdata/provider cache must be reset/rebound by TAB IDENTITY, not only WC pointer. A weak TabInterface check prevents using a dead epoch; kDelete callback should clear its associated cache/binding eagerly.

Normal detach sends WillDetach before DetachTabImpl (`tab_strip_model.cc:510–514`); closing paths also send it4130 or4192. Ordinary core kRemoved notification is too late to dereference cached TabAlertController because TabFeatures may already have been destroyed. On kDelete, cancel subscriptions/clear binding and retire its peer state before native model removal; keep kInsertIntoOtherWindow alive. No general public TabInterface destruction callback was found. TabModel destructor75–77 removes lookup; weak_factory is last member (`tab_model.h:304`) and expires before tab_features_302 teardown. Uninserted TabModels destroyed outside standard detach paths remain a lifecycle edge requiring weak-tab gating/owned disposal rather than assuming every destruction emits kRemoved.

## Minimal exact missing creation seam, if strict lifetime is required

No public production tab-created observer was found in TabInterface/TabModel/TabFeatures. TabFeatures' factory accessor is private; public GetUserDataFactoryForTesting and AddOverrideForTesting are test injection, not a production lifecycle API.

The narrow seam is in `TabAlertController` constructor itself (single .cc), once `ContentsObservingTabFeature` and scoped userdata/member caches have initialized, before its first media/callback subscriptions. A Chrome-owned hosted callback can take `tabs::TabInterface*` and synchronously create an app-independent C++ provider/binding, without creating an ArcWebContents wrapper, invoking delegates, posting property tasks, or querying history. No content-layer dependency or observer-vtable extension is required. An optional matching destruction phase would cover nonstandard/uninserted disposal exactly; ordinary detach/discard/transfer already have public APIs. Register hook once before native tabs are constructed. Merely scanning existing tabs cannot recover prior ICE events.

Ownership must be one provider per native tab/controller epoch, with transfer preservation, discard WC migration/reset, kDelete retirement, and no pre-tab peer capture. Current standalone staged helper does not yet implement epoch/subscription orchestration; this is an integration prerequisite, not a helper-body code defect.

## Additional activation prerequisite: observation WC versus receipt WC during discard

The staged helper's `SnapshotCachedTraits()` queries its own `contents_`, which is also the peer-event observation target. Those identities need not be the same as the logical ArcImpl receipt WC throughout a discard handoff. Original GetTraits4378e28 reads CURRENT logical ArcImpl WC from+130 and then resolves its tab/controller. The native controller's discard observation has already switched to new WC while TabModel still exposes the old WC during its callback list447; newWC TabLookup is absent until459. Parent-owned canonical ArcWebContents native migration may occur at a later phase. Therefore a receipt in this interval could obtain original old-WC/tab cached alerts, while the current no-argument helper queries newWC and returns empty. An actual interleaving receipt here has not been proven; this is a caller-phase/API concern, not an observed runtime failure.

Before activation, define how flag receipt selects the CURRENT logical native WC and associated tab/controller epoch. Candidate safe API shape is `SnapshotCachedTraits(content::WebContents* receipt_contents)` for cached-alert lookup, independently of the helper's peer-observation `contents_`, or an explicitly bound TabInterface/controller epoch whose lookup reproduces the logical receipt source. Do not query the new WC's tab during the early discard callback, fabricate a tab marker, defer receipt capture, or reuse a previously returned set to hide the gap. The caller must prove phase gating/identity alignment if retaining the existing no-argument API. No helper-code change was made by this audit.

For SAME native WC with NEW TabInterface after kDelete/extraction/reinsert, retire/reset the old helper epoch and cached13, even if the raw WC pointer is equal. RebindForDiscard is not the semantic name for this separate new-tab epoch; creation of a new provider or a distinct explicit epoch reset is required.

## Concrete narrow native hook recommendation if exact lifetime cannot be proven at existing insertion

Use one Chrome-owned hosted lifecycle slot, e.g. a `base::RepeatingCallback<void(tabs::TabInterface*, HostTabAlertControllerPhase)>`, with phases `kCreated` and `kDestroying`. Put the declaration/slot alongside existing Chrome arcmod hooks rather than a new content dependency. Invoke kCreated inside `TabAlertController` constructor after base/scoped userdata/all member initialization, before the first callback subscriptions; invoke kDestroying from its destructor body before members/subscriptions are dismantled. The slot callback performs only native C++ provider epoch creation/retirement and public tab callback registration. It must not materialize an Arc wrapper or call AppKit/delegates/setters. Keep the callback registration installed before any hosted browser/tab construction. Current source hooks can be host-switch-gated following existing Arcmod pattern; this audit did not edit or compile a hook.

Owned provider storage must be accessible without wrapper existence and keyed by native tab/controller epoch, with optional current logical-WC marker lookup at flag receipt. Public RegisterWillDiscardContents supplies the passed new WC to peer observation and clears ICE at the original phase; RegisterWillDetach distinguishes transfer preservation from kDelete retirement. The destruction phase closes uninserted/nonstandard disposal gaps that the public WillDetach contract alone does not cover. This two-phase slot is a concrete minimal lifecycle seam, not a request for broad observer virtuals or extra trait invalidations. Parent may instead choose the existing insertion approach only if it establishes the remaining constructor-to-insertion no-peer-dispatch guarantee and handles already-existing tabs; that proof is currently absent.
