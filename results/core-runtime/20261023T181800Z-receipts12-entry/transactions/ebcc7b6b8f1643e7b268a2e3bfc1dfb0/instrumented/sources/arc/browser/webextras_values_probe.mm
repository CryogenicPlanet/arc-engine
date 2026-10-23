// Bounded value-only probe. No browser initialization, profile access, or
// native-pointer arguments. Does not establish app-scenario acceptance.
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#include <dlfcn.h>
#include <string_view>
#include <span>

@interface NSObject (WebExtrasProbe)
- (instancetype)initWithAction:(NSUInteger)action;
- (instancetype)initWithAction:(NSUInteger)action value:(NSString*)value;
@end
@interface WebExtrasCookieProbe : NSObject
- (void)setName:(NSString*)name;
- (void)setValue:(NSString*)value;
- (void)setDomain:(NSString*)domain;
- (void)setPath:(NSString*)path;
- (void)setHostOnly:(BOOL)value;
- (void)setHttpOnly:(BOOL)value;
- (void)setSecure:(BOOL)value;
- (void)setSession:(BOOL)value;
- (void)setExpirationDate:(double)value;
- (void)setSameSite:(NSInteger)value;
@end

static NSDictionary* Snapshot(id object, NSString* fields) {
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (NSString* field in [fields componentsSeparatedByString:@" "]) {
    result[field] = [object valueForKey:field] ?: NSNull.null;
  }
  return result;
}

static NSDictionary* Surface(NSString* className, NSString* selectors) {
  Class cls = NSClassFromString(className);
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (NSString* selector in [selectors componentsSeparatedByString:@" "]) {
    Method method = class_getInstanceMethod(cls, NSSelectorFromString(selector));
    result[selector] = method ? [NSString stringWithUTF8String:method_getTypeEncoding(method)] : @"MISSING";
  }
  return result;
}
static NSDictionary* Properties(NSString* className, NSString* fields) {
  NSArray* names = [fields componentsSeparatedByString:@" "];
  unsigned count = 0;
  objc_property_t* storage = class_copyPropertyList(NSClassFromString(className), &count);
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (objc_property_t property : std::span(storage, count)) {
    NSString* name = [NSString stringWithUTF8String:property_getName(property)];
    if ([names containsObject:name]) {
      result[name] = [NSString stringWithUTF8String:property_getAttributes(property)];
    }
  }
  free(storage);
  return result;
}

int main(int argc, const char* argv[]) {
  @autoreleasepool {
    if (argc < 2 || !dlopen(argv[1], RTLD_NOW | RTLD_GLOBAL)) {
      fprintf(stderr, "Unable to load engine: %s\n", dlerror());
      return 1;
    }
    const char* recorder = getenv("ARC_WEBEXTRAS_RECORDER_LIBRARY");
    if (recorder && !dlopen(recorder, RTLD_NOW | RTLD_GLOBAL)) { return 2; }
    NSMutableDictionary* result = [NSMutableDictionary dictionary];
    NSMutableArray* actions = [NSMutableArray array];
    for (NSUInteger action = 0; action < 4; ++action) {
      id plain = [[NSClassFromString(@"ArcAccessibilityActionData") alloc] initWithAction:action];
      [actions addObject:Snapshot(plain, @"action value")];
      for (id text in @[NSNull.null, @"", @"☃ hello", [@"abc" stringByPaddingToLength:512 withString:@"123" startingAtIndex:0]]) {
        id value = [[NSClassFromString(@"ArcAccessibilityActionData") alloc]
            initWithAction:action value:text == NSNull.null ? nil : text];
        [actions addObject:Snapshot(value, @"action value")];
      }
    }
    result[@"actions"] = actions;
    id tree = [[NSClassFromString(@"ArcAccessibilityTreeNode") alloc] init];
    result[@"emptyTree"] = Snapshot(tree, @"nodeId role name value description isClickable isEditable isFocusable childTreeId children");
    id entry = [[NSClassFromString(@"ArcBrowserSidePanelEntryInfo") alloc] init];
    result[@"entry"] = Snapshot(entry, @"hasActiveEntry extensionId cachedWebContents");
    result[@"metadata"] = @{
      @"action":Surface(@"ArcAccessibilityActionData", @"initWithAction: initWithAction:value: action value"),
      @"tree":Surface(@"ArcAccessibilityTreeNode", @"initWithAccessibilityTreeNode: nodeId role name value description isClickable isEditable isFocusable childTreeId children"),
      @"entry":Surface(@"ArcBrowserSidePanelEntryInfo", @"hasActiveEntry setHasActiveEntry: extensionId setExtensionId: cachedWebContents setCachedWebContents:"),
      @"position":Surface(@"ArcMediaPosition", @"playbackRate duration currentTime endOfMedia"),
      @"sidePanel":Surface(@"ArcBrowserSidePanel", @"title webContents"),
      @"devTools":Surface(@"ArcWebContentsDevTools", @"toolboxWebContents webContents")
    };
    result[@"properties"] = @{
      @"action":Properties(@"ArcAccessibilityActionData", @"action value"),
      @"tree":Properties(@"ArcAccessibilityTreeNode", @"nodeId role name value description isClickable isEditable isFocusable childTreeId children"),
      @"entry":Properties(@"ArcBrowserSidePanelEntryInfo", @"hasActiveEntry extensionId cachedWebContents"),
      @"position":Properties(@"ArcMediaPosition", @"playbackRate duration currentTime endOfMedia"),
      @"sidePanel":Properties(@"ArcBrowserSidePanel", @"title webContents"),
      @"devTools":Properties(@"ArcWebContentsDevTools", @"toolboxWebContents")
    };
    if (argc > 2 && std::string_view(argv[2]) == "--cookies") {
    WebExtrasCookieProbe* cookie = [[NSClassFromString(@"ArcCookie") alloc] init];
    NSString* fields = @"name value domain path hostOnly httpOnly secure session expirationDate sameSite";
    result[@"cookieDefault"] = Snapshot(cookie, fields);
    [cookie setName:@"name ☃"]; [cookie setValue:@"value ☃"];
    [cookie setDomain:@".example.test"]; [cookie setPath:@"/path"];
    [cookie setHostOnly:YES]; [cookie setHttpOnly:YES]; [cookie setSecure:YES];
    [cookie setSession:YES]; [cookie setExpirationDate:1700000000.25]; [cookie setSameSite:2];
    result[@"cookieSet"] = Snapshot(cookie, fields);
    [cookie setName:nil]; [cookie setValue:nil]; [cookie setDomain:nil]; [cookie setPath:nil];
    result[@"cookieNil"] = Snapshot(cookie, fields);
    }
    NSData* data = [NSJSONSerialization dataWithJSONObject:result options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:nil];
    puts([[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding].UTF8String);
  }
}
