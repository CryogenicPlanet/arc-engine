// Copyright 2026 ArcMod. Forward native string crash keys to Arc's observer.
#include "arc/api/arc_internal.h"

#include <map>
#include <optional>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/location.h"
#include "base/no_destructor.h"
#include "base/synchronization/lock.h"
#include "base/task/single_thread_task_runner.h"
#include "base/time/time.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "third_party/crashpad/crashpad/client/annotation.h"

@protocol ArcCrashKeysObserver <NSObject>
- (void)onCrashKeyEmitted:(NSString*)name value:(NSString*)value;
@end

namespace {

struct CrashKeyRecord {
  base::TimeTicks last_emission;
  std::optional<std::string> pending_value;
  std::optional<std::string> last_value;
};

struct AppCrashObserverState {
  base::Lock lock;
  __weak id<ArcCrashKeysObserver> observer GUARDED_BY(lock) = nil;
  // The native reporter keeps its per-name records across observer changes.
  // Annotation writes and delayed delivery serialize on the UI thread.
  std::map<std::string, CrashKeyRecord> records;
};

AppCrashObserverState& CrashObserverState() {
  static base::NoDestructor<AppCrashObserverState> state;
  return *state;
}

void DeliverCrashKey(const std::string& name, const std::string& value) {
  AppCrashObserverState& state = CrashObserverState();
  id<ArcCrashKeysObserver> observer;
  {
    base::AutoLock hold(state.lock);
    observer = state.observer;
  }
  if ([observer respondsToSelector:@selector(onCrashKeyEmitted:value:)]) {
    [observer onCrashKeyEmitted:ArcNSString(name) value:ArcNSString(value)];
  }
}

void DeliverPendingCrashKey(std::string name) {
  CrashKeyRecord& record = CrashObserverState().records.at(name);
  if (!record.pending_value) {
    return;
  }
  std::string value = std::move(*record.pending_value);
  record.pending_value.reset();
  record.last_value = value;
  record.last_emission = base::TimeTicks::Now();
  // Resolve the current observer when the timer fires, as the native reporter
  // does. Replacing the observer does not cancel or replay crash annotations.
  DeliverCrashKey(name, value);
}

void RecordCrashKey(std::string name, std::string value) {
  CrashKeyRecord& record = CrashObserverState().records[name];
  // The order matters: a write equal to the last emission leaves an already
  // pending, different value in place.
  if (record.pending_value == value || record.last_value == value) {
    return;
  }
  const base::TimeTicks now = base::TimeTicks::Now();
  const base::TimeDelta remaining =
      base::Seconds(2) - (now - record.last_emission);
  if (record.pending_value ||
      (record.last_value && remaining >= base::TimeDelta())) {
    const bool needs_timer = !record.pending_value;
    record.pending_value = std::move(value);
    if (needs_timer) {
      content::GetUIThreadTaskRunner({})->PostDelayedTask(
          FROM_HERE, base::BindOnce(&DeliverPendingCrashKey, std::move(name)),
          remaining);
    }
    return;
  }
  record.last_value = value;
  record.last_emission = now;
  DeliverCrashKey(name, value);
}

void OnCrashAnnotationSet(const crashpad::Annotation& annotation) {
  if (annotation.type() != crashpad::Annotation::Type::kString) {
    return;
  }
  // SetSize runs while the producer's value is valid. Copy it before posting:
  // StringAnnotation's value buffer is not necessarily NUL-terminated.
  std::string name(annotation.name());
  // Native reporter adapter 0x4556744 excludes the exact prefix "Bug".
  if (name.starts_with("Bug")) {
    return;
  }
  std::string value(static_cast<const char*>(annotation.value()),
                    annotation.size());
  // Native tests the registered Chromium UI sequence, including initialization
  // phases where Cocoa's main thread is not yet that sequence.
  if (content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    RecordCrashKey(std::move(name), std::move(value));
    return;
  }
  content::GetUIThreadTaskRunner({})->PostTask(
      FROM_HERE, base::BindOnce(&RecordCrashKey, std::move(name),
                                std::move(value)));
}

}  // namespace

ARC_EXPORT @interface ArcCrashKeysReporter : NSObject
+ (void)setArcCrashKeysObserver:(id<ArcCrashKeysObserver>)observer;
@end

@implementation ArcCrashKeysReporter
+ (void)setArcCrashKeysObserver:(id<ArcCrashKeysObserver>)observer {
  AppCrashObserverState& state = CrashObserverState();
  {
    base::AutoLock hold(state.lock);
    state.observer = observer;
  }
  crashpad::Annotation::SetSetSizeObserver(observer ? &OnCrashAnnotationSet
                                                   : nullptr);
}
@end
