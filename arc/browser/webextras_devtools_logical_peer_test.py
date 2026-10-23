#!/usr/bin/env python3
"""Execute bounded actual Root/DevTools peer bodies against native/queue mocks.

Reads an explicit precomposed stage. Writes only into that private stage.
Actual protocol transport, JSON implementation, task runner and native UI are
mocks; this is not a Chromium runtime or golden acceptance comparison.
"""
from pathlib import Path
import argparse
import re,subprocess,hashlib,json
parser=argparse.ArgumentParser();parser.add_argument('--stage',type=Path,required=True);args=parser.parse_args();stage=args.stage.resolve();s=(stage/'arc/api/arc_webextras_devtools.mm').read_text()
def method(t,anchor):
 start=t.index(anchor);op=t.index('{',start);d=0;quote=None;comment=None;i=op
 while i<len(t):
  c=t[i];two=t[i:i+2]
  if comment=='//':
   if c=='\n':comment=None
  elif comment=='/*':
   if two=='*/':comment=None;i+=1
  elif quote:
   if c=='\\':i+=1
   elif c==quote:quote=None
  elif two in ('//','/*'):comment=two;i+=1
  elif c in ('"',"'"):quote=c
  elif c=='{':d+=1
  elif c=='}':
   d-=1
   if d==0:return t[start:i+1]
  i+=1
 raise AssertionError(anchor)
# Relocation invariants are checked independently from native mocks: these
# transport/parser/dispatch routines must remain byte-identical (only approved
# receiver syntax changes for moved raw methods).
base_source=(stage/'base-devtools.mm').read_text()
unchanged={}
for anchor in ['class ProtocolClient final','NSError* DevToolsError(','BOOL DevToolsFail(','void Notify(','void NotifySubscription(','void QueryPanel(']:
 a=method(base_source,anchor);b=method(s,anchor);assert a==b,'unchanged helper drift: '+anchor
 unchanged[anchor]=hashlib.sha256(a.encode()).hexdigest()
for old,new in [('- (void)receiveMessage:','void receiveMessage('),('- (NSNumber*)openProtocolConnection:','NSNumber* openProtocolConnection('),('- (BOOL)sendProtocolMessage:','BOOL sendProtocolMessage('),('- (void)closeProtocolConnection:','void closeProtocolConnection(')]:
 a=method(base_source.split('@implementation ArcWebContentsDevTools\n',1)[1],old);a=a[a.index('{'):].replace('[_owner webContents]','_contents.get()')
 b=method(s,new);b=b[b.index('{'):];assert a==b,'relocated parser/raw body drift: '+old
 unchanged[old]=hashlib.sha256(b.encode()).hexdigest()
