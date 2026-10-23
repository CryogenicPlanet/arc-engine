// Copyright 2026 ArcMod. Chromium flags are separate from Arc's runtime feature IDs.
#include "arc/api/arc_internal.h"
#include "base/functional/bind.h"
#include "base/task/sequenced_task_runner.h"
#include "base/values.h"
#include "chrome/browser/about_flags.h"
#include "components/webui/flags/flags_storage.h"

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppFlags)
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
