// Copyright 2026 ArcMod.
#import "arc/api/arc_internal.h"

#include <map>
#include <memory>
#include <limits>
#include "base/containers/span.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/location.h"
#include "base/supports_user_data.h"
#include "net/base/auth.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "chrome/browser/devtools/devtools_settings.h"
#include "chrome/browser/devtools/devtools_toggle_action.h"
#include "chrome/browser/devtools/devtools_ui_bindings.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/profiles/profile.h"
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
using SubscriptionNotification = void (^)(NSNumber*, NSString*, NSError*);

@class ArcWebContentsDevTools;

// BEGIN WEBEXTRAS_DEVTOOLS_DISPLAY
// A queued native notification owns its bridge, whose delegate and service stay
// weak. Non-nil delegate replacement updates that same bridge; nil drops only
// the current bridge and does not invalidate already queued bridge captures.
@interface WebextrasDevToolsDelegateBridge : NSObject
@property(nonatomic, weak) id<WebExtrasDevToolsDelegate> delegate;
@property(nonatomic, weak) ArcWebContentsDevTools* owner;
@end
@implementation WebextrasDevToolsDelegateBridge
@synthesize delegate = _delegate, owner = _owner;
@end

static NSUInteger DevToolsDisplayMode(content::WebContents* frontend, bool docked) {
  if (!docked) { return 1; }
  auto* profile = Profile::FromBrowserContext(frontend->GetBrowserContext());
  switch (DevToolsSettings::GetDockSide(profile)) {
    case devtools::DockSide::kLeft: return 2;
    case devtools::DockSide::kRight: return 3;
    case devtools::DockSide::kBottom: return 4;
    case devtools::DockSide::kNone: return 1;
  }
  return 0;
}

static void PostDevToolsDisplay(WebextrasDevToolsDelegateBridge* bridge,
                               base::WeakPtr<content::WebContents> native_lifetime,
                               NSUInteger mode) {
  if (!bridge) { return; }
  // This owner-WebContents anchor approximates the native Impl guard. It is
  // deliberately independent of frontend identity, navigation and close state.
  content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(^{
    if (!native_lifetime) { return; }
    id<WebExtrasDevToolsDelegate> delegate = bridge.delegate;
    ArcWebContentsDevTools* owner = bridge.owner;
    if ([delegate respondsToSelector:@selector(webContentsDevTools:displayAs:)]) {
      [delegate webContentsDevTools:owner displayAs:mode];
    }
  }));
}
// END WEBEXTRAS_DEVTOOLS_DISPLAY

@interface NSObject (WebExtrasDevToolsInternal)
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked;
@end

namespace {
constexpr size_t kMaximumProtocolBytes = 8 * 1024 * 1024;
// Native subscriptions allocate IDs across all DevTools services on the UI sequence.
constinit int g_nextSubscription = 0;
constinit int g_nextCommand = 0;
constinit char kDevToolsBindingKey = 0;
class DevToolsBinding final : public base::SupportsUserData::Data {
 public:
  explicit DevToolsBinding(id value) : owner(value) {}
  __weak id owner;
};
id ToolsForContents(content::WebContents* contents) {
  auto* binding = contents ? static_cast<DevToolsBinding*>(contents->GetUserData(&kDevToolsBindingKey)) : nullptr;
  return binding ? binding->owner : nil;
}
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
void NotifySubscription(SubscriptionNotification callback, NSNumber* identifier,
                        NSString* payload, NSError* error) {
  if (!callback) { return; }
  content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
      base::BindOnce([](SubscriptionNotification callback, NSNumber* identifier,
                        NSString* payload, NSError* error) {
        callback(identifier, payload, error);
      }, callback, identifier, payload, error));
}

