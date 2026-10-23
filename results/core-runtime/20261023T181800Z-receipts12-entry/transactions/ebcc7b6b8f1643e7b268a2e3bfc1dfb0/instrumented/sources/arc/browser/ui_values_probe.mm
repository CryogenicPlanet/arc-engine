// Bounded cold-dlopen probe. Only ObjC DTO and the trivial KeyboardAccelerator
// POD APIs are invoked. No browser initialization or C++ standard-library ABI
// crosses from this standalone system-clang executable into either engine.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>
#include <dlfcn.h>
#include <stdint.h>

struct KeyboardAccelerator {
  uint32_t key;
  int32_t modifiers;
};
@interface ArcKeyboardAccelerator : NSObject
- (instancetype)initWithAccelerator:(const KeyboardAccelerator*)accelerator;
@property(nonatomic, readonly) NSUInteger key;
@property(nonatomic, readonly) NSUInteger modifiers;
@end
@interface ArcNotificationButton : NSObject
@property(nonatomic, strong) NSString* title;
@property(nonatomic, strong) NSImage* icon;
@property(nonatomic, strong) NSString* placeholder;
@property(nonatomic) BOOL isTextInput;
@end
@interface ArcNotificationItem : NSObject
@property(nonatomic, strong) NSString* title;
@property(nonatomic, strong) NSString* message;
@end

static NSArray* Words(NSString* text) {
  return text.length ? [text componentsSeparatedByString:@" "] : @[];
}
static id JSONValue(id value) { return value ?: NSNull.null; }
static NSDictionary* Snapshot(id object, NSString* names) {
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (NSString* name in Words(names)) {
    id value = [object valueForKey:name];
    // Freeze mutable strings so subsequent mutations do not rewrite evidence.
    result[name] = JSONValue([value isKindOfClass:NSString.class] ? [value copy] : value);
  }
  return result;
}

static NSDictionary* Surface(NSString* name, NSString* selectors, NSString* fields) {
  Class cls = NSClassFromString(name);
  NSMutableDictionary* methods = [NSMutableDictionary dictionary];
  for (NSString* selector in Words(selectors)) {
    Method method = class_getInstanceMethod(cls, NSSelectorFromString(selector));
    methods[selector] = method
        ? [NSString stringWithUTF8String:method_getTypeEncoding(method)] : @"MISSING";
  }
  NSMutableDictionary* properties = [NSMutableDictionary dictionary];
  for (NSString* field in Words(fields)) {
    objc_property_t property = class_getProperty(cls, field.UTF8String);
    properties[field] = property
        ? [NSString stringWithUTF8String:property_getAttributes(property)] : @"MISSING";
  }
  return @{@"classPresent": @(cls != Nil), @"methods": methods, @"properties": properties};
}

