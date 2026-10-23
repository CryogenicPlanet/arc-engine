// Copyright 2026 ArcMod. Browser identity, creation parameters and Arc-owned windows.

#import "arc/api/arc_internal.h"

#include "arc/browser/arc_browser_window.h"
#include "arc/browser/extensions_bridge.h"
#import <objc/message.h>
#include "base/check.h"
#include "base/memory/weak_ptr.h"
#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "base/location.h"
#include "base/task/single_thread_task_runner.h"
#include <memory>
#include <vector>
#include "base/scoped_observation.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/media/router/media_router_feature.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/web_modal/web_contents_modal_dialog_manager.h"
#include "chrome/browser/ui/browser_init_state.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/create_browser_window.h"
#include "components/sessions/core/session_id.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include <optional>

@implementation ArcSessionID {
  std::optional<int32_t> _identifier;
}
- (instancetype)initWithID:(NSInteger)identifier {
  if ((self = [super init])) {
    _identifier = static_cast<int32_t>(identifier);
  }
  return self;
}
- (instancetype)initWithSessionId:(const SessionID*)identifier {
  if ((self = [super init])) {
    _identifier = identifier->id();
  }
  return self;
}
- (NSInteger)identifier {
  CHECK(_identifier);
  return *_identifier;
}
- (NSUInteger)hash {
  CHECK(_identifier);
  return static_cast<NSUInteger>(static_cast<NSInteger>(*_identifier));
}
- (BOOL)isEqual:(id)other {
  return self == other || ([other isKindOfClass:[ArcSessionID class]] &&
                           _identifier == ((ArcSessionID*)other)->_identifier);
}
@end

ARC_EXPORT @interface ArcBrowserCreateParams : NSObject
- (instancetype)initWithBrowserCreateParams:(const void*)params;
@property(nonatomic, readonly) NSString* userTitle;
@property(nonatomic, readonly) BOOL canResize;
@property(nonatomic, readonly) BOOL canMaximize;
@property(nonatomic, readonly) BOOL canFullscreen;
@property(nonatomic, readonly) CGRect initialBounds;
@property(nonatomic, readonly) BOOL initialOriginSpecified;
@property(nonatomic, readonly) CGSize pipSize;
@end

@implementation ArcBrowserCreateParams {
  NSString* _userTitle;
  BOOL _canResize;
  BOOL _canMaximize;
  BOOL _canFullscreen;
  CGRect _initialBounds;
  BOOL _initialOriginSpecified;
  CGSize _pipSize;
}
- (instancetype)initWithBrowserCreateParams:(const void*)params {
  if ((self = [super init])) {
    const auto& native = *static_cast<const BrowserWindowCreateParams*>(params);
    _userTitle = ArcNSString(native.user_title);
    _canResize = native.can_resize;
    _canMaximize = native.can_maximize;
    _canFullscreen = native.can_fullscreen;
    _initialBounds = CGRectMake(native.initial_bounds.x(), native.initial_bounds.y(),
                                native.initial_bounds.width(), native.initial_bounds.height());
    _initialOriginSpecified = native.initial_origin_specified ==
                             BrowserWindowCreateParams::ValueSpecified::kSpecified;
    if (native.pip_options) {
      _pipSize = CGSizeMake(native.pip_options->width, native.pip_options->height);
    }
  }
  return self;
}
- (NSString*)userTitle { return _userTitle; }
- (BOOL)canResize { return _canResize; }
- (BOOL)canMaximize { return _canMaximize; }
- (BOOL)canFullscreen { return _canFullscreen; }
- (CGRect)initialBounds { return _initialBounds; }
- (BOOL)initialOriginSpecified { return _initialOriginSpecified; }
- (CGSize)pipSize { return _pipSize; }
@end

@protocol ArcCoreBrowserDelegate <NSObject>
- (void)browser:(ArcBrowser*)browser didCreateNewWebContents:(ArcWebContents*)contents isActive:(BOOL)isActive;
@end

