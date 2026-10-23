// Copyright 2026 ArcMod. Native data owned by the host UI adapters.
#ifndef ARC_BROWSER_UI_VALUES_H_
#define ARC_BROWSER_UI_VALUES_H_

#import "arc/api/arc_internal.h"

#include <functional>
#include <optional>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "ui/gfx/geometry/size.h"

namespace arc {

// These are our producer contracts, not the proprietary ArcCore Impl ABI.
// The public const-void initializers below copy the corresponding value.
struct UISelectOptionData {
  std::optional<std::string> value;
  std::optional<std::string> text;
  bool is_selected = false;
  bool is_disabled = false;
};

struct UIFormControlInfoData {
  int control_type = 0;
  std::optional<std::string> field_name;
  std::optional<std::string> field_value;
  std::optional<std::string> placeholder;
  std::vector<UISelectOptionData> select_options;
  bool is_checked = false;
  bool is_required = false;
  bool is_redacted = false;
};

struct UIWebDialogData {
  std::u16string accessible_title;
  gfx::Size preferred_size;
  gfx::Size minimum_size;
  std::u16string title;
  base::WeakPtr<content::WebContents> web_contents;
};

struct UIWebLoginDialogData {
  bool is_proxy_login = false;
  bool is_url_potentially_trustworthy = false;
  std::u16string authority;
  std::function<void(const std::string&, const std::string&)> accepted;
  std::function<void()> cancelled;
  std::function<void()> closed;
};

struct UIExternalProtocolDialogData {
  std::u16string program_name;
  std::u16string origin;
  std::function<void(bool)> accepted;
  std::function<void()> cancelled;
  std::function<void()> closed;
};

struct UIDevToolsConnectionDialogData {
  std::function<void()> accepted;
  std::function<void()> denied;
  std::function<void()> disabled;
};

}  // namespace arc

ARC_EXPORT @interface ArcSelectOption : NSObject
- (instancetype)initWithSelectOption:(const void*)option;
@property(nonatomic, readonly, copy) NSString* value;
@property(nonatomic, readonly, copy) NSString* text;
@property(nonatomic, readonly) BOOL isSelected;
@property(nonatomic, readonly) BOOL isDisabled;
@end

ARC_EXPORT @interface ArcFormControlInfo : NSObject
- (instancetype)initWithFormControlInfo:(const void*)info;
@property(nonatomic, readonly) NSInteger controlType;
@property(nonatomic, readonly, copy) NSString* fieldName;
@property(nonatomic, readonly, copy) NSString* fieldValue;
@property(nonatomic, readonly, copy) NSString* placeholder;
@property(nonatomic, readonly) BOOL isChecked;
@property(nonatomic, readonly) BOOL isRequired;
@property(nonatomic, readonly) BOOL isRedacted;
@property(nonatomic, readonly) NSArray* selectOptions;
@end

ARC_EXPORT @interface ArcNotificationButton : NSObject
@property(nonatomic, strong) NSString* title;
@property(nonatomic, strong) NSImage* icon;
@property(nonatomic, strong) NSString* placeholder;
@property(nonatomic) BOOL isTextInput;
@end

ARC_EXPORT @interface ArcNotificationItem : NSObject
@property(nonatomic, strong) NSString* title;
@property(nonatomic, strong) NSString* message;
@end

ARC_EXPORT @interface ArcWebDialog : NSObject
- (instancetype)initWithWebDialog:(const void*)dialog;
@property(nonatomic, readonly) NSString* accessibleTitle;
@property(nonatomic, readonly) CGSize preferredSize;
@property(nonatomic, readonly) CGSize minimumSize;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) ArcWebContents* webContents;
@end

ARC_EXPORT @interface ArcWebLoginDialog : NSObject
- (instancetype)initWithWebLoginDialog:(const void*)dialog;
@property(nonatomic, readonly) BOOL isProxyLogin;
@property(nonatomic, readonly) BOOL isURLPotentiallyTrustworthy;
@property(nonatomic, readonly) NSString* authority;
- (void)close;
- (void)accept:(NSString*)username with:(NSString*)password;
- (void)cancel;
@end

ARC_EXPORT @interface ArcExternalProtocolDialog : NSObject
- (instancetype)initWithExternalProtocolDialog:(const void*)dialog;
@property(nonatomic, readonly, copy) NSString* programName;
@property(nonatomic, readonly, copy) NSString* origin;
- (void)close;
- (void)accept:(BOOL)remember;
- (void)cancel;
@end

ARC_EXPORT @interface ArcDevToolsConnectionDialog : NSObject
- (instancetype)initWithDevToolsConnectionDialog:(const void*)dialog;
- (void)accept;
- (void)deny;
- (void)disable;
@end

#endif  // ARC_BROWSER_UI_VALUES_H_
