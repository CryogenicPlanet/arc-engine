// Copyright 2026 ArcMod. Browser identity, creation parameters and Arc-owned windows.

#import "arc/api/arc_internal.h"

#include "arc/browser/arc_browser_window.h"
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

@implementation ArcSessionID {
  int32_t _identifier;
}
- (instancetype)initWithID:(NSInteger)identifier {
  if ((self = [super init])) {
    _identifier = static_cast<int32_t>(identifier);
  }
  return self;
}
- (NSInteger)identifier {
  return _identifier;
}
- (NSUInteger)hash {
  return static_cast<NSUInteger>(static_cast<NSInteger>(_identifier));
}
- (BOOL)isEqual:(id)other {
  return self == other || ([other isKindOfClass:[ArcSessionID class]] &&
                           _identifier == [(ArcSessionID*)other identifier]);
}
@end

ARC_EXPORT @interface ArcBrowserCreateParams : NSObject
- (instancetype)initWithBrowserCreateParams:(const void*)params;
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

@interface ArcBrowser (NativeTabs)
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active;
@end

namespace {
class ArcBrowserTabsObserver : public TabStripModelObserver {
 public:
  ArcBrowserTabsObserver(TabStripModel* tabs, ArcBrowser* wrapper) : wrapper_(wrapper) {
    tabs->AddObserver(this);
  }
  void OnTabStripModelChanged(TabStripModel* tabs, const TabStripModelChange& change,
                             const TabStripSelectionChange& selection) override {
    if (change.type() == TabStripModelChange::kInserted) {
      for (const auto& entry : change.GetInsert()->contents) {
        [wrapper_ didInsertWebContents:entry.contents.get() active:entry.contents == selection.new_contents];
      }
    }
  }
 private:
  __weak ArcBrowser* wrapper_;
};
const char kArcBrowserKey[] = "ArcBrowser";
struct ArcBrowserHolder : base::SupportsUserData::Data {
  ArcBrowser* __strong wrapper;
};
}  // namespace

@interface ArcBrowser ()
- (instancetype)initWithChromeBrowser:(BrowserWindowInterface*)browser;
- (void)takeOwnershipOfWebContents:(ArcWebContents*)contents;
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
    _sidePanel = [[NSClassFromString(@"ArcBrowserSidePanel") alloc] init];
    _windowIdentifier = @"";
    _tabsObserver = std::make_unique<ArcBrowserTabsObserver>(browser->GetTabStripModel(), self);
  }
  return self;
}
- (void)setWindowIdentifier:(NSString*)identifier { _windowIdentifier = [identifier copy] ?: @""; }
- (BrowserWindowInterface*)browser { return _browser.get(); }
- (BOOL)isInitialized { return !!_browser; }
- (NSInteger)type { return _browser ? static_cast<NSInteger>(_browser->GetType()) : 0; }
- (ArcBrowserContext*)context { return _context; }
- (ArcBrowserCreateParams*)browserCreateParams { return _browserCreateParams; }
- (id)sidePanel { return _sidePanel; }
- (ArcSessionID*)sessionId {
  return _browser ? [[ArcSessionID alloc] initWithID:_browser->GetSessionID().id()] : nil;
}
- (void)setDelegate:(id)delegate { _delegate = delegate; }
- (void)setWindow:(NSWindow*)window {
  if (_browser) {
    static_cast<arc::ArcBrowserWindow*>(_browser->GetWindow())->SetNativeWindow(gfx::NativeWindow(window));
  }
}
- (BOOL)isEqual:(id)other { return self == other; }
- (void)didInsertWebContents:(content::WebContents*)contents active:(BOOL)active {
  ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:contents];
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
  browser.windowIdentifier = identifier ?: @"";
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
