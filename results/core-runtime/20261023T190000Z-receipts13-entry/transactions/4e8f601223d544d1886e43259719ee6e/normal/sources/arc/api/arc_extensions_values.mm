// Copyright 2026 ArcMod. See arc/ARCHITECTURE.md.

#import "arc/browser/extensions_values.h"

#include "ui/gfx/image/image.h"

namespace arc {

class ImageImpl {
 public:
  explicit ImageImpl(const gfx::Image& image) : image(image) {}
  gfx::Image image;
};

Image::Image() = default;
Image::Image(const gfx::Image& image) {
  if (!image.IsEmpty()) {
    impl_ = std::make_shared<ImageImpl>(image);
  }
}
Image::Image(const Image&) = default;
Image& Image::operator=(const Image&) = default;
Image::~Image() = default;

const gfx::Image* Image::gfxImage() const {
  return impl_ ? &impl_->image : nullptr;
}

}  // namespace arc

namespace {

NSImage* NativeImage(const arc::Image& image) {
  const gfx::Image* native = image.gfxImage();
  return native ? native->AsNSImage() : nil;
}

}  // namespace

@implementation ArcExtensionInfo {
  std::optional<arc::ExtensionInfo> _extension_info;
}

- (instancetype)init {
  if ((self = [super init])) {
    _extension_info.emplace();
  }
  return self;
}

- (instancetype)initWithExtensionInfo:(const void*)info {
  if ((self = [super init])) {
    if (info) {
      _extension_info = *static_cast<const arc::ExtensionInfo*>(info);
    } else {
      _extension_info.emplace();
    }
  }
  return self;
}

- (void*)mutableExtensionInfo {
  return _extension_info ? &*_extension_info : nullptr;
}
- (NSString*)name {
  return _extension_info ? ArcNSString(_extension_info->name) : @"";
}
- (NSString*)id {
  return _extension_info ? ArcNSString(_extension_info->id) : @"";
}
- (NSString*)description {
  return _extension_info ? ArcNSString(_extension_info->description) : @"";
}
- (NSURL*)url {
  return _extension_info ? ArcNSURL(GURL(_extension_info->url)) : nil;
}
- (NSArray<NSString*>*)permissions {
  NSMutableArray<NSString*>* result = [NSMutableArray array];
  if (_extension_info) {
    for (const auto& permission : _extension_info->permissions) {
      [result addObject:ArcNSString(permission)];
    }
  }
  return result;
}
- (NSImage*)icon {
  return _extension_info ? NativeImage(_extension_info->icon) : nil;
}
- (double)rating {
  return _extension_info ? _extension_info->rating : 0;
}
- (long long)numRatings {
  return _extension_info ? _extension_info->num_ratings : 0;
}
- (BOOL)enabled {
  return _extension_info && _extension_info->enabled;
}
- (BOOL)hasAction {
  return _extension_info && _extension_info->has_action;
}
- (NSString*)version {
  return _extension_info ? ArcNSString(_extension_info->version) : @"";
}
- (NSString*)omniboxKeyword {
  return _extension_info ? ArcNSString(_extension_info->omnibox_keyword) : @"";
}
- (BOOL)fromWebstore {
  return _extension_info && _extension_info->from_webstore;
}

@end

@implementation ArcExtensionCommand {
  arc::ExtensionCommand _extension_command;
}

- (instancetype)initWithExtensionCommand:(const void*)command {
  if ((self = [super init]) && command) {
    _extension_command = *static_cast<const arc::ExtensionCommand*>(command);
  }
  return self;
}
- (NSString*)extensionId {
  return ArcNSString(_extension_command.extension_id);
}
- (NSString*)commandName {
  return ArcNSString(_extension_command.command_name);
}
- (NSString*)commandDescription {
  return ArcNSString(_extension_command.command_description);
}
- (NSString*)shortcut {
  return _extension_command.shortcut ? ArcNSString(*_extension_command.shortcut) : nil;
}
- (BOOL)isGlobal {
  return _extension_command.is_global;
}

