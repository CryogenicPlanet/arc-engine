// Copyright 2026 ArcMod. Native Chrome context menu models exposed to Arc's UI.
#import "arc/api/arc_internal.h"
#include "arc/browser/ui_hooks.h"

#include <array>
#include <optional>
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

ARC_EXPORT @interface ArcMenuModel : NSObject
- (instancetype)initWithMenuModel:(const void*)model;
@property(nonatomic, readonly) NSArray* items;
- (void)menuWillShow;
- (void)menuWillClose;
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
@property(nonatomic, readonly) NSImage* icon;
- (void)activated;
- (void)setDelegate:(id)delegate;
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
struct MenuItemData {
  base::WeakPtr<ui::MenuModel> model;
  size_t index;
};
}  // namespace

@implementation ArcMenuModel {
  base::WeakPtr<ui::MenuModel> _model;
  NSArray* _items;
}
@synthesize items = _items;
- (instancetype)initWithMenuModel:(const void*)model {
  CHECK(model);
  if ((self = [super init])) {
    _model = static_cast<ui::MenuModel*>(const_cast<void*>(model))->AsWeakPtr();
  }
  return self;
}
- (NSArray*)items {
  if (!_items) {
    NSMutableArray* items = [NSMutableArray array];
    if (_model) {
      for (size_t index = 0; index < _model->GetItemCount(); ++index) {
        MenuItemData data{_model, index};
        [items addObject:[[ArcMenuItemModel alloc] initWithMenuItemModel:&data]];
      }
    }
    _items = items;
  }
  return _items;
}
- (void)menuWillShow { if (_model) { _model->MenuWillShow(); } }
- (void)menuWillClose { if (_model) { _model->MenuWillClose(); } }
@end

@implementation ArcMenuItemModel {
  MenuItemData _item;
  ArcMenuModel* _submenuModel;
  __weak id _delegate;
}
@synthesize submenuModel = _submenuModel;
- (instancetype)initWithMenuItemModel:(const void*)item {
  CHECK(item);
  if ((self = [super init])) { _item = *static_cast<const MenuItemData*>(item); }
  return self;
}
- (ui::MenuModel*)liveModel {
  return _item.model && _item.index < _item.model->GetItemCount() ? _item.model.get() : nullptr;
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
- (NSImage*)icon {
  auto* m = [self liveModel];
  if (!m) { return nil; }
  auto image = m->GetIconAt(_item.index);
  return image.IsImage() ? image.GetImage().AsNSImage() : nil;
}
- (void)activated { auto* m = [self liveModel]; if (m) { m->ActivatedAt(_item.index); } }
- (void)setDelegate:(id)delegate { _delegate = delegate; }
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
