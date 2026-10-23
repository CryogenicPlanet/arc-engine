// Tests-owned PID-targeted Little Arc fixture delivery. No private Arc selectors.
// Compilation MUST pin the canonical Arc repo root. No default paths or launches.
#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#import <Foundation/Foundation.h>
#import <libproc.h>
#import <sys/stat.h>
#import <fcntl.h>
#import <unistd.h>
#import <limits.h>

#ifndef LITTLE_ARC_EXPECTED_ARC_ROOT
#error LITTLE_ARC_EXPECTED_ARC_ROOT must be a canonical compile-time path
#endif
#ifndef LITTLE_ARC_OFFLINE_TEST
#define LITTLE_ARC_OFFLINE_TEST 0
#endif

static void Need(BOOL ok, NSString *message) {
  if (!ok) @throw [NSException exceptionWithName:@"LittleArcDeliveryRejected" reason:message userInfo:nil];
}
static BOOL IsBoolean(id value) { return value && CFGetTypeID((__bridge CFTypeRef)value)==CFBooleanGetTypeID(); }
static NSDictionary *Object(id value, NSArray<NSString *> *keys) {
  Need([value isKindOfClass:NSDictionary.class],@"typed dictionary required");
  Need([(NSDictionary *)value count]==keys.count,@"unknown or missing fields");
  for (NSString *key in keys) Need(value[key]!=nil,@"missing field");
  return value;
}
static NSString *String(id value) { Need([value isKindOfClass:NSString.class],@"typed string required"); return value; }
static long long Integer(id value, long long lo, long long hi) {
  Need([value isKindOfClass:NSNumber.class]&&!IsBoolean(value)&&!CFNumberIsFloatType((__bridge CFNumberRef)value),@"typed integer required");
  long long n=[value longLongValue]; Need(n>=lo&&n<=hi,@"integer out of range"); return n;
}
static void TypedTrue(id value) { Need(IsBoolean(value)&&[value boolValue],@"typed true Boolean required"); }
static void Canonical(NSString *path, BOOL directory) {
  Need(path.length>1&&[path hasPrefix:@"/"]&&![path hasSuffix:@"/"]&&![path containsString:@"//"]&&![path containsString:@"/./"]&&![path containsString:@"/../"]&&![path hasSuffix:@"/."]&&![path hasSuffix:@"/.."],@"absolute normalized path required");
  NSString *at=@"/"; struct stat st;
  for (NSString *part in path.pathComponents) {
    if ([part isEqual:@"/"]) continue;
    at=[at stringByAppendingPathComponent:part];
    Need(lstat(at.fileSystemRepresentation,&st)==0&&!S_ISLNK(st.st_mode),@"missing or symlink path component");
    if (![at isEqual:path]||directory) Need(S_ISDIR(st.st_mode),@"directory component required");
    else Need(S_ISREG(st.st_mode),@"regular file required");
  }
  char resolved[PATH_MAX]; Need(realpath(path.fileSystemRepresentation,resolved)!=NULL,@"canonical path unavailable");
  Need([path isEqual:[NSString stringWithUTF8String:resolved]],@"canonical path mismatch");
}
// Walk every component using descriptor-relative O_NOFOLLOW, not only the leaf.
static int OpenNoFollow(NSString *path, BOOL directory) {
  Need([path hasPrefix:@"/"],@"absolute no-follow path required");
  int fd=open("/",O_RDONLY|O_DIRECTORY|O_CLOEXEC);Need(fd>=0,@"root directory unavailable");
  NSArray *parts=path.pathComponents;
  for(NSUInteger i=1;i<parts.count;i++) {
    BOOL dir=i+1<parts.count||directory;
    int next=openat(fd,[parts[i] fileSystemRepresentation],O_RDONLY|O_NOFOLLOW|O_CLOEXEC|(dir?O_DIRECTORY:0));
    close(fd);Need(next>=0,@"descriptor-relative no-follow open failed");fd=next;
  }
  return fd;
}
static NSData *ReadPlist(NSString *path) {
  Canonical(path,NO);
  int fd=OpenNoFollow(path,NO);
  Need(fd>=0,@"no-follow plist open failed");
  struct stat before,after; BOOL valid=fstat(fd,&before)==0&&S_ISREG(before.st_mode)&&before.st_size>0&&before.st_size<=65536;
  if (!valid) { close(fd); Need(NO,@"bounded regular plist required"); }
  NSMutableData *data=[NSMutableData dataWithLength:(NSUInteger)before.st_size]; NSUInteger offset=0;
  while (offset<data.length) { ssize_t n=read(fd,(char *)data.mutableBytes+offset,data.length-offset); if(n<=0)break; offset+=(NSUInteger)n; }
  valid=offset==data.length&&fstat(fd,&after)==0&&before.st_dev==after.st_dev&&before.st_ino==after.st_ino&&before.st_size==after.st_size&&before.st_mtimespec.tv_sec==after.st_mtimespec.tv_sec&&before.st_mtimespec.tv_nsec==after.st_mtimespec.tv_nsec;
  close(fd); Need(valid,@"plist changed or unreadable"); return data;
}
static NSDictionary *DecodePlist(NSData *data) {
  NSError *error=nil; id value=[NSPropertyListSerialization propertyListWithData:data options:NSPropertyListImmutable format:NULL error:&error];
  Need(error==nil&&[value isKindOfClass:NSDictionary.class],@"valid plist dictionary required"); return value;
}
static NSAppleEventSendOptions Options(void) {
  return NSAppleEventSendWaitForReply|NSAppleEventSendNeverInteract|NSAppleEventSendDontRecord|
    (NSAppleEventSendOptions)kAEDontReconnect|(NSAppleEventSendOptions)kAEDoNotPromptForUserConsent;
}

