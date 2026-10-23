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
#include "base/supports_user_data.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/apps/app_service/web_contents_app_id_utils.h"
#include "arc/browser/arc_browser_window.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
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
#include "ui/base/unowned_user_data/scoped_unowned_user_data.h"

@interface SessionsRestoreIDs : NSObject
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
    __attribute__((objc_precise_lifetime)) SessionsDiscardAdapter* selectedAdapter = weakAdapter;
    ArcWebContents* contents = weakContents;
    id<SessionsDiscardDelegate> delegate = selectedAdapter.delegate;
    if (contents && delegate &&
        [delegate respondsToSelector:@selector(webContents:finalizeDiscardRequest:)]) {
      [delegate webContents:contents finalizeDiscardRequest:discarded];
    }
    state.pending -= 1;
  }));
}
@end
@implementation SessionsRestoreIDs
@synthesize pending = _pending,
    sealed = _sealed, cancelled = _cancelled, completion = _completion;
@end

namespace {
std::optional<SessionID> FindRestoreEntry(const sessions::tab_restore::Entry& entry, SessionID target);
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
- (void)performNativeCreatedBrowserRestore:(sessions::TabRestoreService*)service identifier:(SessionID)identifier;
- (arcmod::SessionBrowserCreatedCallback)prepareFactory:(Profile*)profile params:(BrowserWindowCreateParams&)params;
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
  BOOL _factoryPending;
  ArcBrowser* _factoryWrapper;
  SessionsRestoreIDs* _factoryIDs;
  base::WeakPtr<BrowserWindowInterface> _factoryNative;
  std::unique_ptr<ScopedFullRestoreIDs> _factoryInsertion;
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
  _factoryInsertion.reset();
  _factoryIDs = nil;
  _factoryWrapper = nil;
}
- (arcmod::SessionBrowserCreatedCallback)prepareFactory:(Profile*)profile params:(BrowserWindowCreateParams&)params {
  if (!_factoryPending || ArcProfileFrom(_context) != profile || params.window) {
    return {};
  }
  _factoryPending = NO; // Consume before any app callback can reenter.
  params.window = new arc::ArcBrowserWindow();
  params.should_trigger_session_restore = false;
  params.creation_source = BrowserWindowCreateParams::CreationSource::kSessionRestore;
  return base::BindOnce(^(base::WeakPtr<BrowserWindowInterface> native) {
    if (!native || native->IsDeleteScheduled() || ArcProfileFrom(self->_context) != profile) {
      return;
    }
    self->_factoryNative = native;
    ArcBrowser* wrapper = [ArcBrowser wrapperForBrowser:native.get()];
    self->_factoryWrapper = wrapper;
    self->_restoringBrowser = wrapper;
    objc_setAssociatedObject(wrapper, &kFullBrowserRestoring, self, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    self->_factoryInsertion = std::make_unique<ScopedFullRestoreIDs>(wrapper, self->_factoryIDs);
    if (self->_created) {
      self->_created(wrapper);
    }
  });
}
- (void)performNativeCreatedBrowserRestore:(sessions::TabRestoreService*)service identifier:(SessionID)identifier {
  static const bool installed = [] {
    arcmod::GetSessionBrowserFactoryHook() = base::BindRepeating(
        [](Profile* profile, BrowserWindowCreateParams& params) -> arcmod::SessionBrowserCreatedCallback {
          ArcBrowserContext* owner = [ArcBrowserContext wrapperForProfile:profile];
          SessionsRestoreRequest* request = objc_getAssociatedObject(owner, &kActiveBrowserRestore);
          return request ? [request prepareFactory:profile params:params] : arcmod::SessionBrowserCreatedCallback();
        });
    return true;
  }();
  (void)installed;
  __attribute__((objc_precise_lifetime)) SessionsRestoreRequest* keepAlive = self;
  _activeRestore = std::make_unique<ScopedBrowserRestore>(_context, self);
  _factoryIDs = [[SessionsRestoreIDs alloc] init];
  _factoryPending = YES;
  const auto result = service->RestoreEntryById(nullptr, identifier, WindowOpenDisposition::NEW_FOREGROUND_TAB);
  _factoryPending = NO;
  _factoryInsertion.reset();
  _activeRestore.reset();
  SessionsRestoreIDs* state = _factoryIDs;
  ArcBrowser* wrapper = _factoryWrapper;
  auto weakBrowser = _factoryNative;
  const bool succeeded = result.has_value();
  __weak SessionsRestoreIDs* weakState = state;
  state.completion = ^{
    [keepAlive finish:succeeded && !weakState.cancelled && weakBrowser &&
        !weakBrowser->IsDeleteScheduled() ? wrapper : nil];
  };
  state.sealed = YES;
  // Even zero-insertion/cancelled completions run after the API unwinds.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{ CompleteFullRestore(state); }));
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
  const bool matchesRoot = *matchedID == selected->id || *matchedID == selected->original_id;
  const bool createsNativeTabBrowser = selected->type == sessions::tab_restore::TAB &&
      !static_cast<const sessions::tab_restore::Tab&>(*selected).group.has_value();
  if (matchesRoot && (selected->type == sessions::tab_restore::WINDOW || createsNativeTabBrowser)) {
    // NEW_FOREGROUND_TAB + nil context ignores the old TAB browser_id; an
    // ungrouped root TAB therefore reaches native creation/remapping. A grouped
    // TAB may redirect into existing B and has different original completion
    // behavior, so do not arm this request for that unresolved destination.
    [keepAlive performNativeCreatedBrowserRestore:service identifier:*matchedID];
    return;
  }
  // Grouped TAB/GROUP/partial WINDOW paths remain preexisting and uncovered.
  // Reserve the profile before creating the browser or invoking app code.
  // A nested request must not consume the native entry that this request uses.
  _activeRestore = std::make_unique<ScopedBrowserRestore>(_context, self);
  const SessionID restoreID = *matchedID;
  SessionsRestoreIDs* callbackIDs = [[SessionsRestoreIDs alloc] init];
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
  // Native publishes the retained array before enumerating it, then rejects an
  // empty collected history before replacing its native vector.
  _navigations = navigations;
  std::vector<sessions::SerializedNavigationEntry> entries;
  for (ArcSessionNavigation* navigation in navigations) {
    CHECK([navigation isKindOfClass:[ArcSessionNavigation class]]);
    CHECK([navigation chromeNavigation]);
    entries.push_back(*[navigation chromeNavigation]);
  }
  CHECK_GT(entries.size(), 0u);
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
@end
@implementation SessionsInsertionState
@synthesize session = _session;
@end

namespace {
const char kSessionInsertionState = 0;
const char kNativeRestorationSource = 0;

struct SessionsInsertionSource : public base::SupportsUserData::Data {
  int32_t historical;
  int32_t original;
};

base::ScopedClosureRunner NativeRestorationSourceScope(content::WebContents* contents,
                                                      int32_t historical, int32_t original) {
  auto previous = contents->TakeUserData(&kNativeRestorationSource);
  auto source = std::make_unique<SessionsInsertionSource>();
  source->historical = historical;
  source->original = original;
  contents->SetUserData(&kNativeRestorationSource, std::move(source));
  return base::ScopedClosureRunner(base::BindOnce(
      [](base::WeakPtr<content::WebContents> native,
         std::unique_ptr<base::SupportsUserData::Data> previous) {
        if (!native) {
          return;
        }
        if (previous) {
          native->SetUserData(&kNativeRestorationSource, std::move(previous));
        } else {
          native->RemoveUserData(&kNativeRestorationSource);
        }
      }, contents->GetWeakPtr(), std::move(previous)));
}

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
  ScopedSessionInsertion(ArcBrowser* browser, ArcSessionID* session) {
    owner_ = browser;
    previous_ = objc_getAssociatedObject(owner_, &kSessionInsertionState);
    SessionsInsertionState* state = [[SessionsInsertionState alloc] init];
    state.session = session;
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

namespace {
const char kPhysicalCloseObservation = 0;

// Owned by the existing ArcBrowserWindow, not by the ObjC observation: an
// externally retained facade can outlive Browser's UnownedUserDataHost.
class SessionsPhysicalCloseData final : public base::SupportsUserData::Data {
 public:
  DECLARE_USER_DATA(SessionsPhysicalCloseData);

  SessionsPhysicalCloseData(BrowserWindowInterface* browser,
                            SessionsBrowserObservation* observation)
      : observation_(observation),
        registration_(std::make_unique<
            ui::ScopedUnownedUserData<SessionsPhysicalCloseData>>(
            browser->GetUnownedUserDataHost(), *this)) {}

  void Notify(BrowserWindowInterface* browser) {
    // Consume only this transport registration before a deletion-capable
    // callout. This is not a substitute for application registry cleanup.
    __attribute__((objc_precise_lifetime)) SessionsBrowserObservation*
        observation = observation_;
    registration_.reset();
    [observation browserDidClose:browser];
    // The callout can delete the window and this Data; do not access either.
  }

 private:
  __weak SessionsBrowserObservation* observation_;
  std::unique_ptr<ui::ScopedUnownedUserData<SessionsPhysicalCloseData>>
      registration_;
};
DEFINE_USER_DATA(SessionsPhysicalCloseData);

void NotifySessionsPhysicalClose(BrowserWindowInterface* browser) {
  if (auto* data =
          SessionsPhysicalCloseData::Get(browser->GetUnownedUserDataHost())) {
    data->Notify(browser);
  }
}
}  // namespace

@implementation SessionsBrowserObservation {
  std::unique_ptr<SessionsServiceObserver> _observer;
  __weak ArcBrowser* _browser;
}
- (instancetype)initWithBrowser:(ArcBrowser*)browser {
  if ((self = [super init])) {
    _browser = browser;
    _observer = std::make_unique<SessionsServiceObserver>(browser);
    BrowserWindowInterface* native = [browser browser];
    // This is the existing canonical ArcBrowser initialization path; the
    // destructor hook itself never casts a window or creates a facade.
    auto* window = static_cast<arc::ArcBrowserWindow*>(native->GetWindow());
    window->SetUserData(&kPhysicalCloseObservation,
        std::make_unique<SessionsPhysicalCloseData>(native, self));
  }
  return self;
}
- (void)browserDidClose:(BrowserWindowInterface*)browser {
  // Keep the EXISTING facade through payload capture, publication and finish.
  ArcBrowser* existing __attribute__((objc_precise_lifetime)) = _browser;
  Profile* profile = browser->GetProfile();
  std::optional<int32_t> historicalID;
  if (!profile->IsOffTheRecord()) {
    auto* service = TabRestoreServiceFactory::GetForProfileIfExisting(profile);
    if (service && service->IsLoaded()) {
      // Preserve root original_id matching and optional numeric presence.
      for (const auto& entry : service->entries()) {
        if (entry->original_id == browser->GetSessionID()) {
          historicalID = entry->id.id();
          break;
        }
      }
    }
  }
  NSInteger type = 0;
  switch (browser->GetType()) {
    case BrowserWindowInterface::TYPE_POPUP: type = 1; break;
    case BrowserWindowInterface::TYPE_APP: type = 2; break;
    case BrowserWindowInterface::TYPE_APP_POPUP: type = 3; break;
    default: break;
  }
  NSString* windowIdentifier = existing.windowIdentifier;
  ArcBrowserContextID* contextID = [ArcBrowserContext wrapperForProfile:profile].id;

  // Empty output is required: replacing an old closure could drain it early.
  base::OnceClosure afterPublication;
  const bool prepared = arc::PrepareBrowserForNativeDestruction(
      existing, browser, &afterPublication);
  if (!prepared) {
    // Private adapter mismatch policy, not original registry eligibility proof.
    return;
  }
  // Only captured payload is used from here; no native/Profile/facade queries.
  {
    id<SessionsApplicationDelegate> selected
        __attribute__((objc_precise_lifetime)) = ArcBrowserApplicationDelegateObject();
    ArcSessionID* historical = historicalID
        ? [[ArcSessionID alloc] initWithID:*historicalID] : nil;
    if ([selected respondsToSelector:
            @selector(didCloseBrowser:forBrowserContextID:ofType:withRestorationSessionID:)]) {
      [selected didCloseBrowser:windowIdentifier forBrowserContextID:contextID
          ofType:type withRestorationSessionID:historical];
    }
  }  // Release the selected delegate before close-state completion callbacks.
  if (afterPublication) {
    std::move(afterPublication).Run();
  }
}
@end

void ArcSessionsBrowserCreated(ArcBrowser* browser) {
  static const bool sourceHookInstalled = [] {
    arcmod::GetSessionTabRestoreScopeHook() = base::BindRepeating(&NativeRestorationSourceScope);
    arcmod::GetSessionBrowserPhysicalDestructionHook() =
        base::BindRepeating(&NotifySessionsPhysicalClose);
    return true;
  }();
  (void)sourceHookInstalled;
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
  if (state && !state.session) {
    // Foreign restoration explicitly suppresses every insertion notification.
    return YES;
  }
  const auto* source = static_cast<SessionsInsertionSource*>(
      contents->GetUserData(&kNativeRestorationSource));
  if (!source) {
    // Saved-group expansion/movement and unrelated app-created tabs carry no
    // historical source optional, even during full restoration.
    return NO;
  }
  SessionsRestoreIDs* full = objc_getAssociatedObject(browser, &kFullRestoreIDs);
  ArcSessionID* identifier = nil;
  if (full) {
    // Full tag1 uses original_id even when the present optional contains -1.
    identifier = [[ArcSessionID alloc] initWithID:source->original];
  } else if (state.session && (state.session.identifier == source->historical ||
                              state.session.identifier == source->original)) {
    // Explicit tag2 chooses the matched requested ID, historical before
    // original. State belongs to this destination browser, not its profile.
    identifier = state.session;
  } else {
    return NO;
  }
  QueueRestorationCallback(browser, contents, identifier, full);
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
  ScopedSessionInsertion insertion(self, identifier);
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
