#!/usr/bin/env python3
"""Exercise actual production close/reset bodies against bounded Foundation mocks.

This tests wrapper sequencing, reentry, weak observation and ARC lifetime only.
It does not load Arc/Chromium or prove docked/hosted/undocked window semantics.
"""
from pathlib import Path
import hashlib
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
source = (ROOT/'arc/api/arc_webextras_devtools.mm').read_text()


def block(text, anchor, semicolon=False):
    start = text.index(anchor)
    opening = text.index('{', start)
    depth = 0
    quote = None
    comment = None
    index = opening
    while index < len(text):
        char, following = text[index], text[index:index+2]
        if comment == '//':
            if char == '\n': comment = None
        elif comment == '/*':
            if following == '*/': comment = None; index += 1
        elif quote:
            if char == '\\': index += 1
            elif char == quote: quote = None
        elif following in ('//', '/*'):
            comment = following; index += 1
        elif char in ('"', "'"):
            quote = char
        elif char == '{':
            depth += 1
        elif char == '}':
            depth -= 1
            if not depth:
                return text[start:index+1] + (';' if semicolon else '')
        index += 1
    raise ValueError('unclosed production block: '+anchor)


implementation = source.split('@implementation ArcWebContentsDevTools\n', 1)[1].split('\n@end', 1)[0]
production = '\n'.join(block(implementation, anchor) for anchor in (
    '- (void)setDelegate:', '- (void)resetFrontendState {', '- (void)notifyFrontendClosed {',
    '- (void)frontendClosed {', '- (void)close {'))
registration = source[source.index('@interface WebextrasDevToolsDelegateBridge :'):source.index('static NSUInteger DevToolsDisplayMode')]
retired = block(source, 'struct FrontendRetiredState {', True)
observer = block(source, 'class FrontendObserver final', True)