namespace arc {
// Core-only marker query; implemented after the WebContents weak defaults.
// Presence is queried without constructing or retaining an ObjC wrapper.
bool HasCoreWebContentsData(content::WebContents* contents);
}  // namespace arc

@interface ArcBrowser ()
- (BOOL)isInitialized;
- (BOOL)deassociateNativeBrowser:(BrowserWindowInterface*)expected
                    completion:(base::OnceClosure*)afterPublication
    __attribute__((objc_direct));
- (BOOL)hasCoreDelegateBridge __attribute__((objc_direct));
- (id<ArcCoreBrowserDelegate>)coreDelegate __attribute__((objc_direct));
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active
              notifyCreated:(BOOL)notifyCreated __attribute__((objc_direct));
@end
@interface ArcWebContents (CoreTabClosing)
- (void)nativeContentsClosing __attribute__((objc_direct));
- (void)nativeContentsClosingWithRestorationID:(ArcSessionID*)identifier __attribute__((objc_direct));
@end

namespace {
// The native BrowserWindowCreateParams constructor and GetType bridge use
// these exact Chrome ordinals; fail compilation if a later Chromium changes it.
static_assert(BrowserWindowInterface::TYPE_NORMAL == 0);
static_assert(BrowserWindowInterface::TYPE_POPUP == 1);
static_assert(BrowserWindowInterface::TYPE_APP_POPUP == 4);
static_assert(BrowserWindowInterface::TYPE_PICTURE_IN_PICTURE == 5);

void NotifyTabChange(ArcBrowser* browser, ArcWebContents* contents, NSString* change) {
  if (browser && contents) {
    [NSNotificationCenter.defaultCenter postNotificationName:kArcBrowserTabsChangedNotification
        object:browser userInfo:@{@"webContents": contents, @"change": change}];
  }
}

class ArcBrowserTabsObserver : public TabStripModelObserver {
 public:
  ArcBrowserTabsObserver(BrowserWindowInterface* browser, ArcBrowser* wrapper) : wrapper_(wrapper) {
    browser->GetTabStripModel()->AddObserver(this);
    __weak ArcBrowser* weak_wrapper = wrapper;
    close_subscription_ = browser->RegisterBrowserDidClose(base::BindRepeating(
        [](ArcBrowser* __weak wrapper, BrowserWindowInterface*) {
          ArcBrowser* retained = wrapper;
          if (retained) {
            [NSNotificationCenter.defaultCenter postNotificationName:kArcBrowserWillCloseNotification
                object:retained];
          }
        }, weak_wrapper));
  }
  void OnTabStripModelChanged(TabStripModel* tabs, const TabStripModelChange& change,
                             const TabStripSelectionChange& selection) override {
    // Snapshot native identities before calling out: app delegates and notification
    // observers can synchronously change the tab strip again.
    ArcBrowser* wrapper = wrapper_;
    // Native 4306358 checks the FIRST inserted contents' canonical marker for
    // the whole change, before adopting any new contents. Transfers and public
    // createWebContents returns are already managed and do not emit DidCreate.
    const auto* inserted = change.type() == TabStripModelChange::kInserted
        ? change.GetInsert() : nullptr;
    const bool notify_created = inserted && !inserted->contents.empty() &&
        !arc::HasCoreWebContentsData(inserted->contents.front().contents.get()) &&
        [wrapper hasCoreDelegateBridge];
    // Native 43065dc compares each insertion to the current active contents,
    // independently of selection.active_tab_changed(). Popup and app-popup
    // browser types force true (4306614/430663c). Capture before any callout.
    std::vector<bool> inserted_active;
    if (inserted) {
      const content::WebContents* active_at_receipt = tabs->GetActiveWebContents();
      BrowserWindowInterface* native_browser = [wrapper browser];
      const bool popup_active = native_browser &&
          (native_browser->GetType() == BrowserWindowInterface::TYPE_POPUP ||
           native_browser->GetType() == BrowserWindowInterface::TYPE_APP_POPUP);
      inserted_active.reserve(inserted->contents.size());
      for (const auto& entry : inserted->contents) {
        inserted_active.push_back(popup_active || entry.contents.get() == active_at_receipt);
      }
    }
    NSMutableArray<NSDictionary*>* changes = [NSMutableArray array];
    NSMutableArray<NSArray*>* closing = [NSMutableArray array];
    auto append = [&](content::WebContents* contents, NSString* kind, bool active = false) {
      ArcWebContents* tab = [ArcWebContents wrapperForWebContents:contents];
      if (tab) { [changes addObject:@{@"webContents": tab, @"change": kind, @"active": @(active)}]; }
    };
    if (change.type() == TabStripModelChange::kInserted) {
      size_t index = 0;
      for (const auto& entry : change.GetInsert()->contents) {
        append(entry.contents.get(), @"inserted", inserted_active[index++]);
      }
    } else if (change.type() == TabStripModelChange::kRemoved) {
      for (const auto& entry : change.GetRemove()->contents) {
        append(entry.contents.get(), @"removed");
        if (entry.tab_detach_reason == tabs::TabInterface::DetachReason::kDelete) {
          ArcSessionID* identifier = entry.session_id
              ? [[ArcSessionID alloc] initWithSessionId:&*entry.session_id] : nil;
          [closing addObject:@[[ArcWebContents wrapperForWebContents:entry.contents.get()], identifier ?: NSNull.null]];
        }
      }
    } else if (change.type() == TabStripModelChange::kReplaced) {
      append(change.GetReplace()->new_contents.get(), @"replaced");
    } else if (change.type() == TabStripModelChange::kMoved) {
      append(change.GetMove()->contents.get(), @"moved");
    }
    ArcWebContents* activated = selection.active_tab_changed()
        ? [ArcWebContents wrapperForWebContents:selection.new_contents.get()] : nil;
    for (NSArray* entry in closing) {
      ArcSessionID* identifier = entry[1] == NSNull.null ? nil : entry[1];
      [(ArcWebContents*)entry[0] nativeContentsClosingWithRestorationID:identifier];
    }
    for (NSDictionary* entry in changes) {
      ArcWebContents* tab = entry[@"webContents"];
      if ([entry[@"change"] isEqual:@"inserted"]) {
        content::WebContents* native = [tab webContents];
        BrowserWindowInterface* browser = [wrapper browser];
        if (browser && !browser->IsDeleteScheduled() && native) {
          [wrapper didInsertWebContents:native
                                active:[entry[@"active"] boolValue]
                         notifyCreated:notify_created];
        }
      }
      NotifyTabChange(wrapper, tab, entry[@"change"]);
    }
    if (activated) { NotifyTabChange(wrapper, activated, @"activated"); }
  }
  void OnTabChangedAt(tabs::TabInterface* tab, TabChangeType) override {
    NotifyTabChange(wrapper_, [ArcWebContents wrapperForWebContents:tab->GetContents()], @"updated");
  }
 private:
  __weak ArcBrowser* wrapper_;
  base::CallbackListSubscription close_subscription_;
};
// The native API reports cancellation, rather than whether closure succeeded.
// Keep transport independent of the Objective-C wrapper: already queued
// completions survive wrapper/native teardown.
class CoreBrowserCloseState
    : public std::enable_shared_from_this<CoreBrowserCloseState> {
 public:
  using Callback = void (^)(BOOL canceled);
  using Completion = base::OnceCallback<void(bool)>;
  using Batch = std::vector<Completion>;

  explicit CoreBrowserCloseState(BrowserWindowInterface* browser)
      : browser_(browser->GetWeakPtr()) {}

  static std::shared_ptr<CoreBrowserCloseState> Create(
      BrowserWindowInterface* browser) {
    auto state = std::make_shared<CoreBrowserCloseState>(browser);
    state->close_subscription_ = browser->RegisterBrowserDidClose(
        base::BindRepeating(
            [](std::weak_ptr<CoreBrowserCloseState> weak_state,
               BrowserWindowInterface*) {
              if (auto state = weak_state.lock()) {
                state->NativeDidClose();
              }
            },
            std::weak_ptr<CoreBrowserCloseState>(state)));
    return state;
  }

  // Destructor-entry preparation has no app callout or pending-batch detach.
  // The physical browser can still be allocated, but no continuation may use it.
  void BeginNativeDestruction() {
    CHECK(!native_destruction_prepared_);
    native_destruction_prepared_ = true;
    native_closed_ = true;
    finishing_ = true;
    browser_.reset();
  }

  // The caller runs this only after synchronous global close publication.
  // Keep the state alive through callbacks that can release its facade owner.
  void FinishNativeDestruction() {
    auto keep_alive = shared_from_this();
    CHECK(native_destruction_prepared_);
    native_destruction_prepared_ = false;
    NativeDidClose();
  }

  ~CoreBrowserCloseState() {
    native_closed_ = true;
    finishing_ = true;
    RunBatch(TakePending(), false);
  }

  void Request(bool skip_beforeunload, Callback callback) {
    auto keep_alive = shared_from_this();
    Completion completion = base::BindOnce(
        [](Callback callback, bool canceled) {
          if (callback) {
            callback(canceled);
          }
        },
        [callback copy]);
    if (native_closed_ || !browser_ || browser_->IsDeleteScheduled() ||
        finishing_) {
      std::move(completion).Run(false);
      return;
    }

    const bool already_pending = !pending_.empty();
    pending_.push_back(std::move(completion));
    // A force/skip request may escalate an existing confirmation. Ordinary
    // additional callers share its eventual result in insertion order.
    if (already_pending && !skip_beforeunload) {
      return;
    }

    UnloadController* unload = UnloadController::From(browser_.get());
    const bool awaiting_confirmation = unload->RunBeforeUnloadForShutdown(
        skip_beforeunload,
        base::BindRepeating(
            [](std::weak_ptr<CoreBrowserCloseState> weak_state,
               bool confirmed) {
              if (auto state = weak_state.lock()) {
                state->Complete(confirmed);
              }
            },
            weak_from_this()));
    // This API does not invoke its confirmation callback when it returns
    // false (including skip-beforeunload and no-handlers paths).
    if (!awaiting_confirmation) {
      Complete(true);
    }
  }

 private:
  Batch TakePending() {
    Batch batch;
    batch.swap(pending_);
    return batch;
  }

  static void RunBatch(Batch batch, bool canceled) {
    for (auto& completion : batch) {
      std::move(completion).Run(canceled);
    }
  }

  void NativeDidClose() {
    auto keep_alive = shared_from_this();
    native_closed_ = true;
    finishing_ = true;
    if (native_destruction_prepared_) {
      return;
    }
    RunBatch(TakePending(), false);
  }

  void Complete(bool confirmed) {
    auto keep_alive = shared_from_this();
    // A nested native confirmation cannot detach/drain the destructor batch
    // while the global app notification is still being published.
    if (native_destruction_prepared_) {
      return;
    }
    if (pending_.empty()) {
      return;
    }
    Batch batch = TakePending();
    const bool permitted = confirmed || native_closed_ || !browser_ ||
                           browser_->IsDeleteScheduled() || finishing_;
    if (permitted) {
      // Reentrant requests during these callbacks complete inline with NO,
      // as in the original native finishing/already-closing branch.
      finishing_ = true;
      RunBatch(std::move(batch), false);
      auto browser = browser_;
      if (!native_closed_ && browser && !browser->IsDeleteScheduled()) {
        browser->GetWindow()->Close();
      }
      if (!native_closed_ && browser_) {
        finishing_ = false;
      }
      return;
    }

    // Reset first. It may invoke the native repeating callback again; the
    // detached batch cannot be delivered a second time. The posted task owns
    // its callbacks and does not depend on this state or an Objective-C owner.
    UnloadController::From(browser_.get())->ResetBeforeUnloadForShutdown();
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(&CoreBrowserCloseState::RunBatch,
                                  std::move(batch), true));
  }

