// ArcEngineRecorder: records every call across Arc's app <-> engine boundary, for diffing our engine
// against Arc's. Injected with DYLD_INSERT_LIBRARIES (test builds only); writes JSON lines to
// $ARC_RECORD_PATH.
//
// Recorded: every ObjC-typed method of the Arc* classes in ArcCore (calls from the app and the engine's
// own calls through ObjC, e.g. property updates the app observes), and the app's 270 delegate/callback
// implementations (engine -> app). Each line: seq, thread, depth, dir, cls, sel, args, ret.
// Objects are named <Class#n> with n stable per object for the run, so runs can be compared.
//
//   clang -dynamiclib -fobjc-arc -framework Foundation -framework AppKit -o ArcEngineRecorder.dylib ArcEngineRecorder.m

#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

static FILE* gOut;
static pthread_mutex_t gLock = PTHREAD_MUTEX_INITIALIZER;
static unsigned long long gSeq;
static NSMutableDictionary<NSString*, NSString*>* gHooks;  // "Class sel" -> dir
static NSMutableDictionary<NSString*, NSValue*>* gPrevForward;  // Class -> previous forwardInvocation IMP
static const void* kObjectIdKey = &kObjectIdKey;
static unsigned long long gNextObjectId;
static __thread int tDepth;

