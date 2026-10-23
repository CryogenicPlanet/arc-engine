# Addendum: post-created resume versus synchronous destructor deassociation

**Correction: no concrete supported path is established in the exact private v2 composition where the legacy _created continuation resumes native work after facade deassociation while that same physical Browser WeakPtr remains valid. The earlier general requirement for an equality gate overstates what this implementation demonstrates. It was an endpoint-general hypothetical concern, not a proved v2 command-continuation defect. No new equality gate, policy, fix or test is proposed.** Prior reports remain unchanged; read this additive correction with their legacy-continuation claims.

## Exact caller and same-stack ordering

The private native patch installs the hosted hook synchronously at the first operation in Browser::~Browser. Its hook.Run(this) is an ordinary synchronous callback; there is no post/continuation around preparation. The v2 Sessions callback calls PrepareBrowserForNativeDestruction atafter/arc_sessions_restore.mm1081, publishes global close inline1095, runs afterPublication1100 and returns. The exact core endpoint(after/arc_browser.mm595–609) requires the UI sequence; its primary method466–484 prepares close state and clears facade binding. Within this reviewed composition, the sole operational caller is that destructor-entry path. The declaration's reusable shape is not evidence of another supported caller.

Legacy restoration atarc/api/arc_sessions_restore.mm487 captures the physical Browser WeakPtr, invokes _created synchronously493, then obtains weakBrowser.get at495 and rejects a missing native or IsDeleteScheduled at497 before BrowserLiveTabContext::From502 and RestoreEntryById514.

Let R be that outer restore frame, C its _created invocation and D the same Browser's destructor entered while C pumps a nested UI loop. The ordinary same-thread stack is:

    R → C → nested event loop → D → physical-close hook → Prepare/close publication

During publication the facade binding can be empty while the physical weak token remains valid. However, R's post-C instructions495/497/502/514 are suspended below D. Stopping the nested loop does not unwind the intervening synchronous destructor/hook/app-callback frames. Before R can resume after C, D must return normally. Browser::GetWeakPtr returns its member weak_factory_(browser.cc449–450); that factory is the last Browser member(browser.h299–300), and member destruction after the destructor body invalidates its WeakReferenceOwner flag(weak_ptr.cc85–88) before the destructor invocation returns. Therefore weakBrowser is already invalid when R resumes, and497 exits before502/514. The browser allocation cannot be kept physically weak-valid after a completed destructor merely by retaining the ObjC facade.

If C only requests ordinary scheduled close without actual destruction, the native IsDeleteScheduled predicate at497 separately rejects it. If neither destruction nor scheduled close occurred, the exact v2 destructor-only endpoint has not supplied the hypothesized facade deassociation. A separately invoked generic deassociation endpoint could change this conclusion, but no such operational call is established in the composition.

## Native factory's post-created guard has the same property

Currentchromium/src/chrome/browser/ui/browser_live_tab_context.cc568–588 obtains the factory callback, creates the Browser, captures its WeakPtr579–580, invokes **created.Run synchronously582**, then checks that WeakPtr and IsDeleteScheduled at586 before returning BrowserLiveTabContext::From587. Its owned callbackarc_sessions_restore.mm342–355 invokes app _created synchronously353 and returns with no later owned native Browser command inside that block.

If destruction of the same Browser runs in a nested loop during this created invocation, its destructor completes and invalidates the physical token before line586 can resume. The guard therefore rejects that case. If only normal close is scheduled, it rejects IsDeleteScheduled. No additional source-proven native/facade equality requirement is established for this particular post-created return boundary. This conclusion concerns the accepted-hook path with a captured WeakPtr; the declined-hook branch575–577 and arbitrary earlier construction callouts are not re-audited here.

## Scope, assumptions and distinct unresolved cases

The proof assumes supported UI-sequence lifecycle execution, ordinary synchronous C++/ObjC block return, the reviewed destructor-only preparation caller, and the same Browser identity represented by the captured WeakPtr. Cross-thread destruction, manually calling the private endpoint outside destruction, fabricated/private-selector invocation, coroutine/longjmp/exception schemes that bypass ordinary stack return, or a future asynchronous preparation design are not established supported paths. No runtime reachability or callback result parity is claimed.

This corrects only the ancestor restore continuation's alleged command window. The facade-detached/physical-weak-valid interval **does exist while inside D's global close callback**. Fresh commands or new tasks run in a nested loop inside that callback are a different stack. In particular the separate App tryToTerminate/global-collection finding is not discharged: it starts native command work inside D rather than resuming suspended R after D returns. Pending logical notifications, extension backend reads, full-finished callback policy and helper teardown remain separate, previously bounded questions. Do not infer equality cancellation for notification-only queues or a finished(nil) result policy from this stack proof.

No source or old evidence was changed. Only this additive evidence directory was written; no runtime/query, app, Git/index, compiler/build, queue/process controls, Chirp or policy/fix/test was performed. Input and output pins are in review.json; exact excerpts are in source-excerpts.txt.
