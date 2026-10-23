// Copyright 2026 ArcMod. Application-level value records.
#include "arc/api/arc_internal.h"

#include <cstdint>
#include <string>

namespace app_values {
// Names and field widths preserve the native Objective-C aggregate encodings.
struct ErrorStatus {
  // Native no-op C++ lifecycle is nontrivial: aggregate returns use x8 and
  // by-value setters receive an indirect argument, despite the eight-byte size.
  ErrorStatus() {}
  ErrorStatus(const ErrorStatus&) = default;
  ErrorStatus& operator=(const ErrorStatus&) = default;
  ~ErrorStatus() {}
  int32_t process_termination_status;
  int32_t crashed_error_code;
};
struct DebugProperty {
  std::string name;
  std::string value;
};
struct GPULogMessage {
  int32_t severity;
  std::string header;
  std::string message;
};
struct NetworkLoggingConfiguration {
  std::string file_path;
  double logging_duration_in_seconds;
};
static_assert(sizeof(GPULogMessage) == 56);
static_assert(sizeof(NetworkLoggingConfiguration) == 32);
static_assert(sizeof(ErrorStatus) == 8);
static_assert(sizeof(DebugProperty) == 48);
}  // namespace app_values

ARC_EXPORT @interface ArcReference : NSObject {
  NSString* _referenceText;
  NSInteger _minimumCharacterCount;
}
@property(nonatomic, readonly, copy) NSString* referenceText;
@property(nonatomic, readonly) NSInteger minimumCharacterCount;
- (instancetype)initWithReferenceText:(NSString*)text
               minimumCharacterCount:(NSInteger)count;
@end

ARC_EXPORT @interface ArcMemoryDumpMetric : NSObject
@property(nonatomic, readonly) NSString* name;
@property(nonatomic, readonly) NSString* units;
@property(nonatomic, readonly) uint64_t value;
- (instancetype)initWithName:(NSString*)name
                       units:(NSString*)units
                       value:(uint64_t)value;
@end

@implementation ArcMemoryDumpMetric
@synthesize name = _name, units = _units, value = _value;
- (instancetype)initWithName:(NSString*)name
                       units:(NSString*)units
                       value:(uint64_t)value {
  if ((self = [super init])) {
    _name = [name copy];
    _units = [units copy];
    _value = value;
  }
  return self;
}
@end

ARC_EXPORT @interface ArcErrorStatus : NSObject
@property(nonatomic) app_values::ErrorStatus error_status;
@property(nonatomic, readonly) NSInteger processTerminationStatus;
@property(nonatomic, readonly) int crashedErrorCode;
- (instancetype)initWithErrorStatus:(const app_values::ErrorStatus*)status;
@end

@implementation ArcErrorStatus
@synthesize error_status = _error_status;
- (instancetype)initWithErrorStatus:(const app_values::ErrorStatus*)status {
  if ((self = [super init])) {
    _error_status = *status;
  }
  return self;
}
- (NSInteger)processTerminationStatus {
  return _error_status.process_termination_status;
}
- (int)crashedErrorCode { return _error_status.crashed_error_code; }
@end

ARC_EXPORT @interface ArcBrowserApplicationDebugProperty : NSObject
@property(nonatomic) app_values::DebugProperty property;
@property(nonatomic, readonly) NSString* name;
@property(nonatomic, readonly) NSString* value;
- (instancetype)initWithDebugProperty:(app_values::DebugProperty)property;
@end

@implementation ArcBrowserApplicationDebugProperty
@synthesize property = _property;
- (instancetype)initWithDebugProperty:(app_values::DebugProperty)property {
  if ((self = [super init])) {
    _property = property;
  }
  return self;
}
- (NSString*)name { return ArcNSString(_property.name); }
- (NSString*)value { return ArcNSString(_property.value); }
@end

