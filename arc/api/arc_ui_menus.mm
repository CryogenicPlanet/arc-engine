// Copyright 2026 ArcMod. Native Chrome context menu models exposed to Arc's UI.
#import "arc/api/arc_internal.h"
#import <objc/runtime.h>
#include "arc/browser/ui_hooks.h"

#include <array>
#include <optional>
#include <memory>
#include <utility>

#include "base/check.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/context_menu_params.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/cocoa/menu_utils.h"
#include "ui/base/models/image_model.h"
#include "ui/base/models/menu_model.h"
#include "ui/base/models/menu_model_delegate.h"
#include "ui/gfx/image/image.h"
#import "ui/menus/cocoa/menu_controller.h"

@interface ArcWebContents (UIInternal)
- (id)uiDelegate;
@end

// This class's generated shell remains its primary implementation. Only the
// menu-owned adapter is supplied here; its initializer preserves the exact ABI.
struct KeyboardAccelerator {
  unsigned int key;
  int modifiers;
};
@interface ArcKeyboardAccelerator : NSObject
@end
@interface ArcKeyboardAccelerator (HostMenuValues)
- (instancetype)initWithAccelerator:(const KeyboardAccelerator*)accelerator;
- (NSUInteger)key;
- (NSUInteger)modifiers;
@end

namespace {
char kAcceleratorKey;
char kAcceleratorModifiers;
char kCocoaItemWrapper;
}

