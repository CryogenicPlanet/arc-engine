// Copyright 2026 ArcMod.
#import "arc/api/arc_internal.h"

#include <map>
#include <memory>
#include "base/containers/span.h"
#include "base/functional/bind.h"
#include "base/location.h"
#include "net/base/auth.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "chrome/browser/devtools/devtools_toggle_action.h"
#include "chrome/browser/devtools/devtools_ui_bindings.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/devtools_agent_host.h"
#include "content/public/browser/devtools_agent_host_client.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"

@protocol ArcDevToolsProtocolCommand
- (NSString*)domain;
- (NSString*)name;
- (NSString*)params;
@end
@protocol ArcDevToolsProtocolEvent
- (NSString*)domain;
- (NSString*)name;
@end
@protocol WebExtrasDevToolsDelegate <NSObject>
- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;
- (void)webContentsDevToolsDidClose:(id)tools;
- (void)webContentsDevToolsNeedsFocus:(id)tools;
- (void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;
@end

using ProtocolNotification = void (^)(NSString*, NSError*);

@class ArcWebContentsDevTools;
@interface NSObject (WebExtrasDevToolsInternal)
- (id)devTools;
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked;
@end

namespace {
constexpr size_t kMaximumProtocolBytes = 8 * 1024 * 1024;
NSError* DevToolsError(NSInteger code, NSString* text) {
  return [NSError errorWithDomain:@"company.thebrowser.Arc" code:code
                        userInfo:@{NSLocalizedDescriptionKey:text}];
}
BOOL DevToolsFail(NSError** out, NSInteger code, NSString* text) {
  if (out) { *out = DevToolsError(code, text); }
  return NO;
}
void Notify(ProtocolNotification callback, NSString* message, NSError* error) {
  if (!callback) { return; }
  // Copy the callable into the task. Closing/unsubscribing cannot invalidate
  // notifications which were already queued, as in Arc's native transport.
  content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
      base::BindOnce([](ProtocolNotification callback, NSString* message, NSError* error) {
        callback(message, error);
      }, callback, message, error));
}

class ProtocolClient final : public content::DevToolsAgentHostClient,
                             public std::enable_shared_from_this<ProtocolClient> {
 public:
  explicit ProtocolClient(ProtocolNotification callback) : callback_([callback copy]) {}
  ~ProtocolClient() override { Close(); }
  bool Attach(content::WebContents* contents) {
    host_ = content::DevToolsAgentHost::GetOrCreateFor(contents);
    attached_ = host_->AttachClient(this);
    if (!attached_) { host_.reset(); }
    return attached_;
  }
  bool Send(NSString* message, NSError** error) {
    if (!attached_ || closing_) { return DevToolsFail(error, 10, @"DevTools connection closed"); }
    const std::string bytes = ArcUTF8(message);
    if (bytes.size() > kMaximumProtocolBytes) {
      return DevToolsFail(error, 10, @"DevTools message exceeds 8 MiB");
    }
    NSData* data = [message dataUsingEncoding:NSUTF8StringEncoding];
    id envelope = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:nil] : nil;
    if (![envelope isKindOfClass:NSDictionary.class] ||
        ![envelope[@"id"] isKindOfClass:NSNumber.class] ||
        ![envelope[@"method"] isKindOfClass:NSString.class]) {
      return DevToolsFail(error, 2, @"Invalid CDP command envelope");
    }
    host_->DispatchProtocolMessage(this, base::as_byte_span(bytes));
    return true;
  }
  void Close() {
    if (!attached_) { return; }
    attached_ = false;
    host_->DetachClient(this);
    host_.reset();
    Notify(callback_, nil, DevToolsError(10, @"DevTools connection closed"));
  }
  void DispatchProtocolMessage(content::DevToolsAgentHost*,
                               base::span<const uint8_t> bytes) override {
    if (closing_) { return; }
    if (bytes.size() > kMaximumProtocolBytes) {
      closing_ = true;
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce([](std::shared_ptr<ProtocolClient> client) { client->Close(); }, shared_from_this()));
      return;
    }
    NSString* message = [[NSString alloc] initWithBytes:bytes.data() length:bytes.size()
                                             encoding:NSUTF8StringEncoding];
    Notify(callback_, message, nil);
  }
  void AgentHostClosed(content::DevToolsAgentHost*) override {
    if (!attached_) { return; }
    attached_ = false;
    host_.reset();
    Notify(callback_, nil, DevToolsError(10, @"DevTools connection closed"));
  }
 private:
  scoped_refptr<content::DevToolsAgentHost> host_;
  ProtocolNotification callback_;
  bool attached_ = false;
  bool closing_ = false;
};

