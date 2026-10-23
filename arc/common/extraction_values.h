// Copyright 2026 ArcMod. Native snapshot contracts for Arc's extraction API.
#ifndef ARC_COMMON_EXTRACTION_VALUES_H_
#define ARC_COMMON_EXTRACTION_VALUES_H_

#import "arc/browser/ui_values.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace arc {
// These are our producer contracts, not the proprietary ArcCore native ABI.
// Every public const-void initializer copies the corresponding value.
struct ExtractedContentNodeData {
  int type = 1;
  std::vector<ExtractedContentNodeData> children;
  std::optional<std::string> text_content;
  bool has_emphasis = false;
  int text_size = 2;
  uint32_t text_color = 0;
  std::optional<std::string> link_url;
  std::optional<std::string> image_alt;
  std::optional<std::string> iframe_origin;
  std::optional<UIFormControlInfoData> form_control_info;
  std::optional<std::string> form_name;
  std::optional<std::string> form_action_url;
  std::optional<std::string> table_name;
};

struct ExtractedPageContentData {
  std::string url;
  std::string title;
  double extraction_time = 0;
  ExtractedContentNodeData root_node;
};

struct PageContentExtractionData {
  base::WeakPtr<Profile> profile;
  uintptr_t identity = 0;
};
}  // namespace arc

ARC_EXPORT @interface ArcContentNode : NSObject
- (instancetype)initWithContentNode:(const void*)node;
@property(nonatomic, readonly) NSInteger type;
@property(nonatomic, readonly) NSArray* children;
@property(nonatomic, readonly, copy) NSString* textContent;
@property(nonatomic, readonly) BOOL hasEmphasis;
@property(nonatomic, readonly) NSInteger textSize;
@property(nonatomic, readonly, copy) NSString* textColor;
@property(nonatomic, readonly, copy) NSString* linkURL;
@property(nonatomic, readonly, copy) NSString* imageAlt;
@property(nonatomic, readonly, copy) NSString* iframeOrigin;
@property(nonatomic, readonly, copy) NSString* formName;
@property(nonatomic, readonly, copy) NSString* formActionURL;
@property(nonatomic, readonly, copy) NSString* tableName;
@property(nonatomic, readonly) ArcFormControlInfo* formControlInfo;
@end

ARC_EXPORT @interface ArcExtractedPageContent : NSObject
- (instancetype)initWithExtractedPageContent:(const void*)content;
@property(nonatomic, readonly, copy) NSString* url;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly) ArcContentNode* rootNode;
@property(nonatomic, readonly) double extractionTime;
@end

ARC_EXPORT @interface ArcBrowserContextPageContentExtraction : NSObject
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (instancetype)initWithBrowserContextPageContentExtraction:(const void*)service;
- (instancetype)initWithBrowserContextPageContentExtraction:(const void*)service
                                         forBrowserContext:(ArcBrowserContext*)context;
@property(nonatomic, weak) ArcBrowserContext* owner;
- (void)getExtractedContentForWebContents:(ArcWebContents*)contents
                              completion:(void (^)(ArcExtractedPageContent*))completion;
@end

#endif  // ARC_COMMON_EXTRACTION_VALUES_H_
