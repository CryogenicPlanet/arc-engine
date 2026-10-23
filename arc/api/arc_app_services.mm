// Copyright 2026 ArcMod. Process-level application observer and performance services.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include "base/no_destructor.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/performance_manager/public/user_tuning/battery_saver_mode_manager.h"
#include "components/performance_manager/public/user_tuning/prefs.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_accessibility_state.h"
#include "content/public/browser/scoped_accessibility_mode.h"
#include "ui/accessibility/platform/ax_mode_observer.h"
#include "ui/accessibility/platform/ax_platform.h"

@protocol ArcAccessibilityObserver <NSObject>
- (void)accessibilityModeChanged:(NSString*)mode;
@end
@protocol ArcPerformanceObserver <NSObject>
- (void)onBatterySaverModeChanged:(BOOL)active;
@end

namespace {
using BatteryManager = performance_manager::user_tuning::BatterySaverModeManager;

class AccessibilityBridge : public ui::AXModeObserver {
 public:
  explicit AccessibilityBridge(id<ArcAccessibilityObserver> observer) : observer_(observer) {
    ui::AXPlatform::GetInstance().AddModeObserver(this);
  }
  ~AccessibilityBridge() override { ui::AXPlatform::GetInstance().RemoveModeObserver(this); }
  void OnAXModeAdded(ui::AXMode mode) override {
    id<ArcAccessibilityObserver> observer = observer_;
    if ([observer respondsToSelector:@selector(accessibilityModeChanged:)])
      [observer accessibilityModeChanged:ArcNSString(
          content::BrowserAccessibilityState::GetInstance()->GetAccessibilityMode().ToString())];
  }
 private:
  __weak id<ArcAccessibilityObserver> observer_;
};

class PerformanceBridge : public BatteryManager::Observer {
 public:
  void SetObserver(id<ArcPerformanceObserver> observer) {
    observer_ = observer;
    if (!observing_ && BatteryManager::HasInstance()) {
      BatteryManager::GetInstance()->AddObserver(this);
      observing_ = true;
    }
  }
  void OnBatterySaverActiveChanged(bool active) override {
    id<ArcPerformanceObserver> observer = observer_;
    if ([observer respondsToSelector:@selector(onBatterySaverModeChanged:)])
      [observer onBatterySaverModeChanged:active];
  }
  void OnBatterySaverModeManagerDestroyed() override {
    BatteryManager::GetInstance()->RemoveObserver(this);
    observing_ = false;
  }
 private:
  __weak id<ArcPerformanceObserver> observer_;
  bool observing_ = false;
};
PerformanceBridge& Performance() {
  static base::NoDestructor<PerformanceBridge> bridge;
  return *bridge;
}
const char kApplicationServicesKey = 0;
}

@interface ApplicationServicesState : NSObject {
 @public
  std::unique_ptr<AccessibilityBridge> accessibility;
  std::unique_ptr<content::ScopedAccessibilityMode> basic_mode;
  std::unique_ptr<content::ScopedAccessibilityMode> screen_reader_mode;
}
@property(nonatomic, weak) id notificationCenter;
@property(nonatomic, weak) id histogramDelegate;
@property(nonatomic, copy) id breadcrumbCallback;
@property(nonatomic, copy) id dumpWithoutCrashingCallback;
@property(nonatomic, copy) id webContentsCloseCallback;
@end
@implementation ApplicationServicesState
@synthesize notificationCenter = _notificationCenter;
@synthesize histogramDelegate = _histogramDelegate;
@synthesize breadcrumbCallback = _breadcrumbCallback;
@synthesize dumpWithoutCrashingCallback = _dumpWithoutCrashingCallback;
@synthesize webContentsCloseCallback = _webContentsCloseCallback;
@end

namespace {
ApplicationServicesState* Services(id app) {
  @synchronized(app) {
    ApplicationServicesState* state = objc_getAssociatedObject(app, &kApplicationServicesKey);
    if (!state) {
      state = [[ApplicationServicesState alloc] init];
      objc_setAssociatedObject(app, &kApplicationServicesKey, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    return state;
  }
}
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppServices)
- (void)setNotificationCenter:(id)center { Services(self).notificationCenter = center; }
- (void)setHistogramSamplesDelegate:(id)delegate { Services(self).histogramDelegate = delegate; }
- (void)setBreadcrumbCallback:(void (^)(id))callback { Services(self).breadcrumbCallback = callback; }
- (void)setDumpWithoutCrashingCallback:(void (^)(void))callback { Services(self).dumpWithoutCrashingCallback = callback; }
- (void)setOnWebContentsCloseDebugLogCallback:(void (^)(id))callback { Services(self).webContentsCloseCallback = callback; }
- (void)setAccessibilityObserver:(id<ArcAccessibilityObserver>)observer {
  Services(self)->accessibility.reset();
  if (observer) Services(self)->accessibility = std::make_unique<AccessibilityBridge>(observer);
}
- (void)ensureBasicAccessibilityEnabled {
  auto* state = Services(self);
  if (!state->basic_mode)
    state->basic_mode = content::BrowserAccessibilityState::GetInstance()->CreateScopedModeForProcess(ui::AXMode(3));
}
- (void)enableAccessibilityForScreenReader {
  auto* state = Services(self);
  if (!state->screen_reader_mode)
    state->screen_reader_mode = content::BrowserAccessibilityState::GetInstance()->CreateScopedModeForProcess(ui::AXMode(15));
}
- (void)disableAccessibility {
  Services(self)->basic_mode.reset();
  Services(self)->screen_reader_mode.reset();
}
@end

ARC_EXPORT @interface ArcPerformanceController : NSObject
@end
@implementation ArcPerformanceController
+ (BOOL)isBatterySaverModeActive {
  return BatteryManager::HasInstance() && BatteryManager::GetInstance()->IsBatterySaverActive();
}
+ (void)setHighEfficiencyModeEnabled:(BOOL)enabled {
  if (g_browser_process && g_browser_process->local_state())
    g_browser_process->local_state()->SetInteger(
        performance_manager::user_tuning::prefs::kMemorySaverModeState, enabled ? 2 : 0);
}
+ (void)temporarilyDisableBatterySaverMode:(BOOL)disabled {
  if (BatteryManager::HasInstance()) BatteryManager::GetInstance()->SetTemporaryBatterySaverDisabledForSession(disabled);
}
+ (BOOL)isBatterySaverModeTemporarilyDisabled {
  return BatteryManager::HasInstance() && BatteryManager::GetInstance()->IsBatterySaverModeDisabledForSession();
}
+ (void)setArcPerformanceObserver:(id<ArcPerformanceObserver>)observer { Performance().SetObserver(observer); }
@end
