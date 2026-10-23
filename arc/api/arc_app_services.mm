// Copyright 2026 ArcMod. Process-level application observer and performance services.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "base/metrics/histogram_base.h"
#include "base/metrics/statistics_recorder.h"
#include "base/no_destructor.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/performance_manager/public/user_tuning/battery_saver_mode_manager.h"
#include "components/performance_manager/public/user_tuning/prefs.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_accessibility_state.h"
#include "content/public/browser/scoped_accessibility_mode.h"
#include "content/public/browser/gpu_data_manager.h"
#include "content/public/browser/gpu_data_manager_observer.h"
#include "content/browser/gpu/gpu_data_manager_impl.h"
#include "content/browser/gpu/gpu_process_host.h"
#include "content/public/browser/network_service_instance.h"
#include "ui/accessibility/platform/ax_mode_observer.h"
#include "ui/accessibility/platform/ax_platform.h"

// Identical to the native record consumed by ArcGPULogMessage in app_values.
namespace app_values {
struct GPULogMessage {
  int32_t severity;
  std::string header;
  std::string message;
};
static_assert(sizeof(GPULogMessage) == 56);
static_assert(offsetof(GPULogMessage, header) == 8);
static_assert(offsetof(GPULogMessage, message) == 32);
}  // namespace app_values

// The primary implementation and ownership of the copied record live in app_values.
ARC_EXPORT @interface ArcGPULogMessage : NSObject
- (instancetype)initWithGPULogMessage:(const void*)message;
@end

@protocol ArcAccessibilityObserver <NSObject>
- (void)accessibilityModeChanged:(NSString*)mode;
@end
@protocol ArcPerformanceObserver <NSObject>
- (void)onBatterySaverModeChanged:(BOOL)active;
@end
@protocol AppGPUDelegate <NSObject>
- (void)didReceiveGPUInfoUpdate:(BOOL)available;
- (void)didReceiveGPUCrashNotification:(int)count withFallbackCount:(int)fallbackCount;
- (void)didConnectToNetworkProcess;
@end

namespace {
using BatteryManager = performance_manager::user_tuning::BatterySaverModeManager;

class ApplicationGPUBridge final : public content::GpuDataManagerObserver,
                                   public content::NetworkServiceProcessObserver {
 public:
  ApplicationGPUBridge() {
    content::GpuDataManager::GetInstance()->AddObserver(this);
    content::AddNetworkServiceProcessObserver(this);
  }
  void OnGpuInfoUpdate() override {
    // The native Arc producer (0x42bcd48) explicitly stores false in this
    // event's BOOL payload; it does not report hardware acceleration state.
    dispatch_async(dispatch_get_main_queue(), ^{
      id<AppGPUDelegate> delegate = ArcBrowserApplicationDelegateObject();
      if ([delegate respondsToSelector:@selector(didReceiveGPUInfoUpdate:)])
        [delegate didReceiveGPUInfoUpdate:NO];
    });
  }
  void OnGpuProcessCrashed() override {
    const int count = content::GpuProcessHost::GetGpuCrashCount();
    // Arc snapshots the count before requesting a fresh sandboxed GPU host.
    content::GpuProcessHost::Get();
    dispatch_async(dispatch_get_main_queue(), ^{
      id<AppGPUDelegate> delegate = ArcBrowserApplicationDelegateObject();
      if ([delegate respondsToSelector:@selector(didReceiveGPUCrashNotification:withFallbackCount:)])
        [delegate didReceiveGPUCrashNotification:count withFallbackCount:3];
    });
  }
  void OnServiceLaunched(const content::ServiceProcessInfo&) override {
    dispatch_async(dispatch_get_main_queue(), ^{
      id<AppGPUDelegate> delegate = ArcBrowserApplicationDelegateObject();
      if ([delegate respondsToSelector:@selector(didConnectToNetworkProcess)])
        [delegate didConnectToNetworkProcess];
    });
  }
};
void ObserveApplicationGPU() {
  static base::NoDestructor<ApplicationGPUBridge> bridge;
}

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
__weak id g_host_notification_center;
}

@interface ApplicationServicesState : NSObject {
 @public
  std::unique_ptr<AccessibilityBridge> accessibility;
  std::unique_ptr<content::ScopedAccessibilityMode> basic_mode;
  std::unique_ptr<content::ScopedAccessibilityMode> screen_reader_mode;
}
@property(nonatomic, weak) id notificationCenter;
@property(nonatomic, copy) id breadcrumbCallback;
@property(nonatomic, copy) id dumpWithoutCrashingCallback;
@property(nonatomic, copy) id webContentsCloseCallback;
@end
@implementation ApplicationServicesState
@synthesize notificationCenter = _notificationCenter;
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
- (NSArray<ArcGPULogMessage*>*)getGPULogMessages {
  NSMutableArray<ArcGPULogMessage*>* messages = [NSMutableArray array];
  base::ListValue snapshot = content::GpuDataManagerImpl::GetInstance()->GetLogMessages();
  for (const base::Value& entry : snapshot) {
    const base::DictValue* dictionary = entry.GetIfDict();
    if (!dictionary) continue;
    const auto severity = dictionary->FindInt("level");
    const std::string* header = dictionary->FindString("header");
    const std::string* text = dictionary->FindString("message");
    if (!severity.has_value() || !header || !text) continue;
    app_values::GPULogMessage message{*severity, *header, *text};
    [messages addObject:[[ArcGPULogMessage alloc] initWithGPULogMessage:&message]];
  }
  return messages;
}
- (NSString*)getHistogramsJSON {
  return ArcNSString(base::StatisticsRecorder::ToJSON(base::JSON_VERBOSITY_LEVEL_FULL));
}
- (void)setNotificationCenter:(id)center {
  g_host_notification_center = center;
  Services(self).notificationCenter = center;
  // Registration precedes -initialize:, before Chromium's UI runner exists.
  // Install on the first main-loop turn, after hosted initialization.
  dispatch_async(dispatch_get_main_queue(), ^{
    if (content::GpuDataManager::Initialized()) ObserveApplicationGPU();
  });
}
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

// The UI producer resolves a strong local from the weak app registration.
id ArcGetNotificationCenter() { return g_host_notification_center; }
