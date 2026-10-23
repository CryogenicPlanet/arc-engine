// Copyright 2026 ArcMod. Chromium-backed media session boundary.
#import "arc/api/arc_internal.h"

#include <optional>
#include <algorithm>
#import <QuartzCore/QuartzCore.h>
#import <objc/runtime.h>
#include "arc/browser/webextras_media.h"
#include "base/memory/raw_ptr.h"
#include "content/public/browser/context_factory.h"
#include "content/public/browser/overlay_window.h"
#include "ui/accelerated_widget_mac/display_ca_layer_tree.h"
#include "ui/compositor/recyclable_compositor_mac.h"
#include "ui/compositor/layer_surface.h"
#include "ui/gfx/display_color_spaces.h"
#include "base/memory/weak_ptr.h"
#include "base/functional/bind.h"
#include "net/base/auth.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "content/public/browser/media_session.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/picture_in_picture_window_controller.h"
#include "content/public/browser/video_picture_in_picture_window_controller.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "services/media_session/public/cpp/media_position.h"
#include "services/media_session/public/mojom/media_session.mojom.h"

@protocol ArcMediaDelegate <NSObject>
- (void)webContents:(id)owner pipControllerCreated:(id)controller;
- (void)webContentsPiPControllerDestroyed:(id)owner;
- (void)webContents:(id)owner pipVideoResolutionChanged:(CGSize)size;
- (void)webContentsShowPiP:(id)owner;
- (void)webContentsHidePiP:(id)owner;
- (void)webContentsClosePiP:(id)owner;
- (void)webContents:(id)owner pipPlaybackStateChanged:(uint64_t)state;
- (void)webContents:(id)owner pipFeatureStateChanged:(uint64_t)feature turnedOn:(BOOL)on;
- (void)webContents:(id)owner pipMediaPositionChanged:(id)position;
- (void)webContents:(id)owner pipSourceTitleChanged:(NSString*)title;
- (void)webContents:(id)owner pipFaviconImagesChanged:(NSArray*)images;
- (void)webContents:(id)owner didChangeMediaSessionState:(uint64_t)state;
- (void)webContents:(id)owner didChangeAvailableMediaActions:(NSIndexSet*)actions;
- (void)webContents:(id)owner didChangeMediaPosition:(id)position;
- (void)webContents:(id)owner didChangeCameraState:(uint64_t)state;
- (void)webContents:(id)owner didChangeMicrophoneState:(uint64_t)state;
@end

ARC_EXPORT @interface ArcMediaPosition : NSObject {
  std::optional<media_session::MediaPosition> _nativePosition;
}
- (instancetype)initWithMediaPosition:(const void*)position;
@property(nonatomic, readonly) double playbackRate;
@property(nonatomic, readonly) double duration;
@property(nonatomic, readonly) double currentTime;
@property(nonatomic, readonly) BOOL endOfMedia;
@end
@implementation ArcMediaPosition
- (instancetype)initWithMediaPosition:(const void*)position {
  if ((self = [super init]) && position) {
    _nativePosition = *static_cast<const media_session::MediaPosition*>(position);
  }
  return self;
}
- (double)playbackRate { return _nativePosition ? _nativePosition->playback_rate() : 0; }
- (double)duration { return _nativePosition ? _nativePosition->duration().InSecondsF() : 0; }
- (double)currentTime { return _nativePosition ? _nativePosition->GetPosition().InSecondsF() : 0; }
- (BOOL)endOfMedia { return _nativePosition && _nativePosition->end_of_media(); }
@end

