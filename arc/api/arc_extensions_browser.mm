// Copyright 2026 ArcMod. Chrome owns extension execution; Arc's app owns its UI.
#import "arc/browser/extensions_bridge.h"
#import "arc/browser/extensions_values.h"
#import "arc/browser/extensions_context.h"

#include <map>
#include <memory>
#include <utility>

#include "base/functional/bind.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/task/single_thread_task_runner.h"
#include "base/numerics/safe_conversions.h"
#include "arc/browser/arc_browser_window.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "chrome/browser/extensions/api/omnibox/omnibox_api.h"
#include "chrome/browser/extensions/extension_view.h"
#include "chrome/browser/extensions/extension_view_host.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/browser_window/public/profile_browser_collection.h"
#include "chrome/browser/ui/extensions/extension_action_delegate.h"
#include "chrome/browser/ui/extensions/extension_action_view_model.h"
#include "chrome/browser/ui/extensions/extensions_container.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/toolbar/toolbar_actions_model.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "components/sessions/content/session_tab_helper.h"
#include "extensions/common/permissions/permission_message_provider.h"
#include "extensions/common/permissions/permission_set.h"
#include "ui/base/models/image_model.h"
#include "ui/base/models/menu_model.h"
#include "ui/base/unowned_user_data/scoped_unowned_user_data.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/native_theme/native_theme.h"

@protocol ArcExtensionsBrowserDelegate <NSObject>
- (void)browser:(ArcBrowser*)browser didUpdateExtensionToolbarActions:(NSArray*)actions;
- (void)browser:(ArcBrowser*)browser didUpdateExtensionToolbarAction:(ArcExtensionToolbarAction*)action;
- (void)browser:(ArcBrowser*)browser didOpenExtensionPopup:(NSString*)extensionID webContents:(ArcWebContents*)contents;
- (void)browser:(ArcBrowser*)browser didCloseExtensionPopup:(NSString*)extensionID;
- (void)browser:(ArcBrowser*)browser didResizeExtensionPopup:(NSString*)extensionID size:(CGSize)size;
- (long long)browser:(ArcBrowser*)browser extensionGroupIdForTabWithSessionId:(long long)identifier;
- (void)browser:(ArcBrowser*)browser didRequestExtensionPermissions:(ArcExtensionInfo*)info completion:(void (^)(BOOL))completion;
@end

