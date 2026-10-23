// Copyright 2026 ArcMod. Chromium permission requests rendered by the host app.
#import "arc/api/arc_internal.h"
#include "arc/browser/ui_hooks.h"

#include <array>
#include <map>
#include <optional>
#include <utility>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "components/permissions/permission_request.h"
#include "components/url_formatter/elide_url.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "url/origin.h"

@interface ArcWebContents (UIInternal)
- (id)uiDelegate;
@end

namespace {
struct PermissionData {
  NSInteger type;
  std::string origin;
  std::map<NSUInteger, std::string> additional_data;
};
struct DisplayOriginData {
  std::u16string name;
  bool is_origin;
};
}  // namespace

ARC_EXPORT @interface ArcPermissionRequest : NSObject
- (instancetype)initWithPermissionRequest:(const void*)request;
@property(nonatomic, readonly) NSInteger type;
@property(nonatomic, readonly, copy) NSString* originUrl;
@property(nonatomic, readonly) NSDictionary* additionalData;
@end

// A shared value class outside this package. Add only the producer adapter as a
// category; the generated shell remains its primary implementation.
@interface ArcDisplayNameOrOrigin : NSObject
@end
@interface ArcDisplayNameOrOrigin (PermissionUI)
- (instancetype)initWithDisplayNameOrOrigin:(const void*)origin;
- (NSString*)nameOrOrigin;
- (BOOL)isOrigin;
@end

@protocol ArcHostPermissionUI <NSObject>
- (void)webContents:(ArcWebContents*)contents willAskForPermission:(ArcDisplayNameOrOrigin*)origin
       withRequests:(NSArray*)requests withQuietUIReason:(uint64_t)reason
         completion:(void (^)(uint64_t))completion;
- (void)webContentsClosePermission:(ArcWebContents*)contents;
@end

@implementation ArcPermissionRequest {
  std::optional<PermissionData> _request;
}
- (instancetype)initWithPermissionRequest:(const void*)request {
  CHECK(request);
  if ((self = [super init])) { _request = *static_cast<const PermissionData*>(request); }
  return self;
}
- (NSInteger)type { CHECK(_request); return _request->type; }
- (NSString*)originUrl { CHECK(_request); return ArcNSString(_request->origin); }
- (NSDictionary*)additionalData {
  CHECK(_request);
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (const auto& [key, value] : _request->additional_data) { result[@(key)] = ArcNSString(value); }
  return [result copy];
}
@end

