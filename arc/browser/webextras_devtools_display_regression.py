#!/usr/bin/env python3
"""Exercise extracted production display/registration bodies with a bounded FIFO.

The queue and WebContents anchor are mocks. This proves ObjC bridge lifetime,
mode capture, state ordering and reentry, not Chromium task-runner/native parity.
"""
from pathlib import Path
import hashlib
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
source = (ROOT/'arc/api/arc_webextras_devtools.mm').read_text()
helper = source.split('// BEGIN WEBEXTRAS_DEVTOOLS_DISPLAY\n', 1)[1].split('// END WEBEXTRAS_DEVTOOLS_DISPLAY', 1)[0]
implementation = source.split('@implementation ArcWebContentsDevTools\n', 1)[1].split('\n@end', 1)[0]

def body(anchor):
    start = implementation.index(anchor)
    opening = implementation.index('{', start)
    depth = 0
    for position in range(opening, len(implementation)):
        if implementation[position] == '{': depth += 1
        elif implementation[position] == '}':
            depth -= 1
            if not depth: return implementation[start:position+1]
    raise ValueError('unclosed production body')

bodies = '\n'.join(body(anchor) for anchor in (
    '- (void)setDelegate:', '- (void)didShowFrontend:', '- (void)resetFrontendState {'))