ARC_EXPORT @interface ArcWebContentsMediaSessionPiPController : NSObject
- (instancetype)initWithPiPController:(const void*)controller;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner __attribute__((objc_direct));
- (NSView*)videoView;
- (CGRect)sourceElementBounds;
@end
@implementation ArcWebContentsMediaSessionPiPController {
  __weak ArcWebContents* _owner;
}
- (instancetype)initWithPiPController:(const void*)controller {
  // Internal callers pass the ArcWebContents wrapper rather than Arc's private shared handle.
  return [self initWithArcWebContents:(__bridge ArcWebContents*)controller];
}
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) { _owner = owner; }
  return self;
}
- (NSView*)videoView { return objc_getAssociatedObject(_owner, @selector(videoView)); }
- (CGRect)sourceElementBounds {
  auto* contents = [_owner webContents];
  if (!contents || !objc_getAssociatedObject(_owner, @selector(videoView))) { return CGRectZero; }
  const auto& bounds = content::PictureInPictureWindowController::GetOrCreateVideoPictureInPictureController(contents)->GetSourceBounds();
  return CGRectMake(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}
@end

@class ArcWebContentsMediaSession;
namespace {
class ArcMediaObserver final : public media_session::mojom::MediaSessionObserver,
                               public content::WebContentsObserver {
 public:
  ArcMediaObserver(content::WebContents* contents, content::MediaSession* session, ArcWebContentsMediaSession* owner);
  void WebContentsDestroyed() override;
  void MediaSessionInfoChanged(media_session::mojom::MediaSessionInfoPtr info) override;
  void MediaSessionMetadataChanged(const std::optional<media_session::MediaMetadata>&) override {}
  void MediaSessionActionsChanged(const std::vector<media_session::mojom::MediaSessionAction>& actions) override;
  void MediaSessionImagesChanged(const base::flat_map<media_session::mojom::MediaSessionImageType, std::vector<media_session::MediaImage>>&) override {}
  void MediaSessionPositionChanged(const std::optional<media_session::MediaPosition>& position) override;
 private:
  __weak ArcWebContentsMediaSession* owner_;
  mojo::Receiver<media_session::mojom::MediaSessionObserver> receiver_{this};
};
BOOL ArcMediaFailure(NSError** error, NSString* message) {
  if (error) { *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:6 userInfo:@{NSLocalizedDescriptionKey:message}]; }
  return NO;
}
}

ARC_EXPORT @interface ArcWebContentsMediaSession : NSObject
@property(nonatomic, weak) ArcWebContents* owner;
@property(nonatomic, strong) ArcMediaPosition* position;
@property(nonatomic, strong) ArcWebContentsMediaSessionPiPController* pipController;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
- (void)setDelegate:(id<ArcMediaDelegate>)delegate;
- (void)nativeInfoChanged:(media_session::mojom::MediaSessionInfoPtr)info __attribute__((objc_direct));
- (void)nativeActionsChanged:(const std::vector<media_session::mojom::MediaSessionAction>&)actions __attribute__((objc_direct));
- (void)nativePositionChanged:(const std::optional<media_session::MediaPosition>&)position __attribute__((objc_direct));
- (id<ArcMediaDelegate>)nativeDelegate __attribute__((objc_direct));
- (void)nativePiPCreated __attribute__((objc_direct));
- (void)nativePiPDestroyed __attribute__((objc_direct));
@end

