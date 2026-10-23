// Copyright 2026 ArcMod. Session values and Chromium-backed tab restoration.

#import "arc/api/arc_internal.h"
#import <objc/runtime.h>

#include <algorithm>
#include <array>
#include <limits>
#include <optional>
#include <set>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/scoped_observation.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/apps/app_service/web_contents_app_id_utils.h"
#include "arc/browser/arc_browser_window.h"
#include "chrome/browser/performance_manager/policies/page_discarding_helper.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/ui/browser_live_tab_context.h"
#include "chrome/browser/ui/browser_tabrestore.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/create_browser_window.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/web_applications/web_app_helpers.h"
#include "components/sessions/content/session_tab_helper.h"
#include "components/performance_manager/public/performance_manager.h"
#include "components/sessions/core/session_types.h"
#include "components/sessions/core/tab_restore_service.h"
#include "components/sessions/core/tab_restore_service_observer.h"
#include "components/sessions/core/tab_restore_types.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/web_contents.h"

@interface SessionsRestoreIDs : NSObject
@property(nonatomic, copy) NSArray<ArcSessionID*>* identifiers;
@property(nonatomic) NSUInteger next;
@property(nonatomic) NSUInteger pending;
@property(nonatomic) BOOL sealed;
@property(nonatomic) BOOL cancelled;
@property(nonatomic, copy) void (^completion)(void);
@end

@interface ArcWebContents (SessionsDiscardAccess)
- (id)uiDelegate;
@end

@protocol SessionsDiscardDelegate <NSObject>
- (void)webContents:(ArcWebContents*)contents finalizeDiscardRequest:(BOOL)discarded;
@end

@interface SessionsDiscardAdapter : NSObject
@property(nonatomic, weak) id<SessionsDiscardDelegate> delegate;
@end
@implementation SessionsDiscardAdapter
@synthesize delegate = _delegate;
@end

@interface SessionsDiscardState : NSObject
@property(nonatomic, strong) SessionsDiscardAdapter* adapter;
@property(nonatomic) NSUInteger pending;
@property(nonatomic) BOOL active;
@end
@implementation SessionsDiscardState
@synthesize adapter = _adapter, pending = _pending, active = _active;
@end

namespace {
const char kDiscardState = 0;
SessionsDiscardState* DiscardState(ArcWebContents* contents) {
  SessionsDiscardState* state = objc_getAssociatedObject(contents, &kDiscardState);
  if (!state) {
    state = [[SessionsDiscardState alloc] init];
    objc_setAssociatedObject(contents, &kDiscardState, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  return state;
}
}  // namespace

void ArcSessionsUIDelegateChanged(ArcWebContents* contents, id delegate) {
  SessionsDiscardState* state = DiscardState(contents);
  state.active = delegate != nil;
  if (!delegate) {
    // Native setUIDelegate:nil unregisters the current adapter without
    // destroying its ObjC bridge. Already queued calls may still deliver.
    return;
  }
  SessionsDiscardAdapter* adapter = [[SessionsDiscardAdapter alloc] init];
  adapter.delegate = delegate;
  state.adapter = adapter;
}

BOOL ArcSessionsHasPendingDiscard(ArcWebContents* contents) {
  SessionsDiscardState* state = objc_getAssociatedObject(contents, &kDiscardState);
  return state.pending != 0;
}

@implementation ArcWebContents (SessionsDiscard)
- (void)discard {
  content::WebContents* native = [self webContents];
  if (!native || !performance_manager::PerformanceManager::IsAvailable()) {
    return;
  }
  auto page = performance_manager::PerformanceManager::GetPrimaryPageNodeForWebContents(native);
  auto* helper = performance_manager::policies::PageDiscardingHelper::GetFromGraph(
      performance_manager::PerformanceManager::GetGraph());
  const BOOL discarded = page && helper && helper->ImmediatelyDiscardMultiplePages(
      {page.get()}, ::mojom::LifecycleUnitDiscardReason::PROACTIVE, true);
  // A native discard can replace the underlying WebContents. Only touch the
  // wrapper after that operation, and omit the callback if it was detached.
  if (![self webContents]) {
    return;
  }
  SessionsDiscardState* state = DiscardState(self);
  if (!state.adapter) {
    ArcSessionsUIDelegateChanged(self, [self uiDelegate]);
  }
  if (!state.active) {
    return;
  }
  __weak ArcWebContents* weakContents = self;
  __weak SessionsDiscardAdapter* weakAdapter = state.adapter;
  state.pending += 1;
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    ArcWebContents* contents = weakContents;
    id<SessionsDiscardDelegate> delegate = weakAdapter.delegate;
    if (contents && delegate &&
        [delegate respondsToSelector:@selector(webContents:finalizeDiscardRequest:)]) {
      [delegate webContents:contents finalizeDiscardRequest:discarded];
    }
    state.pending -= 1;
  }));
}
@end
@implementation SessionsRestoreIDs
@synthesize identifiers = _identifiers, next = _next, pending = _pending,
    sealed = _sealed, cancelled = _cancelled, completion = _completion;
@end

