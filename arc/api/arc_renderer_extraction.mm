// Copyright 2026 ArcMod. Structured whole-page extraction via Chromium's APC.
#import "arc/common/extraction_values.h"

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "base/strings/utf_string_conversions.h"
#include "base/time/time.h"
#include "chrome/browser/profiles/profile.h"
#include "components/optimization_guide/content/browser/page_content_proto_provider.h"
#include "components/optimization_guide/proto/features/common_quality_data.pb.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "third_party/blink/public/mojom/content_extraction/ai_page_content.mojom.h"

namespace arc {
namespace {
namespace proto = optimization_guide::proto;

class ProfileExtractionIdentity final : public base::SupportsUserData::Data {
 public:
  static scoped_refptr<PageContentExtractionIdentity> For(Profile* profile) {
    auto* data = static_cast<ProfileExtractionIdentity*>(profile->GetUserData(&kKey));
    if (!data) {
      auto owned = std::make_unique<ProfileExtractionIdentity>();
      data = owned.get();
      profile->SetUserData(&kKey, std::move(owned));
    }
    return data->identity_;
  }
 private:
  const scoped_refptr<PageContentExtractionIdentity> identity_ =
      base::MakeRefCounted<PageContentExtractionIdentity>(uint8_t{0});
  inline static const int kKey = 0;
};

NSString* OptionalString(const std::optional<std::string>& value) {
  return value ? ArcNSString(*value) : nil;
}

int ArcNodeType(int proto_type) {
  constexpr auto kTypes = std::to_array<int>(
      {0, 1, 2, 4, 5, 11, 12, 17, 7, 14, 16, 1, 1, 1,
       1, 1, 1, 1, 1, 3, 15, 6, 13, 18, 8, 9, 10});
  return proto_type >= 1 && proto_type <= static_cast<int>(kTypes.size())
             ? kTypes[proto_type - 1]
             : 1;
}

int ArcTextSize(int proto_size) {
  constexpr auto kSizes = std::to_array<int>({0, 1, 3, 4});
  return proto_size >= 1 && proto_size <= static_cast<int>(kSizes.size())
             ? kSizes[proto_size - 1]
             : 2;
}

int ArcControlType(int proto_type) {
  constexpr auto kTypes = std::to_array<int>(
      {8, 12, 16, 17, 18, 19, 22, 24, 25, 27, 30, 31, 0, 33, 1, 2,
       3, 5, 6, 7, 9, 10, 11, 13, 14, 15, 20, 21, 23, 26, 28, 29});
  return proto_type >= 2 && proto_type < static_cast<int>(kTypes.size()) + 2
             ? kTypes[proto_type - 2]
             : 0;
}

UIFormControlInfoData ConvertControl(const proto::FormControlData& input,
                                     proto::RedactionDecision redaction) {
  UIFormControlInfoData output;
  output.control_type = ArcControlType(input.form_control_type());
  if (input.has_field_name()) {
    output.field_name = input.field_name();
  }
  if (input.has_field_value()) {
    output.field_value = input.field_value();
  }
  if (input.has_placeholder()) {
    output.placeholder = input.placeholder();
  }
  output.is_checked = input.is_checked();
  output.is_required = input.is_required();
  // Arc's converter recognizes only the recovered enum value 2, rather than
  // guessing redaction from the DOM/control type or later proto enum values.
  // The current provider mirrors ContentAttributes.redaction_decision into
  // the deprecated FormControlData field read by Arc's original converter.
  output.is_redacted = redaction == proto::REDACTION_DECISION_REDACTED_HAS_BEEN_PASSWORD;
  output.select_options.reserve(input.select_options_size());
  for (const auto& option : input.select_options()) {
    UISelectOptionData native;
    if (option.has_value()) {
      native.value = option.value();
    }
    if (option.has_text()) {
      native.text = option.text();
    }
    native.is_selected = option.is_selected();
    native.is_disabled = option.is_disabled();
    output.select_options.push_back(std::move(native));
  }
  return output;
}

ExtractedContentNodeData ConvertNode(const proto::ContentNode& input) {
  ExtractedContentNodeData output;
  const auto& attributes = input.content_attributes();
  output.type = ArcNodeType(attributes.attribute_type());
  output.children.reserve(input.children_nodes_size());
  for (const auto& child : input.children_nodes()) {
    output.children.push_back(ConvertNode(child));
  }
  // These assignments follow the proto oneof, independently of the type enum.
  // Retain optional string presence, including present empty strings.
  if (attributes.has_text_data()) {
    const auto& text = attributes.text_data();
    if (text.has_text_content()) {
      output.text_content = text.text_content();
    }
    const auto& style = text.text_style();
    output.has_emphasis = style.has_emphasis();
    output.text_size = ArcTextSize(style.text_size());
    output.text_color = style.color();
  } else if (attributes.has_anchor_data()) {
    const auto& anchor = attributes.anchor_data();
    if (anchor.has_url()) {
      output.link_url = anchor.url();
    }
  } else if (attributes.has_image_data()) {
    const auto& image = attributes.image_data();
    if (image.has_image_caption()) {
      output.image_alt = image.image_caption();
    }
  } else if (attributes.has_iframe_data()) {
    const auto& iframe = attributes.iframe_data();
    if (iframe.has_frame_data() && iframe.frame_data().has_security_origin() &&
        iframe.frame_data().security_origin().has_value()) {
      output.iframe_origin = iframe.frame_data().security_origin().value();
    }
  } else if (attributes.has_form_data()) {
    const auto& form = attributes.form_data();
    if (form.has_form_name()) {
      output.form_name = form.form_name();
    }
    if (form.has_action_url()) {
      output.form_action_url = form.action_url();
    }
  } else if (attributes.has_table_data()) {
    const auto& table = attributes.table_data();
    if (table.has_table_name()) {
      output.table_name = table.table_name();
    }
  } else if (attributes.has_form_control_data()) {
    output.form_control_info = ConvertControl(attributes.form_control_data(),
                                               attributes.redaction_decision());
  }
  return output;
}

void CompleteExtraction(
    base::WeakPtr<content::WebContents> contents,
    content::GlobalRenderFrameHostId requested_frame,
    void (^completion)(ArcExtractedPageContent*),
    optimization_guide::AIPageContentResultOrError result) {
  if (!result.has_value() || !contents || !result->proto.has_root_node() ||
      !contents->GetPrimaryMainFrame() ||
      contents->GetPrimaryMainFrame()->GetGlobalId() != requested_frame) {
    completion(nil);
    return;
  }
  ExtractedPageContentData output;
  output.url = contents->GetLastCommittedURL().spec();
  output.title = base::UTF16ToUTF8(contents->GetTitle());
  output.extraction_time = base::Time::Now().InSecondsFSinceUnixEpoch();
  output.root_node = ConvertNode(result->proto.root_node());
  completion([[ArcExtractedPageContent alloc] initWithExtractedPageContent:&output]);
}
}  // namespace
}  // namespace arc

