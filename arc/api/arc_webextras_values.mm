// Copyright 2026 ArcMod.
#import "arc/api/arc_internal.h"

#include "arc/browser/webextras_values.h"
#include "base/check.h"

ARC_EXPORT @interface ArcAccessibilityActionData : NSObject {
  NSUInteger _action;
  NSString* _value;
}
- (instancetype)initWithAction:(NSUInteger)action;
- (instancetype)initWithAction:(NSUInteger)action value:(NSString*)value;
@property(nonatomic, readonly) NSUInteger action;
@property(nonatomic, readonly) NSString* value;
@end

@implementation ArcAccessibilityActionData
- (instancetype)initWithAction:(NSUInteger)action {
  CHECK_LT(action, 4u);
  if ((self = [super init])) { _action = action; }
  return self;
}
- (instancetype)initWithAction:(NSUInteger)action value:(NSString*)value {
  CHECK_LT(action, 4u);
  if ((self = [super init])) {
    _action = action;
    _value = value.length ? [value copy] : nil;
  }
  return self;
}
- (NSUInteger)action { return _action; }
- (NSString*)value { return _value; }
@end

ARC_EXPORT @interface ArcAccessibilityTreeNode : NSObject {
  arc::AccessibilitySnapshot _snapshot;
}
- (instancetype)initWithAccessibilityTreeNode:(const void*)snapshot;
@property(nonatomic, readonly) int nodeId;
@property(nonatomic, readonly, copy) NSString* role;
@property(nonatomic, readonly, copy) NSString* name;
@property(nonatomic, readonly, copy) NSString* value;
@property(nonatomic, readonly, copy) NSString* description;
@property(nonatomic, readonly) BOOL isClickable;
@property(nonatomic, readonly) BOOL isEditable;
@property(nonatomic, readonly) BOOL isFocusable;
@property(nonatomic, readonly, copy) NSString* childTreeId;
@property(nonatomic, readonly, copy) NSArray* children;
@end

@implementation ArcAccessibilityTreeNode
- (instancetype)initWithAccessibilityTreeNode:(const void*)snapshot {
  if ((self = [super init]) && snapshot) {
    _snapshot = *static_cast<const arc::AccessibilitySnapshot*>(snapshot);
  }
  return self;
}
- (int)nodeId { return _snapshot.node_id; }
- (NSString*)role { return ArcNSString(_snapshot.role); }
- (NSString*)name { return ArcNSString(_snapshot.name); }
- (NSString*)value { return ArcNSString(_snapshot.value); }
- (NSString*)description { return ArcNSString(_snapshot.description); }
- (BOOL)isClickable { return _snapshot.is_clickable; }
- (BOOL)isEditable { return _snapshot.is_editable; }
- (BOOL)isFocusable { return _snapshot.is_focusable; }
- (NSString*)childTreeId { return ArcNSString(_snapshot.child_tree_id); }
- (NSArray*)children {
  NSMutableArray* children = [NSMutableArray arrayWithCapacity:_snapshot.children.size()];
  for (int child : _snapshot.children) {
    [children addObject:@(child)];
  }
  return children;
}
@end

ARC_EXPORT @interface ArcBrowserSidePanelEntryInfo : NSObject
@property(nonatomic) BOOL hasActiveEntry;
@property(nonatomic, strong) NSString* extensionId;
@property(nonatomic, strong) ArcWebContents* cachedWebContents;
@end
@implementation ArcBrowserSidePanelEntryInfo
@synthesize hasActiveEntry = _hasActiveEntry;
@synthesize extensionId = _extensionId;
@synthesize cachedWebContents = _cachedWebContents;
@end

