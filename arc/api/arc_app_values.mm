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
static_assert(sizeof(ErrorStatus) == 8);
static_assert(sizeof(DebugProperty) == 48);
}  // namespace app_values

ARC_EXPORT @interface ArcReference : NSObject
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
@synthesize referenceText = _referenceText;
@synthesize minimumCharacterCount = _minimumCharacterCount;
- (instancetype)initWithReferenceText:(NSString*)text
               minimumCharacterCount:(NSInteger)count {
  if ((self = [super init])) {
    _referenceText = [text copy];
    _minimumCharacterCount = count;
  }
  return self;
}
@end
