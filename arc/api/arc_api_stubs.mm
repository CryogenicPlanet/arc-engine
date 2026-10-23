// Copyright 2026 ArcMod. Engine shell for Arc's app layer.
//
// Declares all 146 Arc classes so Arc's app links against this framework, and gives every method we
// haven't implemented yet a stub. A stub logs "unimplemented -[Class selector]" once and returns an
// empty value of the method's return type. Real implementations live in their own files (as categories
// or full @implementations); class_addMethod never replaces them, so the stub set shrinks as code lands.

#import <AppKit/AppKit.h>
#import <objc/runtime.h>

#include <map>
#include <set>
#include <string>
#include <string_view>

#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/synchronization/lock.h"

enum class ArcStubReturn { kVoid, kWord, kFloat, kDouble, kPair, kRect };

namespace {

struct ArcApiMethod {
  const char* class_name;
  bool is_class_method;
  const char* selector;
  const char* types;
  ArcStubReturn returns;
  const char* getter_class;  // declared class of a property getter's result, or ""
};

constexpr ArcApiMethod kArcApiMethods[] = {
#include "arc/api/arc_api_table.inc"
};

void LogUnimplemented(id self, SEL cmd) {
  static base::NoDestructor<base::Lock> lock;
  static base::NoDestructor<std::set<std::string>> seen;
  const bool is_class = object_isClass(self);
  std::string key = std::string(is_class ? "+[" : "-[") +
                    (is_class ? class_getName(static_cast<Class>(self))
                              : object_getClassName(self)) +
                    " " + sel_getName(cmd) + "]";
  base::AutoLock guard(*lock);
  if (seen->insert(key).second) {
    LOG(ERROR) << "ArcCore unimplemented: " << key;
  }
}

// Arguments are ignored, so one IMP per return register class covers every signature.
void StubVoid(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
}
uintptr_t StubWord(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return 0;
}
float StubFloat(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return 0;
}
double StubDouble(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return 0;
}
CGSize StubPair(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return CGSizeZero;
}
CGRect StubRect(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return CGRectZero;
}
// Property getters whose declared class is known return an empty value of that class instead of nil
// (Swift traps on nil where the app expects an object). Still logged as unimplemented.
std::map<std::pair<Class, SEL>, std::string>& GetterClasses() {
  static base::NoDestructor<std::map<std::pair<Class, SEL>, std::string>> classes;
  return *classes;
}

id EmptyValueOfClass(const std::string& name) {
  if (name == "NSString" || name == "NSMutableString") {
    return @"";
  }
  if (name == "NSArray" || name == "NSMutableArray") {
    return @[];
  }
  if (name == "NSDictionary" || name == "NSMutableDictionary") {
    return @{};
  }
  if (name == "NSSet" || name == "NSMutableSet") {
    return [NSSet set];
  }
  if (name == "NSNumber") {
    return @0;
  }
  if (name == "NSData" || name == "NSMutableData") {
    return [NSData data];
  }
  if (name == "NSURL") {
    return [NSURL URLWithString:@"about:blank"];
  }
  if (name == "NSDate") {
    return [NSDate dateWithTimeIntervalSince1970:0];
  }
  if (name.starts_with("Arc")) {
    Class cls = objc_getClass(name.c_str());
    return cls ? [[cls alloc] init] : nil;
  }
  return nil;
}

id StubGetter(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  for (Class cls = object_getClass(self); cls; cls = class_getSuperclass(cls)) {
    auto it = GetterClasses().find({cls, cmd});
    if (it != GetterClasses().end()) {
      return EmptyValueOfClass(it->second);
    }
  }
  return nil;
}

// init-family stubs return the allocated object, so Swift initializers don't trap on nil.
id StubInit(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return self;
}

bool IsNSObjectIdentitySelector(std::string_view selector) {
  for (std::string_view s : {"isEqual:", "hash", "description", "debugDescription", "init", "copyWithZone:",
                             "mutableCopyWithZone:", "class", "self", "superclass"}) {
    if (selector == s) {
      return true;
    }
  }
  return false;
}

IMP StubFor(const ArcApiMethod& m) {
  if (m.returns == ArcStubReturn::kWord && m.types[0] == '@' && !m.is_class_method &&
      std::string_view(m.selector).starts_with("init")) {
    return reinterpret_cast<IMP>(StubInit);
  }
  if (m.getter_class[0] != '\0') {
    return reinterpret_cast<IMP>(StubGetter);
  }
  switch (m.returns) {
    case ArcStubReturn::kVoid:
      return reinterpret_cast<IMP>(StubVoid);
    case ArcStubReturn::kWord:
      return reinterpret_cast<IMP>(StubWord);
    case ArcStubReturn::kFloat:
      return reinterpret_cast<IMP>(StubFloat);
    case ArcStubReturn::kDouble:
      return reinterpret_cast<IMP>(StubDouble);
    case ArcStubReturn::kPair:
      return reinterpret_cast<IMP>(StubPair);
    case ArcStubReturn::kRect:
      return reinterpret_cast<IMP>(StubRect);
  }
}

}  // namespace

#define ARC_CLASS(name)                                     \
  __attribute__((visibility("default"))) @interface name    \
      : NSObject                                            \
      @end                                                  \
      @implementation name                                  \
      @end
// Classes that have a hand-written @implementation are excluded here by the generator's skip list.
#include "arc/api/arc_api_classes.inc"
#undef ARC_CLASS

__attribute__((constructor)) static void ArcInstallApiStubs() {
  for (const ArcApiMethod& m : kArcApiMethods) {
    Class cls = objc_getClass(m.class_name);
    if (!cls) {
      continue;
    }
    SEL sel = sel_registerName(m.selector);
    Class target = m.is_class_method ? object_getClass(cls) : cls;
    // Keep NSObject's identity behavior unless we implement it ourselves. Only these: newer macOS adds
    // NSObject methods that share Arc's selectors (e.g. -addObserver: in macOS 26), and those must
    // reach Arc's implementation or its stub.
    if (IsNSObjectIdentitySelector(m.selector)) {
      continue;
    }
    if (m.getter_class[0] != '\0') {
      GetterClasses()[{target, sel}] = m.getter_class;
    }
    class_addMethod(target, sel, StubFor(m), m.types);  // no-op if already implemented
  }
}