#import <objc/runtime.h>
namespace { const char kDisplayOriginData = 0; }
@implementation ArcDisplayNameOrOrigin (PermissionUI)
- (instancetype)initWithDisplayNameOrOrigin:(const void*)origin {
  CHECK(origin);
  if ((self = [super init])) {
    const auto& data = *static_cast<const DisplayOriginData*>(origin);
    objc_setAssociatedObject(self, &kDisplayOriginData, @[ArcNSString(data.name), @(data.is_origin)],
                             OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  return self;
}
- (NSString*)nameOrOrigin { return [objc_getAssociatedObject(self, &kDisplayOriginData) objectAtIndex:0]; }
- (BOOL)isOrigin { return [[objc_getAssociatedObject(self, &kDisplayOriginData) objectAtIndex:1] boolValue]; }
@end

namespace {
NSInteger RequestType(permissions::RequestType type) {
  // ArcCore 0x433f940, lookup table 0xc9d7090. Do not cast Chromium's enum.
  using T = permissions::RequestType;
  switch (type) {
    case T::kAmbientLogin: return 27;
    case T::kArSession: return 0;
    case T::kCameraPanTiltZoom: return 1;
    case T::kCameraStream: return 2;
    case T::kCapturedSurfaceControl: return 3;
    case T::kClipboard: return 4;
    case T::kTopLevelStorageAccess: return 5;
    case T::kDiskQuota: return 6;
    case T::kFileSystemAccess: return 7;
    case T::kLocalFonts: return 8;
    case T::kGeolocation: return 9;
    case T::kHandTracking: return 10;
    case T::kIdentityProvider: return 11;
    case T::kIdleDetection: return 12;
    case T::kMicStream: return 13;
    case T::kMidiSysex: return 14;
    case T::kMultipleDownloads: return 15;
    case T::kNotifications: return 16;
    case T::kKeyboardLock: return 17;
    case T::kPointerLock: return 18;
    case T::kRegisterProtocolHandler: return 19;
    case T::kSensors: return 20;
    case T::kStorageAccess: return 21;
    case T::kVrSession: return 22;
    case T::kWebAppInstallation: return 23;
    case T::kWindowManagement: return 24;
    case T::kLocalNetwork: return 25;
    case T::kLoopbackNetwork: return 26;
  }
}

uint64_t QuietReason(std::optional<permissions::PermissionUiSelector::QuietUiReason> reason) {
  if (!reason) { return 0; }
  static constexpr std::array<uint64_t, 6> reasons = {1, 2, 3, 4, 5, 5};
  const size_t index = static_cast<size_t>(*reason);
  return index < reasons.size() ? reasons[index] : 0;
}

class HostPermissionPrompt : public permissions::PermissionPrompt {
 public:
  HostPermissionPrompt(content::WebContents* contents, Delegate* delegate)
      : contents_(contents->GetWeakPtr()), delegate_(delegate->GetWeakPtr()) {
    // The original producer and decision consumer both post UI tasks.
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
        base::BindOnce(&HostPermissionPrompt::Show, weak_factory_.GetWeakPtr()));
  }
  ~HostPermissionPrompt() override {
    if (!shown_ || !contents_) { return; }
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_.get()];
    id<ArcHostPermissionUI> delegate = [owner respondsToSelector:@selector(uiDelegate)]
        ? [owner uiDelegate] : nil;
    if ([delegate respondsToSelector:@selector(webContentsClosePermission:)]) {
      [delegate webContentsClosePermission:owner];
    }
  }
  bool UpdateAnchor() override { return true; }
  TabSwitchingBehavior GetTabSwitchingBehavior() override { return kKeepPromptAlive; }
  permissions::PermissionPromptDisposition GetPromptDisposition() const override {
    return permissions::PermissionPromptDisposition::ANCHORED_BUBBLE;
  }
  bool IsAskPrompt() const override { return true; }
  std::optional<gfx::Rect> GetViewBoundsInScreen() const override { return std::nullopt; }
  std::vector<permissions::ElementAnchoredBubbleVariant> GetPromptVariants() const override { return {}; }
  std::optional<permissions::feature_params::PermissionElementPromptPosition>
      GetPromptPosition() const override { return std::nullopt; }

 private:
  void Show() {
    if (!contents_ || !delegate_) { return; }
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_.get()];
    id<ArcHostPermissionUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
    if (![ui respondsToSelector:@selector(webContents:willAskForPermission:withRequests:withQuietUIReason:completion:)]) {
      Resolve(3);
      return;
    }
    NSMutableArray* requests = [NSMutableArray array];
    for (const auto& request : delegate_->Requests()) {
      PermissionData data{RequestType(request->request_type()), request->requesting_origin().spec(), {}};
      [requests addObject:[[ArcPermissionRequest alloc] initWithPermissionRequest:&data]];
    }
    DisplayOriginData display{url_formatter::FormatOriginForSecurityDisplay(
        url::Origin::Create(delegate_->GetRequestingOrigin()),
        url_formatter::SchemeDisplay::OMIT_HTTP_AND_HTTPS), true};
    ArcDisplayNameOrOrigin* origin = [[ArcDisplayNameOrOrigin alloc] initWithDisplayNameOrOrigin:&display];
    const uint64_t quiet = QuietReason(delegate_->ReasonForUsingQuietUi());
    shown_ = true;
    auto weak = weak_factory_.GetWeakPtr();
    [ui webContents:owner willAskForPermission:origin withRequests:requests withQuietUIReason:quiet
        completion:^(uint64_t response) {
          content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
              base::BindOnce(&HostPermissionPrompt::Resolve, weak, response < 3 ? response : 3));
        }];
  }
  void Resolve(uint64_t response) {
    if (!delegate_ || decided_) { return; }
    decided_ = true;
    const PromptOptions options = std::monostate();
    switch (response) {
      case 0: delegate_->Accept(options); break;
      case 1: delegate_->AcceptThisTime(options); break;
      case 2: delegate_->Deny(options); break;
      default: delegate_->Dismiss(options); break;
    }
  }
  base::WeakPtr<content::WebContents> contents_;
  base::WeakPtr<Delegate> delegate_;
  bool shown_ = false;
  bool decided_ = false;
  base::WeakPtrFactory<HostPermissionPrompt> weak_factory_{this};
};
}  // namespace

namespace arc {
std::unique_ptr<permissions::PermissionPrompt> CreatePermissionPrompt(
    content::WebContents* contents, permissions::PermissionPrompt::Delegate* delegate) {
  return std::make_unique<HostPermissionPrompt>(contents, delegate);
}
}  // namespace arc
