// Copyright 2026 ArcMod. Coordinate native beforeunload and application termination.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/task/sequenced_task_runner.h"
#include "base/timer/timer.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/lifetime/application_lifetime_desktop.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/unload_controller.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/storage_partition.h"
#include "services/network/public/mojom/cookie_manager.mojom.h"

namespace {
// Arc's completion is an NSTerminateReply-like integer (0..2), not a BOOL.
// No browser pointers survive a native callback or an iteration of the collection.
class TerminationRequest final {
 public:
  bool active() const { return active_; }
  void Begin(void (^completion)(NSUInteger)) {
    active_ = true;
    completion_ = [completion copy];
    ScheduleCheck();
  }

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
      browsers->ForEach([](BrowserWindowInterface* browser) {
        UnloadController::From(browser)->HandleBeforeClose();
        return true;
      });
      // The native Arc transaction completes after storage flushes, without
      // waiting for the host's NSWindows or Chromium Browser objects to close.
      ProfileManager::ShutdownSessionServices();
      chrome::MarkAsCleanShutdown();
      pending_flushes_ = 1;  // Prevent an inline reply finishing during setup.
      if (PrefService* local_state = g_browser_process->local_state()) {
        ++pending_flushes_;
        local_state->CommitPendingWrite(FlushCompletion());
      }
      for (Profile* profile : g_browser_process->profile_manager()->GetLoadedProfiles()) {
        ++pending_flushes_;
        profile->GetPrefs()->CommitPendingWrite(FlushCompletion());
        profile->ForEachLoadedStoragePartition([this](content::StoragePartition* partition) {
          partition->Flush();
          ++pending_flushes_;
          partition->GetCookieManagerForBrowserProcess()->FlushCookieStore(FlushCompletion());
        });
      }
      FlushDone();
    }
  }
  base::OnceClosure FlushCompletion() {
    return base::BindOnce(&TerminationRequest::FlushDone, weak_factory_.GetWeakPtr());
  }
  void FlushDone() {
    if (!pending_flushes_ || --pending_flushes_) return;
    auto completion = completion_;
    completion_ = nil;
    // Keep active_ set after approval: a repeated request must neither replace
    // the first completion nor start a second close transaction.
    if (completion) completion(1);
  }
  bool active_ = false;
  bool approved_ = false;
  size_t pending_flushes_ = 0;
  void (^completion_)(NSUInteger) = nil;
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