assert '@property(nonatomic, readonly) ArcWebContents* toolboxWebContents;' in s
(stage/'unchanged-bodies.json').write_text(json.dumps(unchanged,indent=2)+'\n')
preamble=r'''
#import <Foundation/Foundation.h>
#include <functional>
#include <initializer_list>
#include <map>
#include <optional>
#include <memory>
#include <string>
#include <utility>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"CHECK failed: %s:%d %s\n",__FILE__,__LINE__,#x);abort();}}while(0)
#define SEQUENCE_CHECKER(x)
#define DCHECK_CALLED_ON_VALID_SEQUENCE(x)
#define FROM_HERE 0
#define ARC_EXPORT
static NSMutableArray<dispatch_block_t>* tasks;
static void Check(bool b, const char* s) { if(!b){fprintf(stderr,"FAILED %s\n",s);abort();} }
static void RunOne(){Check(tasks.count,"no task");auto b=tasks[0];[tasks removeObjectAtIndex:0];b();}
static void Drain(){unsigned count=0;while(tasks.count){Check(++count<100,"unbounded queue");RunOne();}}
namespace base {
template<class T>struct WeakState{T* pointer;};
template<class T>class WeakPtr {
 public:WeakPtr()=default; explicit WeakPtr(std::shared_ptr<WeakState<T>> x):weak_(x){}
 T* get()const{auto s=weak_.lock();return s?s->pointer:nullptr;} T* operator->()const{return get();}
 explicit operator bool()const{return get();}void reset(){weak_.reset();}
 private:std::weak_ptr<WeakState<T>> weak_;
};
template<class T>class WeakPtrFactory {
 public:explicit WeakPtrFactory(T*p):s_(std::make_shared<WeakState<T>>(WeakState<T>{p})){}
 ~WeakPtrFactory(){InvalidateWeakPtrs();}WeakPtr<T>GetWeakPtr(){return WeakPtr<T>(s_);}
 void InvalidateWeakPtrs(){s_->pointer=nullptr;}
 private:std::shared_ptr<WeakState<T>> s_;
};
template<class S>class OnceCallback;
template<class R,class...A>class OnceCallback<R(A...)>{
 public:OnceCallback()=default;template<class F>OnceCallback(F f):f_(std::move(f)){}
 explicit operator bool()const{return(bool)f_;}R Run(A...a){auto f=std::move(f_);return f(std::forward<A>(a)...);}
 private:std::function<R(A...)>f_;
};
template<class F>auto BindOnce(F f){return f;}template<class F,class...A>dispatch_block_t BindOnce(F f,A...a){return ^{f(a...);};}template<class F>auto BindRepeating(F f){return f;}
class SupportsUserData{public:struct Data{virtual~Data()=default;};};
template<class T>class DeleteHelper{public:static void Delete(T*p){delete p;}};
struct Value{explicit Value(std::string){}};
inline constexpr int JSON_PARSE_CHROMIUM_EXTENSIONS=0;
class DictValue{
 public:DictValue():dict_([NSMutableDictionary dictionary]){}explicit DictValue(NSDictionary*d):dict_([d mutableCopy]){}
 std::optional<int>FindInt(const std::string&k)const{id x=dict_[@(k.c_str())];if([x isKindOfClass:NSNumber.class])return [x intValue];return std::nullopt;}
 const std::string*FindString(const std::string&k)const{id x=dict_[@(k.c_str())];if(![x isKindOfClass:NSString.class])return nullptr;return &strings_.insert_or_assign(k,[x UTF8String]).first->second;}
 DictValue*FindDict(const std::string&k)const{id x=dict_[@(k.c_str())];if(![x isKindOfClass:NSDictionary.class])return nullptr;return &dicts_.insert_or_assign(k,DictValue(x)).first->second;}
 void Set(const std::string&k,int x){dict_[@(k.c_str())]=@(x);}void Set(const std::string&k,std::string x){dict_[@(k.c_str())]=@(x.c_str());}void Set(const std::string&k,DictValue x){dict_[@(k.c_str())]=x.dict_;}
 DictValue Clone()const{return DictValue(dict_);}NSMutableDictionary*dict()const{return dict_;}
 private:NSMutableDictionary*dict_;mutable std::map<std::string,std::string>strings_;mutable std::map<std::string,DictValue>dicts_;
};
struct JSONReader{static std::optional<DictValue>ReadDict(std::string s,int,int){id x=[NSJSONSerialization JSONObjectWithData:[@(s.c_str()) dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nil];if([x isKindOfClass:NSDictionary.class])return DictValue(x);return std::nullopt;}};
static std::optional<std::string>WriteJson(const DictValue&d){NSData*b=[NSJSONSerialization dataWithJSONObject:d.dict() options:NSJSONWritingSortedKeys error:nil];if(!b)return std::nullopt;return std::string([[NSString alloc]initWithData:b encoding:NSUTF8StringEncoding].UTF8String);}

}
namespace content {
class BrowserThread{public:enum ID{UI};static bool CurrentlyOn(ID){return true;}};
class WebContents;
class WebContentsObserver{public:explicit WebContentsObserver(WebContents*c=nullptr){Observe(c);}virtual~WebContentsObserver(){Observe(nullptr);}void Observe(WebContents*);virtual void WebContentsDestroyed(){} private:WebContents*c_=nullptr;};
class WebContents{
 public:WebContents():weak_(this){}~WebContents(){while(!observers.empty()){auto*p=observers.back();p->WebContentsDestroyed();if(std::find(observers.begin(),observers.end(),p)!=observers.end())p->Observe(nullptr);}}
 base::WeakPtr<WebContents> GetWeakPtr(){return weak_.GetWeakPtr();}
 void SetUserData(const void*k,std::unique_ptr<base::SupportsUserData::Data>d){userdata[k]=std::move(d);}
 base::SupportsUserData::Data* GetUserData(const void*k){auto i=userdata.find(k);return i==userdata.end()?nullptr:i->second.get();}
 void RemoveUserData(const void*k){userdata.erase(k);}void ClosePage(){++closeCalls;}
 std::vector<WebContentsObserver*>observers;unsigned closeCalls=0;
 private:base::WeakPtrFactory<WebContents>weak_;std::map<const void*,std::unique_ptr<base::SupportsUserData::Data>>userdata;
};
void WebContentsObserver::Observe(WebContents*c){if(c_){auto&v=c_->observers;v.erase(std::remove(v.begin(),v.end(),this),v.end());}c_=c;if(c_)c_->observers.push_back(this);}
struct Runner{void PostTask(int,dispatch_block_t b){[tasks addObject:[b copy]];}template<class T>void DeleteSoon(int,T*p){[tasks addObject:^{base::DeleteHelper<T>::Delete(p);}];}};
static Runner*GetUIThreadTaskRunner(std::initializer_list<int>){static Runner r;return &r;}
}
namespace arc {struct CoreCloseState{};using CoreCloseStateHandle=std::shared_ptr<CoreCloseState>;}
@protocol WebExtrasDevToolsDelegate<NSObject>
- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;
- (void)webContentsDevToolsNeedsFocus:(id)tools;
- (void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;
- (void)webContentsDevToolsDidClose:(id)tools;
@end
@protocol ArcDevToolsProtocolCommand
- (NSString*)domain;- (NSString*)name;- (NSString*)params;
@end
@protocol ArcDevToolsProtocolEvent
- (NSString*)domain;- (NSString*)name;
@end
@class ArcWebContentsDevTools;
@interface ArcWebContents:NSObject
+ (ArcWebContents*)wrapperForWebContents:(content::WebContents*)c;
@end
@implementation ArcWebContents
+ (ArcWebContents*)wrapperForWebContents:(content::WebContents*)c{(void)c;return [ArcWebContents new];}
@end
using ProtocolNotification=void(^)(NSString*,NSError*);
using SubscriptionNotification=void(^)(NSNumber*,NSString*,NSError*);
class ProtocolClient{
 public:explicit ProtocolClient(ProtocolNotification cb,bool post=true):callback_(cb),post_(post){}
 bool Attach(content::WebContents*){return true;}bool is_open()const{return true;}
 bool Send(NSString*s,NSError**){sent.push_back(s);return true;}void Close(){}
 void Receive(NSString*s,NSError*e){callback_(s,e);}
 std::vector<NSString*>sent;
 private:ProtocolNotification callback_;bool post_;
};
static std::string ArcUTF8(NSString*s){return s.UTF8String?:"";}
static NSString* ArcNSString(const std::string&s){return @(s.c_str());}
static bool feature44=true;static bool ArcRuntimeFeatureEnabled(NSUInteger){return feature44;}
struct FrontendRetiredState{ArcWebContents*toolbox;NSArray<ProtocolNotification>*panel_queries;};
class DevToolsUIBindings{public:class Delegate{public:virtual void CloseWindow()=0;};static DevToolsUIBindings*ForWebContents(content::WebContents*){return nullptr;}void CallClientMethod(const char*,const char*,base::Value){}};
class DevToolsWindow:public DevToolsUIBindings::Delegate{public:static DevToolsWindow*AsDevToolsWindow(content::WebContents*){return nullptr;}static DevToolsWindow*GetInstanceForInspectedWebContents(content::WebContents*){return nullptr;}};
static unsigned queryCalls;
static void QueryPanel(content::WebContents*,ProtocolNotification){++queryCalls;}
namespace arcmod{
static std::function<void(content::WebContents*,content::WebContents*,bool)>&GetDevToolsShowHook(){static std::function<void(content::WebContents*,content::WebContents*,bool)>f;return f;}
static std::function<void(content::WebContents*,content::WebContents*)>&GetDevToolsFrontendCreatedHook(){static std::function<void(content::WebContents*,content::WebContents*)>f;return f;}
static std::function<void(content::WebContents*,content::WebContents*,const std::string&)>&GetDevToolsDockPositionHook(){static std::function<void(content::WebContents*,content::WebContents*,const std::string&)>f;return f;}
static std::function<void(content::WebContents*,content::WebContents*,const std::string&)>&GetDevToolsInspectModeHook(){static std::function<void(content::WebContents*,content::WebContents*,const std::string&)>f;return f;}
static std::function<void(content::WebContents*)>&GetDevToolsFocusHook(){static std::function<void(content::WebContents*)>f;return f;}
}
'''
def strip_includes(t):return '\n'.join(l for l in t.splitlines()if not l.startswith('#include'))
root_h=strip_includes((stage/'arc/browser/core_logical_web_contents.h').read_text())
root_mm=strip_includes((stage/'arc/browser/core_logical_web_contents.mm').read_text())
peer_h=strip_includes((stage/'arc/browser/webextras_devtools_logical_peer.h').read_text())
helpers=s[s.index('@interface WebextrasDevToolsDelegateBridge'):s.index('@interface NSObject (WebExtrasDevToolsInternal)')]
binding=s[s.index('constinit int g_nextSubscription'):s.index('class ProtocolClient final')]
observer=s[s.index('class FrontendObserver final'):s.index('void QueryPanel')]
impl=s[s.index('class WebextrasDevToolsPeer::Impl final'):s.index('WebextrasDevToolsPeer::WebextrasDevToolsPeer')]
# Exclude protocol and window-opening operations from this bounded runtime
# fixture. Their mechanically relocated bodies are checked independently below.
for anchor in ['void openWithAction(','void open()','void openInspectView()','void openConsoleView()','void openNetworkView()','void inspectElementAt(','BOOL currentPanel(']:
 m=method(impl,anchor);impl=impl.replace(m,'')
