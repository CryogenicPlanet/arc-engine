// Copyright 2026 ArcMod. Native tracing lifecycle and immutable configuration snapshots.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include <atomic>
#include <cmath>

#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/timer/timer.h"
#include "base/trace_event/trace_config.h"
#include "content/public/browser/tracing_controller.h"

ARC_EXPORT @interface ArcTracingConfiguration : NSObject
@property(nonatomic, strong) NSString* filePath;
@property(nonatomic) double tracingDuration;
@property(nonatomic) BOOL privacyFilteringEnabled;
@end
@implementation ArcTracingConfiguration
@synthesize filePath = _filePath;
@synthesize tracingDuration = _tracingDuration;
@synthesize privacyFilteringEnabled = _privacyFilteringEnabled;
- (instancetype)init {
  if ((self = [super init])) {
    _filePath = @"";
    _privacyFilteringEnabled = YES;
  }
  return self;
}
@end

@class ArcTracingController;
@protocol ArcTracingControllerDelegate <NSObject>
- (void)tracingControllerDidStartTracing:(ArcTracingController*)controller;
- (void)tracingController:(ArcTracingController*)controller didCompleteTracing:(NSString*)result;
- (void)tracingControllerDidAbortTracing:(ArcTracingController*)controller;
@end

ARC_EXPORT @interface ArcTracingController : NSObject
- (void)setDelegate:(id<ArcTracingControllerDelegate>)delegate;
- (BOOL)isTracing;
- (BOOL)start:(ArcTracingConfiguration*)configuration;
- (BOOL)stop;
- (BOOL)abort;
@end

namespace {
// Public calls are allowed from background queues (the launch transcript requests
// the wrapper there); Chromium tracing itself is confined to the main thread.
BOOL OnMain(BOOL (^operation)(void)) {
  if ([NSThread isMainThread]) return operation();
  __block BOOL result = NO;
  dispatch_sync(dispatch_get_main_queue(), ^{ result = operation(); });
  return result;
}
const char kTracingControllerKey = 0;
}

@implementation ArcTracingController {
  __weak id<ArcTracingControllerDelegate> _delegate;
  std::atomic_bool _active;
  base::OneShotTimer _timer;
  NSString* _filePath;
  uint64_t _generation;
}
- (void)setDelegate:(id<ArcTracingControllerDelegate>)delegate {
  @synchronized(self) { _delegate = delegate; }
}
- (BOOL)isTracing { return _active.load(); }
- (BOOL)start:(ArcTracingConfiguration*)configuration {
  NSString* path = [configuration.filePath copy] ?: @"";
  double duration = configuration.tracingDuration;
  if (!std::isfinite(duration)) return NO;
  BOOL privacy = configuration ? configuration.privacyFilteringEnabled : NO;
  return OnMain(^{
    if (self->_active.load()) return NO;
    auto* native = content::TracingController::GetInstance();
    if (native->IsTracing()) return NO;
    base::trace_event::TraceConfig config(
        "disabled-by-default-system_power,disabled-by-default-system_stats", "record-until-full");
    config.EnableSystrace();
    for (const char* name : {"Memory.Browser.PrivateMemoryFootprint", "Memory.Gpu.PrivateMemoryFootprint",
                            "Memory.Renderer.PrivateMemoryFootprint", "Memory.Total.PrivateMemoryFootprint",
                            "Memory.PressureLevel2", "PerformanceMonitor.AverageCPU10.BrowserProcess",
                            "PerformanceMonitor.AverageCPU10.RendererProcess", "PerformanceMonitor.AverageCPU10.Total"})
      config.EnableHistogram(name);
    uint64_t generation = ++self->_generation;
    __weak ArcTracingController* weakSelf = self;
    bool started = native->StartTracing(config, base::BindOnce(^{
      ArcTracingController* controller = weakSelf;
      if (!controller || !controller->_active.load() || controller->_generation != generation) return;
      id<ArcTracingControllerDelegate> delegate;
      @synchronized(controller) { delegate = controller->_delegate; }
      if ([delegate respondsToSelector:@selector(tracingControllerDidStartTracing:)])
        [delegate tracingControllerDidStartTracing:controller];
    }), privacy);
    if (!started) return NO;
    self->_filePath = path;
    self->_active.store(true);
    if (duration > 0) {
      self->_timer.Start(FROM_HERE, base::Seconds(duration), base::BindOnce(^{
        [weakSelf stop];
      }));
    }
    return YES;
  });
}
- (BOOL)stop {
  return OnMain(^{
    if (!self->_active.load()) return NO;
    __weak ArcTracingController* weakSelf = self;
    uint64_t generation = self->_generation;
    auto complete = ^(NSString* result) {
      ArcTracingController* controller = weakSelf;
      if (!controller || controller->_generation != generation) return;
      id<ArcTracingControllerDelegate> delegate;
      @synchronized(controller) { delegate = controller->_delegate; }
      if ([delegate respondsToSelector:@selector(tracingController:didCompleteTracing:)])
        [delegate tracingController:controller didCompleteTracing:result];
    };
    scoped_refptr<content::TracingController::TraceDataEndpoint> endpoint;
    if (self->_filePath.length) {
      NSString* path = self->_filePath;
      endpoint = content::TracingController::CreateFileEndpoint(base::FilePath(ArcUTF8(path)),
          base::BindOnce(^{ complete(path); }));
    } else {
      endpoint = content::TracingController::CreateStringEndpoint(base::BindOnce(
          [](void (^completion)(NSString*), std::unique_ptr<std::string> trace) {
            completion(trace ? ArcNSString(*trace) : @"");
          }, complete));
    }
    if (!content::TracingController::GetInstance()->StopTracing(endpoint)) return NO;
    self->_active.store(false);
    self->_timer.Stop();
    return YES;
  });
}
- (BOOL)abort {
  return OnMain(^{
    if (!self->_active.load()) return NO;
    if (!content::TracingController::GetInstance()->StopTracing(nullptr)) return NO;
    self->_active.store(false);
    ++self->_generation;
    self->_timer.Stop();
    __weak ArcTracingController* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
      ArcTracingController* controller = weakSelf;
      if (!controller) return;
      id<ArcTracingControllerDelegate> delegate;
      @synchronized(controller) { delegate = controller->_delegate; }
      if ([delegate respondsToSelector:@selector(tracingControllerDidAbortTracing:)])
        [delegate tracingControllerDidAbortTracing:controller];
    });
    return YES;
  });
}
@end

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppTracing)
- (ArcTracingController*)tracingController {
  @synchronized(self) {
    ArcTracingController* controller = objc_getAssociatedObject(self, &kTracingControllerKey);
    if (!controller) {
      controller = [[ArcTracingController alloc] init];
      objc_setAssociatedObject(self, &kTracingControllerKey, controller, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    return controller;
  }
}
@end