preamble = r'''
#import <Foundation/Foundation.h>
#include <functional>
#include <map>
#include <memory>
#include <vector>

static NSMutableArray<NSString*>* events;
static void Check(bool value, NSString* message) { if (!value) { NSLog(@"FAILED: %@ (%@)",message,events); abort(); } }
static void Event(NSString* value) { [events addObject:value]; }
using ProtocolNotification = void (^)(NSString*, NSError*);

namespace base {
template<class T> struct WeakState { T* value; };
template<class T> class WeakPtr {
 public:
  WeakPtr() = default;
  explicit WeakPtr(std::shared_ptr<WeakState<T>> state) : state_(state) {}
  T* get() const { auto state=state_.lock(); return state ? state->value : nullptr; }
  explicit operator bool() const { return get()!=nullptr; }
  T* operator->() const { return get(); }
  void reset() { state_.reset(); }
 private:
  std::weak_ptr<WeakState<T>> state_;
};
}
namespace content {
class WebContentsObserver;
struct Observation { WebContentsObserver* observer; };
static std::vector<std::function<void()>> tasks;
class WebContents {
 public:
  WebContents() : weak_(std::make_shared<base::WeakState<WebContents>>(base::WeakState<WebContents>{this})) {}
  ~WebContents() { weak_->value=nullptr; }
  base::WeakPtr<WebContents> GetWeakPtr() { return base::WeakPtr<WebContents>(weak_); }
  void ClosePage() { Event(@"native-page-close"); if (close) close(); }
  void QueueDestruction();
  std::function<void()> close;
  std::vector<std::weak_ptr<Observation>> observers;
 private:
  std::shared_ptr<base::WeakState<WebContents>> weak_;
};
class WebContentsObserver {
 public:
  explicit WebContentsObserver(WebContents* contents) : token_(std::make_shared<Observation>(Observation{this})) { contents->observers.push_back(token_); }
  virtual ~WebContentsObserver() { token_->observer=nullptr; }
  virtual void WebContentsDestroyed() {}
 private:
  std::shared_ptr<Observation> token_;
};
void WebContents::QueueDestruction() {
  for (auto weak : observers) if (auto token=weak.lock()) tasks.push_back([token] { if (token->observer) token->observer->WebContentsDestroyed(); });
}
static void Drain() { auto pending=std::move(tasks); tasks.clear(); for (auto& task:pending) task(); }
}
class DevToolsUIBindings {
 public:
  class Delegate { public: virtual ~Delegate() = default; virtual void CloseWindow() = 0; };
};
class DevToolsWindow final : public DevToolsUIBindings::Delegate {
 public:
  DevToolsWindow(content::WebContents* inspected, content::WebContents* frontend) : inspected_(inspected),frontend_(frontend) {
    if (inspected) inspectedWindows[inspected]=this;
    if (frontend) frontendWindows[frontend]=this;
  }
  ~DevToolsWindow() override {
    if (inspected_ && inspectedWindows[inspected_]==this) inspectedWindows.erase(inspected_);
    if (frontend_ && frontendWindows[frontend_]==this) frontendWindows.erase(frontend_);
  }
  static DevToolsWindow* GetInstanceForInspectedWebContents(content::WebContents* inspected) {
    auto found=inspectedWindows.find(inspected); return found==inspectedWindows.end() ? nullptr : found->second;
  }
  static DevToolsWindow* AsDevToolsWindow(content::WebContents* frontend) {
    auto found=frontendWindows.find(frontend); return found==frontendWindows.end() ? nullptr : found->second;
  }
  std::function<void()> onClose;
 private:
  // Actual production must call through the typed public base. A direct
  // window->CloseWindow() in the extracted body will fail compilation here.
  void CloseWindow() override { Event(@"native-window-close"); auto callback=onClose; if (callback) callback(); }
  content::WebContents* inspected_;
  content::WebContents* frontend_;
  inline static std::map<content::WebContents*,DevToolsWindow*> inspectedWindows;
  inline static std::map<content::WebContents*,DevToolsWindow*> frontendWindows;
};

@interface ArcWebContents : NSObject {
 @public content::WebContents* native;
 @public NSString* releaseEvent;
}
- (content::WebContents*)webContents;
@end
@implementation ArcWebContents
- (content::WebContents*)webContents { return native; }
- (void)dealloc { if (releaseEvent) Event(releaseEvent); }
@end
@protocol WebExtrasDevToolsDelegate <NSObject>
- (void)webContentsDevToolsDidClose:(id)tools;
@end
@class ArcWebContentsDevTools;
static void FrontendClosed(ArcWebContentsDevTools* owner);
'''
interface = r'''
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
- (instancetype)initForOwner:(ArcWebContents*)owner delegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (void)fixtureFrontend:(content::WebContents*)contents shown:(BOOL)shown opening:(BOOL)opening;
- (void)fixtureQuery:(ProtocolNotification)query toolbox:(ArcWebContents*)toolbox;
- (void)checkFrontend:(content::WebContents*)contents shown:(BOOL)shown opening:(BOOL)opening queries:(NSUInteger)queries;
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (WebextrasDevToolsDelegateBridge*)fixtureBridge;
- (void)close;
- (void)frontendClosed __attribute__((objc_direct));
- (void)resetFrontendState __attribute__((objc_direct));
- (void)notifyFrontendClosed __attribute__((objc_direct));
@end
static void FrontendClosed(ArcWebContentsDevTools* owner) { [owner frontendClosed]; }
@implementation ArcWebContentsDevTools
- (instancetype)initForOwner:(ArcWebContents*)owner delegate:(id<WebExtrasDevToolsDelegate>)delegate {
  if ((self=[super init])) { _owner=owner; [self setDelegate:delegate]; _panelQueries=[NSMutableArray array]; }
  return self;
}
- (void)fixtureFrontend:(content::WebContents*)contents shown:(BOOL)shown opening:(BOOL)opening {
  _frontend=contents ? contents->GetWeakPtr() : base::WeakPtr<content::WebContents>();
  _frontendObserver=contents ? std::make_unique<FrontendObserver>(contents,self) : nullptr;
  _shown=shown; _opening=opening; _requestedPanel=@"network";
}
- (void)fixtureQuery:(ProtocolNotification)query toolbox:(ArcWebContents*)toolbox {
  if (query) [_panelQueries addObject:[query copy]];
  _toolboxWebContents=toolbox;
}
- (void)checkFrontend:(content::WebContents*)contents shown:(BOOL)shown opening:(BOOL)opening queries:(NSUInteger)queries {
  Check(_frontend.get()==contents,@"frontend replaced/cleared incorrectly");
  Check(_shown==shown && _opening==opening,@"shown/opening state changed incorrectly");
  Check(_panelQueries.count==queries,@"replacement query lost");
  if (contents || opening) Check([_requestedPanel isEqual:@"network"],@"replacement requested panel lost");
  else Check(!_requestedPanel,@"retired requested panel kept");
}
- (WebextrasDevToolsDelegateBridge*)fixtureBridge { return _registeredDelegateBridge; }
- (void)dealloc { Event(@"service-release"); }
'''
checks = r'''
@end
@interface MockDelegate : NSObject <WebExtrasDevToolsDelegate>
@property(nonatomic,copy) void (^onClose)(id);
@property(nonatomic) NSUInteger count;
@end
@implementation MockDelegate
- (void)webContentsDevToolsDidClose:(id)tools {
  ++_count; Event(@"did-close");
  if (_onClose) _onClose(tools);
}
@end
@interface CaptureLifetime : NSObject
@end
@implementation CaptureLifetime
- (void)dealloc { Event(@"block-capture-release"); }
@end
static ArcWebContents* Owner(content::WebContents* native) { ArcWebContents* owner=[ArcWebContents new]; owner->native=native; return owner; }
static void Exact(NSArray* expected) { Check([events isEqual:expected],@"unexpected event order"); }
static void SeedRetired(ArcWebContentsDevTools* tools, CaptureLifetime* __weak* capture, ArcWebContents* __weak* toolbox) {
  @autoreleasepool {
    CaptureLifetime* token=[CaptureLifetime new]; *capture=token;
    ArcWebContents* wrapper=[ArcWebContents new]; wrapper->releaseEvent=@"toolbox-release"; *toolbox=wrapper;
    ProtocolNotification query=^(NSString* message __attribute__((unused)), NSError* error __attribute__((unused))) { (void)[token description]; };
    [tools fixtureQuery:query toolbox:wrapper];
  }
}
int main() {
  @autoreleasepool {
    // 1. Typed native close precedes synchronous app notification.
    {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front shown:YES opening:NO]; DevToolsWindow window(&inspected,&front);
      events=[NSMutableArray array]; [tools close]; Exact(@[@"native-window-close",@"did-close"]);
      [tools checkFrontend:nullptr shown:NO opening:NO queries:0]; Check(delegate.count==1,@"close callback count");
    }
    // 2. A queued old observer destruction cannot close a new frontend.
    {
      content::WebContents inspected, front1, front2;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front1 shown:YES opening:NO]; DevToolsWindow window(&inspected,&front1);
      front1.QueueDestruction();
      __weak ArcWebContentsDevTools* weakTools=tools;
      window.onClose=[weakTools,&front1,&front2] {
        auto old=front1.observers.front().lock();
        Check(old && !old->observer,@"retired observer was still armed at native close");
        [weakTools fixtureFrontend:&front2 shown:YES opening:NO];
      };
      events=[NSMutableArray array]; [tools close]; content::Drain();
      [tools checkFrontend:&front2 shown:YES opening:NO queries:0]; Check(delegate.count==1,@"old observer closed replacement");
    }
    // 3. Pending opening has a native window before _frontend is populated.
    {
      content::WebContents inspected;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:nullptr shown:NO opening:YES]; DevToolsWindow window(&inspected,nullptr);
      events=[NSMutableArray array]; [tools close]; Exact(@[@"native-window-close",@"did-close"]);
      [tools checkFrontend:nullptr shown:NO opening:NO queries:0];
    }
    // 4. Native close reenters with a new pending opening and queued query.
    {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front shown:YES opening:NO]; DevToolsWindow window(&inspected,&front);
      __weak ArcWebContentsDevTools* weakTools=tools;
      window.onClose=[weakTools] {
        [weakTools fixtureFrontend:nullptr shown:NO opening:YES];
        [weakTools fixtureQuery:^(NSString* a __attribute__((unused)),NSError* b __attribute__((unused))) {} toolbox:nil];
      };
      events=[NSMutableArray array]; [tools close];
      [tools checkFrontend:nullptr shown:NO opening:YES queries:1]; Check(delegate.count==1,@"native reentry duplicate callback");
    }
    // 5. DidClose app reentry opens F2; no trailing old cleanup may clear it.
    {
      content::WebContents inspected, front1, front2;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front1 shown:YES opening:NO]; DevToolsWindow window(&inspected,&front1);
      content::WebContents* replacement=&front2;
      delegate.onClose=^(id value) { [value fixtureFrontend:replacement shown:YES opening:NO]; };
      events=[NSMutableArray array]; [tools close];
      [tools checkFrontend:&front2 shown:YES opening:NO queries:0]; Check(delegate.count==1,@"app reentry duplicate callback");
      delegate.onClose=nil;
    }
    // 6. Retired query captures/toolbox wrappers survive notification in BOTH paths.
    for (BOOL direct : {NO,YES}) {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front shown:YES opening:NO]; DevToolsWindow window(&inspected,&front);
      __weak CaptureLifetime* capture; __weak ArcWebContents* toolbox;
      SeedRetired(tools,&capture,&toolbox);
      Check(capture && toolbox,@"fixture failed to retain old state");
      window.onClose=[&capture,&toolbox] { Check(capture && toolbox,@"retired state released during native close"); };
      delegate.onClose=^(id value __attribute__((unused))) { Check(capture && toolbox,@"retired state released before DidClose"); };
      events=[NSMutableArray array];
      if (direct) [tools frontendClosed]; else [tools close];
      Check(!capture && !toolbox,@"retired state escaped close call");
      NSUInteger notice=[events indexOfObject:@"did-close"];
      Check(notice!=NSNotFound && [events indexOfObject:@"toolbox-release"]>notice && [events indexOfObject:@"block-capture-release"]>notice,@"retired releases preceded notification");
      delegate.onClose=nil;
    }
    // 7. Last external service owner can disappear during native close safely.
    {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      __weak ArcWebContentsDevTools* weakTools=tools;
      [tools fixtureFrontend:&front shown:YES opening:NO]; DevToolsWindow window(&inspected,&front);
      window.onClose=[&tools] { tools=nil; };
      delegate.onClose=^(id value) { Check(value && weakTools==value,@"service died before close notification"); };
      events=[NSMutableArray array]; [tools close];
      Check(!tools && !weakTools,@"service lifetime escaped close");
      Exact(@[@"native-window-close",@"did-close",@"service-release"]); delegate.onClose=nil;
    }
    // 8. Existing frontend with no native window uses native ClosePage fallback.
    {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front shown:YES opening:NO];
      events=[NSMutableArray array]; [tools close]; Exact(@[@"native-page-close",@"did-close"]);
      [tools checkFrontend:nullptr shown:NO opening:NO queries:0];
    }
    // 9. Fully absent frontend/window is safe and does not invent a notification.
    {
      content::WebContents inspected;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      events=[NSMutableArray array]; [tools close]; Exact(@[]); Check(delegate.count==0,@"absent frontend fabricated callback");
    }
    // 10. Native weak registration can still lock a queued old strong bridge
    // after nil; after that queue releases it, nil has no live registration.
    {
      content::WebContents inspected, front;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* old=[MockDelegate new]; MockDelegate* fresh=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:old];
      DevToolsWindow window(&inspected,&front); NSMutableArray<dispatch_block_t>* queued=[NSMutableArray array];
      __weak WebextrasDevToolsDelegateBridge* weakBridge;
      @autoreleasepool {
        WebextrasDevToolsDelegateBridge* bridge=[tools fixtureBridge]; weakBridge=bridge;
        [queued addObject:^{ (void)[bridge class]; }];
      }
      [tools setDelegate:nil]; [tools fixtureFrontend:&front shown:YES opening:NO];
      events=[NSMutableArray array]; [tools close];
      Check(old.count==1 && fresh.count==0,@"nil hid a still-live native registered bridge");
      [queued removeAllObjects]; Check(!weakBridge,@"old bridge fixture remained retained");
      [tools fixtureFrontend:&front shown:YES opening:NO]; [tools close];
      Check(old.count==1 && fresh.count==0,@"expired nil registration fabricated close callback");
      [tools setDelegate:fresh]; [tools fixtureFrontend:&front shown:YES opening:NO]; [tools close];
      Check(old.count==1 && fresh.count==1,@"new non-nil registration failed close routing");
    }
    Check(content::tasks.empty(),@"queued fixture tasks remain");
    puts("10 production DevTools close/reset cases passed; typed native dispatch, reentry and ARC lifetime tested; real window semantics untested");
  }
}
'''
with tempfile.TemporaryDirectory(prefix='webextras-devtools-close-') as temporary:
    directory = Path(temporary)
    fixture = directory/'close.mm'
    fixture.write_text(preamble+'\n'+registration+'\n'+retired+'\n'+observer+'\n'+interface+'\n'+production+'\n'+checks)
    executable = directory/'close'
    subprocess.run(['xcrun','clang++','-std=c++20','-fobjc-arc','-O2','-Wall','-Wextra','-Werror',
                    '-framework','Foundation',str(fixture),'-o',str(executable)],check=True)
    subprocess.run([str(executable)],check=True,timeout=15)
print('Extracted production close/reset/observer SHA256: '+hashlib.sha256((registration+retired+observer+production).encode()).hexdigest())