@end

@implementation ArcExtensionRuntimeErrorStackFrame {
  std::optional<arc::StackFrame> _extension_runtime_error_stack_frame;
}

- (instancetype)initWithExtensionRuntimeErrorStackFrame:(const void*)frame {
  if ((self = [super init]) && frame) {
    _extension_runtime_error_stack_frame = *static_cast<const arc::StackFrame*>(frame);
  }
  return self;
}
- (unsigned int)lineNumber {
  return _extension_runtime_error_stack_frame ? _extension_runtime_error_stack_frame->line_number : 0;
}
- (unsigned int)columnNumber {
  return _extension_runtime_error_stack_frame ? _extension_runtime_error_stack_frame->column_number : 0;
}
- (NSString*)source {
  return _extension_runtime_error_stack_frame ? ArcNSString(_extension_runtime_error_stack_frame->source) : @"";
}
- (NSString*)function {
  return _extension_runtime_error_stack_frame ? ArcNSString(_extension_runtime_error_stack_frame->function) : @"";
}

@end

@implementation ArcExtensionRuntimeError {
  NSArray<ArcExtensionRuntimeErrorStackFrame*>* _stackTrace;
  std::optional<arc::ExtensionRuntimeError> _extension_runtime_error;
}

- (instancetype)initWithExtensionRuntimeError:(const void*)error {
  if ((self = [super init]) && error) {
    _extension_runtime_error = *static_cast<const arc::ExtensionRuntimeError*>(error);
  }
  return self;
}
- (NSString*)extensionID {
  return _extension_runtime_error ? ArcNSString(_extension_runtime_error->extension_id) : @"";
}
- (NSURL*)contextURL {
  return _extension_runtime_error ? ArcNSURL(GURL(_extension_runtime_error->context_url_spec)) : nil;
}
- (NSString*)message {
  return _extension_runtime_error ? ArcNSString(_extension_runtime_error->message) : @"";
}
- (NSString*)source {
  return _extension_runtime_error ? ArcNSString(_extension_runtime_error->source) : @"";
}
- (BOOL)fromIncognito {
  return _extension_runtime_error && _extension_runtime_error->from_incognito;
}
- (long long)severity {
  return _extension_runtime_error ? _extension_runtime_error->severity : 0;
}
- (NSArray<ArcExtensionRuntimeErrorStackFrame*>*)stackTrace {
  if (!_stackTrace) {
    NSMutableArray<ArcExtensionRuntimeErrorStackFrame*>* frames = [NSMutableArray array];
    if (_extension_runtime_error) {
      for (const auto& frame : _extension_runtime_error->stack_trace) {
        [frames addObject:[[ArcExtensionRuntimeErrorStackFrame alloc]
                              initWithExtensionRuntimeErrorStackFrame:&frame]];
      }
    }
    _stackTrace = frames;
  }
  return _stackTrace;
}

@end

@implementation ArcExtensionToolbarAction {
  arc::ExtensionToolbarAction _extension_toolbar_action;
}

- (instancetype)initWithExtensionToolbarAction:(const void*)action {
  if ((self = [super init]) && action) {
    _extension_toolbar_action = *static_cast<const arc::ExtensionToolbarAction*>(action);
  }
  return self;
}
- (NSString*)id {
  return ArcNSString(_extension_toolbar_action.id);
}
- (NSString*)accessibleName {
  return ArcNSString(_extension_toolbar_action.accessible_name);
}
- (NSString*)tooltipText {
  return ArcNSString(_extension_toolbar_action.tooltip_text);
}
- (NSImage*)icon {
  return NativeImage(_extension_toolbar_action.icon);
}
- (BOOL)enabled {
  return _extension_toolbar_action.enabled;
}
- (NSNumber*)pinnedPosition {
  return _extension_toolbar_action.pinned_position ? @(*_extension_toolbar_action.pinned_position) : nil;
}

@end