@implementation ArcKeyboardAccelerator (HostMenuValues)
- (instancetype)initWithAccelerator:(const KeyboardAccelerator*)accelerator {
  CHECK(accelerator);
  if ((self = [super init])) {
    objc_setAssociatedObject(self, &kAcceleratorKey, @(accelerator->key),
                             OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    objc_setAssociatedObject(self, &kAcceleratorModifiers,
                             @((NSUInteger)(intptr_t)accelerator->modifiers),
                             OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  return self;
}
- (NSUInteger)key {
  return [objc_getAssociatedObject(self, &kAcceleratorKey) unsignedIntegerValue];
}
- (NSUInteger)modifiers {
  return [objc_getAssociatedObject(self, &kAcceleratorModifiers) unsignedIntegerValue];
}
@end

ARC_EXPORT @interface ArcMenuModel : NSObject
- (instancetype)initWithMenuModel:(const void*)model;
@property(nonatomic, readonly) NSArray* items;
- (void)menuWillShow;
- (void)menuWillClose;
- (void)invalidateItems;
- (void)trackWithController:(MenuControllerCocoa*)controller;
@end

ARC_EXPORT @interface ArcMenuItemModel : NSObject
- (instancetype)initWithMenuItemModel:(const void*)item;
@property(nonatomic, readonly) NSInteger type;
@property(nonatomic, readonly) NSInteger logicalType;
@property(nonatomic, readonly) NSString* label;
@property(nonatomic, readonly) BOOL isChecked;
@property(nonatomic, readonly) BOOL isEnabled;
@property(nonatomic, readonly) BOOL isVisible;
@property(nonatomic, readonly) BOOL isAlerted;
@property(nonatomic, readonly) BOOL isNewFeature;
@property(nonatomic, readonly) NSString* accessibleName;
@property(nonatomic, readonly) ArcMenuModel* submenuModel;
@property(nonatomic, readonly) BOOL hasAccelerator;
@property(nonatomic, readonly) ArcKeyboardAccelerator* accelerator;
@property(nonatomic, readonly) NSImage* icon;
- (void)activated;
- (void)setDelegate:(id)delegate;
- (ui::MenuModel*)liveModel;
- (void)nativeItemChanged;
- (void)nativeItemRemoved;
@end

@protocol ArcHostMenuItemDelegate <NSObject>
- (void)menuItemModelChanged:(ArcMenuItemModel*)item;
- (void)menuItemModelRemoved:(ArcMenuItemModel*)item;
@end

ARC_EXPORT @interface ArcContextMenuElementInfo : NSObject
- (instancetype)initWithContextMenuElementInfo:(const void*)info;
@property(nonatomic, readonly) int x;
@property(nonatomic, readonly) int y;
@property(nonatomic, readonly) NSURL* topLevelURL;
@property(nonatomic, readonly) NSURL* subFrameURL;
@property(nonatomic, readonly) NSURL* linkURL;
@property(nonatomic, readonly) NSURL* sourceAttributeURL;
@property(nonatomic, readonly) NSString* selectionText;
@property(nonatomic, readonly) NSString* suggestedFilename;
@property(nonatomic, readonly) NSString* misspelledWord;
@property(nonatomic, readonly) NSArray* dictionarySuggestions;
@property(nonatomic, readonly) NSInteger inputType;
@property(nonatomic, readonly) NSInteger nodeType;
@property(nonatomic, readonly) BOOL isEditable;
@end

@protocol ArcHostContextMenuUI <NSObject>
- (BOOL)webContents:(ArcWebContents*)contents willShowContextMenu:(ArcMenuModel*)menu
         withElement:(ArcContextMenuElementInfo*)element;
@end

namespace {
// MenuModel has a single observer slot. Attach only while it is free; never
// replace somebody else's observer. A structural change invalidates the entire
// snapshot since Chromium supplies no stable item IDs or change indices.
class MenuState : public ui::MenuModelDelegate,
                  public std::enable_shared_from_this<MenuState> {
 public:
  explicit MenuState(ui::MenuModel* model) : model_(model->AsWeakPtr()) {
    for (size_t i = 0; i < model->GetItemCount(); ++i) {
      identity_.emplace_back(model->GetCommandIdAt(i), model->GetTypeAt(i));
    }
    if (!model->menu_model_delegate()) {
      attached_ = true;
      model->SetMenuModelDelegate(this);
    }
  }
  ~MenuState() override {
    if (attached_ && model_ && model_->menu_model_delegate() == this) {
      attached_ = false;
      model_->SetMenuModelDelegate(nullptr);
    }
  }
  void AddItem(ArcMenuItemModel* item) { [items_ addObject:item]; }
  void SetOwner(ArcMenuModel* owner) { owner_ = owner; }
  void Track(MenuControllerCocoa* controller) { controller_ = controller; }
  ui::MenuModel* LiveModel(size_t index) {
    if (!valid_ || !model_) { return nullptr; }
    // Also protect menus whose observer slot was already in use, and models
    // that do not emit OnMenuStructureChanged after edits.
    bool unchanged = identity_.size() == model_->GetItemCount();
    for (size_t i = 0; unchanged && i < identity_.size(); ++i) {
      unchanged = identity_[i].first == model_->GetCommandIdAt(i) &&
                  identity_[i].second == model_->GetTypeAt(i);
    }
    if (!unchanged) { Invalidate(); return nullptr; }
    return index < identity_.size() ? model_.get() : nullptr;
  }
  void OnIconChanged(int command) override {
    auto keep_alive = shared_from_this();
    NSArray* snapshot = items_.allObjects;
    for (ArcMenuItemModel* item in snapshot) {
      ui::MenuModel* model = [item liveModel];
      if (model) {
        // The wrapper filters by its captured command through this selector.
        [item nativeItemChangedForCommand:command];
      }
    }
  }
  void OnMenuStructureChanged() override {
    auto keep_alive = shared_from_this();
    Invalidate();
  }
  void OnMenuClearingDelegate() override {
    if (!attached_) { return; }
    auto keep_alive = shared_from_this();
    attached_ = false;
    // The model may be in its destructor. Do not call native methods here.
    Invalidate();
  }
 private:
  void Invalidate() {
    if (!valid_) { return; }
    valid_ = false;
    NSArray* snapshot = items_.allObjects;
    [owner_ invalidateItems];
    [controller_ cancel];
    for (ArcMenuItemModel* item in snapshot) { [item nativeItemRemoved]; }
  }
  base::WeakPtr<ui::MenuModel> model_;
  std::vector<std::pair<int, ui::MenuModel::ItemType>> identity_;
  NSHashTable* __strong items_ = [NSHashTable weakObjectsHashTable];
  ArcMenuModel* __weak owner_;
  MenuControllerCocoa* __weak controller_;
  bool attached_ = false;
  bool valid_ = true;
};
struct MenuItemData {
  std::shared_ptr<MenuState> state;
  size_t index;
  int command;
};
}  // namespace

@implementation ArcMenuModel {
  base::WeakPtr<ui::MenuModel> _model;
  std::shared_ptr<MenuState> _state;
  NSArray* _items;
}
@synthesize items = _items;
- (instancetype)initWithMenuModel:(const void*)model {
  CHECK(model);
  if ((self = [super init])) {
    _model = static_cast<ui::MenuModel*>(const_cast<void*>(model))->AsWeakPtr();
    _state = std::make_shared<MenuState>(_model.get());
    _state->SetOwner(self);
  }
  return self;
}
- (NSArray*)items {
  if (!_items) {
    NSMutableArray* items = [NSMutableArray array];
    if (_model) {
      for (size_t index = 0; index < _model->GetItemCount(); ++index) {
        MenuItemData data{_state, index, _model->GetCommandIdAt(index)};
        [items addObject:[[ArcMenuItemModel alloc] initWithMenuItemModel:&data]];
      }
    }
    _items = items;
  }
  return _items;
}
- (void)menuWillShow { if (_model) { _model->MenuWillShow(); } }
- (void)menuWillClose { if (_model) { _model->MenuWillClose(); } }
- (void)invalidateItems {
  _items = nil;
  // Re-create native observation on the next read, outside native callbacks.
}
- (void)trackWithController:(MenuControllerCocoa*)controller {
  _state->Track(controller);
}
@end

@implementation ArcMenuItemModel {
  MenuItemData _item;
  ArcMenuModel* _submenuModel;
  __weak id _delegate;
}
@synthesize submenuModel = _submenuModel;
- (instancetype)initWithMenuItemModel:(const void*)item {
  CHECK(item);
  if ((self = [super init])) {
    _item = *static_cast<const MenuItemData*>(item);
    _item.state->AddItem(self);
  }
  return self;
}
- (ui::MenuModel*)liveModel {
  return _item.state ? _item.state->LiveModel(_item.index) : nullptr;
}
- (NSInteger)type { auto* m = [self liveModel]; return m ? m->GetTypeAt(_item.index) : 0; }
- (NSInteger)logicalType {
  auto* m = [self liveModel];
  if (!m) { return 24; }
  const int command = m->GetCommandIdAt(_item.index);
  if (command >= 49000 && command <= 50000) { return 14; }
  // Exact ArcCore 1.170 native mapping at 0x432bb60; Arc-only IDs kept intact.
  switch (command) {
    case 33000: return 0;
    case 33001: return 1;
    case 33002: return 2;
    case 35004: return 3;
    case 35002: return 4;
    case 50220: return 5;
    case 50154: return 6;
    case 50155: return 7;
    case 50150: return 8;
    case 50151: return 9;
    case 50152: return 10;
    case 50156: return 11;
    case 50162: return 12;
    case 35003: return 13;
    case 50100: return 15;
    case 50101: return 16;
    case 50111: return 17;
    case 50102: return 18;
    case 53249: return 19;
    case 35011: return 20;
    case 50161: return 21;
    case 53247: return 22;
    case 53246: return 23;
    default: return 24;
  }
}
- (NSString*)label { auto* m = [self liveModel]; return m ? ArcNSString(m->GetLabelAt(_item.index)) : nil; }
- (BOOL)isChecked { auto* m = [self liveModel]; return m && m->IsItemCheckedAt(_item.index); }
- (BOOL)isEnabled { auto* m = [self liveModel]; return m && m->IsEnabledAt(_item.index); }
- (BOOL)isVisible { auto* m = [self liveModel]; return m && m->IsVisibleAt(_item.index); }
- (BOOL)isAlerted { auto* m = [self liveModel]; return m && m->IsAlertedAt(_item.index); }
- (BOOL)isNewFeature { auto* m = [self liveModel]; return m && m->IsNewFeatureAt(_item.index); }
- (NSString*)accessibleName {
  auto* m = [self liveModel]; return m ? ArcNSString(m->GetAccessibleNameAt(_item.index)) : nil;
}
- (ArcMenuModel*)submenuModel {
  if (!_submenuModel) {
    auto* m = [self liveModel];
    auto* submenu = m ? m->GetSubmenuModelAt(_item.index) : nullptr;
    if (submenu) { _submenuModel = [[ArcMenuModel alloc] initWithMenuModel:submenu]; }
  }
  return _submenuModel;
}
- (BOOL)hasAccelerator {
  auto* m = [self liveModel]; ui::Accelerator accelerator;
  return m && m->GetAcceleratorAt(_item.index, &accelerator);
}
- (ArcKeyboardAccelerator*)accelerator {
  auto* model = [self liveModel];
  ui::Accelerator accelerator;
  if (!model || !model->GetAcceleratorAt(_item.index, &accelerator)) { return nil; }
  const KeyboardAccelerator value{
      static_cast<unsigned int>(accelerator.key_code()), accelerator.modifiers()};
  return [[ArcKeyboardAccelerator alloc] initWithAccelerator:&value];
}
- (NSImage*)icon {
  auto* m = [self liveModel];
  if (!m) { return nil; }
  auto image = m->GetIconAt(_item.index);
  return image.IsImage() ? image.GetImage().AsNSImage() : nil;
}
- (void)activated { auto* m = [self liveModel]; if (m) { m->ActivatedAt(_item.index); } }
- (void)setDelegate:(id)delegate { _delegate = delegate; }
- (void)nativeItemChangedForCommand:(int)command {
  if (command == _item.command) { [self nativeItemChanged]; }
}
- (void)nativeItemChanged {
  id<ArcHostMenuItemDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(menuItemModelChanged:)]) {
    [delegate menuItemModelChanged:self];
  }
}
- (void)nativeItemRemoved {
  _submenuModel = nil;
  id<ArcHostMenuItemDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(menuItemModelRemoved:)]) {
    [delegate menuItemModelRemoved:self];
  }
}
@end

