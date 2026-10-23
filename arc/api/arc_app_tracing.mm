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
#include "content/browser/tracing/tracing_controller_impl.h"
#include "services/tracing/public/cpp/perfetto/perfetto_config.h"
#include "third_party/perfetto/include/perfetto/tracing/tracing.h"

ARC_EXPORT @interface ArcTracingConfiguration : NSObject {
  NSString* _filePath;
  double _tracingDuration;
  BOOL _privacyFilteringEnabled;
}
@property(nonatomic, strong) NSString* filePath;
@property(nonatomic) double tracingDuration;
@property(nonatomic) BOOL privacyFilteringEnabled;
@end
@implementation ArcTracingConfiguration
@synthesize filePath = _filePath;
@synthesize tracingDuration = _tracingDuration;
- (instancetype)init {
  if ((self = [super init])) {
    _filePath = @"";
    _privacyFilteringEnabled = YES;
  }
  return self;
}
- (void)setFilePath:(NSString*)path { _filePath = [path copy] ?: @""; }
- (BOOL)privacyFilteringEnabled { return _privacyFilteringEnabled; }
- (void)setPrivacyFilteringEnabled:(BOOL)value { _privacyFilteringEnabled = value; }
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
struct TracingSessionState {
  std::unique_ptr<perfetto::TracingSession> session;
  scoped_refptr<content::TracingController::TraceDataEndpoint> endpoint;
  base::OneShotTimer timer;
  bool stopping = false;
};
const char kTracingControllerKey = 0;
}

@implementation ArcTracingController {
  __weak id<ArcTracingControllerDelegate> _delegate;
  std::atomic_bool _active;
  std::unique_ptr<TracingSessionState> _state;
  NSString* _filePath;
  uint64_t _generation;
}
- (void)dealloc {
  // A wrapper cached from a background queue can also lose its final owner
  // there. Timers and the native session must be destroyed on their UI sequence.
  if (_state && ![NSThread isMainThread]) {
    std::shared_ptr<TracingSessionState> cleanup(std::move(_state));
    dispatch_async(dispatch_get_main_queue(), ^{ (void)cleanup; });
  }
}
- (void)setDelegate:(id<ArcTracingControllerDelegate>)delegate {
  @synchronized(self) { _delegate = delegate; }
}
- (BOOL)isTracing { return _active.load(); }
- (BOOL)start:(ArcTracingConfiguration*)configuration {
  NSString* path = [configuration.filePath copy] ?: @"";
  double duration = configuration.tracingDuration;
  BOOL privacy = configuration ? configuration.privacyFilteringEnabled : NO;
  return OnMain(^{
    if (self->_state && self->_state->session) return NO;
    if (!self->_state) self->_state = std::make_unique<TracingSessionState>();
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
    self->_state->session = perfetto::Tracing::NewTrace(perfetto::BackendType::kCustomBackend);
    if (!self->_state->session) return NO;
    self->_filePath = path;
    self->_state->stopping = false;
    self->_active.store(true);
    self->_state->session->SetOnStartCallback([weakSelf, generation, duration] {
      dispatch_async(dispatch_get_main_queue(), ^{
        ArcTracingController* controller = weakSelf;
        if (!controller || controller->_generation != generation || !controller->_active.load()) return;
        // Conversion saturates to microseconds. Native zero/negative durations
        // still arm the timer, scheduling an immediate stop after acknowledgment.
        controller->_state->timer.Start(FROM_HERE, base::Seconds(duration), base::BindOnce(^{
          [weakSelf stop];
        }));
        id<ArcTracingControllerDelegate> delegate;
        @synchronized(controller) { delegate = controller->_delegate; }
        if ([delegate respondsToSelector:@selector(tracingControllerDidStartTracing:)])
          [delegate tracingControllerDidStartTracing:controller];
      });
    });
    self->_state->session->SetOnErrorCallback([weakSelf, generation](perfetto::TracingError error) {
      dispatch_async(dispatch_get_main_queue(), ^{
        ArcTracingController* controller = weakSelf;
        if (!controller || controller->_generation != generation) return;
        controller->_active.store(false);
        ++controller->_generation;
        controller->_state->timer.Stop();
        controller->_state->endpoint.reset();
        controller->_state->session.reset();
        controller->_state->stopping = false;
      });
    });
    self->_state->session->SetOnStopCallback([weakSelf, generation] {
      dispatch_async(dispatch_get_main_queue(), ^{
        ArcTracingController* controller = weakSelf;
        if (!controller || controller->_generation != generation || !controller->_state->session) return;
        controller->_active.store(false);
        controller->_state->timer.Stop();
        controller->_state->stopping = true;
        NSString* outputPath = controller->_filePath;
        auto endpoint = content::TracingController::CreateFileEndpoint(
            base::FilePath(ArcUTF8(outputPath)), base::BindOnce(^{
              dispatch_async(dispatch_get_main_queue(), ^{
                ArcTracingController* owner = weakSelf;
                if (!owner || owner->_generation != generation) return;
                owner->_state->session.reset();
                owner->_state->endpoint.reset();
                owner->_state->stopping = false;
                id<ArcTracingControllerDelegate> delegate;
                @synchronized(owner) { delegate = owner->_delegate; }
                if ([delegate respondsToSelector:@selector(tracingController:didCompleteTracing:)])
                  [delegate tracingController:owner didCompleteTracing:outputPath];
              });
            }));
        controller->_state->endpoint = content::TracingControllerImpl::CreateCompressedStringEndpoint(
            std::move(endpoint), false);
        controller->_state->session->ReadTrace([weakSelf, generation](
            perfetto::TracingSession::ReadTraceCallbackArgs args) {
          // Perfetto owns args.data only during this invocation. Copy before
          // handing the chunk to the UI sequence and compression endpoint.
          std::string chunk = args.size ? std::string(args.data, args.size) : std::string();
          bool finished = !args.has_more;
          dispatch_async(dispatch_get_main_queue(), ^{
            ArcTracingController* owner = weakSelf;
            if (!owner || owner->_generation != generation || !owner->_state->endpoint) return;
            if (!chunk.empty()) owner->_state->endpoint->ReceiveTraceChunk(std::make_unique<std::string>(chunk));
            if (finished) owner->_state->endpoint->ReceivedTraceFinalContents();
          });
        });
      });
    });
    self->_state->session->Setup(tracing::GetDefaultPerfettoConfig(config, privacy, false));
    self->_state->session->Start();
    return YES;
  });
}
- (BOOL)stop {
  return OnMain(^{
    if (!self->_active.load() || !self->_state || !self->_state->session || self->_state->stopping) return NO;
    self->_state->stopping = true;
    self->_active.store(false);
    self->_state->timer.Stop();
    self->_state->session->Stop();
    return YES;
  });
}
- (BOOL)abort {
  return OnMain(^{
    if (!self->_active.load() || !self->_state || !self->_state->session || self->_state->stopping) return NO;
    self->_active.store(false);
    uint64_t generation = ++self->_generation;
    self->_state->timer.Stop();
    self->_state->endpoint.reset();
    self->_state->session.reset();
    self->_state->stopping = false;
    __weak ArcTracingController* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
      ArcTracingController* controller = weakSelf;
      if (!controller || controller->_generation != generation) return;
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
