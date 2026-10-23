// Copyright 2026 ArcMod. Coordinate native beforeunload and application termination.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/lifetime/application_lifetime_desktop.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/browser_window/public/browser_collection_observer.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/unload_controller.h"

namespace {
// Arc's completion is an NSTerminateReply-like integer (0..2), not a BOOL.
// No browser pointers survive a native callback or an iteration of the collection.
class TerminationRequest final : public BrowserCollectionObserver {
 public:
  bool active() const { return active_; }
  void Begin(void (^completion)(NSUInteger)) {
    active_ = true;
    completion_ = [completion copy];
    observation_.Observe(GlobalBrowserCollection::GetInstance());
    ScheduleCheck();
  }
  void OnBrowserClosed(BrowserWindowInterface*) override { ScheduleCheck(); }

 private:
  void ScheduleCheck() {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(&TerminationRequest::Check, weak_factory_.GetWeakPtr()));
  }
  void BeforeUnloadResult(bool allowed) {
    if (!active_ || approved_) return;
    if (allowed) {
      ScheduleCheck();
      return;
    }
    GlobalBrowserCollection::GetInstance()->ForEach([](BrowserWindowInterface* browser) {
      UnloadController::From(browser)->ResetBeforeUnloadForShutdown();
      return true;
    });
    auto completion = completion_;
    completion_ = nil;
    active_ = false;
    observation_.Reset();
    weak_factory_.InvalidateWeakPtrs();
    if (completion) completion(0);
  }
  void Check() {
    if (!active_ || !completion_) return;
    auto* browsers = GlobalBrowserCollection::GetInstance();
    if (!approved_) {
      bool waiting = false;
      browsers->ForEach([&](BrowserWindowInterface* browser) {
        if (UnloadController::From(browser)->RunBeforeUnloadForShutdown(
                false, base::BindRepeating(&TerminationRequest::BeforeUnloadResult,
                                          weak_factory_.GetWeakPtr()))) {
          waiting = true;
          return false;
        }
        return true;
      });
      if (waiting || !active_) return;
      approved_ = true;
      // Preserve the native session before closing the host-owned windows.
      ProfileManager::ShutdownSessionServices();
      chrome::MarkAsCleanShutdown();
      browsers->ForEach([](BrowserWindowInterface* browser) {
        if (browser->GetWindow()) browser->GetWindow()->Close();
        return true;
      });
    }
    if (!browsers->IsEmpty()) return;
    auto completion = completion_;
    completion_ = nil;
    observation_.Reset();
    // Keep active_ set after approval: a repeated request must neither replace
    // the first completion nor start a second close transaction.
    if (completion) completion(1);
  }
  bool active_ = false;
  bool approved_ = false;
  void (^completion_)(NSUInteger) = nil;
  base::ScopedObservation<GlobalBrowserCollection, BrowserCollectionObserver> observation_{this};
  base::WeakPtrFactory<TerminationRequest> weak_factory_{this};
};
const char kTerminationRequestKey = 0;
}

@interface ApplicationTerminationState : NSObject {
 @public
  TerminationRequest request;
}
@end
@implementation ApplicationTerminationState
@end

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppTermination)
- (NSUInteger)tryToTerminate:(void (^)(NSUInteger))completion {
  if (!g_browser_process || !g_browser_process->profile_manager()) return 1;
  ApplicationTerminationState* state = objc_getAssociatedObject(self, &kTerminationRequestKey);
  if (!state) {
    state = [[ApplicationTerminationState alloc] init];
    objc_setAssociatedObject(self, &kTerminationRequestKey, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  if (!state->request.active()) state->request.Begin(completion);
  return 2;
}
@end
