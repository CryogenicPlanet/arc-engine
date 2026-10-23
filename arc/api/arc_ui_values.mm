// Copyright 2026 ArcMod. UI value transport recovered from specs/<Class>.md.
#import "arc/browser/ui_values.h"

#include "base/check.h"
#include "content/public/browser/web_contents.h"

namespace {

NSString* OptionalString(const std::optional<std::string>& value) {
  return value ? ArcNSString(*value) : nil;
}

}  // namespace

@implementation ArcSelectOption {
  std::optional<arc::UISelectOptionData> _select_option;
}
- (instancetype)initWithSelectOption:(const void*)option {
  CHECK(option);
  if ((self = [super init])) {
    _select_option = *static_cast<const arc::UISelectOptionData*>(option);
  }
  return self;
}
- (NSString*)value {
  CHECK(_select_option);
  return OptionalString(_select_option->value);
}
- (NSString*)text {
  CHECK(_select_option);
  return OptionalString(_select_option->text);
}
- (BOOL)isSelected {
  CHECK(_select_option);
  return _select_option->is_selected;
}
- (BOOL)isDisabled {
  CHECK(_select_option);
  return _select_option->is_disabled;
}
@end

@implementation ArcFormControlInfo {
  NSArray* _selectOptions;
  std::optional<arc::UIFormControlInfoData> _form_control_info;
}
@synthesize selectOptions = _selectOptions;
- (instancetype)initWithFormControlInfo:(const void*)info {
  CHECK(info);
  if ((self = [super init])) {
    _form_control_info = *static_cast<const arc::UIFormControlInfoData*>(info);
  }
  return self;
}
- (NSInteger)controlType {
  CHECK(_form_control_info);
  return _form_control_info->control_type;
}
- (NSString*)fieldName {
  CHECK(_form_control_info);
  return OptionalString(_form_control_info->field_name);
}
- (NSString*)fieldValue {
  CHECK(_form_control_info);
  return OptionalString(_form_control_info->field_value);
}
- (NSString*)placeholder {
  CHECK(_form_control_info);
  return OptionalString(_form_control_info->placeholder);
}
- (BOOL)isChecked {
  CHECK(_form_control_info);
  return _form_control_info->is_checked;
}
- (BOOL)isRequired {
  CHECK(_form_control_info);
  return _form_control_info->is_required;
}
- (BOOL)isRedacted {
  CHECK(_form_control_info);
  return _form_control_info->is_redacted;
}
- (NSArray*)selectOptions {
  if (!_selectOptions) {
    CHECK(_form_control_info);
    NSMutableArray* options = [NSMutableArray
        arrayWithCapacity:_form_control_info->select_options.size()];
    for (const auto& option : _form_control_info->select_options) {
      [options addObject:[[ArcSelectOption alloc] initWithSelectOption:&option]];
    }
    // The original returns the same mutable collection on later reads.
    _selectOptions = options;
  }
  return _selectOptions;
}
@end

@implementation ArcNotificationButton
@synthesize isTextInput = _isTextInput;
@synthesize title = _title;
@synthesize icon = _icon;
@synthesize placeholder = _placeholder;
@end

@implementation ArcNotificationItem
@synthesize title = _title;
@synthesize message = _message;
@end

@implementation ArcWebDialog {
  arc::UIWebDialogData _dialog;
}
- (instancetype)initWithWebDialog:(const void*)dialog {
  CHECK(dialog);
  if ((self = [super init])) {
    _dialog = *static_cast<const arc::UIWebDialogData*>(dialog);
  }
  return self;
}
- (NSString*)accessibleTitle { return ArcNSString(_dialog.accessible_title); }
- (CGSize)preferredSize {
  return CGSizeMake(_dialog.preferred_size.width(),
                    _dialog.preferred_size.height());
}
- (CGSize)minimumSize {
  return CGSizeMake(_dialog.minimum_size.width(), _dialog.minimum_size.height());
}
- (NSString*)title { return ArcNSString(_dialog.title); }
- (ArcWebContents*)webContents {
  content::WebContents* contents = _dialog.web_contents.get();
  return contents ? [ArcWebContents wrapperForWebContents:contents] : nil;
}
@end

@implementation ArcWebLoginDialog {
  arc::UIWebLoginDialogData _dialog;
}
- (instancetype)initWithWebLoginDialog:(const void*)dialog {
  CHECK(dialog);
  if ((self = [super init])) {
    _dialog = *static_cast<const arc::UIWebLoginDialogData*>(dialog);
  }
  return self;
}
- (BOOL)isProxyLogin { return _dialog.is_proxy_login; }
- (BOOL)isURLPotentiallyTrustworthy {
  return _dialog.is_url_potentially_trustworthy;
}
- (NSString*)authority { return ArcNSString(_dialog.authority); }
- (void)close {
  auto callback = _dialog.closed;
  if (callback) {
    callback();
  }
}
- (void)accept:(NSString*)username with:(NSString*)password {
  auto callback = _dialog.accepted;
  if (callback) {
    callback(ArcUTF8(username), ArcUTF8(password));
  }
}
- (void)cancel {
  auto callback = _dialog.cancelled;
  if (callback) {
    callback();
  }
}
@end

@implementation ArcExternalProtocolDialog {
  std::optional<arc::UIExternalProtocolDialogData> _external_protocol_dialog;
}
- (instancetype)initWithExternalProtocolDialog:(const void*)dialog {
  CHECK(dialog);
  if ((self = [super init])) {
    _external_protocol_dialog =
        *static_cast<const arc::UIExternalProtocolDialogData*>(dialog);
  }
  return self;
}
- (NSString*)programName {
  CHECK(_external_protocol_dialog);
  return ArcNSString(_external_protocol_dialog->program_name);
}
- (NSString*)origin {
  CHECK(_external_protocol_dialog);
  return ArcNSString(_external_protocol_dialog->origin);
}
- (void)close {
  CHECK(_external_protocol_dialog);
  auto callback = _external_protocol_dialog->closed;
  if (callback) {
    callback();
  }
}
- (void)accept:(BOOL)remember {
  CHECK(_external_protocol_dialog);
  auto callback = _external_protocol_dialog->accepted;
  if (callback) {
    callback(remember);
  }
}
- (void)cancel {
  CHECK(_external_protocol_dialog);
  auto callback = _external_protocol_dialog->cancelled;
  if (callback) {
    callback();
  }
}
@end

@implementation ArcDevToolsConnectionDialog {
  arc::UIDevToolsConnectionDialogData _dialog;
}
- (instancetype)initWithDevToolsConnectionDialog:(const void*)dialog {
  CHECK(dialog);
  if ((self = [super init])) {
    _dialog = *static_cast<const arc::UIDevToolsConnectionDialogData*>(dialog);
  }
  return self;
}
- (void)accept {
  auto callback = _dialog.accepted;
  if (callback) {
    callback();
  }
}
- (void)deny {
  auto callback = _dialog.denied;
  if (callback) {
    callback();
  }
}
- (void)disable {
  auto callback = _dialog.disabled;
  if (callback) {
    callback();
  }
}
@end
