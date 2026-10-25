// Copyright 2026 ArcMod. Coordinate native beforeunload and application termination.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include <memory>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/task/current_thread.h"
#include "base/timer/timer.h"
#include "chrome/browser/bookmarks/bookmark_model_factory.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/lifetime/browser_shutdown.h"
#include "chrome/browser/notifications/notification_display_service.h"
#include "chrome/browser/notifications/notification_display_service_factory.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/exit_type_service.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/unload_controller.h"
#include "components/bookmarks/browser/bookmark_model.h"
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
    browser_shutdown::SetTryingToQuit(true);
    completion_ = [completion copy];
    // CDP initiates AppKit termination from inside a Chromium task. AppKit
    // waits in a native nested loop after tryToTerminate: returns, so its
    // storage replies and timeout must remain runnable until we reply. A
    // stack guard here would end before that native loop starts.
    if (completion_ &&
        !base::CurrentThread::Get().ApplicationTasksAllowedInNativeNestedLoop()) {
      nested_tasks_ = std::make_unique<
          base::CurrentThread::ScopedAllowApplicationTasksInNativeNestedLoop>();
    }
    // Native starts preflight before returning NSTerminateLater. A posted
    // Chromium task may be suppressed by Cocoa's nested termination loop.
    Check();
  }

 private:
  void BeforeUnloadResult(bool allowed) {
    if (!active_ || approved_) return;
    if (allowed) {
      Check();
      return;
    }
    auto completion = completion_;
    completion_ = nil;
    // Resetting another confirmed browser can synchronously report denial.
    // Retire this transaction's callbacks before resetting those controllers;
    // keep it active until reset finishes so a retry cannot be consumed here.
    weak_factory_.InvalidateWeakPtrs();
    browser_shutdown::SetTryingToQuit(false);
    GlobalBrowserCollection::GetInstance()->ForEach([](BrowserWindowInterface* browser) {
      UnloadController::From(browser)->ResetBeforeUnloadForShutdown();
      return true;
    });
    active_ = false;
    nested_tasks_.reset();
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
      pending_flushes_ = 1;  // Prevent an inline reply finishing during setup.
      // Arc forces permission to terminate after ten seconds if a storage
      // service never replies. Late replies must not invoke the block again.
      flush_timeout_.Start(FROM_HERE, base::Seconds(10),
          base::BindOnce(&TerminationRequest::Finish, weak_factory_.GetWeakPtr()));
      for (Profile* profile : g_browser_process->profile_manager()->GetLoadedProfiles()) {
        if (auto* exit_type = ExitTypeService::GetInstanceForProfile(profile))
          exit_type->SetCurrentSessionExitType(ExitType::kClean);
        ++pending_flushes_;
        profile->GetDefaultStoragePartition()->GetCookieManagerForBrowserProcess()->FlushCookieStore(FlushCompletion());
        // Native obtains the service with create=true before shutting down
        // its platform bridges; this does not replay notification Close events.
        if (auto* service = NotificationDisplayServiceFactory::GetForProfile(profile))
          service->Shutdown();
        profile->ForEachLoadedStoragePartition([](content::StoragePartition* partition) {
          partition->Flush();
        });
        ++pending_flushes_;
        profile->GetPrefs()->CommitPendingWrite(FlushCompletion());
        // Bookmarks save a few seconds after a change, and Arc exits without destroying the profile.
        if (auto* bookmarks = BookmarkModelFactory::GetForBrowserContextIfExists(profile)) {
          ++pending_flushes_;
          bookmarks->CommitPendingWrite(FlushCompletion());
        }
      }
      ProfileManager::ShutdownSessionServices();
      if (PrefService* local_state = g_browser_process->local_state()) {
        ++pending_flushes_;
        local_state->CommitPendingWrite(FlushCompletion());
      }
      FlushDone();
    }
  }
  base::OnceClosure FlushCompletion() {
    return base::BindOnce(&TerminationRequest::FlushDone, weak_factory_.GetWeakPtr());
  }
  void FlushDone() {
    if (!pending_flushes_ || --pending_flushes_) return;
    Finish();
  }
  void Finish() {
    pending_flushes_ = 0;
    flush_timeout_.Stop();
    auto completion = completion_;
    completion_ = nil;
    // Keep active_ set after approval: a repeated request must neither replace
    // the first completion nor start a second close transaction.
    nested_tasks_.reset();
    if (completion) completion(1);
  }
  bool active_ = false;
  bool approved_ = false;
  size_t pending_flushes_ = 0;
  void (^completion_)(NSUInteger) = nil;
  base::OneShotTimer flush_timeout_;
  std::unique_ptr<
      base::CurrentThread::ScopedAllowApplicationTasksInNativeNestedLoop>
      nested_tasks_;
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