namespace arc {
namespace {
class BrowserExtensions;

// The host remains the WebContentsDelegate. This view only supplies Arc's native
// view and size notifications; replacing the host delegate loses extension IPC.
class ActionDelegate : public ExtensionActionDelegate,
                       public extensions::ExtensionView {
 public:
  ActionDelegate(ArcBrowser* browser, id delegate, BrowserExtensions* container)
      : browser_(browser), delegate_(delegate), container_(container) {}
  ~ActionDelegate() override { delegate_ = nil; HidePopup(); }
  void AttachToModel(ExtensionActionViewModel* model) override { model_ = model; }
  void DetachFromModel() override { HidePopup(); model_ = nullptr; }
  void RegisterCommand() override {}
  void UnregisterCommand() override {}
  bool IsShowingPopup() const override { return !!host_; }
  gfx::NativeView GetPopupNativeView() override { return GetNativeView(); }
  gfx::NativeView GetNativeView() override {
    return host_ ? host_->host_contents()->GetNativeView() : gfx::NativeView();
  }
  void HidePopup() override {
    if (!host_) return;
    weak_factory_.InvalidateWeakPtrs();
    ShowPopupCallback pending = std::move(pending_callback_);
    NSString* identifier = ArcNSString(model_->GetId());
    host_->set_view(nullptr);
    host_.reset();
    id<ArcExtensionsBrowserDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browser:didCloseExtensionPopup:)])
      [delegate browser:browser_ didCloseExtensionPopup:identifier];
    if (pending) std::move(pending).Run(nullptr);
  }
  void TriggerPopup(std::unique_ptr<extensions::ExtensionViewHost> host,
                    PopupShowAction show_action, bool by_user,
                    ShowPopupCallback callback) override;
  void ShowContextMenuAsFallback() override;
  void CloseExtensionsMenuIfOpen() override {}
  void ResizeDueToAutoResize(content::WebContents* contents,
                             const gfx::Size& size) override {
    id<ArcExtensionsBrowserDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browser:didResizeExtensionPopup:size:)])
      [delegate browser:browser_ didResizeExtensionPopup:ArcNSString(model_->GetId())
                   size:CGSizeMake(size.width(), size.height())];
  }
  void RenderFrameCreated(content::RenderFrameHost* frame) override {
    if (auto* view = frame->GetView())
      view->EnableAutoResize(extensions::kExtensionPopupMinSize,
                            extensions::kExtensionPopupMaxSize);
  }
  bool HandleKeyboardEvent(content::WebContents*, const input::NativeWebKeyboardEvent&) override { return false; }
  void OnLoaded() override {
    if (!host_) return;
    host_->host_contents()->Focus();
    auto weak = weak_factory_.GetWeakPtr();
    ShowPopupCallback callback = std::move(pending_callback_);
    if (show_action_ == PopupShowAction::kShowAndInspect)
      DevToolsWindow::OpenDevToolsWindow(host_->host_contents(),
          DevToolsToggleAction::ShowConsolePanel(),
          DevToolsOpenedByAction::kContextMenuInspect);
    if (callback) std::move(callback).Run(weak ? weak->host_.get() : nullptr);
  }
  void ShowContextMenu();
  base::WeakPtr<ActionDelegate> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }
 private:
  __weak ArcBrowser* browser_;
  __weak id<ArcExtensionsBrowserDelegate> delegate_;
  raw_ptr<BrowserExtensions> container_;
  raw_ptr<ExtensionActionViewModel> model_ = nullptr;
  std::unique_ptr<extensions::ExtensionViewHost> host_;
  ShowPopupCallback pending_callback_;
  PopupShowAction show_action_ = PopupShowAction::kShow;
  base::WeakPtrFactory<ActionDelegate> weak_factory_{this};
};