@implementation ArcWebContentsMediaSession {
  __weak id<ArcMediaDelegate> _delegate;
  std::unique_ptr<ArcMediaObserver> _observer;
  uint64_t _sessionState;
  uint64_t _cameraState;
  uint64_t _microphoneState;
  BOOL _pipDestroyed;
}
@synthesize owner = _owner, position = _position, pipController = _pipController;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) {
    _owner = owner;
    _sessionState = 3;
    arcmod::GetVideoOverlayWindowHook() = base::BindRepeating(&arc::CreateVideoOverlayWindow);
  }
  return self;
}
- (content::MediaSession*)nativeSession __attribute__((objc_direct)) {
  auto* contents = [_owner webContents];
  return contents ? content::MediaSession::GetIfExists(contents) : nullptr;
}
- (content::MediaSession*)requireSession:(NSError**)error __attribute__((objc_direct)) {
  if (![_owner webContents]) { ArcMediaFailure(error, @"managed webcontents is null"); return nullptr; }
  auto* session = [self nativeSession];
  if (!session) { ArcMediaFailure(error, @"no active media sessions"); }
  return session;
}
- (void)setDelegate:(id<ArcMediaDelegate>)delegate {
  _delegate = delegate;
  if (!delegate) { _observer.reset(); return; }
  if (!_observer) {
    auto* contents = [_owner webContents];
    if (contents) { _observer = std::make_unique<ArcMediaObserver>(contents, content::MediaSession::Get(contents), self); }
  }
}
- (NSIndexSet*)getMediaContentTypes:(NSError**)error {
  auto* session = [self requireSession:error];
  if (!session) { return nil; }
  auto info = session->GetMediaSessionInfoSync();
  NSMutableIndexSet* values = [NSMutableIndexSet indexSet];
  if (info && info->audio_video_states) {
    for (auto value : *info->audio_video_states) {
      auto index = static_cast<NSUInteger>(value);
      if (index < 4) { [values addIndex:index]; }
    }
  }
  return [values copy];
}
- (ArcMediaPosition*)position {
  auto* session = [self nativeSession];
  auto position = session ? session->GetMediaSessionPosition() : std::nullopt;
  _position = position ? [[ArcMediaPosition alloc] initWithMediaPosition:&*position] : nil;
  return _position;
}
- (ArcWebContentsMediaSessionPiPController*)pipController {
  auto* contents = [_owner webContents];
  if (!contents || !objc_getAssociatedObject(_owner, @selector(videoView))) { return nil; }
  _pipController = [[ArcWebContentsMediaSessionPiPController alloc] initWithPiPController:(__bridge const void*)_owner];
  return _pipController;
}
- (BOOL)play:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->Resume(content::MediaSession::SuspendType::kUI); return YES; }
- (BOOL)pause:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->Suspend(content::MediaSession::SuspendType::kUI); return YES; }
- (BOOL)playNextTrack:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->NextTrack(); return YES; }
- (BOOL)playPreviousTrack:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->PreviousTrack(); return YES; }
- (BOOL)skipAd:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->SkipAd(); return YES; }
- (BOOL)toggleMicrophone:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->ToggleMicrophone(); return YES; }
- (BOOL)toggleCamera:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->ToggleCamera(); return YES; }
- (BOOL)hangUp:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->HangUp(); return YES; }
- (BOOL)raise:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->Raise(); return YES; }
- (BOOL)seekTo:(int64_t)time error:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->SeekTo(base::Microseconds(time)); return YES; }
- (BOOL)scrubTo:(int64_t)time error:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->ScrubTo(base::Microseconds(time)); return YES; }
- (BOOL)setMute:(BOOL)mute error:(NSError**)error { auto* s = [self requireSession:error]; if (!s) return NO; s->SetMute(mute); return YES; }
- (BOOL)setVolume:(double)volume error:(NSError**)error {
  auto* s = [self requireSession:error]; if (!s) return NO;
  return ArcMediaFailure(error, @"media volume bridge is unavailable");
}
- (BOOL)requestPictureInPicture:(NSError**)error {
  auto* s = [self requireSession:error]; if (!s) return NO;
  if ([_owner webContents]->HasPictureInPictureVideo()) { return ArcMediaFailure(error, @"webcontents has active picture in picture video"); }
  auto actions = s->GetMediaSessionActionsSync();
  if (std::ranges::find(actions, media_session::mojom::MediaSessionAction::kEnterPictureInPicture) == actions.end()) { return ArcMediaFailure(error, @"media session does not support picture in picture"); }
  s->EnterPictureInPicture(); return YES;
}
- (BOOL)exitPictureInPicture:(BOOL)pause error:(NSError**)error {
  auto* s = [self requireSession:error]; if (!s) return NO;
  auto* contents = [_owner webContents];
  if (contents->HasPictureInPictureVideo()) {
    content::PictureInPictureWindowController::GetOrCreateVideoPictureInPictureController(contents)->Close(pause);
  }
  return YES;
}
- (BOOL)notifyAutoPipTabActivationChanged:(BOOL)active error:(NSError**)error {
  auto* s = [self requireSession:error]; if (!s) return NO;
  s->ReportAutoPictureInPictureInfoChanged(); return YES;
}
- (id<ArcMediaDelegate>)nativeDelegate { return _delegate; }
- (void)nativePiPCreated {
  _pipDestroyed = NO;
  _pipController = [[ArcWebContentsMediaSessionPiPController alloc] initWithPiPController:(__bridge const void*)_owner];
  id<ArcMediaDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(webContents:pipControllerCreated:)]) { [delegate webContents:_owner pipControllerCreated:_pipController]; }
}
- (void)nativePiPDestroyed {
  if (_pipDestroyed) { return; }
  _pipDestroyed = YES;
  _pipController = nil;
  id<ArcMediaDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(webContentsPiPControllerDestroyed:)]) { [delegate webContentsPiPControllerDestroyed:_owner]; }
}
- (void)nativeInfoChanged:(media_session::mojom::MediaSessionInfoPtr)info {
  if (!info) return;
  id<ArcMediaDelegate> delegate = _delegate;
  const uint64_t state = static_cast<uint64_t>(info->state);
  if (state != _sessionState) {
    _sessionState = state;
    if ([delegate respondsToSelector:@selector(webContents:didChangeMediaSessionState:)]) { [delegate webContents:_owner didChangeMediaSessionState:state]; }
  }
  const uint64_t camera = static_cast<uint64_t>(info->camera_state);
  if (camera != _cameraState) {
    _cameraState = camera;
    if ([delegate respondsToSelector:@selector(webContents:didChangeCameraState:)]) { [delegate webContents:_owner didChangeCameraState:camera]; }
  }
  const uint64_t microphone = static_cast<uint64_t>(info->microphone_state);
  if (microphone != _microphoneState) {
    _microphoneState = microphone;
    if ([delegate respondsToSelector:@selector(webContents:didChangeMicrophoneState:)]) { [delegate webContents:_owner didChangeMicrophoneState:microphone]; }
  }
}
- (void)nativeActionsChanged:(const std::vector<media_session::mojom::MediaSessionAction>&)actions {
  NSMutableIndexSet* values = [NSMutableIndexSet indexSet];
  for (auto action : actions) { [values addIndex:static_cast<NSUInteger>(action)]; }
  id<ArcMediaDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(webContents:didChangeAvailableMediaActions:)]) { [delegate webContents:_owner didChangeAvailableMediaActions:values]; }
}
- (void)nativePositionChanged:(const std::optional<media_session::MediaPosition>&)position {
  id<ArcMediaDelegate> delegate = _delegate;
  if (position && [delegate respondsToSelector:@selector(webContents:didChangeMediaPosition:)]) {
    [delegate webContents:_owner didChangeMediaPosition:[[ArcMediaPosition alloc] initWithMediaPosition:&*position]];
  }
}
@end