@implementation ArcReference
- (instancetype)initWithReferenceText:(NSString*)text
               minimumCharacterCount:(NSInteger)count {
  if ((self = [super init])) {
    _referenceText = [text copy];
    _minimumCharacterCount = count;
  }
  return self;
}
- (NSString*)referenceText { return _referenceText; }
- (NSInteger)minimumCharacterCount { return _minimumCharacterCount; }
@end

// Native std::string records preserve copy ownership and indirect aggregate ABI.
ARC_EXPORT @interface ArcGPULogMessage : NSObject
@property(nonatomic) app_values::GPULogMessage gpu_log_message;
@property(nonatomic, readonly) NSInteger severity;
@property(nonatomic, readonly, copy) NSString* header;
@property(nonatomic, readonly, copy) NSString* message;
- (instancetype)initWithGPULogMessage:(const void*)message;
@end

@implementation ArcGPULogMessage
@synthesize gpu_log_message = _gpu_log_message;
- (instancetype)initWithGPULogMessage:(const void*)message {
  if ((self = [super init])) {
    _gpu_log_message = *static_cast<const app_values::GPULogMessage*>(message);
  }
  return self;
}
- (NSInteger)severity { return _gpu_log_message.severity; }
- (NSString*)header { return ArcNSString(_gpu_log_message.header); }
- (NSString*)message { return ArcNSString(_gpu_log_message.message); }
@end

ARC_EXPORT @interface ArcNetworkLoggingConfiguration : NSObject
@property(nonatomic) app_values::NetworkLoggingConfiguration network_logging_configuration;
@property(nonatomic, copy) NSString* filePath;
@property(nonatomic) double loggingDurationInSeconds;
- (instancetype)initWithNetworkLoggingConfiguration:(const void*)configuration;
@end

@implementation ArcNetworkLoggingConfiguration
@synthesize network_logging_configuration = _network_logging_configuration;
- (instancetype)init {
  if ((self = [super init])) {
    // Native init only resets the path. Allocation supplies the initial zero
    // duration; calling init again must preserve a duration already assigned.
    _network_logging_configuration.file_path = std::string();
  }
  return self;
}
- (instancetype)initWithNetworkLoggingConfiguration:(const void*)configuration {
  if ((self = [super init])) {
    _network_logging_configuration =
        *static_cast<const app_values::NetworkLoggingConfiguration*>(configuration);
  }
  return self;
}
- (NSString*)filePath { return ArcNSString(_network_logging_configuration.file_path); }
- (void)setFilePath:(NSString*)path {
  _network_logging_configuration.file_path = ArcUTF8(path);
}
- (double)loggingDurationInSeconds {
  return _network_logging_configuration.logging_duration_in_seconds;
}
- (void)setLoggingDurationInSeconds:(double)duration {
  _network_logging_configuration.logging_duration_in_seconds = duration;
}
@end

namespace app_values {
struct BrowserState {
  unsigned int visible : 1;
  unsigned int in_session : 1;
  unsigned int playing_audio : 1;
  unsigned int reserved : 29;
};
struct HistogramSample {
  int value;
  BrowserState browser_state;
};
static_assert(sizeof(BrowserState) == 4);
static_assert(sizeof(HistogramSample) == 8);
}  // namespace app_values

ARC_EXPORT @interface ArcHistogramSample : NSObject {
  BOOL _browserIsVisible;
  BOOL _browserInSession;
  BOOL _browserIsPlayingAudio;
  NSNumber* _value;
}
@property(nonatomic, readonly) NSNumber* value;
@property(nonatomic, readonly) BOOL browserIsVisible;
@property(nonatomic, readonly) BOOL browserInSession;
@property(nonatomic, readonly) BOOL browserIsPlayingAudio;
- (instancetype)initWithHistogramSample:(const app_values::HistogramSample*)sample;
@end

@implementation ArcHistogramSample
@synthesize value = _value, browserIsVisible = _browserIsVisible;
@synthesize browserInSession = _browserInSession;
@synthesize browserIsPlayingAudio = _browserIsPlayingAudio;
- (instancetype)initWithHistogramSample:(const app_values::HistogramSample*)sample {
  if ((self = [super init])) {
    _value = [NSNumber numberWithInt:sample->value];
    _browserIsVisible = sample->browser_state.visible;
    _browserInSession = sample->browser_state.in_session;
    _browserIsPlayingAudio = sample->browser_state.playing_audio;
  }
  return self;
}
@end