class BrowserExtensions : public base::SupportsUserData::Data,
                          public ExtensionsContainer,
                          public ToolbarActionsModel::Observer,
                          public TabStripModelObserver {
 public:
  BrowserExtensions(ArcBrowser* wrapper, id delegate)
      : browser_([wrapper browser]->GetWeakPtr()), wrapper_(wrapper),
        delegate_(delegate), scoped_data_(browser_->GetUnownedUserDataHost(), *this) {
    model_ = ToolbarActionsModel::Get(browser_->GetProfile());
    if (model_) model_->AddObserver(this);
    browser_->GetTabStripModel()->AddObserver(this);
    close_ = browser_->RegisterBrowserDidClose(base::BindRepeating(
        [](base::WeakPtr<BrowserExtensions> self, BrowserWindowInterface*) {
          if (self) self->Shutdown();
        }, weak_factory_.GetWeakPtr()));
    ScheduleSnapshot();
  }
  ~BrowserExtensions() override { Shutdown(); }
  void Shutdown() {
    weak_factory_.InvalidateWeakPtrs();
    TabStripModelObserver::StopObservingAll(this);
    icon_subscriptions_.clear();
    auto actions = std::move(actions_);
    actions.clear();
    if (model_) model_->RemoveObserver(this);
    model_ = nullptr;
  }
  void SetDelegate(id delegate) {
    // Existing action delegates are weak, so rebuild them on delegate replacement.
    icon_subscriptions_.clear();
    auto actions = std::move(actions_);
    actions.clear(); delegate_ = delegate; ScheduleSnapshot();
  }
  id GetDelegate() const { return delegate_; }
  ToolbarActionViewModel* GetActionForId(const std::string& id) override {
    if (!browser_ || !model_ || !model_->HasAction(id)) return nullptr;
    auto found = actions_.find(id);
    if (found == actions_.end()) {
      auto action = ExtensionActionViewModel::Create(id, browser_.get(),
          std::make_unique<ActionDelegate>(wrapper_, delegate_, this));
      if (!action) return nullptr;
      found = actions_.emplace(id, std::move(action)).first;
      icon_subscriptions_.emplace(id, found->second->RegisterIconUpdateObserver(
          base::BindRepeating([](base::WeakPtr<BrowserExtensions> self, std::string identifier) {
            if (!self) return;
            base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE,
                base::BindOnce(&BrowserExtensions::OnToolbarActionUpdated, self, identifier));
          }, weak_factory_.GetWeakPtr(), id)));
    }
    return found->second.get();
  }
  void HideActivePopup() override {
    std::vector<base::WeakPtr<ActionDelegate>> delegates;
    for (auto& [id, action] : actions_)
      delegates.push_back(static_cast<ActionDelegate*>(action->delegate())->GetWeakPtr());
    for (auto& delegate : delegates) if (delegate) delegate->HidePopup();
  }
  void CloseExtensionsMenuIfOpen() override {}
  bool ShowToolbarActionPopupForAPICall(const std::string& id,
                                        ShowPopupCallback callback) override {
    auto* action = GetActionForId(id);
    if (!action) { std::move(callback).Run(nullptr); return false; }
    action->TriggerPopupForAPI(std::move(callback)); return true;
  }
  void ToggleExtensionsMenu() override {}
  bool HasAnyExtensions() const override { return model_ && !model_->action_ids().empty(); }
  void OnToolbarActionAdded(const std::string&) override { ScheduleSnapshot(); }
  void OnToolbarActionRemoved(const std::string& id) override {
    icon_subscriptions_.erase(id); actions_.erase(id); ScheduleSnapshot();
  }
  void OnToolbarActionUpdated(const std::string& action_id) override {
    id<ArcExtensionsBrowserDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browser:didUpdateExtensionToolbarAction:)]) {
      ArcExtensionToolbarAction* snapshot = Snapshot(action_id);
      if (snapshot) [delegate browser:wrapper_ didUpdateExtensionToolbarAction:snapshot];
    }
  }
  void OnToolbarModelInitialized() override { ScheduleSnapshot(); }
  void OnTabStripModelChanged(TabStripModel*, const TabStripModelChange&,
                             const TabStripSelectionChange& selection) override {
    if (selection.active_tab_changed()) ScheduleActionUpdates();
  }
  void OnTabChangedAt(tabs::TabInterface*, TabChangeType) override {
    ScheduleActionUpdates();
  }
  void ScheduleActionUpdates() {
    if (!model_) return;
    for (const auto& identifier : model_->action_ids())
      base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE,
          base::BindOnce(&BrowserExtensions::OnToolbarActionUpdated,
                         weak_factory_.GetWeakPtr(), identifier));
  }
  void OnToolbarPinnedActionsChanged() override { ScheduleSnapshot(); }
  void OnToolbarActionsModelShutdown() override {
    model_ = nullptr; icon_subscriptions_.clear(); actions_.clear();
  }
  ArcExtensionToolbarAction* Snapshot(const std::string& id) {
    auto* action = GetActionForId(id);
    if (!action || !browser_) return nil;
    auto* contents = browser_->GetTabStripModel()->GetActiveWebContents();
    ExtensionToolbarAction result;
    result.id = id;
    result.accessible_name = action->GetAccessibleName(contents);
    result.tooltip_text = action->GetTooltip(contents);
    result.enabled = action->IsEnabled(contents);
    ui::ImageModel icon = action->GetIcon(contents, gfx::Size(28,28));
    result.icon = Image(gfx::Image(icon.Rasterize(static_cast<ArcBrowserWindow*>(browser_->GetWindow())->GetColorProvider())));
    const auto& pinned = model_->pinned_action_ids();
    for (size_t index = 0; index < pinned.size(); ++index)
      if (pinned[index] == id) result.pinned_position = static_cast<int>(index);
    return [[ArcExtensionToolbarAction alloc] initWithExtensionToolbarAction:&result];
  }
  void ScheduleSnapshot() {
    if (snapshot_pending_) return;
    snapshot_pending_ = true;
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE,
        base::BindOnce(&BrowserExtensions::SendSnapshot, weak_factory_.GetWeakPtr()));
  }
  void SendSnapshot() {
    snapshot_pending_ = false;
    if (!model_ || !model_->actions_initialized()) return;
    NSMutableArray* snapshots = [NSMutableArray array];
    for (const auto& id : model_->action_ids()) {
      ArcExtensionToolbarAction* value = Snapshot(id);
      if (value) [snapshots addObject:value];
    }
    id<ArcExtensionsBrowserDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browser:didUpdateExtensionToolbarActions:)])
      [delegate browser:wrapper_ didUpdateExtensionToolbarActions:[snapshots copy]];
  }
 private:
  base::WeakPtr<BrowserWindowInterface> browser_;
  __weak ArcBrowser* wrapper_;
  __weak id<ArcExtensionsBrowserDelegate> delegate_;
  ui::ScopedUnownedUserData<ExtensionsContainer> scoped_data_;
  raw_ptr<ToolbarActionsModel> model_ = nullptr;
  std::map<std::string, std::unique_ptr<ExtensionActionViewModel>> actions_;
  std::map<std::string, base::CallbackListSubscription> icon_subscriptions_;
  bool snapshot_pending_ = false;
  base::CallbackListSubscription close_;
  base::WeakPtrFactory<BrowserExtensions> weak_factory_{this};
};
const char kExtensionsKey[] = "ArcBrowserExtensions";
BrowserExtensions* Bridge(ArcBrowser* wrapper) {
  auto* browser = [wrapper browser];
  if (!browser) return nullptr;
  auto* window = static_cast<ArcBrowserWindow*>(browser->GetWindow());
  return static_cast<BrowserExtensions*>(window->GetUserData(kExtensionsKey));
}