@interface DeliveryContext : NSObject
@property NSDictionary *request;
@property NSString *operation,*agent,*appPath,*executablePath,*bundleID,*appName,*url,*nonce;
@property pid_t pid;
@property long long seconds,microseconds,sequence;
@property NSData *infoData;
@property NSDictionary *executableIdentity;
- (void)checkFiles;
@end
@implementation DeliveryContext
- (void)checkFiles {
  NSString *root=@LITTLE_ARC_EXPECTED_ARC_ROOT; Canonical(root,YES);
  Canonical(self.appPath,YES); Canonical([self.appPath stringByAppendingPathComponent:@"Contents"],YES);
  Canonical([self.appPath stringByAppendingPathComponent:@"Contents/MacOS"],YES); Canonical(self.executablePath,NO);
  struct stat executable;int fd=OpenNoFollow(self.executablePath,NO);
  BOOL executableOK=fstat(fd,&executable)==0&&S_ISREG(executable.st_mode)&&(executable.st_mode&S_IXUSR);close(fd);Need(executableOK,@"executable file required");
  NSDictionary *identity=@{@"device":@(executable.st_dev),@"inode":@(executable.st_ino),@"size":@(executable.st_size),@"mtimeSeconds":@(executable.st_mtimespec.tv_sec),@"mtimeNanos":@(executable.st_mtimespec.tv_nsec)};
  if(self.executableIdentity)Need([self.executableIdentity isEqual:identity],@"executable changed");else self.executableIdentity=identity;
  NSData *data=ReadPlist([self.appPath stringByAppendingPathComponent:@"Contents/Info.plist"]);
  if (self.infoData) Need([self.infoData isEqual:data],@"bundle plist changed");
  NSDictionary *info=DecodePlist(data);
  Need([String(info[@"CFBundleIdentifier"]) isEqual:self.bundleID]&&[String(info[@"CFBundleName"]) isEqual:self.appName]&&[String(info[@"ArcModAgentSession"]) isEqual:self.agent],@"bundle identity mismatch");
  TypedTrue(info[@"ArcModLocalMode"]); TypedTrue(info[@"ArcModAgentSingleLittleArcParity"]);
  Need([String(info[@"ArcModHostResolverRules"]) length]>0,@"service resolver rules required");
  if (!self.infoData) self.infoData=data;
}
@end
static DeliveryContext *Context(NSDictionary *request) {
  NSMutableArray *keys=[@[@"operation",@"agent",@"pid",@"start",@"appURL",@"executablePath",@"bundleID",@"appName",@"fixturePort",@"fixture",@"nonce",@"sequence"] mutableCopy];
#if LITTLE_ARC_OFFLINE_TEST
  [keys addObject:@"testTransport"];
#endif
  Object(request,keys); DeliveryContext *c=[DeliveryContext new]; c.request=request;
  c.operation=String(request[@"operation"]); Need([@[@"inspect-only",@"prepare-offline",@"deliver-once"] containsObject:c.operation],@"unsupported operation");
  c.agent=String(request[@"agent"]); Need([@[@"trace156",@"ours156"] containsObject:c.agent],@"only trace156 or ours156 allowed");
  c.appName=[@"ArcModAgent-" stringByAppendingString:c.agent]; c.bundleID=[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:c.agent];
  c.appPath=[@LITTLE_ARC_EXPECTED_ARC_ROOT stringByAppendingPathComponent:[NSString stringWithFormat:@"build/%@.app",c.appName]];
  c.executablePath=[c.appPath stringByAppendingPathComponent:@"Contents/MacOS/Arc"];
  Need([String(request[@"appURL"]) isEqual:[NSURL fileURLWithPath:c.appPath isDirectory:YES].absoluteString]&&[String(request[@"executablePath"]) isEqual:c.executablePath]&&[String(request[@"bundleID"]) isEqual:c.bundleID]&&[String(request[@"appName"]) isEqual:c.appName],@"expected canonical app identity required");
  c.pid=(pid_t)Integer(request[@"pid"],2,INT_MAX);
  if([c.operation isEqual:@"inspect-only"]&&request[@"start"]==NSNull.null){c.seconds=0;c.microseconds=0;}
  else {NSDictionary *start=Object(request[@"start"],@[@"seconds",@"microseconds"]);c.seconds=Integer(start[@"seconds"],1,9007199254740991LL);c.microseconds=Integer(start[@"microseconds"],0,999999);}
  long long port=Integer(request[@"fixturePort"],1024,65535); NSString *letter=String(request[@"fixture"]);
  Need([@[@"a",@"b"] containsObject:letter],@"only fixed fixture A or B allowed");
  c.url=[NSString stringWithFormat:@"http://127.0.0.1:%lld/little-arc-7f3a/%@",port,letter];
  c.nonce=String(request[@"nonce"]); NSUUID *uuid=[[NSUUID alloc] initWithUUIDString:c.nonce];
  Need(uuid!=nil&&[uuid.UUIDString.lowercaseString isEqual:c.nonce],@"canonical UUID nonce required");
  c.sequence=Integer(request[@"sequence"],1,64);[c checkFiles];return c;
}