ARC_EXPORT @interface ArcAdhocHistogramSample : NSObject {
  int _value;
  NSUInteger _type;
}
@property(nonatomic) NSUInteger type;
@property(nonatomic) int value;
- (instancetype)initWithType:(NSUInteger)type value:(int)value;
@end

@implementation ArcAdhocHistogramSample
@synthesize type = _type, value = _value;
- (instancetype)initWithType:(NSUInteger)type value:(int)value {
  if ((self = [super init])) {
    _type = type;
    _value = value;
  }
  return self;
}
@end

// Native collection inputs are read only through the offsets proven by the
// original engine constructors. Their C++ type names and leading bytes remain
// unknown; these readers do not assert an allocation size or header meaning.
#include <bit>
#include "base/compiler_specific.h"
#include "base/containers/span.h"
#include "base/numerics/byte_conversions.h"

ARC_EXPORT @interface ArcHistogramSamples : NSObject {
  ArcHistogramSample* _browserMemoryFootprintMb;
  ArcHistogramSample* _totalMemoryFootprintMb;
  ArcHistogramSample* _averageTotalCpuWhileInBackgroundPercent;
  ArcHistogramSample* _averageTotalCpuPercent;
  ArcHistogramSample* _averageBrowserCpuPercent;
  ArcHistogramSample* _idleWakeupsTotalWhileInBackground;
  ArcHistogramSample* _idleWakeupsTotal;
  ArcHistogramSample* _batteryDischargeRateNavigation;
  ArcHistogramSample* _batteryDischargeRateWhileInBackground;
  ArcHistogramSample* _batteryDischargeRate;
  ArcHistogramSample* _memoryPressureWindowDurationCriticalToModerate;
  ArcHistogramSample* _memoryPressureWindowDurationCriticalToNone;
  ArcHistogramSample* _memoryPressureWindowDurationModerateToCritical;
  ArcHistogramSample* _memoryPressureWindowDurationModerateToNone;
  ArcHistogramSample* _averageGpuProcessCpuPercent;
  ArcHistogramSample* _averageNetworkProcessCpuPercent;
  ArcHistogramSample* _averageExtensionEventProcessCpuPercent;
  ArcHistogramSample* _averageExtensionPersistentProcessCpuPercent;
  ArcHistogramSample* _averageRendererProcessCpuPercent;
  ArcHistogramSample* _averageTotalCpuDuringFullscreenVideoPercent;
  ArcHistogramSample* _averageTotalCpuWithInteractionPercent;
  ArcHistogramSample* _averageTotalCpuWithNavigationPercent;
  ArcHistogramSample* _averageTotalCpuWhenIdleForegroundPercent;
  ArcHistogramSample* _gpuMemoryFootprintMb;
  ArcHistogramSample* _rendererMemoryFootprintMb;
  ArcHistogramSample* _totalRendererMemoryFootprintMb;
  ArcHistogramSample* _totalSharedMemoryFootprintMb;
  ArcHistogramSample* _totalPrivateSwapFootprintMb;
  ArcHistogramSample* _totalResidentSetMb;
  ArcHistogramSample* _totalGpuMappedMemoryMb;
  ArcHistogramSample* _totalTileMemoryMb;
  ArcHistogramSample* _networkServiceMemoryFootprintMb;
  ArcHistogramSample* _extensionMemoryFootprintMb;
  ArcHistogramSample* _aiServiceMemoryFootprintMb;
  ArcHistogramSample* _audioServiceMemoryFootprintMb;
  ArcHistogramSample* _utilityMemoryFootprintMb;
  ArcHistogramSample* _averageAIServiceCpuPercent;
}
@property(nonatomic, readonly) ArcHistogramSample* browserMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* totalMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuWhileInBackgroundPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageBrowserCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* idleWakeupsTotalWhileInBackground;
@property(nonatomic, readonly) ArcHistogramSample* idleWakeupsTotal;
@property(nonatomic, readonly) ArcHistogramSample* batteryDischargeRateNavigation;
@property(nonatomic, readonly) ArcHistogramSample* batteryDischargeRateWhileInBackground;
@property(nonatomic, readonly) ArcHistogramSample* batteryDischargeRate;
@property(nonatomic, readonly) ArcHistogramSample* memoryPressureWindowDurationCriticalToModerate;
@property(nonatomic, readonly) ArcHistogramSample* memoryPressureWindowDurationCriticalToNone;
@property(nonatomic, readonly) ArcHistogramSample* memoryPressureWindowDurationModerateToCritical;
@property(nonatomic, readonly) ArcHistogramSample* memoryPressureWindowDurationModerateToNone;
@property(nonatomic, readonly) ArcHistogramSample* averageGpuProcessCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageNetworkProcessCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageExtensionEventProcessCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageExtensionPersistentProcessCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageRendererProcessCpuPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuDuringFullscreenVideoPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuWithInteractionPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuWithNavigationPercent;
@property(nonatomic, readonly) ArcHistogramSample* averageTotalCpuWhenIdleForegroundPercent;
@property(nonatomic, readonly) ArcHistogramSample* gpuMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* rendererMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* totalRendererMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* totalSharedMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* totalPrivateSwapFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* totalResidentSetMb;
@property(nonatomic, readonly) ArcHistogramSample* totalGpuMappedMemoryMb;
@property(nonatomic, readonly) ArcHistogramSample* totalTileMemoryMb;
@property(nonatomic, readonly) ArcHistogramSample* networkServiceMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* extensionMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* aiServiceMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* audioServiceMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* utilityMemoryFootprintMb;
@property(nonatomic, readonly) ArcHistogramSample* averageAIServiceCpuPercent;
- (instancetype)initWithPeriodicHistogramSample:(const void*)sample;
@end