void ActionDelegate::TriggerPopup(std::unique_ptr<extensions::ExtensionViewHost> host,
                                  PopupShowAction show_action, bool by_user,
                                  ShowPopupCallback callback) {
  auto weak_before_hide = weak_factory_.GetWeakPtr();
  container_->HideActivePopup();
  if (!weak_before_hide) {
    if (callback) std::move(callback).Run(nullptr);
    return;
  }
  host_ = std::move(host);
  pending_callback_ = std::move(callback);
  show_action_ = show_action;
  host_->set_view(this);
  host_->SetCloseHandler(base::BindOnce([](base::WeakPtr<ActionDelegate> self, extensions::ExtensionHost*) {
    if (!self) return;
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE,
        base::BindOnce(&ActionDelegate::HidePopup, self));
  }, weak_factory_.GetWeakPtr()));
  host_->CreateRendererSoon();
  auto weak = weak_factory_.GetWeakPtr();
  id<ArcExtensionsBrowserDelegate> delegate = delegate_;
  if ([delegate respondsToSelector:@selector(browser:didOpenExtensionPopup:webContents:)])
    [delegate browser:browser_ didOpenExtensionPopup:ArcNSString(model_->GetId())
              webContents:[ArcWebContents wrapperForWebContents:host_->host_contents()]];
  if (weak && weak->host_ && weak->host_->has_loaded_once()) weak->OnLoaded();
}

// NSMenu targets carry the model index, including submenus. Chrome's menu model
// owns command enablement, toggles and dispatch, rather than Arc-specific copies.
}  // namespace
}  // namespace arc

