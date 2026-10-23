// Copyright 2026 ArcMod. Tab-group values transported between Arc and Chrome.
#ifndef ARC_BROWSER_CORE_VALUES_H_
#define ARC_BROWSER_CORE_VALUES_H_

#include <optional>
#include <string>

// Keep these global POD tags: their names and scalar encodings are part of the
// public Objective-C initializer signatures.
struct KeyboardAccelerator {
  unsigned int key;
  int modifiers;
};
struct FindResult {
  int num_matches;
  int active_match_ordinal;
};

namespace arc {
struct RedirectEntry {
  std::string url;
  std::u16string title;
  bool hidden = false;
  int redirect_type = 0;
};
static_assert(sizeof(RedirectEntry) == 56);
static_assert(sizeof(std::optional<RedirectEntry>) == 64);

struct FindTextWithDirectiveResult {
  std::string directive;
  bool result = false;
};
static_assert(sizeof(FindTextWithDirectiveResult) == 32);
}  // namespace arc

namespace arc::bindings {
// The boundary stores colors as signed 32-bit enum values. Producers provide
// the ordinal; these value classes do not interpret or mutate a native group.
enum class TabGroupColor : int {};
struct TabGroupProperties {
  std::optional<std::string> title;
  std::optional<TabGroupColor> color;
  std::optional<bool> collapsed;
};
static_assert(sizeof(TabGroupProperties) == 48);
}  // namespace arc::bindings

#ifdef __OBJC__
#import "arc/api/arc_internal.h"

ARC_EXPORT @interface ArcRedirectEntry : NSObject
- (instancetype)initWithRedirectEntry:(const void*)entry;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly) BOOL hidden;
@property(nonatomic, readonly) NSInteger redirectType;
@property(nonatomic, readonly) std::optional<arc::RedirectEntry> redirect_entry;
@end

ARC_EXPORT @interface ArcKeyboardAccelerator : NSObject
- (instancetype)initWithAccelerator:(const KeyboardAccelerator*)accelerator;
@property(nonatomic, readonly) NSUInteger key;
@property(nonatomic, readonly) NSUInteger modifiers;
@end

ARC_EXPORT @interface ArcKeyEventData : NSObject
- (instancetype)initWithEventType:(NSUInteger)eventType domCode:(NSString*)domCode
                             key:(NSString*)key windowsVirtualKeyCode:(int)code;
@property(nonatomic, readonly) NSUInteger eventType;
@property(nonatomic, readonly, copy) NSString* domCode;
@property(nonatomic, readonly, copy) NSString* key;
@property(nonatomic, readonly) int windowsVirtualKeyCode;
@end

ARC_EXPORT @interface ArcOverscrollBehavior : NSObject
@property(nonatomic) NSInteger x;
@property(nonatomic) NSInteger y;
@end

ARC_EXPORT @interface ArcOverscrollParams : NSObject
@property(nonatomic, strong) ArcOverscrollBehavior* overscrollBehavior;
@end

ARC_EXPORT @interface ArcFindResult : NSObject
- (instancetype)initWithFindResult:(const FindResult*)result;
@property(nonatomic, readonly) NSInteger numMatches;
@property(nonatomic, readonly) NSInteger activeMatchOrdinal;
@end

ARC_EXPORT @interface ArcFindTextWithDirectiveResult : NSObject
- (instancetype)initWithFindTextWithDirectiveResult:(const void*)result;
@property(nonatomic, readonly, copy) NSString* directive;
@property(nonatomic, readonly) BOOL result;
@property(nonatomic) arc::FindTextWithDirectiveResult find_text_with_directive_result;
@end

ARC_EXPORT @interface ArcTabGroupInfo : NSObject
@property(nonatomic, copy) NSString* groupId;
@property(nonatomic) NSInteger extensionGroupId;
@property(nonatomic, copy) NSString* title;
@property(nonatomic) NSInteger color;
@property(nonatomic) BOOL collapsed;
@property(nonatomic, copy) NSArray* tabSessionIds;
@end

ARC_EXPORT @interface ArcTabGroupResult : NSObject
@property(nonatomic) BOOL success;
@property(nonatomic, copy) NSString* error;
@property(nonatomic, copy) NSString* groupId;
@property(nonatomic) NSInteger extensionGroupId;
@end

ARC_EXPORT @interface ArcTabGroupProperties : NSObject
- (instancetype)initWithTabGroupProperties:(const void*)properties;
@property(nonatomic, copy) NSString* title;
@property(nonatomic, strong) NSNumber* color;
@property(nonatomic, strong) NSNumber* collapsed;
@property(nonatomic) arc::bindings::TabGroupProperties tab_group_properties;
@end
#endif  // __OBJC__

#endif  // ARC_BROWSER_CORE_VALUES_H_
