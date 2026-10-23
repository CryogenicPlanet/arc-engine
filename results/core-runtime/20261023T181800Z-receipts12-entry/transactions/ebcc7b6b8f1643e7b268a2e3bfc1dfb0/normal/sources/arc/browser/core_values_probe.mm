// Bounded core value comparison; loads frameworks without starting a browser.
#import "arc/browser/core_values.h"
#import <objc/runtime.h>

#include <dlfcn.h>
#include <array>

@interface CoreProbeConfiguration : NSObject
@property(nonatomic) CGSize forcedViewSize;
@end

// SessionID's pointer initializer consumes one signed int. Define only that
// portable record here; no Chromium constructor or validation is involved.
class SessionID {
 public:
  int id;
};
static_assert(sizeof(SessionID) == 4);

namespace {
NSDictionary* Snapshot(id object, NSString* fields) {
  NSMutableDictionary* values = [NSMutableDictionary dictionary];
  for (NSString* field in [fields componentsSeparatedByString:@" "]) {
    id value = [object valueForKey:field];
    values[field] = [value isKindOfClass:[NSURL class]] ? [value absoluteString] : value ?: NSNull.null;
  }
  return values;
}

NSDictionary* Methods(NSString* class_name, NSString* selectors) {
  Class type = NSClassFromString(class_name);
  NSMutableDictionary* methods = [NSMutableDictionary dictionary];
  for (NSString* name in [selectors componentsSeparatedByString:@" "]) {
    Method method = class_getInstanceMethod(type, NSSelectorFromString(name));
    methods[name] = method ? @(method_getTypeEncoding(method)) : @"MISSING";
  }
  return methods;
}

NSDictionary* Properties(NSString* class_name, NSString* fields) {
  Class type = NSClassFromString(class_name);
  NSMutableDictionary* properties = [NSMutableDictionary dictionary];
  for (NSString* field in [fields componentsSeparatedByString:@" "]) {
    objc_property_t property = class_getProperty(type, field.UTF8String);
    properties[field] = property ? @(property_getAttributes(property)) : @"MISSING";
  }
  return properties;
}
}  // namespace