@implementation ArcHistogramSamples
@synthesize browserMemoryFootprintMb = _browserMemoryFootprintMb;
@synthesize totalMemoryFootprintMb = _totalMemoryFootprintMb;
@synthesize averageTotalCpuWhileInBackgroundPercent = _averageTotalCpuWhileInBackgroundPercent;
@synthesize averageTotalCpuPercent = _averageTotalCpuPercent;
@synthesize averageBrowserCpuPercent = _averageBrowserCpuPercent;
@synthesize idleWakeupsTotalWhileInBackground = _idleWakeupsTotalWhileInBackground;
@synthesize idleWakeupsTotal = _idleWakeupsTotal;
@synthesize batteryDischargeRateNavigation = _batteryDischargeRateNavigation;
@synthesize batteryDischargeRateWhileInBackground = _batteryDischargeRateWhileInBackground;
@synthesize batteryDischargeRate = _batteryDischargeRate;
@synthesize memoryPressureWindowDurationCriticalToModerate = _memoryPressureWindowDurationCriticalToModerate;
@synthesize memoryPressureWindowDurationCriticalToNone = _memoryPressureWindowDurationCriticalToNone;
@synthesize memoryPressureWindowDurationModerateToCritical = _memoryPressureWindowDurationModerateToCritical;
@synthesize memoryPressureWindowDurationModerateToNone = _memoryPressureWindowDurationModerateToNone;
@synthesize averageGpuProcessCpuPercent = _averageGpuProcessCpuPercent;
@synthesize averageNetworkProcessCpuPercent = _averageNetworkProcessCpuPercent;
@synthesize averageExtensionEventProcessCpuPercent = _averageExtensionEventProcessCpuPercent;
@synthesize averageExtensionPersistentProcessCpuPercent = _averageExtensionPersistentProcessCpuPercent;
@synthesize averageRendererProcessCpuPercent = _averageRendererProcessCpuPercent;
@synthesize averageTotalCpuDuringFullscreenVideoPercent = _averageTotalCpuDuringFullscreenVideoPercent;
@synthesize averageTotalCpuWithInteractionPercent = _averageTotalCpuWithInteractionPercent;
@synthesize averageTotalCpuWithNavigationPercent = _averageTotalCpuWithNavigationPercent;
@synthesize averageTotalCpuWhenIdleForegroundPercent = _averageTotalCpuWhenIdleForegroundPercent;
@synthesize gpuMemoryFootprintMb = _gpuMemoryFootprintMb;
@synthesize rendererMemoryFootprintMb = _rendererMemoryFootprintMb;
@synthesize totalRendererMemoryFootprintMb = _totalRendererMemoryFootprintMb;
@synthesize totalSharedMemoryFootprintMb = _totalSharedMemoryFootprintMb;
@synthesize totalPrivateSwapFootprintMb = _totalPrivateSwapFootprintMb;
@synthesize totalResidentSetMb = _totalResidentSetMb;
@synthesize totalGpuMappedMemoryMb = _totalGpuMappedMemoryMb;
@synthesize totalTileMemoryMb = _totalTileMemoryMb;
@synthesize networkServiceMemoryFootprintMb = _networkServiceMemoryFootprintMb;
@synthesize extensionMemoryFootprintMb = _extensionMemoryFootprintMb;
@synthesize aiServiceMemoryFootprintMb = _aiServiceMemoryFootprintMb;
@synthesize audioServiceMemoryFootprintMb = _audioServiceMemoryFootprintMb;
@synthesize utilityMemoryFootprintMb = _utilityMemoryFootprintMb;
@synthesize averageAIServiceCpuPercent = _averageAIServiceCpuPercent;
- (instancetype)initWithPeriodicHistogramSample:(const void*)sample {
  if ((self = [super init])) {
    // SAFETY: This ABI takes an existing native sample without a length. The
    // original reader accesses through byte 0x1c0; the caller owns that storage.
    const auto bytes = UNSAFE_BUFFERS(base::span<const uint8_t>(
        static_cast<const uint8_t*>(sample), size_t{0x1c1}));
    auto wrap = [&](size_t payload, size_t presence) -> ArcHistogramSample* {
      if (!bytes[presence]) return nil;
      app_values::HistogramSample captured;
      captured.value = base::I32FromNativeEndian(bytes.subspan(payload).first<4>());
      captured.browser_state = std::bit_cast<app_values::BrowserState>(
          base::U32FromNativeEndian(bytes.subspan(payload + 4).first<4>()));
      return [[ArcHistogramSample alloc] initWithHistogramSample:&captured];
    };
    // Constructor order differs from both native field and Objective-C slot order.
    _browserMemoryFootprintMb = wrap(0x14, 0x1c);
    _totalMemoryFootprintMb = wrap(0x8, 0x10);
    _averageTotalCpuWhileInBackgroundPercent = wrap(0x20, 0x28);
    _averageTotalCpuPercent = wrap(0x2c, 0x34);
    _averageBrowserCpuPercent = wrap(0x38, 0x40);
    _idleWakeupsTotalWhileInBackground = wrap(0x44, 0x4c);
    _idleWakeupsTotal = wrap(0x50, 0x58);
    _batteryDischargeRateWhileInBackground = wrap(0x5c, 0x64);
    _batteryDischargeRateNavigation = wrap(0x68, 0x70);
    _batteryDischargeRate = wrap(0x74, 0x7c);
    _memoryPressureWindowDurationCriticalToModerate = wrap(0x80, 0x88);
    _memoryPressureWindowDurationCriticalToNone = wrap(0x8c, 0x94);
    _memoryPressureWindowDurationModerateToCritical = wrap(0x98, 0xa0);
    _memoryPressureWindowDurationModerateToNone = wrap(0xa4, 0xac);
    _averageGpuProcessCpuPercent = wrap(0xb0, 0xb8);
    _averageNetworkProcessCpuPercent = wrap(0xbc, 0xc4);
    _averageExtensionEventProcessCpuPercent = wrap(0xc8, 0xd0);
    _averageExtensionPersistentProcessCpuPercent = wrap(0xd4, 0xdc);
    _averageRendererProcessCpuPercent = wrap(0xe0, 0xe8);
    _averageTotalCpuDuringFullscreenVideoPercent = wrap(0xec, 0xf4);
    _averageTotalCpuWithInteractionPercent = wrap(0xf8, 0x100);
    _averageTotalCpuWithNavigationPercent = wrap(0x104, 0x10c);
    _averageTotalCpuWhenIdleForegroundPercent = wrap(0x110, 0x118);
    _gpuMemoryFootprintMb = wrap(0x128, 0x130);
    _rendererMemoryFootprintMb = wrap(0x11c, 0x124);
    _totalRendererMemoryFootprintMb = wrap(0x17c, 0x184);
    _totalSharedMemoryFootprintMb = wrap(0x188, 0x190);
    _totalPrivateSwapFootprintMb = wrap(0x194, 0x19c);
    _totalResidentSetMb = wrap(0x1a0, 0x1a8);
    _totalGpuMappedMemoryMb = wrap(0x1ac, 0x1b4);
    _totalTileMemoryMb = wrap(0x1b8, 0x1c0);
    _networkServiceMemoryFootprintMb = wrap(0x134, 0x13c);
    _extensionMemoryFootprintMb = wrap(0x140, 0x148);
    _aiServiceMemoryFootprintMb = wrap(0x158, 0x160);
    _audioServiceMemoryFootprintMb = wrap(0x164, 0x16c);
    _utilityMemoryFootprintMb = wrap(0x170, 0x178);
    _averageAIServiceCpuPercent = wrap(0x14c, 0x154);
  }
  return self;
}
@end