@protocol DeliveryTransport <NSObject>
@property(readonly) NSUInteger processQueries,permissionQueries,sendCalls;
- (NSDictionary *)inspect:(pid_t)pid;
- (OSStatus)permission:(NSAppleEventDescriptor *)target;
- (NSAppleEventDescriptor *)send:(NSAppleEventDescriptor *)event error:(NSError **)error;
@end

#if !LITTLE_ARC_OFFLINE_TEST
@interface ProductionTransport : NSObject <DeliveryTransport>
@property NSUInteger processQueries,permissionQueries,sendCalls;
@end
@implementation ProductionTransport
- (NSDictionary *)inspect:(pid_t)pid {
  self.processQueries++;
  struct proc_bsdinfo info; int count=proc_pidinfo(pid,PROC_PIDTBSDINFO,0,&info,sizeof(info));
  Need(count==sizeof(info)&&info.pbi_uid==geteuid()&&info.pbi_pid==(uint32_t)pid,@"owned exact process start unavailable");char path[PROC_PIDPATHINFO_MAXSIZE];
  Need(proc_pidpath(pid,path,sizeof(path))>0,@"exact process path unavailable");
  NSRunningApplication *app=[NSRunningApplication runningApplicationWithProcessIdentifier:pid];
  Need(app!=nil&&!app.isTerminated&&app.processIdentifier==pid&&app.bundleURL&&app.executableURL&&app.bundleIdentifier,@"exact running app unavailable");
  return @{@"pid":@(pid),@"uid":@(info.pbi_uid),@"seconds":@(info.pbi_start_tvsec),@"microseconds":@(info.pbi_start_tvusec),@"executablePath":[NSString stringWithUTF8String:path],@"appPath":app.bundleURL.path,@"appExecutablePath":app.executableURL.path,@"bundleID":app.bundleIdentifier,@"terminated":@NO};
}
- (OSStatus)permission:(NSAppleEventDescriptor *)target {
  self.permissionQueries++;Need(target.aeDesc!=NULL,@"PID target descriptor missing");
  return AEDeterminePermissionToAutomateTarget(target.aeDesc,kInternetEventClass,kAEGetURL,false);
}
- (NSAppleEventDescriptor *)send:(NSAppleEventDescriptor *)event error:(NSError **)error {
  Need(self.sendCalls==0,@"delivery already attempted");self.sendCalls++;
  return [event sendEventWithOptions:Options() timeout:3.0 error:error];
}
@end
#else
// Offline binary omits the production process/permission/send implementation.
@interface FakeTransport : NSObject <DeliveryTransport>
@property NSDictionary *spec;
@property NSUInteger processQueries,permissionQueries,sendCalls;
@end
@implementation FakeTransport
- (NSDictionary *)inspect:(pid_t)pid {
  (void)pid;NSArray *values=self.spec[@"identities"];Need([values isKindOfClass:NSArray.class]&&self.processQueries<values.count,@"fake inspection exhausted");return values[self.processQueries++];
}
- (OSStatus)permission:(NSAppleEventDescriptor *)target {
  Need(target.descriptorType==typeKernelProcessID,@"PID target required");self.permissionQueries++;
  return (OSStatus)Integer(self.spec[@"permissionStatus"],INT_MIN,INT_MAX);
}
- (NSAppleEventDescriptor *)send:(NSAppleEventDescriptor *)event error:(NSError **)error {
  Need(self.sendCalls==0,@"delivery already attempted");self.sendCalls++;
  Need(event.eventClass==kInternetEventClass&&event.eventID==kAEGetURL,@"fixed event class/id required");
  NSString *outcome=String(self.spec[@"sendOutcome"]);
  if ([outcome isEqual:@"timeout"]||[outcome isEqual:@"error"]) {*error=[NSError errorWithDomain:NSOSStatusErrorDomain code:([outcome isEqual:@"timeout"]?-1712:-1743) userInfo:nil];return nil;}
  if ([outcome isEqual:@"nil-reply"]) return nil;
  Need([@[@"success",@"reply-error",@"malformed-reply"] containsObject:outcome],@"invalid fake outcome");
  NSAppleEventDescriptor *reply=[NSAppleEventDescriptor appleEventWithEventClass:kCoreEventClass eventID:kAEAnswer targetDescriptor:nil returnID:kAutoGenerateReturnID transactionID:kAnyTransactionID];
  [reply setParamDescriptor:([outcome isEqual:@"malformed-reply"]?[NSAppleEventDescriptor descriptorWithString:@"bad"]:[NSAppleEventDescriptor descriptorWithInt32:([outcome isEqual:@"reply-error"]?-1708:0)]) forKeyword:keyErrorNumber];return reply;
}
@end
#endif

