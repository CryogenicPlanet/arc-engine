// Copyright 2026 ArcMod. Chromium flags are separate from Arc's runtime feature IDs.
#include "arc/api/arc_internal.h"
#include "base/feature_list.h"
#include "base/functional/bind.h"
#include "base/no_destructor.h"
#include "base/synchronization/lock.h"
#include "base/task/sequenced_task_runner.h"
#include "base/values.h"
#include "chrome/browser/about_flags.h"
#include "components/webui/flags/flags_storage.h"

#include <map>
#include <variant>

namespace {
BASE_FEATURE(kArcMacCoalescedResizing, "ArcMacCoalescedResizing", base::FEATURE_ENABLED_BY_DEFAULT);
// Arc's IDs are its own runtime configuration table, distinct from chrome://flags.
// Defaults below were recovered from ArcCore 1.170's DAT_0dff3480 initializers;
// platform/environment-specific overrides need a separate mapping when recovered.
struct RuntimeFeatures {
  using Value = std::variant<bool, int64_t, std::string>;
  RuntimeFeatures() {
    for (NSUInteger identifier = 0; identifier <= 45; ++identifier)
      if (identifier != 8) values.emplace(identifier, false);
    for (NSUInteger identifier : {0u, 3u, 13u, 17u, 33u, 43u}) values[identifier] = true;
    values[20] = int64_t{200};
    values[21] = int64_t{100};
    values[22] = int64_t{60};
    values[23] = int64_t{600};
    values[27] = std::string();
    values[28] = std::string();
  }
  base::Lock lock;
  std::map<NSUInteger, Value> values;
};
RuntimeFeatures& Features() {
  static base::NoDestructor<RuntimeFeatures> features;
  return *features;
}
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppFlags)
- (BOOL)isFeatureEnabled:(NSUInteger)identifier {
  // Arc's getter special-cases this upstream-style feature independently of its
  // mutable Arc runtime table, including after setFeatureEnabled:enabled:.
  if (identifier == 45) return base::FeatureList::IsEnabled(kArcMacCoalescedResizing);
  auto& features = Features();
  base::AutoLock hold(features.lock);
  auto entry = features.values.find(identifier);
  if (entry == features.values.end()) return NO;
  const bool* enabled = std::get_if<bool>(&entry->second);
  return enabled && *enabled;
}
- (void)setFeatureEnabled:(NSUInteger)identifier enabled:(BOOL)enabled {
  auto& features = Features();
  base::AutoLock hold(features.lock);
  features.values[identifier] = bool{enabled};
}
- (void)setStringFeatureValue:(NSUInteger)identifier value:(NSString*)value {
  auto& features = Features();
  base::AutoLock hold(features.lock);
  features.values[identifier] = ArcUTF8(value);
}
- (void)setIntFeatureValue:(NSUInteger)identifier value:(NSInteger)value {
  auto& features = Features();
  base::AutoLock hold(features.lock);
  features.values[identifier] = int64_t{value};
}
- (void)getNonDefaultFlags:(void (^)(NSArray<NSString*>*))completion {
  if (!completion) return;
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    about_flags::GetStorage(nullptr, base::BindOnce(^(std::unique_ptr<flags_ui::FlagsStorage> storage, flags_ui::FlagAccess access) {
      base::ListValue supported, unsupported;
      about_flags::GetFlagFeatureEntries(storage.get(), access, supported, unsupported);
      NSMutableArray<NSString*>* names = [NSMutableArray array];
      for (const base::Value& entry : supported) {
        const base::DictValue& dict = entry.GetDict();
        if (dict.FindBool("is_default").value_or(true)) continue;
        const std::string* name = dict.FindString("internal_name");
        [names addObject:name ? ArcNSString(*name) : @"non default feature with unknown name"];
      }
      completion(names);
    }));
  }));
}
- (void)setAllFlagsToDefault:(void (^)(void))completion {
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    about_flags::GetStorage(nullptr, base::BindOnce(^(std::unique_ptr<flags_ui::FlagsStorage> storage, flags_ui::FlagAccess access) {
      about_flags::ResetAllFlags(storage.get());
      if (completion) completion();
    }));
  }));
}
@end