int main(int argc, char** argv) {
  @autoreleasepool {
    if (argc != 2 || !dlopen([NSProcessInfo.processInfo.arguments objectAtIndex:1].UTF8String,
                            RTLD_NOW | RTLD_LOCAL)) {
      return 2;
    }
    const char* recorder = getenv("ARC_CORE_VALUES_RECORDER_LIBRARY");
    if (recorder && !dlopen(recorder, RTLD_NOW | RTLD_LOCAL)) {
      return 3;
    }
    NSString* info_fields = @"groupId extensionGroupId title color collapsed tabSessionIds";
    NSString* result_fields = @"success error groupId extensionGroupId";
    NSString* properties_fields = @"title color collapsed";
    NSMutableDictionary* root = [NSMutableDictionary dictionary];
    root[@"metadata"] = @{
      @"ArcSessionID": Methods(@"ArcSessionID", @"initWithID: initWithSessionId: isEqual: hash identifier"),
      @"ArcWebContentsConfiguration": Methods(@"ArcWebContentsConfiguration", @"foreground setForeground: untracked setUntracked: forceDisplayPDFInline setForceDisplayPDFInline: disallowDownloads setDisallowDownloads: disallowAutoplay setDisallowAutoplay: forceAllowAutoplay setForceAllowAutoplay: disallowContextMenus setDisallowContextMenus: transparentBackground setTransparentBackground: initiallyMuted setInitiallyMuted: initiallyHidden setInitiallyHidden: forcedViewSize setForcedViewSize:"),
      @"ArcRedirectEntry": Methods(@"ArcRedirectEntry", @"initWithRedirectEntry: url title hidden redirectType"),
      @"ArcKeyboardAccelerator": Methods(@"ArcKeyboardAccelerator", @"initWithAccelerator: key modifiers"),
      @"ArcKeyEventData": Methods(@"ArcKeyEventData", @"initWithEventType:domCode:key:windowsVirtualKeyCode: eventType domCode key windowsVirtualKeyCode"),
      @"ArcOverscrollBehavior": Methods(@"ArcOverscrollBehavior", @"init x setX: y setY:"),
      @"ArcOverscrollParams": Methods(@"ArcOverscrollParams", @"init overscrollBehavior setOverscrollBehavior:"),
      @"ArcFindResult": Methods(@"ArcFindResult", @"initWithFindResult: numMatches activeMatchOrdinal"),
      @"ArcFindTextWithDirectiveResult": Methods(@"ArcFindTextWithDirectiveResult", @"initWithFindTextWithDirectiveResult: directive result"),
      @"ArcTabGroupInfo": Methods(@"ArcTabGroupInfo", @"init groupId setGroupId: extensionGroupId setExtensionGroupId: title setTitle: color setColor: collapsed setCollapsed: tabSessionIds setTabSessionIds:"),
      @"ArcTabGroupResult": Methods(@"ArcTabGroupResult", @"init success setSuccess: error setError: groupId setGroupId: extensionGroupId setExtensionGroupId:"),
      @"ArcTabGroupProperties": Methods(@"ArcTabGroupProperties", @"init initWithTabGroupProperties: title setTitle: color setColor: collapsed setCollapsed:")
    };
    root[@"instanceSizes"] = @{
      @"ArcSessionID": @(class_getInstanceSize(NSClassFromString(@"ArcSessionID"))),
      @"ArcWebContentsConfiguration": @(class_getInstanceSize(NSClassFromString(@"ArcWebContentsConfiguration"))),
      @"ArcRedirectEntry": @(class_getInstanceSize(NSClassFromString(@"ArcRedirectEntry"))),
      @"ArcKeyboardAccelerator": @(class_getInstanceSize(NSClassFromString(@"ArcKeyboardAccelerator"))),
      @"ArcKeyEventData": @(class_getInstanceSize(NSClassFromString(@"ArcKeyEventData"))),
      @"ArcOverscrollBehavior": @(class_getInstanceSize(NSClassFromString(@"ArcOverscrollBehavior"))),
      @"ArcOverscrollParams": @(class_getInstanceSize(NSClassFromString(@"ArcOverscrollParams"))),
      @"ArcFindResult": @(class_getInstanceSize(NSClassFromString(@"ArcFindResult"))),
      @"ArcFindTextWithDirectiveResult": @(class_getInstanceSize(NSClassFromString(@"ArcFindTextWithDirectiveResult"))),
      @"ArcTabGroupInfo": @(class_getInstanceSize(NSClassFromString(@"ArcTabGroupInfo"))),
      @"ArcTabGroupResult": @(class_getInstanceSize(NSClassFromString(@"ArcTabGroupResult"))),
      @"ArcTabGroupProperties": @(class_getInstanceSize(NSClassFromString(@"ArcTabGroupProperties")))
    };
    objc_property_t session_identifier_property =
        class_getProperty(NSClassFromString(@"ArcSessionID"), "identifier");
    root[@"propertyMetadata"] = @{
      @"ArcSessionID.identifier": session_identifier_property
          ? @(property_getAttributes(session_identifier_property)) : @"MISSING"
    };
    NSString* configuration_fields = @"foreground untracked forceDisplayPDFInline disallowDownloads disallowAutoplay forceAllowAutoplay disallowContextMenus transparentBackground initiallyMuted initiallyHidden";
    CoreProbeConfiguration* configuration = [[NSClassFromString(@"ArcWebContentsConfiguration") alloc] init];
    root[@"defaultConfiguration"] = Snapshot(configuration, configuration_fields);
    NSMutableDictionary* configuration_properties = [NSMutableDictionary dictionary];
    for (NSString* field in [[configuration_fields stringByAppendingString:@" forcedViewSize"] componentsSeparatedByString:@" "]) {
      objc_property_t property = class_getProperty(NSClassFromString(@"ArcWebContentsConfiguration"), field.UTF8String);
      configuration_properties[field] = property ? @(property_getAttributes(property)) : @"MISSING";
      if (![field isEqual:@"forcedViewSize"]) { [configuration setValue:@YES forKey:field]; }
    }
    root[@"configurationProperties"] = configuration_properties;
    root[@"nativeWrapperPropertyMetadata"] = @{
      @"ArcBrowserCreateParams": Properties(@"ArcBrowserCreateParams", @"userTitle canResize canMaximize canFullscreen initialBounds initialOriginSpecified pipSize"),
      @"ArcBackForwardList": Properties(@"ArcBackForwardList", @"backList forwardList backItem currentItem forwardItem"),
      @"ArcWebContents": Properties(@"ArcWebContents", @"backForwardList rootBackgroundColor pdfController userActivityDelegate")
    };
    root[@"enabledConfiguration"] = Snapshot(configuration, configuration_fields);
    NSMutableArray* forced_sizes = [NSMutableArray array];
    const std::array<CGSize, 7> configuration_sizes = {
        CGSizeZero, CGSizeMake(127.75, 255.25), CGSizeMake(0.1000000005, 20),
        CGSizeMake(0.100000002, 20), CGSizeMake(-1, 20), CGSizeMake(20, 0.1),
        CGSizeMake(2147483647, 1)};
    for (const CGSize size : configuration_sizes) {
      configuration.forcedViewSize = size;
      const CGSize actual = configuration.forcedViewSize;
      [forced_sizes addObject:@[@(actual.width), @(actual.height)]];
    }
    root[@"configurationForcedSizes"] = forced_sizes;
    id info = [[NSClassFromString(@"ArcTabGroupInfo") alloc] init];
    id result = [[NSClassFromString(@"ArcTabGroupResult") alloc] init];
    id properties = [[NSClassFromString(@"ArcTabGroupProperties") alloc] init];
    root[@"defaultInfo"] = Snapshot(info, info_fields);
    root[@"defaultResult"] = Snapshot(result, result_fields);
    root[@"defaultProperties"] = Snapshot(properties, properties_fields);

    NSMutableString* title = [@"Title 🌙" mutableCopy];
    NSMutableString* group = [@"group-original" mutableCopy];
    NSMutableString* error = [@"operation failed" mutableCopy];
    NSMutableString* first_id = [@"id-original" mutableCopy];
    NSMutableArray* ids = [NSMutableArray arrayWithObjects:first_id, @2, nil];
    [info setValue:title forKey:@"title"];
    [info setValue:group forKey:@"groupId"];
    [info setValue:@(-42) forKey:@"extensionGroupId"];
    [info setValue:@(-17) forKey:@"color"];
    [info setValue:@YES forKey:@"collapsed"];
    [info setValue:ids forKey:@"tabSessionIds"];
    [result setValue:@YES forKey:@"success"];
    [result setValue:error forKey:@"error"];
    [result setValue:group forKey:@"groupId"];
    [result setValue:@1234567890123LL forKey:@"extensionGroupId"];
    [properties setValue:title forKey:@"title"];
    [properties setValue:@4294967297LL forKey:@"color"];
    [properties setValue:@2 forKey:@"collapsed"];
    [title appendString:@" mutated"];
    [group appendString:@" mutated"];
    [error appendString:@" mutated"];
    [ids removeLastObject];
    [first_id appendString:@" shallow"];
    root[@"copiedInfo"] = Snapshot(info, info_fields);
    root[@"copiedResult"] = Snapshot(result, result_fields);
    root[@"setProperties"] = Snapshot(properties, properties_fields);

    for (NSString* field in @[@"groupId", @"title", @"tabSessionIds"]) {
      [info setValue:nil forKey:field];
    }
    [result setValue:nil forKey:@"error"];
    [result setValue:nil forKey:@"groupId"];
    for (NSString* field in @[@"title", @"color", @"collapsed"]) {
      [properties setValue:nil forKey:field];
    }
    root[@"clearedInfo"] = Snapshot(info, info_fields);
    root[@"clearedResult"] = Snapshot(result, result_fields);
    root[@"clearedProperties"] = Snapshot(properties, properties_fields);
    [properties setValue:@"" forKey:@"title"];
    [properties setValue:@(-2147483649LL) forKey:@"color"];
    [properties setValue:@NO forKey:@"collapsed"];
    root[@"emptyProperties"] = Snapshot(properties, properties_fields);

    arc::bindings::TabGroupProperties native;
    native.title = std::string("native\0tail", 11);
    native.color = static_cast<arc::bindings::TabGroupColor>(-2147483647 - 1);
    native.collapsed = false;
    id native_copy = [[NSClassFromString(@"ArcTabGroupProperties") alloc]
        initWithTabGroupProperties:&native];
    native.title = "mutated";
    native.color.reset();
    native.collapsed = true;
    root[@"nativeProperties"] = Snapshot(native_copy, properties_fields);
    arc::bindings::TabGroupProperties absent;
    id native_absent = [[NSClassFromString(@"ArcTabGroupProperties") alloc]
        initWithTabGroupProperties:&absent];
    root[@"absentNativeProperties"] = Snapshot(native_absent, properties_fields);

    NSMutableArray* accelerators = [NSMutableArray array];
    for (auto record : std::array<KeyboardAccelerator, 4>{{
             {0, 0}, {0xffffffffU, -1}, {0x80000000U, -2147483647 - 1}, {65, 2147483647}}}) {
      id accelerator = [[NSClassFromString(@"ArcKeyboardAccelerator") alloc]
          initWithAccelerator:&record];
      record.key = 1;
      record.modifiers = 0;
      [accelerators addObject:Snapshot(accelerator, @"key modifiers")];
    }
    root[@"accelerators"] = accelerators;

    NSMutableString* code = [@"KeyA physical" mutableCopy];
    NSMutableString* key = [@"logical 🌙" mutableCopy];
    id event = [[NSClassFromString(@"ArcKeyEventData") alloc]
        initWithEventType:NSUIntegerMax domCode:code key:key
        windowsVirtualKeyCode:-2147483647 - 1];
    [code appendString:@" mutated"];
    [key appendString:@" mutated"];
    root[@"keyEvent"] = Snapshot(event, @"eventType domCode key windowsVirtualKeyCode");
    id nil_event = [[NSClassFromString(@"ArcKeyEventData") alloc]
        initWithEventType:0 domCode:nil key:nil windowsVirtualKeyCode:2147483647];
    root[@"nilKeyEvent"] = Snapshot(nil_event, @"eventType domCode key windowsVirtualKeyCode");

    id params = [[NSClassFromString(@"ArcOverscrollParams") alloc] init];
    id behavior = [params valueForKey:@"overscrollBehavior"];
    id other_params = [[NSClassFromString(@"ArcOverscrollParams") alloc] init];
    root[@"defaultOverscroll"] = Snapshot(behavior, @"x y");
    root[@"separateOverscrollDefaults"] = @([other_params valueForKey:@"overscrollBehavior"] != behavior);
    id replacement = [[NSClassFromString(@"ArcOverscrollBehavior") alloc] init];
    [params setValue:replacement forKey:@"overscrollBehavior"];
    [replacement setValue:@(-9223372036854775807LL - 1) forKey:@"x"];
    [replacement setValue:@9223372036854775807LL forKey:@"y"];
    root[@"overscrollIdentity"] = @([params valueForKey:@"overscrollBehavior"] == replacement);
    root[@"sharedOverscroll"] = Snapshot([params valueForKey:@"overscrollBehavior"], @"x y");
    [params setValue:nil forKey:@"overscrollBehavior"];
    root[@"clearedOverscroll"] = Snapshot(params, @"overscrollBehavior");

    NSMutableArray* find_results = [NSMutableArray array];
    for (auto record : std::array<FindResult, 3>{{{0, 0}, {-1, -2147483647 - 1}, {2147483647, -17}}}) {
      id value = [[NSClassFromString(@"ArcFindResult") alloc] initWithFindResult:&record];
      record.num_matches = 0;
      record.active_match_ordinal = 0;
      [find_results addObject:Snapshot(value, @"numMatches activeMatchOrdinal")];
    }
    root[@"findResults"] = find_results;
    NSMutableArray* directives = [NSMutableArray array];
    for (auto record : std::array<arc::FindTextWithDirectiveResult, 4>{{
             {"", false}, {"long directive fragment with UTF-8 🌙", true},
             {std::string("find\0tail", 9), false}, {std::string(1, '\xff'), true}}}) {
      id value = [[NSClassFromString(@"ArcFindTextWithDirectiveResult") alloc]
          initWithFindTextWithDirectiveResult:&record];
      record.directive = "mutated";
      record.result = !record.result;
      [directives addObject:Snapshot(value, @"directive result")];
    }
    root[@"directives"] = directives;

    NSMutableArray* redirects = [NSMutableArray array];
    for (auto record : std::array<arc::RedirectEntry, 4>{{
             {"HTTP://Example.test:80/a b?q=1#fragment", u"Title 🌙 long enough for a native allocation", true, -2147483647 - 1},
             {"https://example.test/例?q=🌙", std::u16string(u"find\0tail", 9), false, 2147483647},
             {"", u"", false, 0}, {"not a URL", u"invalid URL", true, -1}}}) {
      ArcRedirectEntry* value = [[NSClassFromString(@"ArcRedirectEntry") alloc]
          initWithRedirectEntry:&record];
      record.url = "mutated";
      record.title = u"mutated";
      record.hidden = !record.hidden;
      record.redirect_type = 0;
      NSMutableDictionary* snapshot = [Snapshot(value, @"url title hidden redirectType") mutableCopy];
      auto native_result = value.redirect_entry;
      snapshot[@"nativeEngaged"] = @(native_result.has_value());
      snapshot[@"nativeURL"] = [[NSString alloc] initWithBytes:native_result->url.data()
          length:native_result->url.size() encoding:NSUTF8StringEncoding];
      snapshot[@"nativeTitle"] = [[NSString alloc] initWithBytes:native_result->title.data()
          length:native_result->title.size() * sizeof(char16_t)
          encoding:NSUTF16LittleEndianStringEncoding];
      native_result->url = "changed returned copy";
      native_result->title = u"changed returned copy";
      snapshot[@"titleAfterNativeCopyMutation"] = value.title;
      [redirects addObject:snapshot];
    }
    root[@"redirects"] = redirects;
    ArcRedirectEntry* empty_redirect = [[NSClassFromString(@"ArcRedirectEntry") alloc] init];
    root[@"emptyRedirectNativeEngaged"] = @(empty_redirect.redirect_entry.has_value());

    NSMutableArray* native_sessions = [NSMutableArray array];
    for (int identifier : std::array<int, 5>{{-2147483647 - 1, -1, 0, 1, 2147483647}}) {
      SessionID record{identifier};
      id value = [[NSClassFromString(@"ArcSessionID") alloc] initWithSessionId:&record];
      record.id = 42;
      [native_sessions addObject:Snapshot(value, @"identifier hash")];
    }
    root[@"nativeSessionIDs"] = native_sessions;
    NSMutableArray* integer_sessions = [NSMutableArray array];
    for (NSInteger identifier : std::array<NSInteger, 10>{{
             -9223372036854775807LL - 1, -4294967297LL, -4294967296LL,
             -2147483649LL, -2147483648LL, 0, 2147483648LL, 4294967296LL,
             4294967297LL, 9223372036854775807LL}}) {
      id value = [[NSClassFromString(@"ArcSessionID") alloc] initWithID:identifier];
      NSMutableDictionary* snapshot = [Snapshot(value, @"identifier hash") mutableCopy];
      snapshot[@"input"] = @(identifier);
      [integer_sessions addObject:snapshot];
    }
    root[@"integerSessionIDs"] = integer_sessions;
    id session_one = [[NSClassFromString(@"ArcSessionID") alloc] initWithID:1];
    id session_one_copy = [[NSClassFromString(@"ArcSessionID") alloc] initWithID:4294967297LL];
    id session_two = [[NSClassFromString(@"ArcSessionID") alloc] initWithID:2];
    id session_zero = [[NSClassFromString(@"ArcSessionID") alloc] initWithID:0];
    id session_empty = [[NSClassFromString(@"ArcSessionID") alloc] init];
    id session_empty_copy = [[NSClassFromString(@"ArcSessionID") alloc] init];
    Class session_subclass = objc_allocateClassPair(NSClassFromString(@"ArcSessionID"),
                                                   "CoreProbeSessionIDSubclass", 0);
    objc_registerClassPair(session_subclass);
    id session_subclass_one = [[session_subclass alloc] initWithID:1];
    root[@"sessionEquality"] = @{
      @"self": @([session_one isEqual:session_one]),
      @"sameNarrowedValue": @([session_one isEqual:session_one_copy]),
      @"differentValue": @([session_one isEqual:session_two]),
      @"nil": @([session_one isEqual:nil]),
      @"number": @([session_one isEqual:@1]),
      @"string": @([session_one isEqual:@"1"]),
      @"subclass": @([session_one isEqual:session_subclass_one]),
      @"subclassReverse": @([session_subclass_one isEqual:session_one]),
      @"bothEmpty": @([session_empty isEqual:session_empty_copy]),
      @"emptyAndZero": @([session_empty isEqual:session_zero]),
      @"zeroAndEmpty": @([session_zero isEqual:session_empty])
    };

    NSData* json = [NSJSONSerialization dataWithJSONObject:root
        options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:nil];
    if (!json) {
      return 4;
    }
    [NSFileHandle.fileHandleWithStandardOutput writeData:json];
  }
}