static NSDictionary *Inspect(DeliveryContext *c,id<DeliveryTransport> transport) {
  [c checkFiles];NSDictionary *got=Object([transport inspect:c.pid],@[@"pid",@"uid",@"seconds",@"microseconds",@"executablePath",@"appPath",@"appExecutablePath",@"bundleID",@"terminated"]);
  long long sec=Integer(got[@"seconds"],1,9007199254740991LL),usec=Integer(got[@"microseconds"],0,999999);
  Need(Integer(got[@"pid"],2,INT_MAX)==c.pid&&Integer(got[@"uid"],0,UINT_MAX)==geteuid(),@"owned PID required");
  if(c.seconds==0){Need([c.operation isEqual:@"inspect-only"],@"start binding unavailable");c.seconds=sec;c.microseconds=usec;}
  Need(sec==c.seconds&&usec==c.microseconds,@"process lifetime mismatch");
  Need(IsBoolean(got[@"terminated"])&&![got[@"terminated"] boolValue],@"process terminated");
  Need([String(got[@"executablePath"]) isEqual:c.executablePath]&&[String(got[@"appExecutablePath"]) isEqual:c.executablePath]&&[String(got[@"appPath"]) isEqual:c.appPath]&&[String(got[@"bundleID"]) isEqual:c.bundleID],@"running app identity mismatch");
  [c checkFiles];return got;
}
static NSAppleEventDescriptor *Prepare(DeliveryContext *c,NSMutableDictionary *receipt,NSAppleEventDescriptor **targetOut) {
  NSAppleEventDescriptor *target=[NSAppleEventDescriptor descriptorWithProcessIdentifier:c.pid];
  Need(target&&target.descriptorType==typeKernelProcessID&&target.data.length==sizeof(pid_t),@"exact PID target descriptor required");
  pid_t encoded=0;[target.data getBytes:&encoded length:sizeof(encoded)];Need(encoded==c.pid,@"PID target encoding mismatch");
  NSAppleEventDescriptor *event=[NSAppleEventDescriptor appleEventWithEventClass:kInternetEventClass eventID:kAEGetURL targetDescriptor:target returnID:kAutoGenerateReturnID transactionID:kAnyTransactionID];
  [event setParamDescriptor:[NSAppleEventDescriptor descriptorWithString:c.url] forKeyword:keyDirectObject];
  Need(event.eventClass==kInternetEventClass&&event.eventID==kAEGetURL&&[[event paramDescriptorForKeyword:keyDirectObject].stringValue isEqual:c.url],@"prepared event mismatch");
  receipt[@"preparedEvent"]=@{@"eventClass":@(event.eventClass),@"eventID":@(event.eventID),@"targetType":@(target.descriptorType),@"targetPID":@(encoded),@"directObject":c.url,@"sendOptions":@(Options()),@"timeoutSeconds":@3,@"askUserIfNeeded":@NO};
  *targetOut=target;return event;
}
int main(int argc,const char *argv[]) {
 @autoreleasepool {
  NSMutableDictionary *receipt=[@{@"version":@1,@"productionTransport":(LITTLE_ARC_OFFLINE_TEST?@NO:@YES),@"attempts":@0,@"processQueries":@0,@"permissionQueries":@0,@"sendCalls":@0,@"retryAllowed":@NO,@"deliveryOutcome":@"not-attempted",@"ok":@NO,@"runtimePanelOrNavigationVerified":@NO} mutableCopy];
  id<DeliveryTransport> transport=nil; BOOL returned=NO;int code=1;
  @try {
   Need(argc==1,@"no CLI arguments accepted; one bounded JSON object on stdin required");(void)argv;
   NSMutableData *data=[NSMutableData data];unsigned char buf[4096];ssize_t count;
   while((count=read(STDIN_FILENO,buf,sizeof(buf)))>0){Need(data.length+(NSUInteger)count<=16384,@"request too large");[data appendBytes:buf length:(NSUInteger)count];}Need(count==0&&data.length>0,@"request unreadable or empty");
   NSError *parse=nil;id object=[NSJSONSerialization JSONObjectWithData:data options:0 error:&parse];Need(parse==nil,@"valid JSON required");DeliveryContext *c=Context(object);
   [receipt addEntriesFromDictionary:@{@"operation":c.operation,@"agent":c.agent,@"pid":@(c.pid),@"start":@{@"seconds":@(c.seconds),@"microseconds":@(c.microseconds)},@"nonce":c.nonce,@"sequence":@(c.sequence),@"appPath":c.appPath,@"executablePath":c.executablePath,@"bundleID":c.bundleID,@"url":c.url}];
#if LITTLE_ARC_OFFLINE_TEST
   NSDictionary *spec=Object(c.request[@"testTransport"],@[@"identities",@"permissionStatus",@"sendOutcome"]);FakeTransport *fake=[FakeTransport new];fake.spec=spec;transport=fake;
#else
   transport=[ProductionTransport new];
#endif
   if([c.operation isEqual:@"inspect-only"]){receipt[@"identityBefore"]=Inspect(c,transport);receipt[@"identityAfter"]=Inspect(c,transport);receipt[@"start"]=@{@"seconds":@(c.seconds),@"microseconds":@(c.microseconds)};receipt[@"identityVerified"]=@YES;receipt[@"ok"]=@YES;code=0;}
   else {
    NSAppleEventDescriptor *target=nil,*event=Prepare(c,receipt,&target);
    if([c.operation isEqual:@"prepare-offline"]){receipt[@"deliveryOutcome"]=@"prepared-only";receipt[@"identityVerified"]=@NO;receipt[@"ok"]=@YES;code=0;}
    else {
     receipt[@"identityBefore"]=Inspect(c,transport);OSStatus permission=[transport permission:target];receipt[@"permissionStatus"]=@(permission);Need(permission==noErr,@"permission unavailable without prompting");
     receipt[@"identityImmediatelyBeforeSend"]=Inspect(c,transport);receipt[@"attempts"]=@1;receipt[@"deliveryOutcome"]=@"uncertain";
     NSError *error=nil;NSAppleEventDescriptor *reply=[transport send:event error:&error];returned=YES;receipt[@"sendReturned"]=@YES;
     receipt[@"identityAfter"]=Inspect(c,transport);receipt[@"identityVerified"]=@YES;
     if(error){receipt[@"transportError"]=@{@"domain":error.domain,@"code":@(error.code)};Need(NO,@"send returned error; reconcile without retry");}
     Need(reply!=nil,@"missing reply; reconcile without retry");NSAppleEventDescriptor *eventError=[reply paramDescriptorForKeyword:keyErrorNumber];
     Need(eventError==nil||eventError.descriptorType==typeSInt32,@"malformed event error; reconcile without retry");
     int32_t eventCode=eventError?eventError.int32Value:0;receipt[@"eventErrorNumber"]=@(eventCode);receipt[@"replyReceived"]=@YES;
     receipt[@"deliveryOutcome"]=eventCode==0?@"reply-received":@"event-rejected";Need(eventCode==0,@"target rejected event; no retry");receipt[@"ok"]=@YES;code=0;
    }
   }
  } @catch(NSException *exception) {receipt[@"error"]=exception.reason?:@"rejected";if([receipt[@"attempts"] integerValue]>0&&![receipt[@"deliveryOutcome"] isEqual:@"event-rejected"])receipt[@"deliveryOutcome"]=@"uncertain";}
  receipt[@"sendReturned"]=@(returned);if(transport){receipt[@"processQueries"]=@(transport.processQueries);receipt[@"permissionQueries"]=@(transport.permissionQueries);receipt[@"sendCalls"]=@(transport.sendCalls);}
  NSError *error=nil;NSData *output=[NSJSONSerialization dataWithJSONObject:receipt options:NSJSONWritingSortedKeys error:&error];if(!output||error)return 2;
  [NSFileHandle.fileHandleWithStandardOutput writeData:output];[NSFileHandle.fileHandleWithStandardOutput writeData:[@"\n" dataUsingEncoding:NSUTF8StringEncoding]];return code;
 }
}
