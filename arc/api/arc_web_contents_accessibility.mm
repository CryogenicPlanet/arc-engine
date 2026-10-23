// Copyright 2026 ArcMod. Accessibility-tree API of ArcWebContents (agent-driven page access).
#import "arc/api/arc_internal.h"

#include <deque>
#include <string>
#include <vector>

#include "arc/browser/webextras_values.h"
#include "base/time/time.h"
#include "components/input/native_web_keyboard_event.h"
#include "content/browser/renderer_host/render_frame_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "third_party/blink/public/common/input/web_input_event.h"
#include "ui/accessibility/ax_action_data.h"
#include "ui/accessibility/ax_enum_util.h"
#include "ui/accessibility/ax_node.h"
#include "ui/accessibility/ax_role_properties.h"
#include "ui/accessibility/ax_tree.h"
#include "ui/accessibility/ax_tree_id.h"
#include "ui/accessibility/platform/browser_accessibility.h"
#include "ui/accessibility/platform/browser_accessibility_manager.h"
#include "ui/events/keycodes/dom/keycode_converter.h"

@class ArcKeyEventData;
@interface ArcKeyEventData : NSObject
@property(nonatomic, readonly) NSUInteger eventType;
@property(nonatomic, readonly, copy) NSString* domCode;
@property(nonatomic, readonly, copy) NSString* key;
@property(nonatomic, readonly) int windowsVirtualKeyCode;
@end

@interface ArcAccessibilityActionData : NSObject
@property(nonatomic, readonly) NSUInteger action;
@property(nonatomic, readonly) NSString* value;
@end

@interface ArcAccessibilityTreeNode : NSObject
- (instancetype)initWithAccessibilityTreeNode:(const void*)snapshot;
@end

namespace {

// The frame of a (non-unknown) accessibility tree id, or null.
content::RenderFrameHostImpl* FrameForTreeId(NSString* tree_id) {
  const ui::AXTreeID id = ui::AXTreeID::FromString(ArcUTF8(tree_id));
  if (id == ui::AXTreeIDUnknown()) {
    return nullptr;
  }
  return static_cast<content::RenderFrameHostImpl*>(
      content::RenderFrameHost::FromAXTreeID(id));
}

// Public action codes 0...3 of ArcAccessibilityActionData. The app never builds
// the object in the decompile, so the mapping is the most plausible one.
bool ActionForCode(NSUInteger code, ax::mojom::Action* action) {
  switch (code) {
    case 0: *action = ax::mojom::Action::kDoDefault; return true;
    case 1: *action = ax::mojom::Action::kFocus; return true;
    case 2: *action = ax::mojom::Action::kSetValue; return true;
    case 3: *action = ax::mojom::Action::kScrollToMakeVisible; return true;
    default: return false;
  }
}

// Public ArcKeyEventData.eventType 0...3, as the engine maps them to blink types.
bool KeyEventTypeForCode(NSUInteger code, blink::WebInputEvent::Type* type) {
  switch (code) {
    case 0: *type = blink::WebInputEvent::Type::kRawKeyDown; return true;
    case 1: *type = blink::WebInputEvent::Type::kKeyDown; return true;
    case 2: *type = blink::WebInputEvent::Type::kKeyUp; return true;
    case 3: *type = blink::WebInputEvent::Type::kChar; return true;
    default: return false;
  }
}

arc::AccessibilitySnapshot SnapshotOf(const ui::AXNode& node) {
  const ui::AXNodeData& data = node.data();
  arc::AccessibilitySnapshot snapshot;
  snapshot.node_id = node.id();
  snapshot.role = ui::ToString(data.role);
  snapshot.name = node.GetNameUTF8();
  snapshot.value = node.GetValueForControl();
  snapshot.description =
      data.GetStringAttribute(ax::mojom::StringAttribute::kDescription);
  snapshot.is_clickable = ui::IsClickable(data.role);
  snapshot.is_editable = data.HasState(ax::mojom::State::kEditable);
  snapshot.is_focusable =
      data.HasState(ax::mojom::State::kFocusable) || node.IsFocusable();
  snapshot.child_tree_id =
      data.GetStringAttribute(ax::mojom::StringAttribute::kChildTreeId);
  for (const ui::AXNode* child : node.GetAllChildren()) {
    snapshot.children.push_back(child->id());
  }
  return snapshot;
}

}  // namespace

@implementation ArcWebContents (AccessibilityTree)