namespace {
std::optional<SessionID> FindRestoreEntry(const sessions::tab_restore::Entry& entry, SessionID target);
NSArray* RestorationIDsForEntry(const sessions::tab_restore::Entry& entry, SessionID target,
                                std::optional<SessionID> explicitRequest = std::nullopt);
const char kActiveBrowserRestore = 0;
const char kFullRestoreIDs = 0;
const char kFullBrowserRestoring = 0;

void CompleteFullRestore(SessionsRestoreIDs* state) {
  if (state.sealed && !state.pending && state.completion) {
    void (^completion)(void) = state.completion;
    state.completion = nil;
    completion();
  }
}

class ScopedFullRestoreIDs {
 public:
  ScopedFullRestoreIDs(ArcBrowser* owner, SessionsRestoreIDs* state) : owner_(owner) {
    previous_ = objc_getAssociatedObject(owner, &kFullRestoreIDs);
    objc_setAssociatedObject(owner, &kFullRestoreIDs, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  ~ScopedFullRestoreIDs() {
    objc_setAssociatedObject(owner_, &kFullRestoreIDs, previous_, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
 private:
  ArcBrowser* __strong owner_;
  SessionsRestoreIDs* __strong previous_;
};

class ScopedBrowserRestore {
 public:
  ScopedBrowserRestore(ArcBrowserContext* owner, id request) : owner_(owner) {
    previous_ = objc_getAssociatedObject(owner, &kActiveBrowserRestore);
    objc_setAssociatedObject(owner, &kActiveBrowserRestore, request, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  ~ScopedBrowserRestore() {
    objc_setAssociatedObject(owner_, &kActiveBrowserRestore, previous_, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
 private:
  ArcBrowserContext* __strong owner_;
  id __strong previous_;
};

// Arc's session getters differ from ArcNSDate: a null native time is the Unix
// epoch, not nil. Time::InSecondsFSinceUnixEpoch preserves infinity sentinels.
NSDate* SessionDate(base::Time time) {
  return [NSDate dateWithTimeIntervalSince1970:time.InSecondsFSinceUnixEpoch()];
}
}  // namespace

ARC_EXPORT @interface ArcSessionNavigation : NSObject
- (instancetype)initWithSessionNavigation:(const void*)navigation;
- (const sessions::SerializedNavigationEntry*)chromeNavigation __attribute__((objc_direct));
@property(nonatomic, readonly) NSURL* faviconURL;
@property(nonatomic, readonly) NSURL* virtualURL;
@property(nonatomic, readonly) NSURL* originalRequestURL;
@property(nonatomic, readonly) NSURL* referrerURL;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly) BOOL hasPostData;
@property(nonatomic, readonly) NSDate* timestamp;
@property(nonatomic, readonly) BOOL isOverridingUserAgent;
@end

// Full-window restore is asynchronous when the native service still needs to
// read its persisted session. The request is retained by the canonical profile
// wrapper; the native service and profile themselves are never kept alive.
@interface SessionsRestoreRequest : NSObject
+ (void)startWithContext:(ArcBrowserContext*)context identifier:(ArcSessionID*)identifier
    created:(void (^)(ArcBrowser*))created finished:(void (^)(ArcBrowser*))finished;
- (void)serviceLoaded;
- (void)serviceDestroyed;
@end

namespace {
const char kPendingSessionRequests = 0;

class SessionsLoadObserver : public sessions::TabRestoreServiceObserver {
 public:
  SessionsLoadObserver(sessions::TabRestoreService* service, SessionsRestoreRequest* request)
      : request_(request) {
    observation_.Observe(service);
  }
  void TabRestoreServiceLoaded(sessions::TabRestoreService* service) override {
    [request_ serviceLoaded];
  }
  void TabRestoreServiceDestroyed(sessions::TabRestoreService* service) override {
    observation_.Reset();
    [request_ serviceDestroyed];
  }
 private:
  // Keep the pending completion alive even if the profile wrapper disappears
  // before service shutdown. Loaded/destroyed posts a task that resets this
  // observer and breaks the temporary ownership cycle on the UI sequence.
  SessionsRestoreRequest* __strong request_;
  base::ScopedObservation<sessions::TabRestoreService, sessions::TabRestoreServiceObserver> observation_{this};
};
}  // namespace

@interface SessionsRestoreRequest ()
- (void)performRestore;
- (void)retryAfterActiveRestore;
- (void)finish:(ArcBrowser*)browser;
@end

@implementation SessionsRestoreRequest {
  __weak ArcBrowserContext* _context;
  ArcSessionID* _identifier;
  void (^_created)(ArcBrowser*);
  void (^_finished)(ArcBrowser*);
  std::unique_ptr<SessionsLoadObserver> _observer;
  std::unique_ptr<ScopedBrowserRestore> _activeRestore;
  __weak ArcBrowser* _restoringBrowser;
  BOOL _scheduled;
  BOOL _completed;
  BOOL _serviceDestroyed;
}
+ (void)startWithContext:(ArcBrowserContext*)context identifier:(ArcSessionID*)identifier
    created:(void (^)(ArcBrowser*))created finished:(void (^)(ArcBrowser*))finished {
  Profile* profile = ArcProfileFrom(context);
  if (!profile || profile->IsOffTheRecord()) {
    if (finished) {
      finished(nil);
    }
    return;
  }
  auto* service = TabRestoreServiceFactory::GetForProfile(profile);
  if (!service) {
    if (finished) {
      finished(nil);
    }
    return;
  }
  SessionsRestoreRequest* request = [[self alloc] init];
  request->_context = context;
  request->_identifier = identifier;
  request->_created = [created copy];
  request->_finished = [finished copy];
  NSMutableSet* pending = objc_getAssociatedObject(context, &kPendingSessionRequests);
  if (!pending) {
    pending = [NSMutableSet set];
    objc_setAssociatedObject(context, &kPendingSessionRequests, pending, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  [pending addObject:request];
  if (service->IsLoaded()) {
    [request performRestore];
  } else {
    request->_observer = std::make_unique<SessionsLoadObserver>(service, request);
    service->LoadTabsFromLastSession();
  }
}
- (void)serviceLoaded {
  if (_scheduled || _completed) {
    return;
  }
  _scheduled = YES;
  // Defer so removing the load observer cannot destroy it in its own callback.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    [self performRestore];
  }));
}
- (void)serviceDestroyed {
  _serviceDestroyed = YES;
  [self serviceLoaded];
}
- (void)finish:(ArcBrowser*)browser {
  if (_completed) {
    return;
  }
  _completed = YES;
  _observer.reset();
  void (^finished)(ArcBrowser*) = _finished;
  _created = nil;
  _finished = nil;
  NSMutableSet* pending = objc_getAssociatedObject(_context, &kPendingSessionRequests);
  [pending removeObject:self];
  if (finished) {
    finished(browser);
  }
  // Original full restoration clears its variant only after finished returns.
  objc_setAssociatedObject(_restoringBrowser, &kFullBrowserRestoring, nil,
                          OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  _activeRestore.reset();
}
- (void)retryAfterActiveRestore {
  if (_scheduled || _completed) {
    return;
  }
  _scheduled = YES;
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(FROM_HERE, base::BindOnce(^{
    self->_scheduled = NO;
    [self performRestore];
  }), base::Milliseconds(10));
}
- (void)performRestore {
  __attribute__((objc_precise_lifetime)) SessionsRestoreRequest* keepAlive = self;
  if (_completed) {
    return;
  }
  _observer.reset();
  Profile* profile = ArcProfileFrom(_context);
  if (_serviceDestroyed || !profile ||
      GetBrowserWindowCreationStatusForProfile(*profile) != BrowserWindowInterface::CreationStatus::kOk) {
    [keepAlive finish:nil];
    return;
  }
  auto* service = TabRestoreServiceFactory::GetForProfileIfExisting(profile);
  if (service && (service->IsRestoring() ||
                  objc_getAssociatedObject(_context, &kActiveBrowserRestore))) {
    _scheduled = NO;
    [keepAlive retryAfterActiveRestore];
    return;
  }
  const sessions::tab_restore::Entry* selected = nullptr;
  const NSInteger requested = _identifier ? _identifier.identifier : -1;
  const SessionID target = requested > 0 && requested <= std::numeric_limits<int32_t>::max()
      ? SessionID::FromSerializedValue(static_cast<int32_t>(requested)) : SessionID::InvalidValue();
  std::optional<SessionID> matchedID;
  if (service) {
    for (const auto& entry : service->entries()) {
      // Original IDs default to -1, so the native invalid ID may match the
      // newest entry through that field; it is not a WINDOW-only sentinel.
      if (auto found = FindRestoreEntry(*entry, target)) {
        selected = entry.get();
        matchedID = found;
        break;
      }
    }
  }
  if (!selected) {
    [keepAlive finish:nil];
    return;
  }
  // Reserve the profile before creating the browser or invoking app code.
  // A nested request must not consume the native entry that this request uses.
  _activeRestore = std::make_unique<ScopedBrowserRestore>(_context, self);
  const SessionID restoreID = *matchedID;
  SessionsRestoreIDs* callbackIDs = [[SessionsRestoreIDs alloc] init];
  callbackIDs.identifiers = RestorationIDsForEntry(*selected, restoreID);
  BrowserWindowCreateParams params(profile, true);
  if (selected->type == sessions::tab_restore::WINDOW) {
    const auto& window = static_cast<const sessions::tab_restore::Window&>(*selected);
    // A descendant tab uses RestoreTab's fresh normal-window defaults. Only
    // whole-window restoration consumes the stored window configuration.
    if (restoreID == window.id || restoreID == window.original_id || !window.app_name.empty()) {
      // SessionWindow and BrowserWindowInterface use different type enums.
      // Native restoration creates an app window only for a restorable app
      // name; otherwise even stored popup/devtools types use a normal window.
      const std::string appID = web_app::GetAppIdFromApplicationName(window.app_name);
      if (!window.app_name.empty() && (appID.empty() || apps::IsInstalledApp(profile, appID))) {
        params = window.window_type == sessions::SessionWindow::TYPE_APP_POPUP
            ? BrowserWindowCreateParams::CreateForAppPopup(
                  window.app_name, true, window.bounds, profile, true)
            : BrowserWindowCreateParams::CreateForApp(
                  window.app_name, true, window.bounds, profile, true);
      } else {
        params.initial_bounds = window.bounds;
      }
      params.initial_show_state = window.show_state;
      params.initial_workspace = window.workspace;
      params.user_title = window.user_title;
    }
  }
  params.should_trigger_session_restore = false;
  params.creation_source = BrowserWindowCreateParams::CreationSource::kSessionRestore;
  params.window = new arc::ArcBrowserWindow();
  BrowserWindowInterface* native = CreateBrowserWindow(std::move(params));
  if (!native) {
    [keepAlive finish:nil];
    return;
  }
  auto weakBrowser = native->GetWeakPtr();
  ArcBrowser* wrapper = [ArcBrowser wrapperForBrowser:native];
  _restoringBrowser = wrapper;
  objc_setAssociatedObject(wrapper, &kFullBrowserRestoring, self,
                          OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  if (_created) {
    _created(wrapper);
  }
  native = weakBrowser.get();
  profile = ArcProfileFrom(_context);
  if (!native || native->IsDeleteScheduled() || !profile) {
    [keepAlive finish:nil];
    return;
  }
  service = TabRestoreServiceFactory::GetForProfileIfExisting(profile);
  auto* context = BrowserLiveTabContext::From(native);
  if (!service || !context || service->IsRestoring() ||
      !std::ranges::any_of(service->entries(), [restoreID](const auto& entry) {
        return FindRestoreEntry(*entry, restoreID).has_value();
      })) {
    [keepAlive finish:nil];
    return;
  }
  // Host mode reuses this explicitly supplied Arc context for whole-window
  // entries (ChromeTabRestoreServiceClient host gate, lead-owned patch).
  // Native TabRestoreService retains its complete group/split/history logic.
  ScopedFullRestoreIDs insertionIDs(wrapper, callbackIDs);
  const auto restored = service->RestoreEntryById(context, restoreID, WindowOpenDisposition::NEW_FOREGROUND_TAB);
  // The service has consumed the entry. Other browsers in this profile may
  // restore independently while this browser's posted callbacks are pending.
  _activeRestore.reset();
  const bool succeeded = restored.has_value();
  __weak SessionsRestoreIDs* weakCallbackIDs = callbackIDs;
  callbackIDs.completion = ^{
    [keepAlive finish:succeeded && !weakCallbackIDs.cancelled && weakBrowser &&
        !weakBrowser->IsDeleteScheduled() ? wrapper : nil];
  };
  callbackIDs.sealed = YES;
  CompleteFullRestore(callbackIDs);
}
@end

@interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (Sessions)
- (void)restoreBrowser:(ArcBrowserContext*)context withSessionID:(ArcSessionID*)identifier
    browserCreatedCallback:(void (^)(ArcBrowser*))created finishedCallback:(void (^)(ArcBrowser*))finished {
  [SessionsRestoreRequest startWithContext:context identifier:identifier created:created finished:finished];
}
@end

@implementation ArcSessionNavigation {
  std::shared_ptr<sessions::SerializedNavigationEntry> _navigation;
}
- (instancetype)initWithSessionNavigation:(const void*)navigation {
  if ((self = [super init])) {
    // This selector is native-only. Our peer is an owning shared snapshot,
    // rather than the unavailable private arc::SessionNavigation::Impl ABI.
    if (navigation) {
      _navigation = *static_cast<const std::shared_ptr<sessions::SerializedNavigationEntry>*>(navigation);
    }
  }
  return self;
}
- (const sessions::SerializedNavigationEntry*)chromeNavigation { return _navigation.get(); }
- (NSURL*)faviconURL { return _navigation ? ArcNSURL(_navigation->favicon_url()) : nil; }
- (NSURL*)virtualURL { return _navigation ? ArcNSURL(_navigation->virtual_url()) : nil; }
- (NSURL*)originalRequestURL { return _navigation ? ArcNSURL(_navigation->original_request_url()) : nil; }
- (NSURL*)referrerURL { return _navigation ? ArcNSURL(_navigation->referrer_url()) : nil; }
- (NSString*)title { return _navigation ? ArcNSString(_navigation->title()) : @""; }
- (BOOL)hasPostData { return _navigation && _navigation->has_post_data(); }
- (NSDate*)timestamp { return _navigation ? SessionDate(_navigation->timestamp()) : nil; }
- (BOOL)isOverridingUserAgent { return _navigation && _navigation->is_overriding_user_agent(); }
@end

ARC_EXPORT @interface ArcSessionTab : NSObject
- (instancetype)initWithSessionTab:(const void*)tab;
- (const sessions::SessionTab*)chromeSessionTab __attribute__((objc_direct));
@property(nonatomic, readonly) BOOL isPinned;
@property(nonatomic) int currentNavigationIndex;
@property(nonatomic, readonly) NSDate* lastActiveTime;
@property(nonatomic, strong) NSArray<ArcSessionNavigation*>* navigations;
@property(nonatomic, readonly, copy) NSString* groupId;
@end

@implementation ArcSessionTab {
  std::shared_ptr<sessions::SessionTab> _tab;
  NSArray<ArcSessionNavigation*>* _navigations;
}
@synthesize navigations = _navigations;
- (instancetype)initWithSessionTab:(const void*)tab {
  if ((self = [super init]) && tab) {
    _tab = *static_cast<const std::shared_ptr<sessions::SessionTab>*>(tab);
  }
  return self;
}
- (const sessions::SessionTab*)chromeSessionTab { return _tab.get(); }
- (BOOL)isPinned { return _tab && _tab->pinned; }
- (int)currentNavigationIndex { return _tab ? _tab->current_navigation_index : 0; }
- (void)setCurrentNavigationIndex:(int)index {
  // The original engine traps instead of silently clamping this setter.
  CHECK(_tab);
  CHECK_GE(index, 0);
  CHECK_LT(static_cast<size_t>(index), _tab->navigations.size());
  _tab->current_navigation_index = index;
}
- (NSDate*)lastActiveTime { return _tab ? SessionDate(_tab->last_active_time) : nil; }
- (NSArray<ArcSessionNavigation*>*)navigations {
  if (!_navigations && _tab) {
    NSMutableArray<ArcSessionNavigation*>* result = [NSMutableArray array];
    for (const auto& entry : _tab->navigations) {
      auto snapshot = std::make_shared<sessions::SerializedNavigationEntry>(entry);
      [result addObject:[[ArcSessionNavigation alloc] initWithSessionNavigation:&snapshot]];
    }
    _navigations = result;
  }
  return _navigations;
}
- (void)setNavigations:(NSArray<ArcSessionNavigation*>*)navigations {
  CHECK(_tab);
  // Native SetNavigations rejects an empty history before replacing its vector.
  CHECK_GT(navigations.count, 0u);
  std::vector<sessions::SerializedNavigationEntry> entries;
  entries.reserve(navigations.count);
  for (ArcSessionNavigation* navigation in navigations) {
    CHECK([navigation isKindOfClass:[ArcSessionNavigation class]]);
    CHECK([navigation chromeNavigation]);
    entries.push_back(*[navigation chromeNavigation]);
  }
  _navigations = navigations;
  _tab->navigations = std::move(entries);
}
- (NSString*)groupId { return _tab && _tab->group ? ArcNSString(_tab->group->ToString()) : nil; }
@end

ARC_EXPORT @interface ArcSessionTabGroup : NSObject
- (instancetype)initWithChromeSessionTabGroup:(std::shared_ptr<sessions::SessionTabGroup>)group
    __attribute__((objc_direct));
@property(nonatomic, readonly, copy) NSString* groupId;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly) NSInteger color;
@property(nonatomic, readonly) BOOL isCollapsed;
@end

@implementation ArcSessionTabGroup {
  std::shared_ptr<sessions::SessionTabGroup> _group;
}
- (instancetype)initWithChromeSessionTabGroup:(std::shared_ptr<sessions::SessionTabGroup>)group {
  if ((self = [super init])) {
    _group = std::move(group);
  }
  return self;
}
- (NSString*)groupId { return _group ? ArcNSString(_group->id.ToString()) : nil; }
- (NSString*)title { return _group ? ArcNSString(_group->visual_data.title()) : @""; }
- (NSInteger)color { return _group ? static_cast<NSInteger>(_group->visual_data.color()) : 0; }
- (BOOL)isCollapsed { return _group && _group->visual_data.is_collapsed(); }
@end

ARC_EXPORT @interface ArcSessionWindow : NSObject
- (instancetype)initWithChromeSessionWindow:(std::shared_ptr<sessions::SessionWindow>)window
    __attribute__((objc_direct));
@property(nonatomic, readonly) NSInteger windowType;
@property(nonatomic, readonly) NSArray<ArcSessionTab*>* tabs;
@property(nonatomic, readonly) int selectedTabIndex;
@property(nonatomic, readonly) NSDate* lastModified;
@property(nonatomic, readonly) NSInteger showState;
@property(nonatomic, readonly) NSArray<ArcSessionTabGroup*>* tabGroups;
@end

@implementation ArcSessionWindow {
  std::shared_ptr<sessions::SessionWindow> _window;
  NSArray<ArcSessionTab*>* _tabWrappers;
  NSArray<ArcSessionTabGroup*>* _groupWrappers;
}
- (instancetype)initWithChromeSessionWindow:(std::shared_ptr<sessions::SessionWindow>)window {
  if ((self = [super init])) {
    _window = std::move(window);
  }
  return self;
}
- (NSInteger)windowType { return _window ? static_cast<NSInteger>(_window->type) : 0; }
- (NSArray<ArcSessionTab*>*)tabs {
  if (!_tabWrappers) {
    NSMutableArray<ArcSessionTab*>* tabs = [NSMutableArray array];
    if (_window) {
      for (const auto& tab : _window->tabs) {
        // Aliasing ownership keeps the parent snapshot and this tab alive.
        std::shared_ptr<sessions::SessionTab> peer(_window, tab.get());
        [tabs addObject:[[ArcSessionTab alloc] initWithSessionTab:&peer]];
      }
    }
    _tabWrappers = [tabs copy];
  }
  // The original returns fresh mutable arrays; wrapper identity is canonical
  // under our architecture contract, including each tab's navigation cache.
  return [_tabWrappers mutableCopy];
}
- (int)selectedTabIndex { return _window ? _window->selected_tab_index : 0; }
- (NSDate*)lastModified { return _window ? SessionDate(_window->timestamp) : nil; }
- (NSInteger)showState {
  // ArcCore 1.170 arm64 lookup at 0xc9d8180: 0,1,2,3,4,5,0.
  static constexpr std::array<NSInteger, 7> states = {0, 1, 2, 3, 4, 5, 0};
  const auto index = _window ? static_cast<size_t>(_window->show_state) : 0;
  CHECK_LT(index, states.size());
  return states[index];
}
- (NSArray<ArcSessionTabGroup*>*)tabGroups {
  if (!_groupWrappers) {
    NSMutableArray<ArcSessionTabGroup*>* groups = [NSMutableArray array];
    if (_window) {
      for (const auto& group : _window->tab_groups) {
        [groups addObject:[[ArcSessionTabGroup alloc] initWithChromeSessionTabGroup:
            std::shared_ptr<sessions::SessionTabGroup>(_window, group.get())]];
      }
    }
    _groupWrappers = [groups copy];
  }
  return [_groupWrappers mutableCopy];
}
@end

@interface ArcBrowser (SessionsNative)
- (id)delegate;
- (void)restoreWebContentsWithSessionId:(ArcSessionID*)identifier;
@end

@protocol SessionsBrowserDelegate <NSObject>
- (void)browser:(ArcBrowser*)browser didRestoreWebContents:(ArcWebContents*)contents
    withSessionId:(ArcSessionID*)session;
@end

@protocol SessionsApplicationDelegate <NSObject>
- (void)didCloseBrowser:(NSString*)identifier forBrowserContextID:(ArcBrowserContextID*)context
    ofType:(NSInteger)type withRestorationSessionID:(ArcSessionID*)session;
@end

// Associated state is held only for the synchronous Chromium insertion scope;
// its absence leaves core's normal new-tab notification unchanged.
@interface SessionsInsertionState : NSObject
@property(nonatomic, strong) ArcSessionID* session;
@property(nonatomic, copy) NSArray* identifiers;
@property(nonatomic) NSUInteger next;
@end
@implementation SessionsInsertionState
@synthesize session = _session, identifiers = _identifiers, next = _next;
@end

namespace {
const char kSessionInsertionState = 0;

void QueueRestorationCallback(ArcBrowser* browser, content::WebContents* contents,
                              ArcSessionID* identifier, SessionsRestoreIDs* full) {
  __weak ArcBrowser* weakBrowser = browser;
  auto nativeBrowser = [browser browser]->GetWeakPtr();
  ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:contents];
  if (full) {
    full.pending += 1;
  }
  // Native OnTabStripModelChanged posts the dispatcher; it retains the Arc
  // contents peer, but neither the Chrome browser nor Chrome WebContents.
  // Delivery tests that peer's current binding, including a completed rebind.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    ArcBrowser* liveBrowser = weakBrowser;
    if (liveBrowser && nativeBrowser && [wrapper webContents]) {
      id<SessionsBrowserDelegate> delegate = [liveBrowser delegate];
      if ([delegate respondsToSelector:@selector(browser:didRestoreWebContents:withSessionId:)]) {
        [delegate browser:liveBrowser didRestoreWebContents:wrapper withSessionId:identifier];
      }
    } else if (full) {
      full.cancelled = YES;
    }
    if (full) {
      full.pending -= 1;
      CompleteFullRestore(full);
    }
  }));
}

class ScopedSessionInsertion {
 public:
  ScopedSessionInsertion(ArcBrowser* browser, ArcSessionID* session,
                         NSArray* identifiers = nil) {
    owner_ = browser;
    previous_ = objc_getAssociatedObject(owner_, &kSessionInsertionState);
    SessionsInsertionState* state = [[SessionsInsertionState alloc] init];
    state.session = session;
    state.identifiers = identifiers;
    objc_setAssociatedObject(owner_, &kSessionInsertionState, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  ~ScopedSessionInsertion() {
    objc_setAssociatedObject(owner_, &kSessionInsertionState, previous_, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
 private:
  ArcBrowser* __strong owner_;
  SessionsInsertionState* __strong previous_;
};

// Match the native restoration service, including descendants of windows,
// groups and split entries, and historical original IDs from prior sessions.
std::optional<SessionID> FindRestoreEntry(const sessions::tab_restore::Entry& entry, SessionID target) {
  if (entry.id == target || entry.original_id == target) {
    return entry.id;
  }
  const auto* tabs = [&]() -> const std::vector<std::unique_ptr<sessions::tab_restore::Tab>>* {
    switch (entry.type) {
      case sessions::tab_restore::TAB: return nullptr;
      case sessions::tab_restore::WINDOW:
        return &static_cast<const sessions::tab_restore::Window&>(entry).tabs;
      case sessions::tab_restore::GROUP:
        return &static_cast<const sessions::tab_restore::Group&>(entry).tabs;
      case sessions::tab_restore::SPLIT:
        return &static_cast<const sessions::tab_restore::Split&>(entry).tabs;
    }
    return nullptr;
  }();
  if (tabs) {
    for (const auto& tab : *tabs) {
      if (auto found = FindRestoreEntry(*tab, target)) {
        return found;
      }
    }
  }
  if (entry.type == sessions::tab_restore::WINDOW) {
    const auto& window = static_cast<const sessions::tab_restore::Window&>(entry);
    for (const auto& [id, group] : window.tab_groups) {
      if (auto found = FindRestoreEntry(*group, target)) {
        return found;
      }
    }
    for (const auto& [id, split] : window.split_tabs) {
      if (auto found = FindRestoreEntry(*split, target)) {
        return found;
      }
    }
  } else if (entry.type == sessions::tab_restore::GROUP) {
    for (const auto& [id, split] : static_cast<const sessions::tab_restore::Group&>(entry).split_tabs) {
      if (auto found = FindRestoreEntry(*split, target)) {
        return found;
      }
    }
  }
  return std::nullopt;
}

NSArray* RestorationIDsForEntry(const sessions::tab_restore::Entry& entry, SessionID target,
                                std::optional<SessionID> explicitRequest) {
  NSMutableArray* result = [NSMutableArray array];
  auto append = [&](const sessions::tab_restore::Tab& tab) {
    if (explicitRequest) {
      // Native tag2 matches this browser's requested ID against historical
      // first, then original; unmatched children retain ordinary didCreate.
      [result addObject:tab.id == *explicitRequest || tab.original_id == *explicitRequest
          ? [[ArcSessionID alloc] initWithID:explicitRequest->id()] : NSNull.null];
    } else {
      [result addObject:[[ArcSessionID alloc] initWithID:tab.original_id.id()]];
    }
  };
  bool whole = entry.id == target || entry.original_id == target;
  const std::vector<std::unique_ptr<sessions::tab_restore::Tab>>* tabs = nullptr;
  std::optional<tab_groups::TabGroupId> selectedGroup;
  std::optional<split_tabs::SplitTabId> selectedSplit;
  switch (entry.type) {
    case sessions::tab_restore::TAB: {
      const auto& tab = static_cast<const sessions::tab_restore::Tab&>(entry);
      append(tab);
      return result;
    }
    case sessions::tab_restore::WINDOW: {
      const auto& window = static_cast<const sessions::tab_restore::Window&>(entry);
      whole = whole || !window.app_name.empty();
      tabs = &window.tabs;
      if (!whole) {
        for (const auto& [id, group] : window.tab_groups) {
          if (group->id == target || group->original_id == target) { selectedGroup = id; }
        }
        for (const auto& [id, split] : window.split_tabs) {
          if (split->id == target || split->original_id == target) { selectedSplit = id; }
        }
      }
      break;
    }
    case sessions::tab_restore::GROUP: {
      const auto& group = static_cast<const sessions::tab_restore::Group&>(entry);
      tabs = &group.tabs;
      if (!whole) {
        for (const auto& [id, split] : group.split_tabs) {
          if (split->id == target || split->original_id == target) { selectedSplit = id; }
        }
      }
      break;
    }
    case sessions::tab_restore::SPLIT:
      tabs = &static_cast<const sessions::tab_restore::Split&>(entry).tabs;
      break;
  }
  for (const auto& tab : *tabs) {
    if (whole || tab->id == target || tab->original_id == target ||
        (selectedGroup && tab->group == selectedGroup) ||
        (selectedSplit && tab->split_id == selectedSplit)) {
      // Original AddRestoredTab passes Tab.original_id as the optional ID
      // consumed by the full-restoration didRestore callback, including -1.
      append(*tab);
    }
  }
  return result;
}

std::optional<SessionID> RestoreID(sessions::TabRestoreService* service, ArcSessionID* identifier) {
  if (!service || !identifier) {
    return std::nullopt;
  }
  const auto value = identifier.identifier;
  if (value <= 0 || value > std::numeric_limits<int32_t>::max()) {
    return std::nullopt;
  }
  const auto target = SessionID::FromSerializedValue(static_cast<int32_t>(value));
  for (const auto& entry : service->entries()) {
    // Public ArcBrowser canRestore/restore first scan only top-level entries
    // (native 0x42fae00/0x42faed0), unlike application-level restoreBrowser.
    if (entry->id == target || entry->original_id == target) {
      return entry->id;
    }
  }
  return std::nullopt;
}

void RememberRestorationIDs(const sessions::tab_restore::Entry& entry, std::set<SessionID>& identifiers) {
  identifiers.insert(entry.id);
  if (entry.type == sessions::tab_restore::TAB) {
    return;
  }
  const auto& tabs = [&]() -> const std::vector<std::unique_ptr<sessions::tab_restore::Tab>>& {
    switch (entry.type) {
      case sessions::tab_restore::WINDOW:
        return static_cast<const sessions::tab_restore::Window&>(entry).tabs;
      case sessions::tab_restore::GROUP:
        return static_cast<const sessions::tab_restore::Group&>(entry).tabs;
      case sessions::tab_restore::SPLIT:
        return static_cast<const sessions::tab_restore::Split&>(entry).tabs;
      case sessions::tab_restore::TAB: NOTREACHED();
    }
  }();
  for (const auto& tab : tabs) {
    RememberRestorationIDs(*tab, identifiers);
  }
  if (entry.type == sessions::tab_restore::WINDOW) {
    const auto& window = static_cast<const sessions::tab_restore::Window&>(entry);
    for (const auto& [id, group] : window.tab_groups) { RememberRestorationIDs(*group, identifiers); }
    for (const auto& [id, split] : window.split_tabs) { RememberRestorationIDs(*split, identifiers); }
  } else if (entry.type == sessions::tab_restore::GROUP) {
    for (const auto& [id, split] : static_cast<const sessions::tab_restore::Group&>(entry).split_tabs) {
      RememberRestorationIDs(*split, identifiers);
    }
  }
}

void PopulateOriginalTabIDs(sessions::tab_restore::Entry& entry, BrowserWindowInterface* browser,
                            const std::set<SessionID>& seen) {
  if (entry.type == sessions::tab_restore::TAB) {
    auto& tab = static_cast<sessions::tab_restore::Tab&>(entry);
    // A partial restore can promote an existing child to the root list. Its
    // close-time index is no longer a valid way to identify the original tab.
    if (seen.contains(tab.id) || tab.original_id.is_valid()) {
      return;
    }
    auto* tabs = browser->GetTabStripModel();
    if (tab.browser_id == browser->GetSessionID().id() && tab.tabstrip_index >= 0 &&
        tab.tabstrip_index < tabs->count()) {
      const auto live = sessions::SessionTabHelper::IdForTab(tabs->GetWebContentsAt(tab.tabstrip_index));
      // Arc's PopulateTab does this; stock Chromium leaves original_id empty.
      tab.original_id = live;
    }
    return;
  }
  const auto& tabs = [&]() -> const std::vector<std::unique_ptr<sessions::tab_restore::Tab>>& {
    switch (entry.type) {
      case sessions::tab_restore::WINDOW:
        return static_cast<sessions::tab_restore::Window&>(entry).tabs;
      case sessions::tab_restore::GROUP:
        return static_cast<sessions::tab_restore::Group&>(entry).tabs;
      case sessions::tab_restore::SPLIT:
        return static_cast<sessions::tab_restore::Split&>(entry).tabs;
      case sessions::tab_restore::TAB:
        NOTREACHED();
    }
  }();
  if (entry.type == sessions::tab_restore::WINDOW && !seen.contains(entry.id) &&
      !entry.original_id.is_valid() && std::ranges::any_of(tabs, [browser](const auto& tab) {
        return tab->browser_id == browser->GetSessionID().id();
      })) {
    // Native Arc's window producer records the browser's live ID separately
    // from the newly allocated historical window ID, just as PopulateTab does.
    entry.original_id = browser->GetSessionID();
  }
  for (const auto& tab : tabs) {
    PopulateOriginalTabIDs(*tab, browser, seen);
  }
}

class SessionsServiceObserver : public sessions::TabRestoreServiceObserver {
 public:
  explicit SessionsServiceObserver(ArcBrowser* browser) : browser_(browser) {
    Profile* profile = [browser browser]->GetProfile();
    if (!profile->IsOffTheRecord()) {
      if (auto* service = TabRestoreServiceFactory::GetForProfile(profile)) {
        for (const auto& entry : service->entries()) {
          RememberRestorationIDs(*entry, seen_entries_);
        }
        service_observation_.Observe(service);
      }
    }
  }
  void TabRestoreServiceChanged(sessions::TabRestoreService* service) override {
    BrowserWindowInterface* browser = [browser_ browser];
    if (!browser) {
      return;
    }
    std::set<SessionID> current;
    for (const auto& entry : service->entries()) {
      RememberRestorationIDs(*entry, current);
      if (!seen_entries_.contains(entry->id)) {
        // AddEntry notifies before native tab removal, so browser_id/index
        // identify the original live tab exactly, including window children.
        PopulateOriginalTabIDs(*entry, browser, seen_entries_);
      }
    }
    seen_entries_ = std::move(current);
  }
  void TabRestoreServiceDestroyed(sessions::TabRestoreService* service) override {
    service_observation_.Reset();
    seen_entries_.clear();
  }
 private:
  __weak ArcBrowser* browser_;
  std::set<SessionID> seen_entries_;
  base::ScopedObservation<sessions::TabRestoreService, sessions::TabRestoreServiceObserver>
      service_observation_{this};
};
}  // namespace

@interface SessionsBrowserObservation : NSObject
- (instancetype)initWithBrowser:(ArcBrowser*)browser;
- (void)browserDidClose:(BrowserWindowInterface*)browser;
@end
@implementation SessionsBrowserObservation {
  std::unique_ptr<SessionsServiceObserver> _observer;
  base::CallbackListSubscription _closeSubscription;
  __weak ArcBrowser* _browser;
}
- (instancetype)initWithBrowser:(ArcBrowser*)browser {
  if ((self = [super init])) {
    _browser = browser;
    _observer = std::make_unique<SessionsServiceObserver>(browser);
    __weak SessionsBrowserObservation* weakSelf = self;
    _closeSubscription = [browser browser]->RegisterBrowserDidClose(base::BindRepeating(
        ^(BrowserWindowInterface* native) { [weakSelf browserDidClose:native]; }));
  }
  return self;
}
- (void)browserDidClose:(BrowserWindowInterface*)browser {
  ArcBrowser* wrapper = _browser;
  Profile* profile = browser->GetProfile();
  ArcSessionID* historical = nil;
  if (!profile->IsOffTheRecord()) {
    auto* service = TabRestoreServiceFactory::GetForProfileIfExisting(profile);
    if (service && service->IsLoaded()) {
      // Native Arc's close producer matches root original_id, then reports
      // that entry's newly allocated historical ID (0x42febc8-0x42fec08).
      for (const auto& entry : service->entries()) {
        if (entry->original_id == browser->GetSessionID()) {
          historical = [[ArcSessionID alloc] initWithID:entry->id.id()];
          break;
        }
      }
    }
  }
  id<SessionsApplicationDelegate> delegate = ArcBrowserApplicationDelegateObject();
  NSInteger type = 0;
  switch (browser->GetType()) {
    case BrowserWindowInterface::TYPE_POPUP: type = 1; break;
    case BrowserWindowInterface::TYPE_APP: type = 2; break;
    case BrowserWindowInterface::TYPE_APP_POPUP: type = 3; break;
    default: break;
  }
  if ([delegate respondsToSelector:
          @selector(didCloseBrowser:forBrowserContextID:ofType:withRestorationSessionID:)]) {
    [delegate didCloseBrowser:wrapper.windowIdentifier
        forBrowserContextID:[ArcBrowserContext wrapperForProfile:profile].id
        ofType:type withRestorationSessionID:historical];
  }
}
@end

void ArcSessionsBrowserCreated(ArcBrowser* browser) {
  static const char kObservation = 0;
  if ([browser browser] && !objc_getAssociatedObject(browser, &kObservation)) {
    objc_setAssociatedObject(browser, &kObservation,
        [[SessionsBrowserObservation alloc] initWithBrowser:browser], OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    Profile* profile = [browser browser]->GetProfile();
    if (!profile->IsOffTheRecord()) {
      if (auto* service = TabRestoreServiceFactory::GetForProfile(profile)) {
        service->LoadTabsFromLastSession();
      }
    }
  }
}

BOOL ArcSessionsDidInsertWebContents(ArcBrowser* browser, content::WebContents* contents) {
  BrowserWindowInterface* native = [browser browser];
  if (!native) {
    return NO;
  }
  SessionsInsertionState* state = objc_getAssociatedObject(browser, &kSessionInsertionState);
  if (!state) {
    SessionsRestoreIDs* full = objc_getAssociatedObject(browser, &kFullRestoreIDs);
    if (!full || full.next >= full.identifiers.count) {
      return NO;
    }
    ArcSessionID* identifier = full.identifiers[full.next];
    full.next += 1;
    QueueRestorationCallback(browser, contents, identifier, full);
    return YES;
  }
  if (state.session) {
    ArcSessionID* identifier = state.session;
    if (state.next < state.identifiers.count) {
      id matched = state.identifiers[state.next];
      state.next += 1;
      if (matched == NSNull.null) {
        return NO;
      }
      identifier = matched;
    }
    // Only this requesting browser's matching inserted record is a restore;
    // native group redirection into another browser retains ordinary creation.
    QueueRestorationCallback(browser, contents, identifier, nil);
  }
  return YES;
}

@implementation ArcBrowser (Sessions)
- (BOOL)canRestoreWebContentsWithSessionId:(ArcSessionID*)identifier {
  BrowserWindowInterface* browser = [self browser];
  if (!browser || browser->GetProfile()->IsOffTheRecord()) {
    return NO;
  }
  if (objc_getAssociatedObject(self, &kFullBrowserRestoring)) {
    return NO;
  }
  auto* service = TabRestoreServiceFactory::GetForProfile(browser->GetProfile());
  return RestoreID(service, identifier).has_value();
}
- (void)restoreWebContentsWithSessionId:(ArcSessionID*)identifier {
  BrowserWindowInterface* browser = [self browser];
  if (!browser || browser->GetProfile()->IsOffTheRecord()) {
    return;
  }
  auto* service = TabRestoreServiceFactory::GetForProfile(browser->GetProfile());
  if (service && (service->IsRestoring() || objc_getAssociatedObject(self, &kFullBrowserRestoring) ||
      objc_getAssociatedObject([ArcBrowserContext wrapperForProfile:browser->GetProfile()],
                               &kActiveBrowserRestore))) {
    __weak ArcBrowser* weakBrowser = self;
    base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(FROM_HERE, base::BindOnce(^{
      [weakBrowser restoreWebContentsWithSessionId:identifier];
    }), base::Milliseconds(10));
    return;
  }
  const auto restore_id = RestoreID(service, identifier);
  auto* context = BrowserLiveTabContext::From(browser);
  if (!restore_id || !context) {
    return;
  }
  NSArray* callbackIDs = nil;
  for (const auto& entry : service->entries()) {
    if (FindRestoreEntry(*entry, *restore_id)) {
      callbackIDs = RestorationIDsForEntry(*entry, *restore_id,
          SessionID::FromSerializedValue(static_cast<int32_t>(identifier.identifier)));
      break;
    }
  }
  ScopedSessionInsertion insertion(self, identifier, callbackIDs);
  service->RestoreEntryById(context, *restore_id, WindowOpenDisposition::NEW_FOREGROUND_TAB);
}
- (ArcWebContents*)restoreForeignTab:(ArcSessionTab*)tab {
  BrowserWindowInterface* browser = [self browser];
  const auto* native = [tab chromeSessionTab];
  if (!browser || !native || native->navigations.empty()) {
    return nil;
  }
  // Unlike RestoreForeignSessionTab, AddRestoredTab supports an empty browser
  // and preserves group and last-active metadata rather than discarding it.
  ScopedSessionInsertion insertion(self, nil);
  content::WebContents* restored = chrome::AddRestoredTab(
      browser, native->navigations, browser->GetTabStripModel()->active_index() + 1,
      native->normalized_navigation_index(), native->extension_app_id, native->group,
      false, native->pinned, base::TimeTicks(), native->last_active_time, nullptr,
      native->user_agent_override, native->extra_data, false, std::nullopt);
  if (restored) {
    restored->GetController().LoadIfNecessary();
  }
  return [ArcWebContents wrapperForWebContents:restored];
}
@end