namespace {
ArcMediaObserver::ArcMediaObserver(content::WebContents* contents, content::MediaSession* session, ArcWebContentsMediaSession* owner)
    : content::WebContentsObserver(contents), owner_(owner) {
  session->AddObserver(receiver_.BindNewPipeAndPassRemote());
}
void ArcMediaObserver::WebContentsDestroyed() { [owner_ nativePiPDestroyed]; Observe(nullptr); }
void ArcMediaObserver::MediaSessionInfoChanged(media_session::mojom::MediaSessionInfoPtr info) { [owner_ nativeInfoChanged:std::move(info)]; }
void ArcMediaObserver::MediaSessionActionsChanged(const std::vector<media_session::mojom::MediaSessionAction>& actions) { [owner_ nativeActionsChanged:actions]; }
void ArcMediaObserver::MediaSessionPositionChanged(const std::optional<media_session::MediaPosition>& position) { [owner_ nativePositionChanged:position]; }
}

@interface ArcWebContents (ArcMediaAccess)
- (ArcWebContentsMediaSession*)mediaSession;
@end

@interface PiPNSView : NSView
@property(nonatomic, copy) void (^resizeHandler)(CGSize);
@end
@implementation PiPNSView
@synthesize resizeHandler = _resizeHandler;
- (BOOL)isFlipped { return YES; }
- (void)setFrameSize:(NSSize)size {
  [super setFrameSize:size];
  if (_resizeHandler) { _resizeHandler(size); }
}
@end