preamble = r'''
#import <Foundation/Foundation.h>
#include <initializer_list>
#include <memory>
#include <string>
#define FROM_HERE 0
static NSMutableArray<dispatch_block_t>* tasks;
static void Check(bool value, NSString* message) { if(!value) { NSLog(@"FAILED: %@",message); abort(); } }
static void RunOne() { Check(tasks.count>0,@"no queued task"); dispatch_block_t task=tasks[0]; [tasks removeObjectAtIndex:0]; task(); }
static void Drain() { unsigned count=0; while(tasks.count) { Check(++count<=12,@"unbounded delivery"); RunOne(); } }
namespace base {
template<class T> struct WeakState { T* value; };
template<class T> class WeakPtr {
 public:
  WeakPtr()=default;
  explicit WeakPtr(std::shared_ptr<WeakState<T>> state):state_(state){}
  T* get() const { auto state=state_.lock(); return state ? state->value : nullptr; }
  explicit operator bool() const { return get()!=nullptr; }
  void reset() { state_.reset(); }
 private: std::weak_ptr<WeakState<T>> state_;
};
static dispatch_block_t BindOnce(dispatch_block_t task) { return [task copy]; }
struct Value { explicit Value(std::string) {} };
}
namespace devtools { enum class DockSide {kNone,kLeft,kRight,kBottom}; }
class Profile {
 public:
  static Profile* FromBrowserContext(Profile* profile) { return profile; }
  devtools::DockSide side=devtools::DockSide::kRight;
};
class DevToolsSettings { public: static devtools::DockSide GetDockSide(Profile* profile) { return profile->side; } };
namespace content {
class WebContents {
 public:
  WebContents():state_(std::make_shared<base::WeakState<WebContents>>(base::WeakState<WebContents>{this})){}
  ~WebContents(){state_->value=nullptr;}
  base::WeakPtr<WebContents> GetWeakPtr(){return base::WeakPtr<WebContents>(state_);}
  Profile* GetBrowserContext(){return &profile;}
  template<class T> void SetUserData(const void*,std::unique_ptr<T>){}
  Profile profile;
 private: std::shared_ptr<base::WeakState<WebContents>> state_;
};
struct Runner { void PostTask(int,dispatch_block_t task){[tasks addObject:[task copy]];} };
static Runner* GetUIThreadTaskRunner(std::initializer_list<int>){static Runner runner;return &runner;}
}
@protocol WebExtrasDevToolsDelegate <NSObject>
- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;
@end
@class ArcWebContentsDevTools;
@interface ArcWebContents : NSObject { @public content::WebContents* native; }
- (content::WebContents*)webContents;
@end
@implementation ArcWebContents
- (content::WebContents*)webContents{return native;}
@end
'''
interface = r'''
using ProtocolNotification=void(^)(NSString*,NSError*);
static char kDevToolsBindingKey;
class DevToolsBinding { public: explicit DevToolsBinding(id){} };
class FrontendObserver { public: FrontendObserver(content::WebContents*,id){} };
class DevToolsUIBindings {
 public:
  static DevToolsUIBindings* ForWebContents(content::WebContents*){return nullptr;}
  void CallClientMethod(const char*,const char*,base::Value){}
};
static std::string ArcUTF8(NSString* text){return text.UTF8String ?: "";}
static unsigned queryCalls;
static void QueryPanel(content::WebContents*,ProtocolNotification){++queryCalls;}
@interface ArcWebContentsDevTools : NSObject {
  __weak ArcWebContents* _owner;
  __weak id<WebExtrasDevToolsDelegate> _delegate;
  WebextrasDevToolsDelegateBridge* _delegateBridge;
  __weak WebextrasDevToolsDelegateBridge* _registeredDelegateBridge;
  ArcWebContents* _toolboxWebContents;
  base::WeakPtr<content::WebContents> _frontend;
  std::unique_ptr<FrontendObserver> _frontendObserver;
  NSMutableArray<ProtocolNotification>* _panelQueries;
  NSString* _requestedPanel;
  BOOL _opening;
  BOOL _shown;
}
- (instancetype)initForOwner:(ArcWebContents*)owner;
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked;
- (void)resetFrontendState __attribute__((objc_direct));
- (void)checkShown:(content::WebContents*)frontend;
- (void)checkReset;
- (void)seedQuery;
@end
@implementation ArcWebContentsDevTools
- (instancetype)initForOwner:(ArcWebContents*)owner { if((self=[super init])){_owner=owner;_panelQueries=[NSMutableArray array];}return self; }
- (void)checkShown:(content::WebContents*)frontend { Check(_shown && !_opening && _frontend.get()==frontend && _panelQueries.count==0,@"synchronous state/query work lost"); }
- (void)checkReset { Check(!_shown && !_opening && !_frontend && _panelQueries.count==0,@"queued delivery reapplied frontend state"); }
- (void)seedQuery { [_panelQueries addObject:^(NSString* a __attribute__((unused)),NSError* b __attribute__((unused))){}]; }
'''
checks = r'''
@end
@interface MockDelegate : NSObject <WebExtrasDevToolsDelegate>
@property(nonatomic,strong) NSMutableArray* modes;
@property(nonatomic,strong) NSMutableArray* owners;
@property(nonatomic,copy) void(^onDisplay)(id,NSUInteger);
@end
@implementation MockDelegate
- (instancetype)init { if((self=[super init])){_modes=[NSMutableArray array];_owners=[NSMutableArray array];}return self; }
- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode { [_modes addObject:@(mode)];[_owners addObject:tools ?: NSNull.null];if(_onDisplay)_onDisplay(tools,mode); }
@end
static ArcWebContents* Owner(content::WebContents* native){ArcWebContents* owner=[ArcWebContents new];owner->native=native;return owner;}
int main(){@autoreleasepool{
 tasks=[NSMutableArray array];unsigned cases=0;
 // Mode mapping and capture at receipt, with state/queries completed synchronously.
 for(auto side:{devtools::DockSide::kNone,devtools::DockSide::kLeft,devtools::DockSide::kRight,devtools::DockSide::kBottom}){
  content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];
  front.profile.side=side;[tools seedQuery];unsigned before=queryCalls;[tools didShowFrontend:&front docked:YES];[tools checkShown:&front];
  Check(queryCalls==before+1 && delegate.modes.count==0 && tasks.count==1,@"display was synchronous or queries deferred");
  front.profile.side=devtools::DockSide::kRight;Drain();
  NSUInteger expected=side==devtools::DockSide::kNone?1:side==devtools::DockSide::kLeft?2:side==devtools::DockSide::kRight?3:4;
  Check([delegate.modes isEqual:@[@(expected)]],@"docked mode changed after receipt");
 }
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];[tools didShowFrontend:&front docked:NO];Drain();Check([delegate.modes isEqual:@[@1]],@"undocked mode");}++cases;
 // Existing non-nil registration is mutable, read at delivery.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* a=[MockDelegate new];auto* b=[MockDelegate new];[tools setDelegate:a];[tools didShowFrontend:&front docked:YES];[tools setDelegate:b];Drain();Check(!a.modes.count && [b.modes isEqual:@[@3]],@"non-nil update did not retarget existing bridge");}++cases;
 // Nil drops current registration only: old queued bridge survives nil/new.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* a=[MockDelegate new];auto* b=[MockDelegate new];[tools setDelegate:a];[tools didShowFrontend:&front docked:YES];[tools setDelegate:nil];[tools setDelegate:b];front.profile.side=devtools::DockSide::kLeft;[tools didShowFrontend:&front docked:YES];Drain();Check([a.modes isEqual:@[@3]] && [b.modes isEqual:@[@2]],@"nil/new canceled or redirected old bridge");}++cases;
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* a=[MockDelegate new];[tools setDelegate:a];[tools didShowFrontend:&front docked:YES];[tools setDelegate:nil];Drain();Check([a.modes isEqual:@[@3]],@"nil canceled strongly queued bridge");}++cases;
 // Native weak registration remains usable during nil while a task owns it.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* a=[MockDelegate new];[tools setDelegate:a];[tools didShowFrontend:&front docked:YES];[tools setDelegate:nil];[tools didShowFrontend:&front docked:NO];Check(tasks.count==2,@"nil dropped still-live native weak registration");Drain();Check([a.modes isEqual:@[@3,@1]],@"new nil-period receipt missed old bridge");[tools didShowFrontend:&front docked:YES];Check(tasks.count==0 && a.modes.count==2,@"expired old weak registration accepted a receipt");}++cases;
 // Weak delegate expiry: the bridge must not keep the app delegate alive.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];__weak MockDelegate* weak;
  @autoreleasepool{auto* delegate=[MockDelegate new];weak=delegate;[tools setDelegate:delegate];[tools didShowFrontend:&front docked:YES];}
  Check(!weak,@"queued bridge retained delegate");Drain();}++cases;
 // Weak ObjC service may expire while the explicit native anchor still lives.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* delegate=[MockDelegate new];__weak ArcWebContentsDevTools* weak;
  @autoreleasepool{auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];weak=tools;[tools setDelegate:delegate];[tools didShowFrontend:&front docked:YES];}
  Check(!weak,@"queued bridge retained service");Drain();Check([delegate.owners isEqual:@[NSNull.null]],@"nil-service bridge ABI was suppressed");}++cases;
 // Owner-WebContents guard is an explicit proxy, not native Impl lifetime proof.
 {auto native=std::make_unique<content::WebContents>();content::WebContents front;ArcWebContents* owner=Owner(native.get());auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];[tools didShowFrontend:&front docked:YES];native.reset();Drain();Check(!delegate.modes.count,@"expired native anchor delivered");}++cases;
 // Closing/resetting the frontend does not cancel or reapply queued display.
 {content::WebContents native,front;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];[tools didShowFrontend:&front docked:YES];[tools resetFrontendState];Drain();[tools checkReset];Check([delegate.modes isEqual:@[@3]],@"close canceled queued display");}++cases;
 // Reentry queues another display, and its own bridge is independently mutable.
 {content::WebContents native,front1,front2;ArcWebContents* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* a=[MockDelegate new];auto* b=[MockDelegate new];auto* c=[MockDelegate new];[tools setDelegate:a];[tools didShowFrontend:&front1 docked:YES];[tools setDelegate:nil];[tools setDelegate:b];content::WebContents* replacement=&front2;
  a.onDisplay=^(id value,NSUInteger mode __attribute__((unused))){[value setDelegate:c];[value didShowFrontend:replacement docked:NO];};
  RunOne();Check([a.modes isEqual:@[@3]] && !b.modes.count && !c.modes.count && tasks.count==1,@"reentry nested inline or selected wrong bridge");Drain();Check([c.modes isEqual:@[@1]],@"reentrant display did not use current bridge");[tools checkShown:&front2];a.onDisplay=nil;
 }++cases;
 Check(tasks.count==0 && cases==10,@"test fixture task/count mismatch");
 puts("10 production async display groups passed; bridge ARC/lifetime, modes, state and FIFO reentry; native UI runner/proxy equivalence unverified");
}}
'''
with tempfile.TemporaryDirectory(prefix='webextras-devtools-display-') as temporary:
    folder=Path(temporary); fixture=folder/'display.mm'; executable=folder/'display'
    fixture.write_text(preamble+'\n'+helper+'\n'+interface+'\n'+bodies+'\n'+checks)
    subprocess.run(['xcrun','clang++','-std=c++20','-fobjc-arc','-O2','-Wall','-Wextra','-Werror','-framework','Foundation',str(fixture),'-o',str(executable)],check=True)
    subprocess.run([str(executable)],check=True,timeout=15)
print('Extracted production display/registration SHA256: '+hashlib.sha256((helper+bodies).encode()).hexdigest())