  base::WeakPtr<BrowserWindowInterface> browser_;
  Batch pending_;
  bool native_closed_ = false;
  bool finishing_ = false;
  bool native_destruction_prepared_ = false;
  base::CallbackListSubscription close_subscription_;
};

const char kArcBrowserKey[] = "ArcBrowser";
struct ArcBrowserHolder : base::SupportsUserData::Data {
  ArcBrowser* __strong wrapper;
};
}  // namespace

@interface ArcBrowser ()
- (instancetype)initWithChromeBrowser:(BrowserWindowInterface*)browser;
- (void)takeOwnershipOfWebContents:(ArcWebContents*)contents;
- (void)initializeWindowIdentifier:(NSString*)identifier __attribute__((objc_direct));
@end

@implementation ArcBrowser {
  base::WeakPtr<BrowserWindowInterface> _browser;
  ArcBrowserContext* _context;
  ArcBrowserCreateParams* _browserCreateParams;
  id _sidePanel;
  __weak id<ArcCoreBrowserDelegate> _delegate;
  // Native registration outlives a weak app delegate. Only setDelegate:nil
  // clears it; any later nonnil delegate reuses/reestablishes registration.
  BOOL _hasDelegateBridge;
  std::unique_ptr<ArcBrowserTabsObserver> _tabsObserver;
  std::shared_ptr<CoreBrowserCloseState> _closeState;
}
@synthesize windowIdentifier = _windowIdentifier;

