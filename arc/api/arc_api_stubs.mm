// Copyright 2026 ArcMod. Engine shell for Arc's app layer.
//
// Declares all 146 Arc classes so Arc's app links against this framework, and gives every method we
// haven't implemented yet a stub. A stub logs "unimplemented -[Class selector]" once and returns an
// empty value of the method's return type. Real implementations live in their own files (as categories
// or full @implementations); class_addMethod never replaces them, so the stub set shrinks as code lands.

#import <AppKit/AppKit.h>
#import <objc/runtime.h>

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
// init-family stubs return the allocated object, so Swift initializers don't trap on nil.
id StubInit(id self, SEL cmd) {
  LogUnimplemented(self, cmd);
  return self;
}

IMP StubFor(const ArcApiMethod& m) {
  if (m.returns == ArcStubReturn::kWord && m.types[0] == '@' && !m.is_class_method &&
      std::string_view(m.selector).starts_with("init")) {
    return reinterpret_cast<IMP>(StubInit);
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
    // Keep NSObject behavior (isEqual:, hash, description, ...) unless we implement it ourselves.
    if (m.is_class_method ? [NSObject respondsToSelector:sel]
                          : [NSObject instancesRespondToSelector:sel]) {
      continue;
    }
    class_addMethod(target, sel, StubFor(m), m.types);  // no-op if already implemented
  }
}