@implementation ArcContextMenuElementInfo {
  std::optional<content::ContextMenuParams> _info;
}
- (instancetype)initWithContextMenuElementInfo:(const void*)info {
  CHECK(info);
  if ((self = [super init])) { _info = *static_cast<const content::ContextMenuParams*>(info); }
  return self;
}
- (int)x { CHECK(_info); return _info->x; }
- (int)y { CHECK(_info); return _info->y; }
- (NSURL*)topLevelURL { CHECK(_info); return ArcNSURL(_info->page_url); }
- (NSURL*)subFrameURL { CHECK(_info); return ArcNSURL(_info->frame_url); }
- (NSURL*)linkURL { CHECK(_info); return ArcNSURL(_info->link_url); }
- (NSURL*)sourceAttributeURL { CHECK(_info); return ArcNSURL(_info->src_url); }
- (NSString*)selectionText { CHECK(_info); return ArcNSString(_info->selection_text); }
- (NSString*)suggestedFilename { CHECK(_info); return ArcNSString(_info->suggested_filename); }
- (NSString*)misspelledWord { CHECK(_info); return ArcNSString(_info->misspelled_word); }
- (NSArray*)dictionarySuggestions {
  CHECK(_info);
  NSMutableArray* suggestions = [NSMutableArray arrayWithCapacity:_info->dictionary_suggestions.size()];
  for (const auto& text : _info->dictionary_suggestions) { [suggestions addObject:ArcNSString(text)]; }
  return suggestions;
}
- (NSInteger)inputType {
  CHECK(_info);
  if (!_info->form_control_type) { return 0; }
  // Arc's recovered native table for number/password/.../tel/text, respectively.
  static constexpr std::array<NSInteger, 9> types = {3, 2, 5, 5, 5, 5, 5, 4, 1};
  const int type = static_cast<int>(*_info->form_control_type);
  return type >= 15 && type <= 23 ? types[type - 15] : 5;
}
- (NSInteger)nodeType {
  CHECK(_info);
  static constexpr std::array<NSInteger, 7> types = {0, 1, 3, 2, 4, 5, 6};
  const int type = static_cast<int>(_info->media_type);
  CHECK(type >= 0 && type < 7);
  return types[type];
}
- (BOOL)isEditable { CHECK(_info); return _info->is_editable; }
@end