@interface ExtensionsMenuTarget : NSObject
- (instancetype)initWithModel:(ui::MenuModel*)model index:(size_t)index;
- (void)invoke:(id)sender;
@end
@implementation ExtensionsMenuTarget {
  base::WeakPtr<ui::MenuModel> _model;
  size_t _index;
}
- (instancetype)initWithModel:(ui::MenuModel*)model index:(size_t)index {
  if ((self = [super init])) { _model = model->AsWeakPtr(); _index = index; } return self;
}
- (void)invoke:(id)sender { if (_model) _model->ActivatedAt(_index); }
@end

namespace arc {
namespace {
NSMenu* MakeMenu(ui::MenuModel* model, NSMutableArray* targets) {
  NSMenu* menu = [[NSMenu alloc] initWithTitle:@""];
  menu.autoenablesItems = NO;
  for (size_t i=0; i<model->GetItemCount(); ++i) {
    if (!model->IsVisibleAt(i)) continue;
    if (model->GetTypeAt(i) == ui::MenuModel::TYPE_SEPARATOR) {
      [menu addItem:[NSMenuItem separatorItem]]; continue;
    }
    NSMenuItem* item = [[NSMenuItem alloc] initWithTitle:ArcNSString(model->GetLabelAt(i))
                                               action:@selector(invoke:) keyEquivalent:@""];
    auto* submenu = model->GetSubmenuModelAt(i);
    if (submenu) item.submenu = MakeMenu(submenu, targets);
    else {
      ExtensionsMenuTarget* target = [[ExtensionsMenuTarget alloc] initWithModel:model index:i];
      [targets addObject:target]; item.target = target;
    }
    item.enabled = model->IsEnabledAt(i);
    item.state = model->IsItemCheckedAt(i) ? NSControlStateValueOn : NSControlStateValueOff;
    [menu addItem:item];
  }
  return menu;
}
void ActionDelegate::ShowContextMenu() {
  auto* model = model_->GetContextMenu(extensions::ExtensionContextMenuModel::ContextMenuSource::kToolbarAction);
  if (!model) return;
  NSMutableArray* targets = [NSMutableArray array];
  NSMenu* menu = MakeMenu(model, targets);
  [menu popUpMenuPositioningItem:nil atLocation:NSEvent.mouseLocation inView:nil];
}
void ActionDelegate::ShowContextMenuAsFallback() { ShowContextMenu(); }
}  // namespace

void InstallBrowserExtensions(ArcBrowser* wrapper, id delegate) {
  if (![wrapper browser]) return;
  arcmod::GetExtensionGroupIdHook() = base::BindRepeating(
      [](content::WebContents* contents) -> std::optional<int> {
        auto* browser = GlobalBrowserCollection::GetInstance()->FindBrowserWithTab(contents);
        if (!browser) return std::nullopt;
        ArcBrowser* wrapper = [ArcBrowser wrapperForBrowser:browser];
        id<ArcExtensionsBrowserDelegate> delegate = BrowserExtensionDelegate(wrapper);
        if (![delegate respondsToSelector:@selector(browser:extensionGroupIdForTabWithSessionId:)])
          return std::nullopt;
        return base::saturated_cast<int>([delegate browser:wrapper
            extensionGroupIdForTabWithSessionId:sessions::SessionTabHelper::IdForTab(contents).id()]);
      });
  arcmod::GetExtensionPermissionsHook() = base::BindRepeating(
      [](Profile* profile, const extensions::Extension* extension,
         const extensions::PermissionSet* permissions,
         base::OnceCallback<void(bool)> done) {
        auto* browser = ProfileBrowserCollection::GetForProfile(profile)->GetLastActiveBrowser();
        if (!browser) return false;
        ArcBrowser* wrapper = [ArcBrowser wrapperForBrowser:browser];
        id<ArcExtensionsBrowserDelegate> delegate = BrowserExtensionDelegate(wrapper);
        if (![delegate respondsToSelector:@selector(browser:didRequestExtensionPermissions:completion:)])
          return false;
        auto snapshot = MakeExtensionInfo(profile, *extension);
        if (permissions) {
          snapshot.permissions.clear();
          auto* provider = extensions::PermissionMessageProvider::Get();
          for (const auto& message : provider->GetPermissionMessages(
                   provider->GetAllPermissionIDs(*permissions, extension->GetType())))
            snapshot.permissions.push_back(message.message());
        }
        auto completion = std::make_shared<base::OnceCallback<void(bool)>>(std::move(done));
        [delegate browser:wrapper didRequestExtensionPermissions:
            [[ArcExtensionInfo alloc] initWithExtensionInfo:&snapshot]
            completion:^(BOOL allowed) {
              dispatch_async(dispatch_get_main_queue(), ^{
                if (*completion) std::move(*completion).Run(allowed);
              });
            }];
        return true;
      });
  if (auto* bridge = Bridge(wrapper)) { bridge->SetDelegate(delegate); return; }
  if (!delegate) return;
  auto* window = static_cast<ArcBrowserWindow*>([wrapper browser]->GetWindow());
  window->SetUserData(kExtensionsKey, std::make_unique<BrowserExtensions>(wrapper,delegate));
}
id BrowserExtensionDelegate(ArcBrowser* wrapper) {
  auto* bridge = Bridge(wrapper);
  return bridge ? bridge->GetDelegate() : nil;
}
}  // namespace arc