# Impl InstallHooks remains exact actual body after the full type is complete.
start=s.index('WebextrasDevToolsPeer::WebextrasDevToolsPeer');end=s.index('ARC_EXPORT @interface ArcWebContentsDevTools : NSObject',start)
peer_defs=s[start:end]
fac=s[s.index('ARC_EXPORT @interface ArcWebContentsDevTools : NSObject'):s.index('ARC_EXPORT @interface ArcBrowserContextDevTools')]
facimpl=fac.split('@implementation ArcWebContentsDevTools\n')[1].split('\n@end')[0]
kept=[]
for anchor in ['- (instancetype)initWithLogicalDevToolsPeer:','- (void)setDelegate:','- (ArcWebContents*)webContents','- (ArcWebContents*)toolboxWebContents','- (void)close','- (NSNumber*)openProtocolConnection:','- (BOOL)sendProtocolMessage:','- (void)closeProtocolConnection:','- (BOOL)execute:','- (NSNumber*)subscribe:','- (BOOL)unsubscribe:']:
 kept.append(method(facimpl,anchor))
fac=fac.split('@implementation ArcWebContentsDevTools')[0]+'@implementation ArcWebContentsDevTools\n'+'\n'.join(kept)+'\n@end\n'+fac.split('@end\nArcWebContentsDevTools* WebextrasCreateDevToolsService',1)[1]
fac=fac.replace('\n(\u200b','\n(')if False else fac
# restore creator declaration prefix split off above
fac=fac.replace('\n(const arc::WebextrasDevToolsPeerHandle& peer)', '\nArcWebContentsDevTools* WebextrasCreateDevToolsService(const arc::WebextrasDevToolsPeerHandle& peer)')
# The original declaration is on the same line and split removed its name only.
if '\n(const arc::' not in fac and 'ArcWebContentsDevTools* WebextrasCreateDevToolsService(' not in fac:
 fac=fac.replace('\n(const arc::','\nArcWebContentsDevTools* WebextrasCreateDevToolsService(const arc::')