+ (instancetype)wrapperForBrowser:(BrowserWindowInterface*)browser {
  if (!browser) {
    return nil;
  }
  auto* window = static_cast<arc::ArcBrowserWindow*>(browser->GetWindow());
  auto* holder = static_cast<ArcBrowserHolder*>(window->GetUserData(kArcBrowserKey));
  if (!holder) {
    auto owned = std::make_unique<ArcBrowserHolder>();
    owned->wrapper = [[self alloc] initWithChromeBrowser:browser];
    holder = owned.get();
    window->SetUserData(kArcBrowserKey, std::move(owned));
  }
  return holder->wrapper;
}
- (instancetype)initWithChromeBrowser:(BrowserWindowInterface*)browser {
  if ((self = [super init])) {
    _browser = browser->GetWeakPtr();
    _context = [ArcBrowserContext wrapperForProfile:browser->GetProfile()];
    _browserCreateParams = [[ArcBrowserCreateParams alloc]
        initWithBrowserCreateParams:&BrowserInitState::From(browser)->create_params()];
    Class sidePanelClass = NSClassFromString(@"ArcBrowserSidePanel");
    SEL withBrowser = NSSelectorFromString(@"initWithArcBrowser:");
    _sidePanel = [sidePanelClass instancesRespondToSelector:withBrowser]
                     ? ((id (*)(id, SEL, id))objc_msgSend)([sidePanelClass alloc], withBrowser, self)
                     : [[sidePanelClass alloc] init];
    _tabsObserver = std::make_unique<ArcBrowserTabsObserver>(browser, self);
    _closeState = CoreBrowserCloseState::Create(browser);
    ArcSessionsBrowserCreated(self);
  }
  return self;
}
- (void)setWindowIdentifier:(NSString*)identifier { _windowIdentifier = [identifier copy]; }
- (void)initializeWindowIdentifier:(NSString*)identifier { _windowIdentifier = [identifier copy]; }
- (BrowserWindowInterface*)browser { return _browser.get(); }
- (BOOL)deassociateNativeBrowser:(BrowserWindowInterface*)expected
                    completion:(base::OnceClosure*)afterPublication {
  CHECK(afterPublication);
  CHECK(afterPublication->is_null());
  if (!expected || _browser.get() != expected) {
    return NO;
  }
  auto state = _closeState;
  if (state) {
    state->BeginNativeDestruction();
  }
  _browser.reset();
  if (state) {
    *afterPublication = base::BindOnce(
        [](std::shared_ptr<CoreBrowserCloseState> retained) {
          retained->FinishNativeDestruction();
        }, std::move(state));
  }
  return YES;
}
- (BOOL)isInitialized { return _browser && !_browser->IsDeleteScheduled(); }
- (NSInteger)type {
  if (!_browser) { return 0; }
  switch (_browser->GetType()) {
    case BrowserWindowInterface::TYPE_NORMAL: return 0;
    case BrowserWindowInterface::TYPE_POPUP: return 1;
    case BrowserWindowInterface::TYPE_APP_POPUP: return 2;
    case BrowserWindowInterface::TYPE_PICTURE_IN_PICTURE: return 3;
    // Original 42fad78 accepts only Chrome 0/1/4/5 (mask 0x33), trapping
    // unsupported live browser types rather than reporting normal/default.
    default: CHECK(false) << "Unsupported Arc browser type";
  }
}
- (void)tryToCloseSkipBeforeUnloadHandlers:(BOOL)skipBeforeUnloadHandlers
                                 callback:(void (^)(BOOL canceled))callback {
  auto state = _closeState;
  if (state) {
    state->Request(skipBeforeUnloadHandlers, callback);
  } else if (callback) {
    callback(NO);
  }
}
- (BOOL)isJavaScriptFromAppleEventsAllowed {
  return _browser && _browser->GetProfile()->GetPrefs()->GetBoolean(
                         prefs::kAllowJavascriptAppleEvents);
}
- (BOOL)isCastEnabled {
  if (!_browser || !media_router::MediaRouterEnabled(_browser->GetProfile())) {
    return NO;
  }
  content::WebContents* active =
      _browser->GetTabStripModel()->GetActiveWebContents();
  auto* manager = active
      ? web_modal::WebContentsModalDialogManager::FromWebContents(active)
      : nullptr;
  // Arc's original getter requires an active contents and its modal manager;
  // chrome::CanRouteMedia also permits the missing-manager case.
  return manager && !manager->IsDialogActive();
}
- (ArcBrowserContext*)context { return _context; }
- (ArcBrowserCreateParams*)browserCreateParams { return _browserCreateParams; }
- (id)sidePanel { return _sidePanel; }
- (ArcSessionID*)sessionId {
  return _browser ? [[ArcSessionID alloc] initWithSessionId:&_browser->GetSessionID()] : nil;
}
- (void)setDelegate:(id)delegate {
  _delegate = delegate;
  _hasDelegateBridge = delegate != nil;
  arc::InstallBrowserExtensions(self, delegate);
  if (delegate) {
    [NSNotificationCenter.defaultCenter postNotificationName:kArcBrowserDidSetDelegateNotification object:self];
  }
}
- (id)delegate { return _delegate; }
- (BOOL)hasCoreDelegateBridge { return _hasDelegateBridge; }
- (id<ArcCoreBrowserDelegate>)coreDelegate { return _delegate; }
- (void)setWindow:(NSWindow*)window {
  if (_browser) {
    static_cast<arc::ArcBrowserWindow*>(_browser->GetWindow())->SetNativeWindow(gfx::NativeWindow(window));
  }
}
- (BOOL)isEqual:(id)other { return self == other; }
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active
              notifyCreated:(BOOL)notifyCreated {
  ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:contents];
  const auto native = contents->GetWeakPtr();
  const auto browser = _browser;
  if (ArcSessionsDidInsertWebContents(self, contents)) {
    return;  // reported as a restore by the sessions package
  }
  if (!notifyCreated) { return; }
  __weak ArcBrowser* weak_owner = self;
  __weak ArcWebContents* weak_wrapper = wrapper;
  // Native 43065b0 first posts a weak Browser task with copied contents/active
  // state. Its 42fcf40 receiver checks native contents and chooses the CURRENT
  // delegate bridge at delivery, then invokes the ObjC bridge synchronously.
  content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(
      [](void (^work)()) { work(); }, ^{
    if (!browser || !native) { return; }
    ArcBrowser* owner __attribute__((objc_precise_lifetime)) = weak_owner;
    ArcWebContents* current __attribute__((objc_precise_lifetime)) = weak_wrapper;
    if (!owner || [owner browser] != browser.get() || !current ||
        [current webContents] != native.get()) { return; }
    id<ArcCoreBrowserDelegate> delegate = [owner coreDelegate];
    if ([delegate respondsToSelector:@selector(browser:didCreateNewWebContents:isActive:)]) {
      [delegate browser:owner didCreateNewWebContents:current isActive:active];
    }
  }));
}
- (ArcWebContents*)createWebContents:(id)configuration {
  if (!_browser) {
    return nil;
  }
  ArcWebContents* wrapper = [ArcWebContents createWithProfile:_browser->GetProfile() configuration:configuration];
  [self takeOwnershipOfWebContents:wrapper];
  return wrapper;
}
- (void)takeOwnershipOfWebContents:(ArcWebContents*)contents {
  if (!_browser) {
    return;
  }
  auto native = [contents takeOwnedWebContents];
  if (native) {
    _browser->GetTabStripModel()->InsertWebContentsAt(
        -1, std::move(native), [contents foregroundForInsertion] ? ADD_ACTIVE : 0);
  }
}
@end