@implementation ArcContentNode {
  arc::ExtractedContentNodeData _data;
  NSArray* _children;
  ArcFormControlInfo* _formControlInfo;
}
@synthesize children = _children, formControlInfo = _formControlInfo;
- (instancetype)initWithContentNode:(const void*)node {
  if ((self = [super init]) && node) {
    _data = *static_cast<const arc::ExtractedContentNodeData*>(node);
  }
  return self;
}
- (NSInteger)type { return _data.type; }
- (NSArray*)children {
  if (!_children) {
    NSMutableArray* children = [NSMutableArray arrayWithCapacity:_data.children.size()];
    for (const auto& child : _data.children) {
      [children addObject:[[ArcContentNode alloc] initWithContentNode:&child]];
    }
    _children = children;
  }
  return _children;
}
- (NSString*)textContent { return arc::OptionalString(_data.text_content); }
- (BOOL)hasEmphasis { return _data.has_emphasis; }
- (NSInteger)textSize { return _data.text_size; }
- (NSString*)textColor {
  return _data.text_color ? [NSString stringWithFormat:@"#%08X", _data.text_color] : nil;
}
- (NSString*)linkURL { return arc::OptionalString(_data.link_url); }
- (NSString*)imageAlt { return arc::OptionalString(_data.image_alt); }
- (NSString*)iframeOrigin { return arc::OptionalString(_data.iframe_origin); }
- (NSString*)formName { return arc::OptionalString(_data.form_name); }
- (NSString*)formActionURL { return arc::OptionalString(_data.form_action_url); }
- (NSString*)tableName { return arc::OptionalString(_data.table_name); }
- (ArcFormControlInfo*)formControlInfo {
  if (!_formControlInfo && _data.form_control_info) {
    _formControlInfo = [[ArcFormControlInfo alloc] initWithFormControlInfo:&*_data.form_control_info];
  }
  return _formControlInfo;
}
@end

