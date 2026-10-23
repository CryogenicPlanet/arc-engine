// Copyright 2026 ArcMod. Browser identity, creation parameters and Arc-owned windows.

#import "arc/api/arc_internal.h"

#include "arc/browser/arc_browser_window.h"
#include "arc/browser/extensions_bridge.h"
#import <objc/message.h>
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_init_state.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/create_browser_window.h"
#include "components/sessions/core/session_id.h"
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

@interface ArcBrowser ()
- (BOOL)isInitialized;
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active __attribute__((objc_direct));
@end
@interface ArcWebContents (CoreTabClosing)
- (void)nativeContentsClosing __attribute__((objc_direct));
- (void)nativeContentsClosingWithRestorationID:(ArcSessionID*)identifier __attribute__((objc_direct));
@end

namespace {
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
    NSMutableArray<NSDictionary*>* changes = [NSMutableArray array];
    NSMutableArray<NSArray*>* closing = [NSMutableArray array];
    auto append = [&](content::WebContents* contents, NSString* kind) {
      ArcWebContents* tab = [ArcWebContents wrapperForWebContents:contents];
      if (tab) { [changes addObject:@{@"webContents": tab, @"change": kind}]; }
    };
    if (change.type() == TabStripModelChange::kInserted) {
      for (const auto& entry : change.GetInsert()->contents) {
        append(entry.contents.get(), @"inserted");
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
          [wrapper didInsertWebContents:native active:tab == activated];
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
  std::unique_ptr<ArcBrowserTabsObserver> _tabsObserver;
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
    ArcSessionsBrowserCreated(self);
  }
  return self;
}
- (void)setWindowIdentifier:(NSString*)identifier { _windowIdentifier = [identifier copy]; }
- (void)initializeWindowIdentifier:(NSString*)identifier { _windowIdentifier = [identifier copy]; }
- (BrowserWindowInterface*)browser { return _browser.get(); }
- (BOOL)isInitialized { return _browser && !_browser->IsDeleteScheduled(); }
- (NSInteger)type {
  if (!_browser) { return 0; }
  switch (_browser->GetType()) {
    case BrowserWindowInterface::TYPE_NORMAL: return 0;
    case BrowserWindowInterface::TYPE_POPUP: return 1;
    case BrowserWindowInterface::TYPE_APP: return 2;
    case BrowserWindowInterface::TYPE_APP_POPUP: return 3;
    default: return 0;
  }
}
- (ArcBrowserContext*)context { return _context; }
- (ArcBrowserCreateParams*)browserCreateParams { return _browserCreateParams; }
- (id)sidePanel { return _sidePanel; }
- (ArcSessionID*)sessionId {
  return _browser ? [[ArcSessionID alloc] initWithSessionId:&_browser->GetSessionID()] : nil;
}
- (void)setDelegate:(id)delegate {
  _delegate = delegate;
  arc::InstallBrowserExtensions(self, delegate);
  if (delegate) {
    [NSNotificationCenter.defaultCenter postNotificationName:kArcBrowserDidSetDelegateNotification object:self];
  }
}
- (id)delegate { return _delegate; }
- (void)setWindow:(NSWindow*)window {
  if (_browser) {
    static_cast<arc::ArcBrowserWindow*>(_browser->GetWindow())->SetNativeWindow(gfx::NativeWindow(window));
  }
}
- (BOOL)isEqual:(id)other { return self == other; }
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active {
  ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:contents];
  if (ArcSessionsDidInsertWebContents(self, contents)) {
    return;  // reported as a restore by the sessions package
  }
  if ([_delegate respondsToSelector:@selector(browser:didCreateNewWebContents:isActive:)]) {
    [_delegate browser:self didCreateNewWebContents:wrapper isActive:active];
  }
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
