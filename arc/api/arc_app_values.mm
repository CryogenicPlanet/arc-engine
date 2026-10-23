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