namespace {
// Arc owns the PiP window and reparents this view into its own native PiP UI.
// Chromium still owns the renderer surface and picture-in-picture session.
class ArcVideoOverlay final : public content::VideoOverlayWindow,
                              public ui::AcceleratedWidgetMacNSView {
 public:
  explicit ArcVideoOverlay(content::VideoPictureInPictureWindowController* controller)
      : controller_(controller), owner_([ArcWebContents wrapperForWebContents:controller->GetWebContents()]) {
    view_ = [[PiPNSView alloc] initWithFrame:NSMakeRect(0, 0, 320, 180)];
    view_.wantsLayer = YES;
    view_.layer = [CALayer layer];
    display_tree_ = std::make_unique<ui::DisplayCALayerTree>(view_.layer);
    compositor_ = std::make_unique<ui::RecyclableCompositorMac>(content::GetContextFactory());
    compositor_->widget()->SetNSView(this);
    surface_layer_ = std::make_unique<ui::LayerSurface>();
    surface_layer_->SetFallbackBackgroundColor(SkColors::kBlack);
    compositor_->compositor()->SetRootLayer(surface_layer_.get());
    auto weak = weak_factory_.GetWeakPtr();
    view_.resizeHandler = ^(CGSize size) { if (weak) { weak->Resize(size); } };
    Resize(view_.bounds.size);
    objc_setAssociatedObject(owner_, @selector(videoView), view_, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [[owner_ mediaSession] nativePiPCreated];
  }
  ~ArcVideoOverlay() override {
    weak_factory_.InvalidateWeakPtrs();
    view_.resizeHandler = nil;
    if (surface_id_.is_valid()) { compositor_->compositor()->RemoveChildFrameSink(surface_id_.frame_sink_id()); }
    compositor_->widget()->ResetNSView();
    compositor_->compositor()->SetRootLayer(nullptr);
    objc_setAssociatedObject(owner_, @selector(videoView), nil, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [[owner_ mediaSession] nativePiPDestroyed];
  }
  bool IsActive() const override { return view_.window.isKeyWindow; }
  void Close() override {
    visible_ = false;
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContentsClosePiP:)]) { [delegate webContentsClosePiP:owner_]; }
  }
  void ShowInactive() override {
    visible_ = true;
    compositor_->Unsuspend();
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContentsShowPiP:)]) { [delegate webContentsShowPiP:owner_]; }
  }
  void Hide() override {
    visible_ = false;
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContentsHidePiP:)]) { [delegate webContentsHidePiP:owner_]; }
  }
  bool IsVisible() const override { return visible_; }
  gfx::Rect GetBounds() override {
    NSRect frame = view_.bounds;
    return gfx::Rect(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
  }
  void UpdateNaturalSize(const gfx::Size& size) override {
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContents:pipVideoResolutionChanged:)]) { [delegate webContents:owner_ pipVideoResolutionChanged:CGSizeMake(size.width(), size.height())]; }
  }
  void SetPlaybackState(PlaybackState state) override {
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContents:pipPlaybackStateChanged:)]) { [delegate webContents:owner_ pipPlaybackStateChanged:state]; }
  }
  void SetPlayPauseButtonVisibility(bool value) override { Feature(4, value); }
  void SetSkipAdButtonVisibility(bool value) override { Feature(6, value); }
  void SetNextTrackButtonVisibility(bool value) override { Feature(3, value); }
  void SetPreviousTrackButtonVisibility(bool value) override { Feature(5, value); }
  void SetHidePictureInPictureButtonVisibility(bool) override {}
  void SetMicrophoneMuted(bool value) override { Feature(2, value); }
  void SetCameraState(bool value) override { Feature(0, value); }
  void SetMediaMuted(bool) override {}
  void SetToggleMicrophoneButtonVisibility(bool value) override { Feature(8, value); }
  void SetToggleCameraButtonVisibility(bool value) override { Feature(7, value); }
  void SetHangUpButtonVisibility(bool value) override { Feature(1, value); }
  void SetNextSlideButtonVisibility(bool value) override { Feature(9, value); }
  void SetPreviousSlideButtonVisibility(bool value) override { Feature(10, value); }
  void SetMediaPosition(const media_session::MediaPosition& position) override {
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContents:pipMediaPositionChanged:)]) {
      [delegate webContents:owner_ pipMediaPositionChanged:[[ArcMediaPosition alloc] initWithMediaPosition:&position]];
    }
  }
  void SetSourceTitle(const std::u16string& title) override {
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContents:pipSourceTitleChanged:)]) { [delegate webContents:owner_ pipSourceTitleChanged:ArcNSString(title)]; }
  }
  void SetFaviconImages(const std::vector<media_session::MediaImage>& images) override {
    // ArcMediaImage conversion is shared with the values package; preserve the event even for no artwork.
    if (images.empty()) {
      auto* delegate = Delegate();
      if ([delegate respondsToSelector:@selector(webContents:pipFaviconImagesChanged:)]) { [delegate webContents:owner_ pipFaviconImagesChanged:@[]]; }
    }
  }
  void SetSurfaceId(const viz::SurfaceId& surface_id) override {
    if (surface_id_.is_valid()) { compositor_->compositor()->RemoveChildFrameSink(surface_id_.frame_sink_id()); }
    surface_id_ = surface_id;
    compositor_->compositor()->AddChildFrameSink(surface_id.frame_sink_id());
    surface_layer_->AsSurface()->SetShowSurface(surface_id, GetBounds().size(), cc::DeadlinePolicy::UseDefaultDeadline(), false);
    compositor_->Unsuspend();
  }
  void SetPlaybackControlsVisibility(bool) override {}
  void SetImmersiveVideoOptions(const content::ImmersiveOptions&) override {}
  void AcceleratedWidgetCALayerParamsUpdated(gfx::CALayerParams params) override { display_tree_->UpdateCALayerTree(std::move(params)); }
 private:
  id<ArcMediaDelegate> Delegate() { return [[owner_ mediaSession] nativeDelegate]; }
  void Feature(uint64_t feature, bool value) {
    auto* delegate = Delegate();
    if ([delegate respondsToSelector:@selector(webContents:pipFeatureStateChanged:turnedOn:)]) { [delegate webContents:owner_ pipFeatureStateChanged:feature turnedOn:value]; }
  }
  void Resize(CGSize size) {
    gfx::Size dimensions(std::max(1, static_cast<int>(size.width)), std::max(1, static_cast<int>(size.height)));
    surface_layer_->SetBounds(gfx::Rect(dimensions));
    compositor_->UpdateSurface(dimensions, 1, gfx::DisplayColorSpaces(), 0, 60);
    if (surface_id_.is_valid()) { surface_layer_->AsSurface()->SetShowSurface(surface_id_, dimensions, cc::DeadlinePolicy::UseDefaultDeadline(), false); }
  }
  raw_ptr<content::VideoPictureInPictureWindowController> controller_;
  __weak ArcWebContents* owner_;
  PiPNSView* __strong view_;
  std::unique_ptr<ui::DisplayCALayerTree> display_tree_;
  std::unique_ptr<ui::RecyclableCompositorMac> compositor_;
  std::unique_ptr<ui::LayerSurface> surface_layer_;
  viz::SurfaceId surface_id_;
  bool visible_ = false;
  base::WeakPtrFactory<ArcVideoOverlay> weak_factory_{this};
};
}

namespace arc {
std::unique_ptr<content::VideoOverlayWindow> CreateVideoOverlayWindow(content::VideoPictureInPictureWindowController* controller) {
  return std::make_unique<ArcVideoOverlay>(controller);
}
}