void FrontendClosed(ArcWebContentsDevTools* owner);

class FrontendObserver final : public content::WebContentsObserver {
 public:
  FrontendObserver(content::WebContents* contents, ArcWebContentsDevTools* owner)
      : content::WebContentsObserver(contents), owner_(owner) {}
  void WebContentsDestroyed() override { FrontendClosed(owner_); }
 private:
  __weak ArcWebContentsDevTools* owner_;
};

void QueryPanel(content::WebContents* frontend, ProtocolNotification completion) {
  if (auto* bindings = DevToolsUIBindings::ForWebContents(frontend)) {
    bindings->CallClientMethod("DevToolsAPI", "currentPanel", {}, {}, {},
        base::BindOnce([](ProtocolNotification callback, base::Value result) {
          if (callback) { callback(result.is_string() ? ArcNSString(result.GetString()) : nil, nil); }
        }, completion));
  }
}
}  // namespace

ARC_EXPORT @interface ArcWebContentsDevTools : NSObject {
  __weak ArcWebContents* _owner;
  __weak id<WebExtrasDevToolsDelegate> _delegate;
  std::map<int, std::shared_ptr<ProtocolClient>> _connections;
  std::shared_ptr<ProtocolClient> _commands;
  NSMutableDictionary<NSNumber*, ProtocolNotification>* _pending;
  NSMutableDictionary<NSNumber*, NSDictionary*>* _subscriptions;
  int _nextConnection;
  int _nextCommand;
  int _nextSubscription;
  __weak ArcWebContents* _toolbox;
  base::WeakPtr<content::WebContents> _frontend;
  std::unique_ptr<FrontendObserver> _frontendObserver;
  NSMutableArray<ProtocolNotification>* _panelQueries;
  NSString* _requestedPanel;
  BOOL _opening;
  BOOL _shown;
}
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (void)receiveMessage:(NSString*)message error:(NSError*)error __attribute__((objc_direct));
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked;
- (void)frontendClosed __attribute__((objc_direct));
- (BOOL)ensureCommandClient:(NSError**)error __attribute__((objc_direct));
- (BOOL)sendCommand:(NSString*)method params:(NSDictionary*)params
        completion:(ProtocolNotification)completion error:(NSError**)error __attribute__((objc_direct));
@end

namespace {
void FrontendClosed(ArcWebContentsDevTools* owner) { [owner frontendClosed]; }
}  // namespace

