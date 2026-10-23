// Bounded native-record/getter ABI comparison. Does not initialize profiles.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>
#include <dlfcn.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

@interface NSObject (ExtensionProbe)
- (instancetype)initWithExtensionInfo:(const void*)record;
- (instancetype)initWithExtensionCommand:(const void*)record;
- (instancetype)initWithExtensionRuntimeError:(const void*)record;
- (instancetype)initWithExtensionRuntimeErrorStackFrame:(const void*)record;
- (instancetype)initWithExtensionToolbarAction:(const void*)record;
- (void*)mutableExtensionInfo;
@end
namespace {
struct ImageSlot { void* vtable = nullptr; std::shared_ptr<void> impl; };
struct Info {
  std::string name, id, url, description;
  std::vector<std::u16string> permissions;
  ImageSlot icon;
  double rating = 0;
  int num_ratings = 0;
  bool enabled = false, has_action = false;
  std::string version, keyword;
  bool from_webstore = false;
};
struct Command { std::string id, name, description; std::optional<std::string> shortcut; bool global = false; };
struct Frame { unsigned line=0, column=0; std::u16string source,function; };
struct Error { std::string id,url; std::u16string message,source; bool incognito=false; int severity=0; std::vector<Frame> stack; };
struct Action { std::string id; std::u16string name,tooltip; ImageSlot icon; bool enabled=false; std::optional<int> position; };
static_assert(sizeof(Info)==216 && sizeof(Command)==112 && sizeof(Frame)==56 && sizeof(Error)==128 && sizeof(Action)==112);
id JSONValue(id value) {
  if (!value) return NSNull.null;
  if ([value isKindOfClass:[NSURL class]]) return [value absoluteString];
  if ([value isKindOfClass:[NSImage class]]) return @"NSImage";
  return value;
}
NSDictionary* Snapshot(id object, NSString* names) {
  NSMutableDictionary* data=[NSMutableDictionary dictionary];
  for (NSString* field in [names componentsSeparatedByString:@" "])
    data[field]=JSONValue([object valueForKey:field]);
  return data;
}
NSDictionary* Methods(NSString* name, NSString* names) {
  NSMutableDictionary* data=[NSMutableDictionary dictionary];
  for (NSString* field in [names componentsSeparatedByString:@" "]) {
    Method method=class_getInstanceMethod(NSClassFromString(name),NSSelectorFromString(field));
    data[field]=method ? @(method_getTypeEncoding(method)) : @"MISSING";
  }
  return data;
}
}
int main(int argc,char**argv) {
  @autoreleasepool {
    if (argc!=2 || !dlopen(argv[1],RTLD_NOW|RTLD_LOCAL)) { fprintf(stderr,"engine load: %s\n",dlerror());return 2; }
    const char* recorder=getenv("ARC_EXTENSIONS_RECORDER_LIBRARY");
    if (recorder && !dlopen(recorder,RTLD_NOW|RTLD_LOCAL)) { fprintf(stderr,"recorder: %s\n",dlerror());return 3; }
    NSMutableDictionary* root=[NSMutableDictionary dictionary];
    NSString* infoFields=@"name id description url permissions icon rating numRatings enabled hasAction version omniboxKeyword fromWebstore";
    NSString* commandFields=@"extensionId commandName commandDescription shortcut isGlobal";
    NSString* frameFields=@"lineNumber columnNumber source function";
    NSString* errorFields=@"extensionID contextURL message source fromIncognito severity";
    NSString* actionFields=@"id accessibleName tooltipText icon enabled pinnedPosition";
    root[@"metadata"]=@{
      @"ArcExtensionInfo":Methods(@"ArcExtensionInfo",[infoFields stringByAppendingString:@" init initWithExtensionInfo: mutableExtensionInfo"]),
      @"ArcExtensionCommand":Methods(@"ArcExtensionCommand",[commandFields stringByAppendingString:@" initWithExtensionCommand:"]),
      @"ArcExtensionRuntimeErrorStackFrame":Methods(@"ArcExtensionRuntimeErrorStackFrame",[frameFields stringByAppendingString:@" initWithExtensionRuntimeErrorStackFrame:"]),
      @"ArcExtensionRuntimeError":Methods(@"ArcExtensionRuntimeError",[errorFields stringByAppendingString:@" stackTrace initWithExtensionRuntimeError:"]),
      @"ArcExtensionToolbarAction":Methods(@"ArcExtensionToolbarAction",[actionFields stringByAppendingString:@" initWithExtensionToolbarAction:"])};
    id info=[[NSClassFromString(@"ArcExtensionInfo") alloc] init];
    root[@"defaultInfo"]=Snapshot(info,infoFields);
    auto* storage=static_cast<Info*>([info mutableExtensionInfo]);
    storage->name="Extension 🌙";storage->id="abcdefghijklmnopabcdefghijklmnop";
    storage->url="https://example.test/path?q=1#fragment";storage->description="line 1\nline 2";
    storage->permissions={u"Read tabs",u"例外 🌙"};storage->rating=4.75;storage->num_ratings=123;
    storage->enabled=true;storage->has_action=true;storage->version="1.2.3";storage->keyword="test";storage->from_webstore=true;
    id infoCopy=[[NSClassFromString(@"ArcExtensionInfo") alloc] initWithExtensionInfo:storage];
    storage->name="changed";storage->permissions.clear();
    root[@"copiedInfo"]=Snapshot(infoCopy,infoFields);
    NSMutableArray* commands=[NSMutableArray array];
    for (bool assigned : {false,true}) {
      Command command{"abcdefghijklmnopabcdefghijklmnop","test-command","Description 🌙",std::nullopt,true};
      if (assigned) command.shortcut="Command+Shift+U";
      id value=[[NSClassFromString(@"ArcExtensionCommand") alloc] initWithExtensionCommand:&command];
      command.name="mutated";command.shortcut="mutated";
      [commands addObject:Snapshot(value,commandFields)];
    }
    root[@"commands"]=commands;
    Frame frame{0xffffffffU,0x80000000U,u"https://example.test/例.js",u"function 🌙"};
    id frameValue=[[NSClassFromString(@"ArcExtensionRuntimeErrorStackFrame") alloc] initWithExtensionRuntimeErrorStackFrame:&frame];
    frame.source=u"mutated";root[@"frame"]=Snapshot(frameValue,frameFields);
    Error error{"abcdefghijklmnopabcdefghijklmnop","https://example.test/extension",u"TypeError 🌙",u"source.js",true,2,{{10,20,u"file.js",u"a"},{30,40,u"file2.js",u"b"}}};
    id errorValue=[[NSClassFromString(@"ArcExtensionRuntimeError") alloc] initWithExtensionRuntimeError:&error];
    error.message=u"mutated";error.stack.clear();
    root[@"error"]=Snapshot(errorValue,errorFields);
    NSArray* frames=[errorValue valueForKey:@"stackTrace"];
    NSMutableArray* stack=[NSMutableArray array];
    for(id value in frames)[stack addObject:Snapshot(value,frameFields)];
    root[@"stack"]=stack;root[@"stackIdentity"]=@([errorValue valueForKey:@"stackTrace"]==frames);
    NSMutableArray* actions=[NSMutableArray array];
    for (bool pinned : {false,true}) {
      Action action;action.id="abcdefghijklmnopabcdefghijklmnop";action.name=u"Action 🌙";
      action.tooltip=u"Tooltip";action.enabled=true;if(pinned)action.position=3;
      id value=[[NSClassFromString(@"ArcExtensionToolbarAction") alloc] initWithExtensionToolbarAction:&action];
      action.name=u"mutated";action.position=9;[actions addObject:Snapshot(value,actionFields)];
    }
    root[@"actions"]=actions;
    NSError* jsonError=nil;
    NSData* json=[NSJSONSerialization dataWithJSONObject:root options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:&jsonError];
    if(!json){fprintf(stderr,"json failure\n");return 4;}
    fwrite(json.bytes,1,json.length,stdout);fputs("\n",stdout);
  }
}