- (BOOL)performAccessibilityAction:(ArcAccessibilityActionData*)actionData
                            treeId:(NSString*)treeId
                            nodeId:(int)nodeId {
  if (![self webContents] || !actionData) {
    return NO;
  }
  ui::AXActionData data;
  if (!ActionForCode(actionData.action, &data.action)) {
    return NO;
  }
  data.target_node_id = nodeId;
  data.value = ArcUTF8(actionData.value);
  content::RenderFrameHostImpl* frame = FrameForTreeId(treeId);
  ui::BrowserAccessibilityManager* manager =
      frame ? frame->browser_accessibility_manager() : nullptr;
  ui::BrowserAccessibility* node = manager ? manager->GetFromID(nodeId) : nullptr;
  if (!node) {
    return NO;
  }
  data.target_tree_id = manager->GetTreeID();
  return node->AccessibilityPerformAction(data);
}

- (NSString*)rootFrameAccessibilityTreeId {
  content::WebContents* contents = [self webContents];
  content::RenderFrameHost* frame =
      contents ? contents->GetPrimaryMainFrame() : nullptr;
  return frame ? ArcNSString(frame->GetAXTreeID().ToString()) : @"";
}

- (NSArray*)accessibilityTreeForTreeId:(NSString*)treeId {
  NSMutableArray* nodes = [NSMutableArray array];
  content::RenderFrameHostImpl* frame =
      [self webContents] ? FrameForTreeId(treeId) : nullptr;
  ui::BrowserAccessibilityManager* manager =
      frame ? frame->browser_accessibility_manager() : nullptr;
  const ui::AXTree* tree = manager ? manager->ax_tree() : nullptr;
  if (!tree || !tree->root()) {
    return nodes;
  }
  // Breadth-first from the frame's root, like the engine's queue walk.
  std::deque<const ui::AXNode*> pending = {tree->root()};
  while (!pending.empty()) {
    const ui::AXNode* node = pending.front();
    pending.pop_front();
    const arc::AccessibilitySnapshot snapshot = SnapshotOf(*node);
    [nodes addObject:[[ArcAccessibilityTreeNode alloc]
                         initWithAccessibilityTreeNode:&snapshot]];
    for (const ui::AXNode* child : node->GetAllChildren()) {
      pending.push_back(child);
    }
  }
  return nodes;
}

- (BOOL)typeText:(NSString*)text inFrameWIthAccessibilityTreeId:(NSString*)treeId {
  content::RenderFrameHostImpl* frame =
      [self webContents] ? FrameForTreeId(treeId) : nullptr;
  auto* widget = frame ? content::RenderWidgetHostImpl::From(frame->GetRenderWidgetHost())
                       : nullptr;
  if (!widget) {
    return NO;
  }
  widget->ImeCommitText(ArcUTF16(text), {}, gfx::Range::InvalidRange(), 0);
  return YES;
}

- (BOOL)sendKeyEvent:(ArcKeyEventData*)event treeId:(NSString*)treeId {
  blink::WebInputEvent::Type type;
  if (!event || !KeyEventTypeForCode(event.eventType, &type)) {
    return NO;
  }
  content::RenderFrameHostImpl* frame =
      [self webContents] ? FrameForTreeId(treeId) : nullptr;
  auto* widget = frame ? content::RenderWidgetHostImpl::From(frame->GetRenderWidgetHost())
                       : nullptr;
  if (!widget) {
    return NO;
  }
  input::NativeWebKeyboardEvent key_event(type, blink::WebInputEvent::kNoModifiers,
                                          base::TimeTicks::Now());
  const ui::DomCode code =
      ui::KeycodeConverter::CodeStringToDomCode(ArcUTF8(event.domCode));
  key_event.dom_code = static_cast<int>(code);
  key_event.native_key_code = ui::KeycodeConverter::DomCodeToNativeKeycode(code);
  key_event.dom_key = ui::KeycodeConverter::KeyStringToDomKey(ArcUTF8(event.key));
  key_event.windows_key_code = event.windowsVirtualKeyCode;
  const std::u16string key = ArcUTF16(event.key);
  if (type == blink::WebInputEvent::Type::kChar && key.size() == 1) {
    key_event.text[0] = key[0];
    key_event.unmodified_text[0] = key[0];
  }
  widget->Focus();
  widget->ForwardKeyboardEvent(key_event);
  return YES;
}

@end