ARC_EXPORT @interface ArcAdhocHistogramSamples : NSObject {
  NSMutableArray* _samples;
}
@property(nonatomic, readonly) NSMutableArray* samples;
- (instancetype)initWithAdhocHistogramSample:(const void*)sample;
@end

@implementation ArcAdhocHistogramSamples
@synthesize samples = _samples;
- (instancetype)initWithAdhocHistogramSample:(const void*)sample {
  if ((self = [super init])) {
    _samples = [[NSMutableArray alloc] init];
    // SAFETY: The native ABI supplies a valid input, whose reader loads a data
    // pointer at +8 and a count at +16. It does not retain either input region.
    const auto header = UNSAFE_BUFFERS(base::span<const uint8_t>(
        static_cast<const uint8_t*>(sample), size_t{24}));
    const uint64_t count = base::U64FromNativeEndian(header.subspan<16, 8>());
    // Match the original reader's scaled-count empty test and byte extent.
    const size_t byte_count = (count & UINT64_C(0x1fffffffffffffff)) * 8;
    if (byte_count) {
      const uintptr_t address = base::U64FromNativeEndian(header.subspan<8, 8>());
      // SAFETY: A valid native count names this many contiguous eight-byte rows.
      const auto rows = UNSAFE_BUFFERS(base::span<const uint8_t>(
          reinterpret_cast<const uint8_t*>(address), byte_count));
      for (size_t offset = 0; offset < rows.size(); offset += 8) {
        ArcAdhocHistogramSample* record = [[ArcAdhocHistogramSample alloc] init];
        const auto row = rows.subspan(offset).first<8>();
        const uint32_t type = base::U32FromNativeEndian(row.first<4>());
        if (type > 4) {
          // Original arm64 instruction at 0x4403608 is 0xd42aa160.
          // Keep its exact BRK immediate; CHECK would add different side effects.
          asm volatile("brk #0x550b");
          __builtin_unreachable();
        }
        [record setType:type];
        [record setValue:base::I32FromNativeEndian(row.subspan<4, 4>())];
        [_samples addObject:record];
      }
    }
  }
  return self;
}
@end