checks=r'''
@interface TestCommand:NSObject<ArcDevToolsProtocolCommand,ArcDevToolsProtocolEvent>
@property(nonatomic,copy)NSString*domain;@property(nonatomic,copy)NSString*name;@property(nonatomic,copy)NSString*params;
@end
@implementation TestCommand
@end

@interface TestDelegate:NSObject<WebExtrasDevToolsDelegate>
@property(nonatomic,strong)NSMutableArray*events;
@property(nonatomic,strong)NSMutableArray*owners;
@property(nonatomic,copy)void(^onCall)(void);
@end
@implementation TestDelegate
- (instancetype)init{if((self=[super init])){_events=[NSMutableArray array];_owners=[NSMutableArray array];}return self;}
- (void)add:(id)owner event:(NSString*)event{[_events addObject:event];[_owners addObject:owner?:NSNull.null];if(_onCall)_onCall();}
- (void)webContentsDevTools:(id)t displayAs:(NSUInteger)m{[self add:t event:[NSString stringWithFormat:@"d%lu",m]];}
- (void)webContentsDevToolsNeedsFocus:(id)t{[self add:t event:@"focus"];}
- (void)webContentsDevTools:(id)t inspectModeDidChange:(NSUInteger)m{[self add:t event:[NSString stringWithFormat:@"i%lu",m]];}
- (void)webContentsDevToolsDidClose:(id)t{[self add:t event:@"close"];}
@end
class FunctionalMedia final:public arc::CoreMediaPeer{
 public:explicit FunctionalMedia(arc::CoreLogicalWebContentsWeakHandle w):root_(w){}
 void AttachToWebContents(base::WeakPtr<content::WebContents>)override{}
 void RetireFromWebContents()override{}
 void Receipt(){auto logical=root_.lock();if(!logical)return;++cache;
  auto retained=logical;content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, ^{ ++deliveries; (void)retained; });}
 unsigned cache=0;inline static unsigned deliveries=0;
 private:arc::CoreLogicalWebContentsWeakHandle root_;
};
static arc::CoreLogicalWebContentsHandle MakeRoot(content::WebContents*c){
 return arc::CreateCoreLogicalWebContents(c->GetWeakPtr(),std::make_shared<arc::CoreCloseState>(),
  [](arc::CoreLogicalWebContentsWeakHandle w)->std::shared_ptr<arc::CoreMediaPeer>{return std::make_shared<FunctionalMedia>(w);},arc::MakeWebextrasDevToolsPeerFactory());
}
static arc::WebextrasDevToolsPeerHandle Peer(const arc::CoreLogicalWebContentsHandle&r){return std::static_pointer_cast<arc::WebextrasDevToolsPeer>(r->GetDevToolsPeer());}
int main(){@autoreleasepool{tasks=[NSMutableArray array];unsigned cases=0;
 // Native weak routing works before any ObjC service exists; both slots are installed.
 {content::WebContents c,f;auto r=MakeRoot(&c);auto p=Peer(r);Check(!PeerForContents(&c),"factory observed before Attach");p->AttachToWebContents(c.GetWeakPtr());Check(PeerForContents(&c).get()==p.get(),"userdata not actual peer");arcmod::GetDevToolsFrontendCreatedHook()(&c,&f);Check(p->state()._frontend.get()==&f,"pre-service route failed");}++cases;
 // Service-only ownership survives Root release, as does functional state.
 {content::WebContents c,f;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];auto w=p->GetWeakPtr();r.reset();p.reset();Check(bool(w),"service did not retain peer");arcmod::GetDevToolsDockPositionHook()(&c,&f,"left");arcmod::GetDevToolsFocusHook()(&c);Drain();Check([d.events isEqual:@[@"d2",@"focus"]],"service-only functional delivery");}++cases;
 // ObjC facade expiry cannot cancel work while Root still owns peer.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*d=[TestDelegate new];__weak ArcWebContentsDevTools* weak;
 @autoreleasepool{auto*svc=WebextrasCreateDevToolsService(p);weak=svc;[svc setDelegate:d];p->state().requestedDockPosition("right");}
 Check(!weak,"task retained service");Drain();Check([d.owners isEqual:@[NSNull.null]],"nil service suppressed");}++cases;
 // Final REAL peer expiry cancels accepted notification, despite captured bridge.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*d=[TestDelegate new];auto*svc=WebextrasCreateDevToolsService(p);[svc setDelegate:d];p->state().requestedDockPosition("bottom");auto w=p->GetWeakPtr();svc=nil;p.reset();r.reset();Check(!w,"task retained peer/root");Drain();Check(d.events.count==0,"final peer expiry delivered");}++cases;
 // Physical retirement and destruction preserve guard; commands see no native.
 {auto c=std::make_unique<content::WebContents>();auto r=MakeRoot(c.get());auto p=Peer(r);p->AttachToWebContents(c->GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().requestedDockPosition("left");auto w=p->GetWeakPtr();c.reset();Check(w&&!p->state()._contents,"physical destroy poisoned real peer");Drain();Check([d.events isEqual:@[@"d2"]],"physical destroy canceled accepted work");}++cases;
 // Non-nil bridge replacement updates selected bridge; nil/new does not retarget old task.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*a=[TestDelegate new];auto*b=[TestDelegate new];auto*z=[TestDelegate new];[svc setDelegate:a];p->state().requestedDockPosition("left");[svc setDelegate:b];Drain();Check(!a.events.count&&[b.events isEqual:@[@"d2"]],"non-nil bridge update lost");p->state().requestedDockPosition("right");[svc setDelegate:nil];[svc setDelegate:z];p->state().inspectModeChanged("showDistances");Drain();Check([b.events isEqual:@[@"d2",@"d3"]]&&[z.events isEqual:@[@"i4"]],"nil/new retargeted old bridge");}++cases;
 // Weak last bridge can accept nested receipts during nil while queued task owns bridge.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().requestedDockPosition("right");[svc setDelegate:nil];p->state().frontendRequestedFocus();Check(tasks.count==2,"nil period weak registration dropped");Drain();p->state().requestedDockPosition("bottom");Check(!tasks.count,"expired bridge accepted work");}++cases;
 // Actual Root ownership carried by a functional Media receipt keeps DevTools sibling alive.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*d=[TestDelegate new];auto*svc=WebextrasCreateDevToolsService(p);[svc setDelegate:d];auto m=std::static_pointer_cast<FunctionalMedia>(r->GetMediaPeer());m->Receipt();p->state().requestedDockPosition("bottom");auto w=p->GetWeakPtr();svc=nil;p.reset();m.reset();r.reset();Check(bool(w),"media work did not retain actual root/sibling");RunOne();Check(!w,"completed media work padded root lifetime");Drain();Check(!d.events.count,"devtools queue retained root itself");}++cases;
 // Reentrant delegate final release: queue guard does not strong-lock real peer.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*d=[TestDelegate new];__block ArcWebContentsDevTools*svc=WebextrasCreateDevToolsService(p);[svc setDelegate:d];auto w=p->GetWeakPtr();auto*rp=&r;auto*pp=&p;d.onCall=^{svc=nil;pp->reset();rp->reset();Check(!w,"delivery locked peer across app callout");};p->state().requestedDockPosition("right");svc=nil;Drain();Check(!w,"reentrant final release did not expire peer");d.onCall=nil;}++cases;
 // Frontend replacement/reset is independent of accepted work.
 {content::WebContents c,f1,f2;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().didCreateFrontend(&f1);p->state().requestedDockPosition("left");p->state().resetFrontendState();p->state().didCreateFrontend(&f2);Drain();Check([d.events isEqual:@[@"d2"]]&&p->state()._frontend.get()==&f2,"replacement canceled/reapplied state");}++cases;
 // All native string maps and explicit-only display source remain exact.
 {content::WebContents c,f;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().didCreateFrontend(&f);p->state().didShowFrontend(&f,YES);Check(!tasks.count,"Show synthesized display");for(auto x:{"undocked","left","right","bottom","other"})p->state().requestedDockPosition(x);for(auto x:{"searchForNode","searchForUAShadowDOM","captureAreaScreenshot","showDistances","none"})p->state().inspectModeChanged(x);Drain();Check([d.events isEqual:@[@"d1",@"d2",@"d3",@"d4",@"d0",@"i1",@"i2",@"i3",@"i4",@"i0"]],"mapping/order changed");}++cases;

 // Actual relocated raw connection API keeps feature-before-binding and ID/error policy.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());NSError*e=nil;feature44=false;Check(!p->state().openProtocolConnection(nil,&e)&&e.code==6,"feature gate changed");feature44=true;NSNumber*n=p->state().openProtocolConnection(nil,&e);Check([n isEqual:@1],"first raw connection id");Check(!p->state().sendProtocolMessage(@"{}",@99,&e)&&e.code==3,"unknown raw id");p->state().closeProtocolConnection(n);Check(!p->state().sendProtocolMessage(@"{}",n,&e)&&e.code==3,"raw close did not remove");}++cases;
 // Actual execute dictionaries/ID routing preserve direct completion + full error JSON.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*cmd=[TestCommand new];cmd.domain=@"Runtime";cmd.name=@"evaluate";cmd.params=@"[]";NSError*e=nil;Check(!p->state().execute(cmd,nil,&e)&&e.code==2,"params dict parser changed");cmd.params=@"{}";__block NSString*result=nil;__block NSError*failure=nil;Check(p->state().execute(cmd,^(NSString*m,NSError*x){result=m;failure=x;},&e),"execute failed");auto id=base::JSONReader::ReadDict(ArcUTF8(p->state()._commands->sent.back()),0,200)->FindInt("id");NSString*reply=[NSString stringWithFormat:@"{\"id\":%d,\"error\":{\"code\":-1,\"message\":\"raw\"}}",*id];p->state()._commands->Receive(reply,nil);Check(failure.code==7&&!result&&[failure.localizedDescription containsString:@"raw"]&&!tasks.count,"command completion shape/scheduling changed");Check(p->state()._pending.count==0,"pending completion not erased before callback");}++cases;
 // Actual subscription router preserves ascending IDs, queued delivery and literal last-event disable.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*ev=[TestCommand new];ev.domain=@"Runtime";ev.name=@"ready";NSMutableArray*got=[NSMutableArray array];auto cb=^(NSNumber*i,NSString*m,NSError*e){[got addObject:i];Check(m&&!e,"subscription payload changed");};auto a=p->state().subscribe(ev,YES,cb);auto b=p->state().subscribe(ev,YES,cb);Check(p->state()._commands->sent.size()==2,"autoEnable no longer per call");p->state().receiveMessage(@"{\"method\":\"Runtime.ready\",\"params\":{}}",nil);Check(!got.count&&tasks.count==2,"event delivered inline");Check(p->state().unsubscribe(a)&&p->state()._commands->sent.size()==2,"disable before last literal event");Check(p->state().unsubscribe(b)&&p->state()._commands->sent.size()==3,"last event disable missing");Drain();Check([got isEqual:@[a,b]],"queued event id order/copy changed");Check(!p->state().unsubscribe(a),"unknown unsubscribe succeeded");}++cases;
 // Command weak callback routes to actual peer after facade dies; final peer expiry skips it.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*cmd=[TestCommand new];cmd.domain=@"Runtime";cmd.name=@"evaluate";cmd.params=@"{}";__block unsigned delivered=0;Check([svc execute:cmd completion:^(NSString*,NSError*){++delivered;} error:nil],"facade execute failed");auto client=p->state()._commands;auto id=base::JSONReader::ReadDict(ArcUTF8(client->sent.back()),0,200)->FindInt("id");svc=nil;client->Receive([NSString stringWithFormat:@"{\"id\":%d,\"result\":{}}",*id],nil);Check(delivered==1,"facade death canceled actual peer router");p.reset();r.reset();client->Receive(@"{}",nil);Check(delivered==1,"expired peer router invoked callback");}++cases;

 // Actual close resets/disarms before native close and synchronous app reentry.
 {content::WebContents c,f1,f2;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().didCreateFrontend(&f1);p->state().didShowFrontend(&f1,YES);auto*raw=p.get();auto*replacement=&f2;d.onCall=^{Check(!raw->state()._frontend,"close callback saw unreset state");raw->state().didCreateFrontend(replacement);};[svc close];Check(f1.closeCalls==1&&[d.events isEqual:@[@"close"]]&&p->state()._frontend.get()==&f2,"close did not preserve reentrant replacement");d.onCall=nil;}++cases;
 // Actual natural frontend observer delivery disarms itself, including pre-Show.
 {content::WebContents c;auto front=std::make_unique<content::WebContents>();auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*svc=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[svc setDelegate:d];p->state().didCreateFrontend(front.get());front.reset();Check([d.events isEqual:@[@"close"]]&&!p->state()._frontend&&!p->state()._frontendObserver,"natural close before Show failed");p->state().frontendClosed();Check(d.events.count==1,"natural close repeated");}++cases;

 // Direct command completion adds no peer/root lock through app reentry either.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*cmd=[TestCommand new];cmd.domain=@"Runtime";cmd.name=@"evaluate";cmd.params=@"{}";auto weak=p->GetWeakPtr();auto*rp=&r;auto*pp=&p;Check(p->state().execute(cmd,^(NSString*,NSError*){pp->reset();rp->reset();Check(!weak,"command callback locked actual peer");},nil),"execute reentry setup failed");auto client=p->state()._commands;auto id=base::JSONReader::ReadDict(ArcUTF8(client->sent.back()),0,200)->FindInt("id");client->Receive([NSString stringWithFormat:@"{\"id\":%d,\"result\":{}}",*id],nil);Check(!weak,"command final release failed");}++cases;
 Check(!tasks.count&&cases==18,"fixture count/queue mismatch");puts("18 actual Root/peer extracted functional lifetime, registration, routing, command, subscription and close groups PASS; native APIs/task runner mocked");
}}
'''
fixture=preamble+'\n'+root_h+'\n'+peer_h+'\n'+root_mm+'\n'+helpers+'\nnamespace {\n'+binding+'\n'+observer+'\n}\nnamespace arc {\n'+impl+'\n'+peer_defs+'\n'+fac+'\n'+checks
(stage/'peer-lifetime-fixture.mm').write_text(fixture)
cmd=['xcrun','clang++','-std=c++20','-fobjc-arc','-O1','-Wall','-Wextra','-Werror','-Wno-unused-function','-Wno-unused-parameter','-Wno-unused-private-field','-framework','Foundation',str(stage/'peer-lifetime-fixture.mm'),'-o',str(stage/'peer-lifetime-fixture')]
p=subprocess.run(cmd,text=True,capture_output=True);(stage/'lifetime-compiler.log').write_text(p.stdout+p.stderr);print('fixture compiler exit',p.returncode);print((p.stdout+p.stderr)[:16000]);assert p.returncode==0,'fixture compiler failed'
if p.returncode==0:
 p=subprocess.run([str(stage/'peer-lifetime-fixture')],text=True,capture_output=True);(stage/'lifetime-run.log').write_text(p.stdout+p.stderr);print('RUN',p.returncode,p.stdout,p.stderr);assert p.returncode==0,'fixture runtime failed'