namespace {
// Cocoa's menu controller holds the native model only while the menu tracks.
// Close tracking synchronously if navigation/tab teardown destroys its owner.
class ContextMenuLifetime : public content::WebContentsObserver {
 public:
  ContextMenuLifetime(content::WebContents* contents, MenuControllerCocoa* controller)
      : content::WebContentsObserver(contents), controller_(controller) {}
  void WebContentsDestroyed() override { [controller_ cancel]; Observe(nullptr); }
 private:
  MenuControllerCocoa* __strong controller_;
};
}  // namespace

namespace arc {
bool ShowContextMenu(content::WebContents* contents, ui::MenuModel* model,
                     const content::ContextMenuParams& params) {
  if (!contents || !model) { return false; }
  auto native = contents->GetWeakPtr();
  auto weak_model = model->AsWeakPtr();
  ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
  id<ArcHostContextMenuUI> delegate = [owner respondsToSelector:@selector(uiDelegate)]
      ? [owner uiDelegate] : nil;
  ArcMenuModel* menu = [[ArcMenuModel alloc] initWithMenuModel:model];
  ArcContextMenuElementInfo* element = [[ArcContextMenuElementInfo alloc]
      initWithContextMenuElementInfo:&params];
  if ([delegate respondsToSelector:@selector(webContents:willShowContextMenu:withElement:)] &&
      [delegate webContents:owner willShowContextMenu:menu withElement:element]) { return true; }
  if (!native || !weak_model) { return true; }
  NSView* view = native->GetNativeView().GetNativeNSView();
  if (!view.window) { return true; }
  // Arc owns the NSWindow rather than a Chrome Views widget. Reuse Chrome's
  // Cocoa menu model/controller directly; its lifecycle calls remain native.
  MenuControllerCocoa* controller = [[MenuControllerCocoa alloc]
      initWithModel:weak_model.get() isContextMenu:YES delegate:nil];
  ContextMenuLifetime lifetime(native.get(), controller);
  NSPoint point = NSMakePoint(params.x, NSHeight(view.bounds) - params.y);
  point = [view convertPoint:point toView:nil];
  NSEvent* event = ui::EventForPositioningContextMenuRelativeToWindow(point, view.window);
  ui::ShowContextMenu(controller.menu, event, view, true);
  return true;
}
}  // namespace arc