@implementation ArcBrowser (Extensions)
- (void)executeExtensionAction:(NSString*)identifier {
  if (auto* bridge = arc::Bridge(self))
    if (auto* action = bridge->GetActionForId(ArcUTF8(identifier)))
      action->ExecuteUserAction(ToolbarActionViewModel::InvocationSource::kToolbarButton);
}
- (void)pinExtension:(NSString*)identifier {
  if (auto* browser = [self browser]; browser && !browser->GetProfile()->IsOffTheRecord())
    if (auto* model = ToolbarActionsModel::Get(browser->GetProfile()))
      if (model->HasAction(ArcUTF8(identifier)) && !model->IsActionPinned(ArcUTF8(identifier)) && !model->IsActionForcePinned(ArcUTF8(identifier)))
        model->SetActionVisibility(ArcUTF8(identifier), true);
}
- (void)unpinExtension:(NSString*)identifier {
  if (auto* browser = [self browser]; browser && !browser->GetProfile()->IsOffTheRecord())
    if (auto* model = ToolbarActionsModel::Get(browser->GetProfile()))
      if (model->IsActionPinned(ArcUTF8(identifier)) && !model->IsActionForcePinned(ArcUTF8(identifier)))
        model->SetActionVisibility(ArcUTF8(identifier), false);
}
- (void)moveExtension:(NSString*)identifier targetIndex:(long long)index {
  if (auto* browser = [self browser]; browser && !browser->GetProfile()->IsOffTheRecord())
    if (auto* model = ToolbarActionsModel::Get(browser->GetProfile()))
      if (model->IsActionPinned(ArcUTF8(identifier)) && index >= 0 && static_cast<size_t>(index) < model->pinned_action_ids().size())
        model->MovePinnedAction(ArcUTF8(identifier), static_cast<size_t>(index));
}
- (void)showExtensionContextMenu:(NSString*)identifier {
  if (auto* bridge = arc::Bridge(self))
    if (auto* action = bridge->GetActionForId(ArcUTF8(identifier)))
      static_cast<arc::ActionDelegate*>(static_cast<ExtensionActionViewModel*>(action)->delegate())->ShowContextMenu();
}
- (void)triggerExtensionOmniboxInput:(NSString*)identifier inputText:(NSString*)input disposition:(long long)disposition {
  if (auto* browser = [self browser]) {
    auto* contents = browser->GetTabStripModel()->GetActiveWebContents();
    if (contents) extensions::ExtensionOmniboxEventRouter::OnInputEntered(
        contents, ArcUTF8(identifier), ArcUTF8(input), static_cast<WindowOpenDisposition>(disposition));
  }
}
@end
