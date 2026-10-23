// Copyright 2026 ArcMod. Session values and Chromium-backed tab restoration.

#import "arc/api/arc_internal.h"
#import <objc/runtime.h>

#include <array>
#include <limits>
#include <map>
#include <optional>

#include "base/check.h"
#include "base/functional/bind.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/ui/browser_live_tab_context.h"
#include "chrome/browser/ui/browser_tabrestore.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "components/sessions/content/session_tab_helper.h"
#include "components/sessions/core/session_types.h"
#include "components/sessions/core/tab_restore_service.h"
#include "components/sessions/core/tab_restore_types.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/web_contents.h"

namespace {
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
const char kSessionAliases[] = "ArcSessionAliases";

struct SessionAliases : base::SupportsUserData::Data {
  std::map<SessionID, SessionID> historical_ids;
};

SessionAliases& AliasesForProfile(Profile* profile) {
  auto* aliases = static_cast<SessionAliases*>(profile->GetUserData(kSessionAliases));
  if (!aliases) {
    auto owned = std::make_unique<SessionAliases>();
    aliases = owned.get();
    profile->SetUserData(kSessionAliases, std::move(owned));
  }
  return *aliases;
}

class ScopedSessionInsertion {
 public:
  ScopedSessionInsertion(ArcBrowser* browser, ArcSessionID* session, bool pause = false) {
    owner_ = [ArcBrowserContext wrapperForProfile:[browser browser]->GetProfile()];
    previous_ = objc_getAssociatedObject(owner_, &kSessionInsertionState);
    SessionsInsertionState* state = nil;
    if (!pause) {
      state = [[SessionsInsertionState alloc] init];
      state.session = session;
    }
    objc_setAssociatedObject(owner_, &kSessionInsertionState, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  ~ScopedSessionInsertion() {
    objc_setAssociatedObject(owner_, &kSessionInsertionState, previous_, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
 private:
  ArcBrowserContext* __strong owner_;
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
  return std::nullopt;
}

std::optional<SessionID> RestoreID(Profile* profile, sessions::TabRestoreService* service,
                                  ArcSessionID* identifier) {
  if (!service || !identifier) {
    return std::nullopt;
  }
  const auto value = identifier.identifier;
  if (value <= 0 || value > std::numeric_limits<int32_t>::max()) {
    return std::nullopt;
  }
  auto target = SessionID::FromSerializedValue(static_cast<int32_t>(value));
  const auto& aliases = AliasesForProfile(profile).historical_ids;
  if (auto found = aliases.find(target); found != aliases.end()) {
    target = found->second;
  }
  for (const auto& entry : service->entries()) {
    if (auto found = FindRestoreEntry(*entry, target)) {
      return found;
    }
  }
  return std::nullopt;
}

class SessionsTabsObserver : public TabStripModelObserver {
 public:
  explicit SessionsTabsObserver(ArcBrowser* browser) : browser_(browser) {
    // TabStripModelObserver tracks observed models and detaches on destruction.
    [browser browser]->GetTabStripModel()->AddObserver(this);
  }
  void OnTabStripModelChanged(TabStripModel* tabs, const TabStripModelChange& change,
                             const TabStripSelectionChange& selection) override {
    ArcBrowser* wrapper = browser_;
    BrowserWindowInterface* browser = [wrapper browser];
    if (!browser || change.type() != TabStripModelChange::kRemoved ||
        browser->GetProfile()->IsOffTheRecord()) {
      return;
    }
    Profile* profile = browser->GetProfile();
    auto& aliases = AliasesForProfile(profile).historical_ids;
    for (const auto& removed : change.GetRemove()->contents) {
      if (removed.session_id) {
        const auto live_id = sessions::SessionTabHelper::IdForTab(removed.contents.get());
        if (live_id.is_valid()) {
          aliases.insert_or_assign(live_id, *removed.session_id);
        }
      }
    }
    // Bound this cache to records still owned by the native restoration service.
    auto* service = TabRestoreServiceFactory::GetForProfile(profile);
    std::erase_if(aliases, [service](const auto& alias) {
      if (service) {
        for (const auto& entry : service->entries()) {
          if (FindRestoreEntry(*entry, alias.second)) {
            return false;
          }
        }
      }
      return true;
    });
  }
 private:
  __weak ArcBrowser* browser_;
};
}  // namespace

@interface SessionsBrowserObservation : NSObject
- (instancetype)initWithBrowser:(ArcBrowser*)browser;
- (void)browserDidClose:(BrowserWindowInterface*)browser;
@end
@implementation SessionsBrowserObservation {
  std::unique_ptr<SessionsTabsObserver> _observer;
  base::CallbackListSubscription _closeSubscription;
  __weak ArcBrowser* _browser;
}
- (instancetype)initWithBrowser:(ArcBrowser*)browser {
  if ((self = [super init])) {
    _browser = browser;
    _observer = std::make_unique<SessionsTabsObserver>(browser);
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
    if (service) {
      // BrowserClosing already wrote the native historical window before any
      // tabs were removed. Its children retain the exact originating browser
      // ID, whereas the historical window gets a fresh restoration ID.
      for (const auto& entry : service->entries()) {
        if (entry->type != sessions::tab_restore::WINDOW) {
          continue;
        }
        const auto& window = static_cast<const sessions::tab_restore::Window&>(*entry);
        if (std::ranges::any_of(window.tabs, [browser](const auto& tab) {
              return tab->browser_id == browser->GetSessionID().id();
            })) {
          historical = [[ArcSessionID alloc] initWithID:entry->id.id()];
          break;
        }
      }
    }
  }
  id<SessionsApplicationDelegate> delegate = ArcBrowserApplicationDelegateObject();
  if ([delegate respondsToSelector:
          @selector(didCloseBrowser:forBrowserContextID:ofType:withRestorationSessionID:)]) {
    [delegate didCloseBrowser:wrapper.windowIdentifier
        forBrowserContextID:[ArcBrowserContext wrapperForProfile:profile].id
        ofType:static_cast<NSInteger>(browser->GetType()) withRestorationSessionID:historical];
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
  ArcBrowserContext* owner = [ArcBrowserContext wrapperForProfile:native->GetProfile()];
  SessionsInsertionState* state = objc_getAssociatedObject(owner, &kSessionInsertionState);
  if (!state) {
    return NO;
  }
  if (state.session) {
    // A grouped native restore may insert into a different browser in this
    // profile. Pause before calling app code so unrelated reentrant new tabs
    // keep their normal creation callback.
    ScopedSessionInsertion pause(browser, nil, true);
    id<SessionsBrowserDelegate> delegate = [browser delegate];
    if ([delegate respondsToSelector:@selector(browser:didRestoreWebContents:withSessionId:)]) {
      [delegate browser:browser didRestoreWebContents:[ArcWebContents wrapperForWebContents:contents]
          withSessionId:state.session];
    }
  }
  return YES;
}

@implementation ArcBrowser (Sessions)
- (BOOL)canRestoreWebContentsWithSessionId:(ArcSessionID*)identifier {
  BrowserWindowInterface* browser = [self browser];
  if (!browser || browser->GetProfile()->IsOffTheRecord()) {
    return NO;
  }
  auto* service = TabRestoreServiceFactory::GetForProfile(browser->GetProfile());
  return RestoreID(browser->GetProfile(), service, identifier).has_value();
}
- (void)restoreWebContentsWithSessionId:(ArcSessionID*)identifier {
  BrowserWindowInterface* browser = [self browser];
  if (!browser || browser->GetProfile()->IsOffTheRecord()) {
    return;
  }
  auto* service = TabRestoreServiceFactory::GetForProfile(browser->GetProfile());
  const auto restore_id = RestoreID(browser->GetProfile(), service, identifier);
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
      native->user_agent_override, native->extra_data, true, std::nullopt);
  if (restored) {
    restored->GetController().LoadIfNecessary();
  }
  return [ArcWebContents wrapperForWebContents:restored];
}
@end