class ProtocolClient final : public content::DevToolsAgentHostClient,
                             public std::enable_shared_from_this<ProtocolClient> {
 public:
  explicit ProtocolClient(ProtocolNotification callback, bool post_notifications = true)
      : callback_([callback copy]), post_notifications_(post_notifications) {}
  ~ProtocolClient() override { Close(); }
  bool Attach(content::WebContents* contents) {
    host_ = content::DevToolsAgentHost::GetOrCreateFor(contents);
    attached_ = host_->AttachClient(this);
    if (!attached_) { host_.reset(); }
    return attached_;
  }
  bool is_open() const { return attached_ && !closing_; }
  bool Send(NSString* message, NSError** error) {
    if (!attached_ || closing_) { return DevToolsFail(error, 10, @"DevTools connection closed"); }
    const std::string bytes = ArcUTF8(message);
    if (post_notifications_ && bytes.size() > kMaximumProtocolBytes) {
      return DevToolsFail(error, 10, @"DevTools message exceeds 8 MiB");
    }
    auto envelope = base::JSONReader::ReadDict(bytes, base::JSON_PARSE_RFC, 200);
    if (!envelope || !envelope->FindInt("id") || !envelope->FindString("method")) {
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
    callback_ = nil;
  }
  void DispatchProtocolMessage(content::DevToolsAgentHost*,
                               base::span<const uint8_t> bytes) override {
    if (closing_) { return; }
    if (post_notifications_ && bytes.size() > kMaximumProtocolBytes) {
      closing_ = true;
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce([](std::shared_ptr<ProtocolClient> client) { client->Close(); }, shared_from_this()));
      return;
    }
    NSString* message = [[NSString alloc] initWithBytes:bytes.data() length:bytes.size()
                                             encoding:NSUTF8StringEncoding];
    if (post_notifications_) { Notify(callback_, message, nil); }
    else if (callback_) {
      // A synchronous command completion may release the owner/client.
      auto keep_alive = shared_from_this();
      callback_(message, nil);
    }
  }
  void AgentHostClosed(content::DevToolsAgentHost*) override {
    if (!attached_) { return; }
    auto keep_alive = shared_from_this();
    attached_ = false;
    host_.reset();
    Notify(callback_, nil, DevToolsError(10, @"DevTools connection closed"));
    callback_ = nil;
  }
 private:
  scoped_refptr<content::DevToolsAgentHost> host_;
  ProtocolNotification callback_;
  const bool post_notifications_;
  bool attached_ = false;
  bool closing_ = false;
};

// Keep retired blocks/wrappers alive until state reset and close notification
// finish. Releasing them during reset could reenter the service halfway through.
struct FrontendRetiredState {
  ArcWebContents* toolbox;
  NSArray<ProtocolNotification>* panel_queries;
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
          if (!callback) { return; }
          if (result.is_string()) { callback(ArcNSString(result.GetString()), nil); }
          else { callback(nil, DevToolsError(3, @"panelName not found")); }
        }, completion));
  }
}
}  // namespace

ARC_EXPORT @interface ArcWebContentsDevTools : NSObject {
  __weak ArcWebContents* _owner;
  __weak id<WebExtrasDevToolsDelegate> _delegate;
  WebextrasDevToolsDelegateBridge* _delegateBridge;
  __weak WebextrasDevToolsDelegateBridge* _registeredDelegateBridge;
  std::map<int, std::shared_ptr<ProtocolClient>> _connections;
  std::shared_ptr<ProtocolClient> _commands;
  NSMutableDictionary<NSNumber*, ProtocolNotification>* _pending;
  NSMutableDictionary<NSNumber*, NSDictionary*>* _subscriptions;
  int _nextConnection;
  ArcWebContents* _toolboxWebContents;
  base::WeakPtr<content::WebContents> _frontend;
  std::unique_ptr<FrontendObserver> _frontendObserver;
  NSMutableArray<ProtocolNotification>* _panelQueries;
  NSString* _requestedPanel;
  BOOL _opening;
  BOOL _shown;
}
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
@property(nonatomic, readonly) ArcWebContents* toolboxWebContents;
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (void)receiveMessage:(NSString*)message error:(NSError*)error __attribute__((objc_direct));
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked;
- (void)frontendClosed __attribute__((objc_direct));
- (void)resetFrontendState __attribute__((objc_direct));
- (void)notifyFrontendClosed __attribute__((objc_direct));
- (BOOL)ensureCommandClient:(NSError**)error __attribute__((objc_direct));
- (BOOL)sendCommand:(NSString*)method params:(const base::DictValue&)params
        completion:(ProtocolNotification)completion error:(NSError**)error __attribute__((objc_direct));