static NSString* Quote(NSString* s) {
  if (s.length > 300) s = [[s substringToIndex:300] stringByAppendingString:@"…"];
  NSData* d = [NSJSONSerialization dataWithJSONObject:@[ s ] options:0 error:nil];
  NSString* j = [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
  return [j substringWithRange:NSMakeRange(1, j.length - 2)];
}

static NSString* Describe(id obj, int depth) {
  if (!obj) return @"null";
  if ([obj isKindOfClass:[NSString class]]) return Quote(obj);
  if ([obj isKindOfClass:[NSNumber class]]) {
    double v = [obj doubleValue];
    return isfinite(v) ? [obj stringValue] : Quote([obj stringValue]);
  }
  if ([obj isKindOfClass:[NSURL class]]) return Quote([@"url:" stringByAppendingString:[obj absoluteString] ?: @""]);
  if ([NSStringFromClass([obj class]) containsString:@"Block"]) return @"\"<block>\"";
  if ([obj isKindOfClass:[NSError class]])
    return Quote([NSString stringWithFormat:@"NSError(%@,%ld)", [obj domain], (long)[obj code]]);
  if ([obj isKindOfClass:[NSData class]]) return Quote([NSString stringWithFormat:@"data(%lu)", (unsigned long)[obj length]]);
  if ([obj isKindOfClass:[NSDate class]]) return @"\"<date>\"";
  if ([obj isKindOfClass:[NSImage class]]) {
    NSSize s = [obj size];
    return Quote([NSString stringWithFormat:@"image(%gx%g)", s.width, s.height]);
  }
  if (depth < 2 && ([obj isKindOfClass:[NSArray class]] || [obj isKindOfClass:[NSSet class]] ||
                    [obj isKindOfClass:[NSOrderedSet class]])) {
    NSMutableArray* parts = [NSMutableArray array];
    for (id x in obj) {
      if (parts.count == 8) { [parts addObject:@"\"…\""]; break; }
      [parts addObject:Describe(x, depth + 1)];
    }
    return [NSString stringWithFormat:@"[%@]", [parts componentsJoinedByString:@","]];
  }
  if (depth < 2 && [obj isKindOfClass:[NSDictionary class]]) {
    NSMutableArray* parts = [NSMutableArray array];
    for (id k in [[obj allKeys] sortedArrayUsingSelector:@selector(compare:)]) {
      if (parts.count == 8) break;
      [parts addObject:[NSString stringWithFormat:@"%@:%@", Quote([k description]), Describe(obj[k], depth + 1)]];
    }
    return [NSString stringWithFormat:@"{%@}", [parts componentsJoinedByString:@","]];
  }
  NSNumber* n = objc_getAssociatedObject(obj, kObjectIdKey);
  if (!n) {
    n = @(__atomic_add_fetch(&gNextObjectId, 1, __ATOMIC_RELAXED));
    objc_setAssociatedObject(obj, kObjectIdKey, n, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  NSString* cls = NSStringFromClass(object_getClass(obj));
  if ([cls hasPrefix:@"NSKVONotifying_"]) cls = [cls substringFromIndex:15];
  return Quote([NSString stringWithFormat:@"<%@#%@>", cls, n]);
}

static NSString* DescribeValue(const char* type, void* buf) {
  while (*type && strchr("rnNoORV", *type)) type++;
  switch (*type) {
    case '@':
      if (type[1] == '?') return @"\"<block>\"";
      return Describe((__bridge id)(*(void**)buf), 0);
    case '#': return Quote(NSStringFromClass((__bridge Class)(*(void**)buf)));
    case ':': return Quote(NSStringFromSelector(*(SEL*)buf));
    case 'B': return *(bool*)buf ? @"true" : @"false";
    case 'c': return [NSString stringWithFormat:@"%d", *(signed char*)buf];
    case 'C': return [NSString stringWithFormat:@"%u", *(unsigned char*)buf];
    case 's': return [NSString stringWithFormat:@"%d", *(short*)buf];
    case 'S': return [NSString stringWithFormat:@"%u", *(unsigned short*)buf];
    case 'i': case 'l': return [NSString stringWithFormat:@"%d", *(int*)buf];
    case 'I': case 'L': return [NSString stringWithFormat:@"%u", *(unsigned*)buf];
    case 'q': return [NSString stringWithFormat:@"%lld", *(long long*)buf];
    case 'Q': return [NSString stringWithFormat:@"%llu", *(unsigned long long*)buf];
    case 'f': return isfinite(*(float*)buf) ? [NSString stringWithFormat:@"%g", *(float*)buf] : Quote([NSString stringWithFormat:@"%g", *(float*)buf]);
    case 'd': return isfinite(*(double*)buf) ? [NSString stringWithFormat:@"%g", *(double*)buf] : Quote([NSString stringWithFormat:@"%g", *(double*)buf]);
    case '*': return *(char**)buf ? Quote(@(*(char**)buf)) : @"null";
    case '^': return *(void**)buf ? @"\"<ptr>\"" : @"null";
    case '{':
      if (!strncmp(type, "{CGRect", 7)) { CGRect r = *(CGRect*)buf; return Quote(NSStringFromRect(r)); }
      if (!strncmp(type, "{CGSize", 7)) { CGSize s = *(CGSize*)buf; return Quote(NSStringFromSize(s)); }
      if (!strncmp(type, "{CGPoint", 8)) { CGPoint p = *(CGPoint*)buf; return Quote(NSStringFromPoint(p)); }
      return @"\"<struct>\"";
    case 'v': return @"null";
  }
  return @"\"<?>\"";
}

static NSString* ThreadName(void) {
  if ([NSThread isMainThread]) return @"main";
  char name[64] = {0};
  pthread_getname_np(pthread_self(), name, sizeof(name));
  return name[0] ? @(name) : @"bg";
}

static void Emit(NSString* line) {
  pthread_mutex_lock(&gLock);
  fprintf(gOut, "%s\n", line.UTF8String);
  pthread_mutex_unlock(&gLock);
}

// A class and its metaclass share a name; keep them apart.
static NSString* ClassKey(Class cls) {
  return [NSString stringWithFormat:@"%s%s", class_isMetaClass(cls) ? "+" : "-", class_getName(cls)];
}

static NSString* HookKey(Class cls, SEL sel) {
  return [NSString stringWithFormat:@"%@ %s", ClassKey(cls), sel_getName(sel)];
}

static SEL AliasFor(SEL sel) {
  return NSSelectorFromString([@"__arcrec_" stringByAppendingString:NSStringFromSelector(sel)]);
}

// Who called: the image of the first stack frame outside the recorder, the ObjC runtime and the forwarding
// machinery ("app" = Arc's executable, "engine" = ArcCore, else the image name).
static NSString* Caller(void) {
  static const void* selfBase;
  if (!selfBase) {
    Dl_info me;
    if (dladdr((const void*)&Caller, &me)) selfBase = me.dli_fbase;
  }
  NSArray<NSNumber*>* frames = [NSThread callStackReturnAddresses];
  for (NSUInteger i = 1; i < frames.count && i < 24; i++) {
    Dl_info info;
    if (!dladdr((const void*)frames[i].unsignedLongValue, &info) || !info.dli_fname) continue;
    if (info.dli_fbase == selfBase) continue;
    const char* base = strrchr(info.dli_fname, '/');
    base = base ? base + 1 : info.dli_fname;
    if (!strcmp(base, "libobjc.A.dylib") || !strcmp(base, "CoreFoundation") || !strcmp(base, "Foundation")) continue;
    if (!strcmp(base, "Arc")) return @"app";
    if (!strcmp(base, "ArcCore")) return @"engine";
    return @(base);
  }
  return @"?";
}

static void RecForwardInvocation(id self, SEL _cmd, NSInvocation* inv) {
  SEL sel = inv.selector;
  Class hit = Nil;
  NSString* dir = nil;
  BOOL isClass = object_isClass(self);
  for (Class c = object_getClass(self); c; c = class_getSuperclass(c)) {
    dir = gHooks[HookKey(c, sel)];
    if (dir) { hit = c; break; }
  }
  if (!dir) {
    for (Class c = object_getClass(self); c; c = class_getSuperclass(c)) {
      NSValue* prev = gPrevForward[ClassKey(c)];
      if (prev) { ((void (*)(id, SEL, NSInvocation*))prev.pointerValue)(self, _cmd, inv); return; }
    }
    [self doesNotRecognizeSelector:sel];
    return;
  }
  NSMethodSignature* sig = inv.methodSignature;
  NSMutableArray* args = [NSMutableArray array];
  for (NSUInteger i = 2; i < sig.numberOfArguments; i++) {
    NSUInteger size = 0;
    NSGetSizeAndAlignment([sig getArgumentTypeAtIndex:i], &size, NULL);
    void* buf = calloc(1, MAX(size, 16));
    [inv getArgument:buf atIndex:i];
    [args addObject:DescribeValue([sig getArgumentTypeAtIndex:i], buf)];
    free(buf);
  }
  unsigned long long seq = __atomic_add_fetch(&gSeq, 1, __ATOMIC_RELAXED);
  NSString* cls = NSStringFromClass(hit);
  NSString* common = [NSString stringWithFormat:@"\"thread\":%@,\"depth\":%d,\"from\":\"%@\",\"dir\":\"%@\",\"cls\":%@,\"kind\":\"%@\",\"sel\":%@,\"self\":%@",
                       Quote(ThreadName()), tDepth, Caller(), dir, Quote(cls), isClass ? @"+" : @"-", Quote(NSStringFromSelector(sel)),
                       isClass ? @"null" : Describe(self, 0)];
  Emit([NSString stringWithFormat:@"{\"seq\":%llu,\"ev\":\"call\",%@,\"args\":[%@]}", seq, common, [args componentsJoinedByString:@","]]);
  tDepth++;
  inv.selector = AliasFor(sel);
  [inv invoke];
  inv.selector = sel;
  tDepth--;
  NSString* ret = @"null";
  if (sig.methodReturnLength) {
    void* buf = calloc(1, MAX(sig.methodReturnLength, 16));
    [inv getReturnValue:buf];
    ret = DescribeValue(sig.methodReturnType, buf);
    free(buf);
  }
  Emit([NSString stringWithFormat:@"{\"seq\":%llu,\"ev\":\"ret\",%@,\"ret\":%@}",
                                  __atomic_add_fetch(&gSeq, 1, __ATOMIC_RELAXED), common, ret]);
}

static BOOL Skip(const char* sel, const char* types) {
  static const char* kSkip[] = {"dealloc", "hash", "isEqual:", "description", "debugDescription", "retain",
                                "release", "autorelease", "class", "respondsToSelector:", "forwardInvocation:",
                                "methodSignatureForSelector:", "init", NULL};
  for (int i = 0; kSkip[i]; i++) if (!strcmp(sel, kSkip[i])) return YES;
  if (sel[0] == '.' || !strncmp(sel, "__arcrec_", 9)) return YES;
  // C arrays (e.g. [20f]) can't go through NSInvocation.
  if (strchr(types, '[')) return YES;
  // C++-typed signatures are ArcCore's internal plumbing; NSInvocation can't carry them.
  for (const char* p = strchr(types, '{'); p; p = strchr(p + 1, '{'))
    if (strncmp(p, "{CGRect", 7) && strncmp(p, "{CGSize", 7) && strncmp(p, "{CGPoint", 8)) return YES;
  return NO;
}

static void HookClass(Class cls, NSSet<NSString*>* only, NSString* dir) {
  unsigned n = 0;
  Method* ms = class_copyMethodList(cls, &n);
  BOOL any = NO;
  for (unsigned i = 0; i < n; i++) {
    SEL sel = method_getName(ms[i]);
    const char* types = method_getTypeEncoding(ms[i]) ?: "";
    if (only && ![only containsObject:NSStringFromSelector(sel)]) continue;
    if (Skip(sel_getName(sel), types)) continue;
    class_addMethod(cls, AliasFor(sel), method_getImplementation(ms[i]), types);
    method_setImplementation(ms[i], _objc_msgForward);
    gHooks[HookKey(cls, sel)] = dir;
    any = YES;
  }
  free(ms);
  if (any && !gPrevForward[ClassKey(cls)]) {
    IMP prev = class_getMethodImplementation(cls, @selector(forwardInvocation:));
    gPrevForward[ClassKey(cls)] = [NSValue valueWithPointer:(const void*)prev];
    class_replaceMethod(cls, @selector(forwardInvocation:), (IMP)RecForwardInvocation, "v@:@");
  }
}

static const char* const kAppCallbacks[][2] = {
#include "app_callbacks.inc"
};

static void HookAppCallbacks(void) {
  NSMutableDictionary<NSString*, NSMutableSet*>* byClass = [NSMutableDictionary dictionary];
  for (size_t i = 0; i < sizeof(kAppCallbacks) / sizeof(kAppCallbacks[0]); i++) {
    NSString* c = @(kAppCallbacks[i][0]);
    if (!byClass[c]) byClass[c] = [NSMutableSet set];
    [byClass[c] addObject:@(kAppCallbacks[i][1])];
  }
  for (NSString* c in byClass) {
    Class cls = objc_getClass(c.UTF8String);
    if (cls) HookClass(cls, byClass[c], @"engine->app");
  }
}

static void OnImage(const struct mach_header* mh, intptr_t slide) {
  Dl_info info;
  if (!dladdr(mh, &info) || !strstr(info.dli_fname, "/ArcCore.framework/")) return;
  unsigned n = 0;
  const char** names = objc_copyClassNamesForImage(info.dli_fname, &n);
  for (unsigned i = 0; i < n; i++) {
    if (strncmp(names[i], "Arc", 3)) continue;
    Class cls = objc_getClass(names[i]);
    HookClass(cls, nil, @"->engine");
    HookClass(object_getClass(cls), nil, @"->engine");
  }
  free(names);
  Emit([NSString stringWithFormat:@"{\"seq\":0,\"ev\":\"hooked\",\"engine_classes\":%u}", n]);
}

__attribute__((constructor)) static void Start(void) {
  const char* path = getenv("ARC_RECORD_PATH");
  if (!path) return;
  gOut = fopen(path, "w");
  if (!gOut) return;
  setvbuf(gOut, NULL, _IOLBF, 0);
  gHooks = [NSMutableDictionary dictionary];
  gPrevForward = [NSMutableDictionary dictionary];
  unsetenv("DYLD_INSERT_LIBRARIES");  // don't follow into helper processes
  _dyld_register_func_for_add_image(OnImage);
  HookAppCallbacks();  // app classes are in the main image, already mapped
}
