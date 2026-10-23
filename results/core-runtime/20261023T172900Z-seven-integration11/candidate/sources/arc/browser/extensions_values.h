// Copyright 2026 ArcMod. Extension snapshots at the ArcCore boundary.

#ifndef ARC_BROWSER_EXTENSIONS_VALUES_H_
#define ARC_BROWSER_EXTENSIONS_VALUES_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace gfx {
class Image;
}

namespace arc {

class ImageImpl;

// The native Arc record contains a polymorphic, shared image value (24 bytes),
// rather than an inline gfx::Image. Its replacement implementation owns a Chrome
// image. Never interpret an original-engine ImageImpl as this implementation.
class Image {
 public:
  Image();
  explicit Image(const gfx::Image& image);
  Image(const Image& image);
  Image& operator=(const Image& image);
  virtual ~Image();

  const gfx::Image* gfxImage() const;

 private:
  std::shared_ptr<ImageImpl> impl_;
};

// Field order, native widths and string encodings follow the original metadata.
// Public initWith...: methods take pointers to these records, never pointers to
// similarly named Chromium extension classes.
struct ExtensionInfo {
  std::string name;
  std::string id;
  std::string url;
  std::string description;
  std::vector<std::u16string> permissions;
  Image icon;
  double rating = 0;
  int num_ratings = 0;
  bool enabled = false;
  bool has_action = false;
  std::string version;
  std::string omnibox_keyword;
  bool from_webstore = false;
};

struct ExtensionCommand {
  std::string extension_id;
  std::string command_name;
  std::string command_description;
  std::optional<std::string> shortcut;
  bool is_global = false;
};

struct StackFrame {
  unsigned int line_number = 0;
  unsigned int column_number = 0;
  std::u16string source;
  std::u16string function;
};

struct ExtensionRuntimeError {
  std::string extension_id;
  std::string context_url_spec;
  std::u16string message;
  std::u16string source;
  bool from_incognito = false;
  int severity = 0;
  std::vector<StackFrame> stack_trace;
};

struct ExtensionToolbarAction {
  std::string id;
  std::u16string accessible_name;
  std::u16string tooltip_text;
  Image icon;
  bool enabled = false;
  std::optional<int> pinned_position;
};

static_assert(sizeof(Image) == 24);
static_assert(sizeof(ExtensionInfo) == 216);
static_assert(sizeof(ExtensionCommand) == 112);
static_assert(sizeof(StackFrame) == 56);
static_assert(sizeof(ExtensionRuntimeError) == 128);
static_assert(sizeof(ExtensionToolbarAction) == 112);

}  // namespace arc

#ifdef __OBJC__
#import "arc/api/arc_internal.h"

ARC_EXPORT @interface ArcExtensionInfo : NSObject
- (instancetype)initWithExtensionInfo:(const void*)info;
- (void*)mutableExtensionInfo;
@property(nonatomic, readonly, copy) NSString* name;
@property(nonatomic, readonly, copy) NSString* id;
@property(nonatomic, readonly, copy) NSString* description;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly, copy) NSArray<NSString*>* permissions;
@property(nonatomic, readonly) NSImage* icon;
@property(nonatomic, readonly) double rating;
@property(nonatomic, readonly) long long numRatings;
@property(nonatomic, readonly) BOOL enabled;
@property(nonatomic, readonly) BOOL hasAction;
@property(nonatomic, readonly, copy) NSString* version;
@property(nonatomic, readonly, copy) NSString* omniboxKeyword;
@property(nonatomic, readonly) BOOL fromWebstore;
@end

ARC_EXPORT @interface ArcExtensionCommand : NSObject
- (instancetype)initWithExtensionCommand:(const void*)command;
@property(nonatomic, readonly, copy) NSString* extensionId;
@property(nonatomic, readonly, copy) NSString* commandName;
@property(nonatomic, readonly, copy) NSString* commandDescription;
@property(nonatomic, readonly, copy) NSString* shortcut;
@property(nonatomic, readonly) BOOL isGlobal;
@end

ARC_EXPORT @interface ArcExtensionRuntimeErrorStackFrame : NSObject
- (instancetype)initWithExtensionRuntimeErrorStackFrame:(const void*)frame;
@property(nonatomic, readonly) unsigned int lineNumber;
@property(nonatomic, readonly) unsigned int columnNumber;
@property(nonatomic, readonly, copy) NSString* source;
@property(nonatomic, readonly, copy) NSString* function;
@end

ARC_EXPORT @interface ArcExtensionRuntimeError : NSObject
- (instancetype)initWithExtensionRuntimeError:(const void*)error;
@property(nonatomic, readonly, copy) NSString* extensionID;
@property(nonatomic, readonly) NSURL* contextURL;
@property(nonatomic, readonly, copy) NSString* message;
@property(nonatomic, readonly, copy) NSString* source;
@property(nonatomic, readonly) BOOL fromIncognito;
@property(nonatomic, readonly) long long severity;
@property(nonatomic, readonly) NSArray<ArcExtensionRuntimeErrorStackFrame*>* stackTrace;
@end

ARC_EXPORT @interface ArcExtensionToolbarAction : NSObject
- (instancetype)initWithExtensionToolbarAction:(const void*)action;
@property(nonatomic, readonly, copy) NSString* id;
@property(nonatomic, readonly, copy) NSString* accessibleName;
@property(nonatomic, readonly, copy) NSString* tooltipText;
@property(nonatomic, readonly) NSImage* icon;
@property(nonatomic, readonly) BOOL enabled;
@property(nonatomic, readonly) NSNumber* pinnedPosition;
@end
#endif  // __OBJC__

#endif  // ARC_BROWSER_EXTENSIONS_VALUES_H_