@end

namespace {
void FrontendClosed(ArcWebContentsDevTools* owner) { [owner frontendClosed]; }
}  // namespace

@implementation ArcWebContentsDevTools
@synthesize toolboxWebContents = _toolboxWebContents;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) {
    _owner = owner;
    _pending = [NSMutableDictionary dictionary];
    _subscriptions = [NSMutableDictionary dictionary];
    _panelQueries = [NSMutableArray array];
    if (auto* contents = [owner webContents]) {
      contents->SetUserData(&kDevToolsBindingKey, std::make_unique<DevToolsBinding>(self));
    }
    arcmod::GetDevToolsShowHook() = base::BindRepeating(
        [](content::WebContents* inspected, content::WebContents* frontend, bool docked) {
          id tools = ToolsForContents(inspected);
          [tools didShowFrontend:frontend docked:docked];
        });
  }
  return self;
}
- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate {
  _delegate = delegate;
  if (!delegate) { _delegateBridge = nil; return; }
  if (!_delegateBridge) {
    _delegateBridge = [WebextrasDevToolsDelegateBridge new];
    _delegateBridge.owner = self;
    _registeredDelegateBridge = _delegateBridge;
  }
  _delegateBridge.delegate = delegate;
}
- (ArcWebContents*)webContents {
  return _frontend ? [ArcWebContents wrapperForWebContents:_frontend.get()] : nil;
}
- (ArcWebContents*)toolboxWebContents { return _toolboxWebContents; }
- (void)didShowFrontend:(content::WebContents*)frontend docked:(BOOL)docked {
  const NSUInteger mode = DevToolsDisplayMode(frontend, docked);
  _opening = NO;
  _shown = YES;
  _frontend = frontend->GetWeakPtr();
  frontend->SetUserData(&kDevToolsBindingKey, std::make_unique<DevToolsBinding>(self));
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
  auto* contents = [_owner webContents];
  PostDevToolsDisplay(_registeredDelegateBridge,
      contents ? contents->GetWeakPtr() : base::WeakPtr<content::WebContents>(), mode);
}
- (void)resetFrontendState {
  _toolboxWebContents = nil;
  _frontend.reset();
  _opening = NO;
  _shown = NO;
  _requestedPanel = nil;
  _panelQueries = [NSMutableArray array];
  // A retired frontend's delayed destruction must not close a replacement.
  _frontendObserver.reset();
}
- (void)notifyFrontendClosed {
  WebextrasDevToolsDelegateBridge* bridge = _registeredDelegateBridge;
  id<WebExtrasDevToolsDelegate> delegate = bridge.delegate;
  ArcWebContentsDevTools* owner = bridge.owner;
  if ([delegate respondsToSelector:@selector(webContentsDevToolsDidClose:)]) {
    [delegate webContentsDevToolsDidClose:owner];
  }
}
- (void)frontendClosed {
  const BOOL notify = _shown;
  [[maybe_unused]] FrontendRetiredState retired{_toolboxWebContents, _panelQueries};
  [self resetFrontendState];
  if (notify) { [self notifyFrontendClosed]; }
}
- (void)openWithAction:(const DevToolsToggleAction&)action {
  auto* contents = [_owner webContents];
  if (!contents) { return; }
  auto weakContents = contents->GetWeakPtr();
  DevToolsWindow::OpenDevToolsWindow(contents, action, DevToolsOpenedByAction::kUnknown);
  // Display delegates may destroy the inspected contents while opening the UI.
  _opening = !_shown && weakContents &&
      DevToolsWindow::GetInstanceForInspectedWebContents(weakContents.get());
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
  ArcWebContentsDevTools* retainedSelf = self;
  base::WeakPtr<content::WebContents> frontend = _frontend;
  auto* contents = [_owner webContents];
  auto* window = frontend ? DevToolsWindow::AsDevToolsWindow(frontend.get()) : nullptr;
  if (!window && contents) { window = DevToolsWindow::GetInstanceForInspectedWebContents(contents); }
  const BOOL notify = _shown || window;
  [[maybe_unused]] FrontendRetiredState retired{_toolboxWebContents, _panelQueries};
  [self resetFrontendState];
  // Use the public delegate interface: docked close enters Chromium's normal
  // before-unload path. Native close precedes the synchronous app notification.
  if (window) { static_cast<DevToolsUIBindings::Delegate*>(window)->CloseWindow(); }
  else if (frontend) { frontend->ClosePage(); }
  if (notify) { [retainedSelf notifyFrontendClosed]; }
  // No state cleanup here: native/app reentry may already have opened a new UI.
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
    DevToolsFail(error, 10, @"Associated WebContents is not available");
    return nil;
  }
  auto client = std::make_shared<ProtocolClient>(notification);
  if (!client->Attach(contents)) {
    DevToolsFail(error, 10, @"Unable to attach DevTools client");
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
- (void)closeProtocolConnection:(NSNumber*)identifier {
  auto found = _connections.find(identifier.intValue);
  if (found == _connections.end()) { return; }
  auto client = found->second;
  _connections.erase(found);
  // A pending oversized-message task can still own the client. Detach now,
  // rather than relying on its eventual destruction to close the connection.
  client->Close();
}
- (BOOL)ensureCommandClient:(NSError**)error {
  if (_commands) { return YES; }
  auto* contents = [_owner webContents];
  if (!contents) { return DevToolsFail(error, 10, @"Associated WebContents is not available."); }
  __weak ArcWebContentsDevTools* weakSelf = self;
  auto client = std::make_shared<ProtocolClient>(^(NSString* message, NSError* failure) {
    [weakSelf receiveMessage:message error:failure];
  }, false);
  if (!client->Attach(contents)) { return DevToolsFail(error, 10, @"Failed to attach DevTools client"); }
  _commands = std::move(client);
  return YES;
}
- (BOOL)sendCommand:(NSString*)method params:(const base::DictValue&)params
        completion:(ProtocolNotification)completion error:(NSError**)error {
  if (![self ensureCommandClient:error]) { return NO; }
  if (!_commands->is_open()) { return DevToolsFail(error, 10, @"DevTools agent host closed"); }
  NSNumber* identifier = @(++g_nextCommand);
  base::DictValue envelope;
  envelope.Set("id", identifier.intValue);
  envelope.Set("method", ArcUTF8(method));
  envelope.Set("params", params.Clone());
  auto data = base::WriteJson(envelope);
  if (completion) { _pending[identifier] = [completion copy]; }
  auto client = _commands;
  if (!client->Send(data ? ArcNSString(*data) : nil, error)) {
    [_pending removeObjectForKey:identifier];
    return NO;
  }
  return YES;
}
- (BOOL)execute:(id<ArcDevToolsProtocolCommand>)command completion:(ProtocolNotification)completion error:(NSError**)error {
  NSString* domain = [command.domain copy];
  NSString* name = [command.name copy];
  NSString* text = [command.params copy];
  if (![_owner webContents]) { return DevToolsFail(error, 10, @"Associated WebContents is not available."); }
  auto params = base::JSONReader::ReadDict(ArcUTF8(text), base::JSON_PARSE_CHROMIUM_EXTENSIONS, 200);
  if (!params) { return DevToolsFail(error, 2, @"Malformed params."); }
  return [self sendCommand:[NSString stringWithFormat:@"%@.%@",domain ?: @"",name ?: @""]
                   params:*params completion:completion error:error];
}
- (NSNumber*)subscribe:(id<ArcDevToolsProtocolEvent>)event autoEnable:(BOOL)enable notification:(SubscriptionNotification)notification {
  NSString* domain = [event.domain copy] ?: @"";
  NSString* name = [event.name copy] ?: @"";
  if (![self ensureCommandClient:nil]) { return nil; }
  NSNumber* identifier = @(++g_nextSubscription);
  NSMutableDictionary* subscription = [@{@"method":[NSString stringWithFormat:@"%@.%@",domain,name],
                                         @"domain":domain, @"identifier":identifier} mutableCopy];
  if (notification) { subscription[@"callback"] = [notification copy]; }
  _subscriptions[identifier] = subscription;
  if (enable) {
    [self sendCommand:[domain stringByAppendingString:@".enable"] params:base::DictValue() completion:nil error:nil];
  }
  return identifier;
}
- (BOOL)unsubscribe:(NSNumber*)identifier {
  NSDictionary* removed = _subscriptions[identifier];
  if (!removed) { return NO; }
  [_subscriptions removeObjectForKey:identifier];
  BOOL stillSubscribed = NO;
  for (NSDictionary* other in _subscriptions.allValues) {
    if ([other[@"method"] isEqual:removed[@"method"]]) { stillSubscribed = YES; }
  }
  if (!stillSubscribed) {
    [self sendCommand:[removed[@"domain"] stringByAppendingString:@".disable"] params:base::DictValue() completion:nil error:nil];
  }
  return YES;
}
- (void)receiveMessage:(NSString*)message error:(NSError*)error {
  // The native command router ignores the empty envelope delivered when its
  // transport closes. It keeps pending completions and never reconnects here.
  if (error) { return; }
  auto envelope = base::JSONReader::ReadDict(ArcUTF8(message), base::JSON_PARSE_CHROMIUM_EXTENSIONS, 200);
  if (!envelope) { return; }
  auto commandID = envelope->FindInt("id");
  if (commandID) {
    NSNumber* identifier = @(*commandID);
    ProtocolNotification callback = _pending[identifier];
    [_pending removeObjectForKey:identifier];
    if (!callback) { return; }
    if (auto* result = envelope->FindDict("result")) {
      auto bytes = base::WriteJson(*result);
      callback(bytes ? ArcNSString(*bytes) : nil, nil);
    } else if (auto* failure = envelope->FindDict("error")) {
      auto bytes = base::WriteJson(*failure);
      callback(nil, DevToolsError(7, bytes ? ArcNSString(*bytes) : @""));
    } else { callback(nil, DevToolsError(1, @"Missing the `result` or `error` field")); }
  } else {
    NSString* payload = nil;
    NSError* failure = nil;
    if (auto* params = envelope->FindDict("params")) {
      auto bytes = base::WriteJson(*params);
      payload = bytes ? ArcNSString(*bytes) : nil;
    } else {
      failure = DevToolsError(3, @"Missing the `params`");
    }
    NSArray<NSNumber*>* identifiers = [_subscriptions.allKeys sortedArrayUsingSelector:@selector(compare:)];
    for (NSNumber* identifier in identifiers) {
      NSDictionary* subscription = _subscriptions[identifier];
      auto* method = envelope->FindString("method");
      if (method && [subscription[@"method"] isEqual:ArcNSString(*method)]) {
        SubscriptionNotification callback = subscription[@"callback"];
        NotifySubscription(callback, subscription[@"identifier"], payload, failure);
      }
    }
  }
}
@end

ARC_EXPORT @interface ArcBrowserContextDevTools : NSObject {
  __weak ArcBrowserContext* _context;
  base::WeakPtr<content::WebContents> _nativeContents;
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
  if ((self = [super init]) && native) {
    _nativeContents = *static_cast<const base::WeakPtr<content::WebContents>*>(native);
  }
  return self;
}
- (ArcWebContents*)webContents {
  return _nativeContents ? [ArcWebContents wrapperForWebContents:_nativeContents.get()] : nil;
}
- (void)close { if (_nativeContents) { _nativeContents->ClosePage(); } }
@end
