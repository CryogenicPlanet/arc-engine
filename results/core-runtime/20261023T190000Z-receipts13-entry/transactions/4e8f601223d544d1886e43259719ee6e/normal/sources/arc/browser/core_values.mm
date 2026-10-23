// Copyright 2026 ArcMod. Boundary tab-group snapshots and partial updates.
#import "arc/browser/core_values.h"

#include <utility>

#include "base/check.h"

@implementation ArcRedirectEntry {
  std::optional<arc::RedirectEntry> _redirect_entry;
}
- (instancetype)initWithRedirectEntry:(const void*)entry {
  if ((self = [super init])) {
    _redirect_entry = *static_cast<const arc::RedirectEntry*>(entry);
  }
  return self;
}
- (NSURL*)url {
  CHECK(_redirect_entry);
  return ArcNSURL(GURL(_redirect_entry->url));
}
- (NSString*)title {
  CHECK(_redirect_entry);
  return ArcNSString(_redirect_entry->title);
}
- (BOOL)hidden {
  CHECK(_redirect_entry);
  return _redirect_entry->hidden;
}
- (NSInteger)redirectType {
  CHECK(_redirect_entry);
  return _redirect_entry->redirect_type;
}
- (std::optional<arc::RedirectEntry>)redirect_entry { return _redirect_entry; }
@end

@implementation ArcKeyboardAccelerator {
  NSUInteger _key;
  NSUInteger _modifiers;
}
@synthesize key = _key, modifiers = _modifiers;
- (instancetype)initWithAccelerator:(const KeyboardAccelerator*)accelerator {
  if ((self = [super init])) {
    _key = accelerator->key;
    _modifiers = static_cast<NSUInteger>(static_cast<NSInteger>(accelerator->modifiers));
  }
  return self;
}
@end

@implementation ArcKeyEventData {
  int _windowsVirtualKeyCode;
  NSUInteger _eventType;
  NSString* _domCode;
  NSString* _key;
}
@synthesize eventType = _eventType, domCode = _domCode;
@synthesize key = _key, windowsVirtualKeyCode = _windowsVirtualKeyCode;
- (instancetype)initWithEventType:(NSUInteger)eventType domCode:(NSString*)domCode
                             key:(NSString*)key windowsVirtualKeyCode:(int)code {
  if ((self = [super init])) {
    _windowsVirtualKeyCode = code;
    _eventType = eventType;
    _domCode = [domCode copy];
    _key = [key copy];
  }
  return self;
}
@end

@implementation ArcOverscrollBehavior
@synthesize x = _x, y = _y;
- (instancetype)init {
  return [super init];
}
@end

@implementation ArcOverscrollParams
@synthesize overscrollBehavior = _overscrollBehavior;
- (instancetype)init {
  if ((self = [super init])) {
    _overscrollBehavior = [[ArcOverscrollBehavior alloc] init];
  }
  return self;
}
@end

@implementation ArcFindResult {
  NSInteger _numMatches;
  NSInteger _activeMatchOrdinal;
}
@synthesize numMatches = _numMatches, activeMatchOrdinal = _activeMatchOrdinal;
- (instancetype)initWithFindResult:(const FindResult*)result {
  if ((self = [super init])) {
    _numMatches = result->num_matches;
    _activeMatchOrdinal = result->active_match_ordinal;
  }
  return self;
}
@end

@implementation ArcFindTextWithDirectiveResult {
  arc::FindTextWithDirectiveResult _find_text_with_directive_result;
}
- (instancetype)initWithFindTextWithDirectiveResult:(const void*)result {
  if ((self = [super init])) {
    _find_text_with_directive_result =
        *static_cast<const arc::FindTextWithDirectiveResult*>(result);
  }
  return self;
}
- (NSString*)directive { return ArcNSString(_find_text_with_directive_result.directive); }
- (BOOL)result { return _find_text_with_directive_result.result; }
- (arc::FindTextWithDirectiveResult)find_text_with_directive_result {
  return _find_text_with_directive_result;
}
- (void)setFind_text_with_directive_result:(arc::FindTextWithDirectiveResult)result {
  _find_text_with_directive_result = std::move(result);
}
@end

@implementation ArcTabGroupInfo {
  BOOL _collapsed;
  NSString* _groupId;
  NSInteger _extensionGroupId;
  NSString* _title;
  NSInteger _color;
  NSArray* _tabSessionIds;
}
@synthesize groupId = _groupId, extensionGroupId = _extensionGroupId;
@synthesize title = _title, color = _color, collapsed = _collapsed;
@synthesize tabSessionIds = _tabSessionIds;
- (instancetype)init {
  if ((self = [super init])) {
    _groupId = @"";
    _extensionGroupId = -1;
    _title = @"";
    _tabSessionIds = @[];
  }
  return self;
}
@end

@implementation ArcTabGroupResult {
  BOOL _success;
  NSString* _error;
  NSString* _groupId;
  NSInteger _extensionGroupId;
}
@synthesize success = _success, error = _error;
@synthesize groupId = _groupId, extensionGroupId = _extensionGroupId;
- (instancetype)init {
  if ((self = [super init])) {
    _extensionGroupId = -1;
  }
  return self;
}
@end

@implementation ArcTabGroupProperties {
  arc::bindings::TabGroupProperties _tab_group_properties;
}
- (instancetype)init {
  return [super init];
}
- (instancetype)initWithTabGroupProperties:(const void*)properties {
  if ((self = [super init])) {
    _tab_group_properties =
        *static_cast<const arc::bindings::TabGroupProperties*>(properties);
  }
  return self;
}
- (NSString*)title {
  return _tab_group_properties.title ? ArcNSString(*_tab_group_properties.title) : nil;
}
- (void)setTitle:(NSString*)title {
  if (title) {
    _tab_group_properties.title = ArcUTF8(title);
  } else {
    _tab_group_properties.title.reset();
  }
}
- (NSNumber*)color {
  return _tab_group_properties.color
      ? @(static_cast<NSInteger>(*_tab_group_properties.color)) : nil;
}
- (void)setColor:(NSNumber*)color {
  if (color) {
    _tab_group_properties.color =
        static_cast<arc::bindings::TabGroupColor>(static_cast<int>(color.integerValue));
  } else {
    _tab_group_properties.color.reset();
  }
}
- (NSNumber*)collapsed {
  return _tab_group_properties.collapsed ? @(*_tab_group_properties.collapsed) : nil;
}
- (void)setCollapsed:(NSNumber*)collapsed {
  if (collapsed) {
    _tab_group_properties.collapsed = collapsed.boolValue;
  } else {
    _tab_group_properties.collapsed.reset();
  }
}
- (arc::bindings::TabGroupProperties)tab_group_properties {
  return _tab_group_properties;
}
- (void)setTab_group_properties:(arc::bindings::TabGroupProperties)properties {
  _tab_group_properties = std::move(properties);
}
@end