namespace arc {
bool PrepareBrowserForNativeDestruction(
    ArcBrowser* existing, BrowserWindowInterface* expected,
    base::OnceClosure* after_publication) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  CHECK(after_publication);
  // Releasing a previous closure here could drain its retained close state.
  // Require an empty output rather than replacing an unknown owned callback.
  CHECK(after_publication->is_null());
  ArcBrowser* retained __attribute__((objc_precise_lifetime)) = existing;
  if (!retained) {
    return false;
  }
  return [retained deassociateNativeBrowser:expected
                                completion:after_publication];
}
}  // namespace arc

@interface ArcBrowserApplication : NSObject
@end

@implementation ArcBrowserApplication (Core)
- (ArcBrowser*)createBrowser:(ArcBrowserContext*)context withDelegate:(id)delegate forWindow:(NSString*)identifier {
  Profile* profile = ArcProfileFrom(context);
  if (!profile || GetBrowserWindowCreationStatusForProfile(*profile) !=
                      BrowserWindowInterface::CreationStatus::kOk) {
    return nil;
  }
  BrowserWindowCreateParams params(profile, false);
  params.should_trigger_session_restore = false;
  params.window = new arc::ArcBrowserWindow();
  BrowserWindowInterface* native = CreateBrowserWindow(std::move(params));
  ArcBrowser* browser = [ArcBrowser wrapperForBrowser:native];
  [browser initializeWindowIdentifier:identifier];
  [browser setDelegate:delegate];
  return browser;
}
- (ArcBrowser*)createPopupBrowser:(ArcBrowserContext*)context withDelegate:(id)delegate {
  Profile* profile = ArcProfileFrom(context);
  if (!profile || GetBrowserWindowCreationStatusForProfile(*profile) !=
                      BrowserWindowInterface::CreationStatus::kOk) {
    return nil;
  }
  BrowserWindowCreateParams params(BrowserWindowInterface::TYPE_POPUP, profile, false);
  params.should_trigger_session_restore = false;
  params.window = new arc::ArcBrowserWindow();
  ArcBrowser* browser = [ArcBrowser wrapperForBrowser:CreateBrowserWindow(std::move(params))];
  [browser setDelegate:delegate];
  return browser;
}
@end