int main(int argc, char** argv) {
  @autoreleasepool {
    if (argc != 2) { return 2; }
    if (!dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)) {
      fprintf(stderr, "Engine: %s\n", dlerror());
      return 3;
    }
    // Load after the engine's Objective-C classes register, as in ai_values_probe.
    const char* recorder = getenv("ARC_UI_RECORDER_LIBRARY");
    if (recorder && !dlopen(recorder, RTLD_NOW | RTLD_LOCAL)) {
      fprintf(stderr, "Recorder: %s\n", dlerror());
      return 4;
    }
    // Explicit implemented public surface. C++ getters/setters, .cxx methods,
    // native accessors and package-private helper selectors are excluded.
    NSDictionary* selectors = @{
      @"ArcSelectOption": @"initWithSelectOption: value text isSelected isDisabled",
      @"ArcFormControlInfo": @"initWithFormControlInfo: controlType fieldName fieldValue placeholder isChecked isRequired isRedacted selectOptions",
      @"ArcNotificationButton": @"title setTitle: icon setIcon: placeholder setPlaceholder: isTextInput setIsTextInput:",
      @"ArcNotificationItem": @"title setTitle: message setMessage:",
      @"ArcWebDialog": @"initWithWebDialog: accessibleTitle preferredSize minimumSize title webContents",
      @"ArcWebLoginDialog": @"initWithWebLoginDialog: isProxyLogin isURLPotentiallyTrustworthy authority close accept:with: cancel",
      @"ArcExternalProtocolDialog": @"initWithExternalProtocolDialog: programName origin close accept: cancel",
      @"ArcDevToolsConnectionDialog": @"initWithDevToolsConnectionDialog: accept deny disable",
      @"ArcMenuModel": @"initWithMenuModel: items menuWillShow menuWillClose",
      @"ArcMenuItemModel": @"initWithMenuItemModel: type logicalType label isChecked isEnabled isVisible isAlerted isNewFeature accessibleName submenuModel hasAccelerator accelerator icon activated setDelegate:",
      @"ArcContextMenuElementInfo": @"initWithContextMenuElementInfo: x y topLevelURL subFrameURL linkURL sourceAttributeURL selectionText suggestedFilename misspelledWord dictionarySuggestions inputType nodeType isEditable",
      @"ArcPermissionRequest": @"initWithPermissionRequest: type originUrl additionalData",
      @"ArcDisplayNameOrOrigin": @"initWithDisplayNameOrOrigin: nameOrOrigin isOrigin",
      @"ArcConfirmInfobar": @"initWithConfirmInfobar: messageText accept",
      @"ArcHungRendererDialog": @"initWithHungRendererDialog: crashProcess wait",
      @"ArcNotification": @"initWithNotification: type priority title id browserContextLocalID browserContextQualifiedID message contextMessage progressStatus origin documentURL progress renotify permanent browserContext showSnoozeButton showSettingsButton items buttons icon image smallImage category didClose: didClick:input: didClickSettings didReceiveResponse: iconURL setIconURL:",
      @"ArcKeyboardAccelerator": @"initWithAccelerator: key modifiers"
    };
    NSDictionary* fields = @{
      @"ArcSelectOption": @"value text isSelected isDisabled",
      @"ArcFormControlInfo": @"controlType fieldName fieldValue placeholder isChecked isRequired isRedacted selectOptions",
      @"ArcNotificationButton": @"title icon placeholder isTextInput",
      @"ArcNotificationItem": @"title message",
      @"ArcWebDialog": @"accessibleTitle preferredSize minimumSize title webContents",
      @"ArcWebLoginDialog": @"isProxyLogin isURLPotentiallyTrustworthy authority",
      @"ArcExternalProtocolDialog": @"programName origin",
      @"ArcDevToolsConnectionDialog": @"",
      @"ArcMenuModel": @"items",
      @"ArcMenuItemModel": @"type logicalType label isChecked isEnabled isVisible isAlerted isNewFeature accessibleName submenuModel hasAccelerator accelerator icon",
      @"ArcContextMenuElementInfo": @"x y topLevelURL subFrameURL linkURL sourceAttributeURL selectionText suggestedFilename misspelledWord dictionarySuggestions inputType nodeType isEditable",
      @"ArcPermissionRequest": @"type originUrl additionalData",
      @"ArcDisplayNameOrOrigin": @"nameOrOrigin isOrigin",
      @"ArcConfirmInfobar": @"",
      @"ArcHungRendererDialog": @"",
      @"ArcNotification": @"type priority title id browserContextLocalID browserContextQualifiedID message contextMessage progressStatus origin documentURL progress renotify permanent browserContext showSnoozeButton showSettingsButton items buttons icon image smallImage category iconURL",
      @"ArcKeyboardAccelerator": @"key modifiers"
    };
    NSMutableDictionary* metadata = [NSMutableDictionary dictionary];
    for (NSString* name in selectors) {
      metadata[name] = Surface(name, selectors[name], fields[name]);
    }
    for (NSString* name in @[@"ArcNotificationButton", @"ArcNotificationItem", @"ArcKeyboardAccelerator"]) {
      if (!NSClassFromString(name)) {
        fprintf(stderr, "Missing DTO class: %s\n", name.UTF8String);
        return 5;
      }
    }

    ArcNotificationButton* button = [NSClassFromString(@"ArcNotificationButton") new];
    ArcNotificationItem* item = [NSClassFromString(@"ArcNotificationItem") new];
    NSMutableDictionary* values = [NSMutableDictionary dictionary];
    values[@"buttonDefaults"] = Snapshot(button, @"title placeholder isTextInput");
    values[@"buttonDefaultIconNil"] = @(button.icon == nil);
    values[@"itemDefaults"] = Snapshot(item, @"title message");
    NSMutableString* text = [@"π/東京/🧪" mutableCopy];
    NSMutableString* placeholder = [@"reply" mutableCopy];
    NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(2, 3)];
    button.title = text;
    button.placeholder = placeholder;
    button.icon = image;
    button.isTextInput = YES;
    item.title = text;
    item.message = placeholder;
    values[@"retainsObjects"] = @{
      @"buttonTitle": @(button.title == text),
      @"buttonPlaceholder": @(button.placeholder == placeholder),
      @"buttonIcon": @(button.icon == image),
      @"itemTitle": @(item.title == text),
      @"itemMessage": @(item.message == placeholder)};
    values[@"buttonBeforeMutation"] = Snapshot(button, @"title placeholder isTextInput");
    values[@"itemBeforeMutation"] = Snapshot(item, @"title message");
    [text appendString:@" changed"];
    [placeholder appendString:@" changed"];
    values[@"buttonAfterMutation"] = Snapshot(button, @"title placeholder isTextInput");
    values[@"itemAfterMutation"] = Snapshot(item, @"title message");
    button.title = nil;
    button.placeholder = nil;
    button.icon = nil;
    button.isTextInput = NO;
    item.title = nil;
    item.message = nil;
    values[@"buttonAfterClear"] = Snapshot(button, @"title placeholder isTextInput");
    values[@"buttonClearedIconNil"] = @(button.icon == nil);
    values[@"itemAfterClear"] = Snapshot(item, @"title message");

    NSMutableArray* accelerators = [NSMutableArray array];
    for (NSNumber* key in @[@0, @65, @UINT32_MAX]) {
      for (NSNumber* modifiers in @[@0, @1, @(-1), @INT32_MIN]) {
        KeyboardAccelerator native{key.unsignedIntValue, modifiers.intValue};
        ArcKeyboardAccelerator* accelerator =
            [(ArcKeyboardAccelerator*)[NSClassFromString(@"ArcKeyboardAccelerator") alloc]
                initWithAccelerator:&native];
        [accelerators addObject:@{@"nativeKey": key, @"nativeModifiers": modifiers,
          @"key": @(accelerator.key), @"modifiers": @(accelerator.modifiers)}];
      }
    }
    values[@"accelerators"] = accelerators;
    NSDictionary* output = @{@"metadata": metadata, @"values": values};
    NSError* error = nil;
    NSData* data = [NSJSONSerialization dataWithJSONObject:output
        options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&error];
    if (!data) {
      fprintf(stderr, "JSON: %s\n", error.localizedDescription.UTF8String);
      return 6;
    }
    fwrite(data.bytes, 1, data.length, stdout);
    fputc('\n', stdout);
  }
  return 0;
}