@implementation ArcExtractedPageContent {
  arc::ExtractedPageContentData _data;
  ArcContentNode* _rootNode;
}
@synthesize rootNode = _rootNode;
- (instancetype)initWithExtractedPageContent:(const void*)content {
  if ((self = [super init]) && content) {
    _data = *static_cast<const arc::ExtractedPageContentData*>(content);
  }
  return self;
}
- (NSString*)url { return ArcNSString(_data.url); }
- (NSString*)title { return ArcNSString(_data.title); }
- (double)extractionTime { return _data.extraction_time; }
- (ArcContentNode*)rootNode {
  if (!_rootNode) {
    _rootNode = [[ArcContentNode alloc] initWithContentNode:&_data.root_node];
  }
  return _rootNode;
}
@end

@implementation ArcBrowserContextPageContentExtraction {
  base::WeakPtr<Profile> _profile;
  scoped_refptr<arc::PageContentExtractionIdentity> _serviceIdentity;
}
@synthesize owner = _owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  arc::PageContentExtractionData native;
  Profile* profile = [context profile];
  if (profile) {
    native.profile = profile->GetWeakPtr();
    native.identity = arc::ProfileExtractionIdentity::For(profile);
  }
  return [self initWithBrowserContextPageContentExtraction:&native forBrowserContext:context];
}
- (instancetype)initWithBrowserContextPageContentExtraction:(const void*)service {
  if ((self = [super init])) {
    if (service) {
      const auto& native = *static_cast<const arc::PageContentExtractionData*>(service);
      _profile = native.profile;
      _serviceIdentity = native.identity;
    }
    if (!_serviceIdentity) {
      _serviceIdentity = base::MakeRefCounted<arc::PageContentExtractionIdentity>(uint8_t{0});
    }
  }
  return self;
}
- (instancetype)initWithBrowserContextPageContentExtraction:(const void*)service
                                         forBrowserContext:(ArcBrowserContext*)context {
  if ((self = [self initWithBrowserContextPageContentExtraction:service])) {
    _owner = context;
  }
  return self;
}
- (BOOL)isEqual:(id)object {
  if (object == self) {
    return YES;
  }
  if (![object isKindOfClass:[ArcBrowserContextPageContentExtraction class]]) {
    return NO;
  }
  ArcBrowserContextPageContentExtraction* other = object;
  return _serviceIdentity && _serviceIdentity.get() == other->_serviceIdentity.get();
}
- (NSUInteger)hash {
  return _serviceIdentity ? reinterpret_cast<NSUInteger>(_serviceIdentity.get()) : [super hash];
}
- (void)getExtractedContentForWebContents:(ArcWebContents*)contents
                              completion:(void (^)(ArcExtractedPageContent*))completion {
  if (!completion) {
    return;
  }
  if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    __weak ArcBrowserContextPageContentExtraction* weakSelf = self;
    __weak ArcWebContents* weakContents = contents;
    void (^copiedCompletion)(ArcExtractedPageContent*) = [completion copy];
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(^{
      ArcBrowserContextPageContentExtraction* service = weakSelf;
      if (service) {
        [service getExtractedContentForWebContents:weakContents completion:copiedCompletion];
      } else {
        copiedCompletion(nil);
      }
    }));
    return;
  }
  auto* native = [contents webContents];
  if (!_profile || !native ||
      !native->GetPrimaryMainFrame() || !native->GetPrimaryMainFrame()->IsRenderFrameLive()) {
    // The recovered unavailable-native branches invoke their empty result
    // synchronously, without a task post or queue hop.
    completion(nil);
    return;
  }
  optimization_guide::GetAIPageContent(
      native, optimization_guide::DefaultAIPageContentOptions(false),
      base::BindOnce(&arc::CompleteExtraction, native->GetWeakPtr(),
                     native->GetPrimaryMainFrame()->GetGlobalId(), [completion copy]));
}
@end