@implementation ArcWebContentsDevTools
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) {
    _owner = owner;
    _pending = [NSMutableDictionary dictionary];
    _subscriptions = [NSMutableDictionary dictionary];
    _panelQueries = [NSMutableArray array];
    arcmod::GetDevToolsShowHook() = base::BindRepeating(
        [](content::WebContents* inspected, content::WebContents* frontend, bool docked) {
          id tools = [[ArcWebContents wrapperForWebContents:inspected] devTools];
          [tools didShowFrontend:frontend docked:docked];
        });
  }
  return self;
}
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate { _delegate = delegate; }
- (ArcWebContents*)webContents {
  return _frontend ? [ArcWebContents wrapperForWebContents:_frontend.get()] : nil;
}
- (ArcWebContents*)toolboxWebContents { return _toolbox; }
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked {
  _opening = NO;
  _shown = YES;
  _frontend = frontend->GetWeakPtr();
  _frontendObserver = std::make_unique<FrontendObserver>(frontend, self);
  if (_requestedPanel) {
    if (auto* bindings = DevToolsUIBindings::ForWebContents(frontend)) {
      bindings->CallClientMethod("DevToolsAPI", "showPanel", base::Value(ArcUTF8(_requestedPanel)));
    }
    _requestedPanel = nil;
  }
  NSArray* queries = [_panelQueries copy];
  [_panelQueries removeAllObjects];
  for (ProtocolNotification callback in queries) { QueryPanel(frontend, callback); }
  id<WebExtrasDevToolsDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(webContentsDevTools:displayAs:)]) {
    [delegate webContentsDevTools:self displayAs:docked ? 3 : 4];
  }
}
- (void)frontendClosed {
  _toolbox = nil;
  _frontend.reset();
  _opening = NO;
  [_panelQueries removeAllObjects];
  if (!_shown) { return; }
  _shown = NO;
  id<WebExtrasDevToolsDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(webContentsDevToolsDidClose:)]) {
    [delegate webContentsDevToolsDidClose:self];
  }
}
- (void)openWithAction:(const DevToolsToggleAction&)action {
  auto* contents = [_owner webContents];
  if (!contents) { return; }
  DevToolsWindow::OpenDevToolsWindow(contents, action, DevToolsOpenedByAction::kUnknown);
  _opening = !_shown && DevToolsWindow::GetInstanceForInspectedWebContents(contents);
}
- (void)open { [self openWithAction:DevToolsToggleAction::ShowElementsPanel()]; }
- (void)openInspectView { [self openWithAction:DevToolsToggleAction::Inspect()]; }
- (void)openConsoleView { [self openWithAction:DevToolsToggleAction::ShowConsolePanel()]; }
- (void)openNetworkView {
  _requestedPanel = @"network";
  [self openWithAction:DevToolsToggleAction::Show()];
  auto* frontend = _frontend.get();
  if (frontend) {
    if (auto* bindings = DevToolsUIBindings::ForWebContents(frontend)) {
      bindings->CallClientMethod("DevToolsAPI", "showPanel", base::Value("network"));
      _requestedPanel = nil;
    }
  }
}
- (void)inspectElementAt:(NSPoint)point {
  auto* contents = [_owner webContents];
  if (contents) {
    DevToolsWindow::InspectElement(contents->GetPrimaryMainFrame(), point.x, point.y);
  }
}
- (void)close {
  base::WeakPtr<content::WebContents> frontend = _frontend;
  [self frontendClosed];
  if (frontend) { frontend->ClosePage(); }
}
- (BOOL)currentPanel:(ProtocolNotification)completion error:(NSError**)error {
  auto* frontend = _frontend.get();
  auto* bindings = frontend ? DevToolsUIBindings::ForWebContents(frontend) : nullptr;
  if (!bindings) {
    if (_opening) {
      if (completion) { [_panelQueries addObject:[completion copy]]; }
      return YES;
    }
    return DevToolsFail(error, 10, @"DevTools window not open");
  }
  QueryPanel(frontend, completion);
  return YES;
}
- (NSNumber*)openProtocolConnection:(ProtocolNotification)notification error:(NSError**)error {
  auto* contents = [_owner webContents];
  if (!contents) {
    DevToolsFail(error, 10, @"Associated WebContents is not available.");
    return nil;
  }
  auto client = std::make_shared<ProtocolClient>(notification);
  if (!client->Attach(contents)) {
    DevToolsFail(error, 10, @"Failed to attach DevTools client");
    return nil;
  }
  int identifier = ++_nextConnection;
  _connections.emplace(identifier, std::move(client));
  return @(identifier);
}
- (BOOL)sendProtocolMessage:(NSString*)message connectionID:(NSNumber*)identifier error:(NSError**)error {
  auto found = _connections.find(identifier.intValue);
  if (found == _connections.end()) { return DevToolsFail(error, 3, @"Unknown DevTools connection"); }
  auto client = found->second;
  return client->Send(message, error);
}
- (void)closeProtocolConnection:(NSNumber*)identifier { _connections.erase(identifier.intValue); }
- (BOOL)ensureCommandClient:(NSError**)error {
  if (_commands) { return YES; }
  auto* contents = [_owner webContents];
  if (!contents) { return DevToolsFail(error, 10, @"Associated WebContents is not available."); }
  __weak ArcWebContentsDevTools* weakSelf = self;
  auto client = std::make_shared<ProtocolClient>(^(NSString* message, NSError* failure) {
    [weakSelf receiveMessage:message error:failure];
  });
  if (!client->Attach(contents)) { return DevToolsFail(error, 10, @"Failed to attach DevTools client"); }
  _commands = std::move(client);
  return YES;
}
- (BOOL)sendCommand:(NSString*)method params:(NSDictionary*)params
        completion:(ProtocolNotification)completion error:(NSError**)error {
  if (![self ensureCommandClient:error]) { return NO; }
  NSNumber* identifier = @(++_nextCommand);
  NSData* data = [NSJSONSerialization dataWithJSONObject:@{@"id":identifier, @"method":method, @"params":params}
                                                options:0 error:nil];
  if (completion) { _pending[identifier] = [completion copy]; }
  auto client = _commands;
  if (!client->Send([[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding], error)) {
    [_pending removeObjectForKey:identifier];
    return NO;
  }
  return YES;
}
- (BOOL)execute:(id<ArcDevToolsProtocolCommand>)command completion:(ProtocolNotification)completion error:(NSError**)error {
  NSString* text = command.params;
  NSData* data = [text dataUsingEncoding:NSUTF8StringEncoding];
  id params = data ? [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil] : nil;
  if (![params isKindOfClass:NSDictionary.class]) { return DevToolsFail(error, 2, @"Malformed params."); }
  return [self sendCommand:[NSString stringWithFormat:@"%@.%@",command.domain ?: @"",command.name ?: @""]
                   params:params completion:completion error:error];
}
- (NSNumber*)subscribe:(id<ArcDevToolsProtocolEvent>)event autoEnable:(BOOL)enable notification:(ProtocolNotification)notification {
  if (![self ensureCommandClient:nil]) { return nil; }
  NSString* domain = event.domain ?: @"";
  NSNumber* identifier = @(++_nextSubscription);
  NSMutableDictionary* subscription = [@{@"method":[NSString stringWithFormat:@"%@.%@",domain,event.name ?: @""],
                                         @"domain":domain, @"enable":@(enable)} mutableCopy];
  if (notification) { subscription[@"callback"] = [notification copy]; }
  _subscriptions[identifier] = subscription;
  if (enable) {
    BOOL alreadyEnabled = NO;
    for (NSNumber* key in _subscriptions) {
      NSDictionary* other = _subscriptions[key];
      if (![key isEqual:identifier] && [other[@"domain"] isEqual:domain] && [other[@"enable"] boolValue]) {
        alreadyEnabled = YES;
      }
    }
    if (!alreadyEnabled) { [self sendCommand:[domain stringByAppendingString:@".enable"] params:@{} completion:nil error:nil]; }
  }
  return identifier;
}
- (BOOL)unsubscribe:(NSNumber*)identifier {
  NSDictionary* removed = _subscriptions[identifier];
  if (!removed) { return NO; }
  [_subscriptions removeObjectForKey:identifier];
  if ([removed[@"enable"] boolValue]) {
    BOOL stillEnabled = NO;
    for (NSDictionary* other in _subscriptions.allValues) {
      if ([other[@"domain"] isEqual:removed[@"domain"]] && [other[@"enable"] boolValue]) { stillEnabled = YES; }
    }
    if (!stillEnabled) {
      [self sendCommand:[removed[@"domain"] stringByAppendingString:@".disable"] params:@{} completion:nil error:nil];
    }
  }
  return YES;
}
- (void)receiveMessage:(NSString*)message error:(NSError*)error {
  if (error) {
    NSArray* callbacks = _pending.allValues;
    [_pending removeAllObjects];
    _commands.reset();
    for (ProtocolNotification callback in callbacks) { callback(nil, error); }
    return;
  }
  NSData* data = [message dataUsingEncoding:NSUTF8StringEncoding];
  id envelope = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:nil] : nil;
  if (![envelope isKindOfClass:NSDictionary.class]) { return; }
  NSNumber* identifier = envelope[@"id"];
  if (identifier) {
    ProtocolNotification callback = _pending[identifier];
    [_pending removeObjectForKey:identifier];
    if (!callback) { return; }
    if (envelope[@"error"]) {
      id failure = envelope[@"error"];
      callback(nil, DevToolsError(10, [failure isKindOfClass:NSDictionary.class] ? failure[@"message"] ?: @"DevTools command failed" : @"DevTools command failed"));
    } else {
      id result = envelope[@"result"] ?: @{};
      NSData* bytes = [NSJSONSerialization dataWithJSONObject:result options:NSJSONWritingFragmentsAllowed error:nil];
      callback([[NSString alloc] initWithData:bytes encoding:NSUTF8StringEncoding], nil);
    }
  } else {
    NSArray* subscriptions = _subscriptions.allValues;
    for (NSDictionary* subscription in subscriptions) {
      if ([subscription[@"method"] isEqual:envelope[@"method"]]) {
        ProtocolNotification callback = subscription[@"callback"];
        if (callback) { callback(message, nil); }
      }
    }
  }
}
@end

ARC_EXPORT @interface ArcBrowserContextDevTools : NSObject {
  __weak ArcBrowserContext* _context;
  __weak ArcWebContents* _contents;
}
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (instancetype)initWithBrowserContextDevTools:(const void*)native;
@end
@implementation ArcBrowserContextDevTools
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) { _context = context; }
  return self;
}
- (instancetype)initWithBrowserContextDevTools:(const void*)native {
  return [self initWithArcBrowserContext:nil];
}
- (ArcWebContents*)webContents { return _contents; }
- (void)close { if (auto* contents = [_contents webContents]) { contents->ClosePage(); } }
@end
