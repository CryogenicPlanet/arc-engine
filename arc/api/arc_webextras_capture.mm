// Copyright 2026 ArcMod. Thumbnail and PDF controllers backed by Chrome helpers.
#import "arc/api/arc_internal.h"

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/callback_list.h"
#include "base/task/single_thread_task_runner.h"
#include "chrome/browser/ui/thumbnails/thumbnail_tab_helper.h"
#include "chrome/browser/ui/thumbnails/thumbnail_image.h"
#include "components/tabs/public/tab_interface.h"
#include "components/pdf/browser/pdf_document_helper.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/visibility.h"
#include "ui/gfx/image/image_skia_util_mac.h"

namespace {
class ArcPDFObserver final : public content::WebContentsObserver,
                             public content::WebContentsUserData<ArcPDFObserver> {
 public:
  void DidFinishNavigation(content::NavigationHandle* navigation) override;
 private:
  friend class content::WebContentsUserData<ArcPDFObserver>;
  ArcPDFObserver(content::WebContents* contents, ArcWebContents* owner);
  void WatchPDF();
  void PDFReady(uint64_t generation);
  __weak ArcWebContents* owner_;
  bool has_controller_ = false;
  uint64_t generation_ = 0;
  base::CallbackListSubscription creation_subscription_;
  base::WeakPtrFactory<ArcPDFObserver> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
BOOL CaptureFailure(NSError** error, NSString* description) {
  if (error) { *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:6 userInfo:@{NSLocalizedDescriptionKey:description}]; }
  return NO;
}
}
@protocol ArcThumbnailObserver <NSObject>
- (void)webContents:(ArcWebContents*)contents thumbnailReady:(NSImage*)image;
@end
ARC_EXPORT @interface ArcWebContentsThumbnailsGenerator : NSObject
@property(nonatomic, weak) ArcWebContents* owner;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
- (void)addObserver:(id<ArcThumbnailObserver>)observer;
- (void)removeObserver:(id<ArcThumbnailObserver>)observer;
- (BOOL)requestThumbnail:(NSError**)error;
- (void)thumbnailChanged:(const gfx::ImageSkia&)image __attribute__((objc_direct));
@end
@implementation ArcWebContentsThumbnailsGenerator {
  NSHashTable<id<ArcThumbnailObserver>>* _weakObservers;
  scoped_refptr<ThumbnailImage> _thumbnail;
  std::unique_ptr<ThumbnailImage::Subscription> _subscription;
}
@synthesize owner = _owner;
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) {
    _owner = owner;
    if (auto* contents = [owner webContents]) { ArcPDFObserver::CreateForWebContents(contents, owner); }
    _weakObservers = [NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality];
  }
  return self;
}
- (ThumbnailTabHelper*)nativeHelper __attribute__((objc_direct)) {
  auto* contents = [_owner webContents];
  auto* tab = contents ? tabs::TabInterface::MaybeGetFromContents(contents) : nullptr;
  return tab ? ThumbnailTabHelper::From(tab) : nullptr;
}
- (void)subscribeIfAvailable __attribute__((objc_direct)) {
  if (_subscription || !_weakObservers.count) { return; }
  auto* helper = [self nativeHelper];
  if (!helper) { return; }
  _thumbnail = helper->thumbnail();
  _subscription = _thumbnail->Subscribe();
  __weak ArcWebContentsThumbnailsGenerator* weakSelf = self;
  _subscription->SetUncompressedImageCallback(base::BindRepeating(^(gfx::ImageSkia image) {
    [weakSelf thumbnailChanged:image];
  }));
}
- (void)addObserver:(id<ArcThumbnailObserver>)observer {
  if (!observer || [_weakObservers containsObject:observer]) { return; }
  [_weakObservers addObject:observer];
  [self subscribeIfAvailable];
  // Native observer registration also requests an already available image.
  if (_thumbnail) { _thumbnail->RequestThumbnailImage(); }
}
- (void)removeObserver:(id<ArcThumbnailObserver>)observer {
  [_weakObservers removeObject:observer];
  if (!_weakObservers.count) {
    // ThumbnailImage forbids destroying the subscription inside its callback.
    dispatch_async(dispatch_get_main_queue(), ^{
      if (!self->_weakObservers.count) { self->_subscription.reset(); self->_thumbnail.reset(); }
    });
  }
}
- (BOOL)requestThumbnail:(NSError**)error {
  auto* contents = [_owner webContents];
  if (!contents) { return CaptureFailure(error, @"managed webcontents is null"); }
  if (!_weakObservers.count || !_subscription) { return YES; }
  auto* helper = [self nativeHelper];
  if (!helper) { return CaptureFailure(error, @"thumbnail helper is unavailable"); }
  if (contents->GetVisibility() == content::Visibility::VISIBLE) {
    helper->CaptureThumbnailOnTabBackgrounded();
  } else { _thumbnail->RequestThumbnailImage(); }
  return YES;
}
- (void)thumbnailChanged:(const gfx::ImageSkia&)image {
  NSImage* converted = image.isNull() ? nil : gfx::NSImageFromImageSkia(image);
  for (id<ArcThumbnailObserver> observer in _weakObservers.allObjects) {
    if ([observer respondsToSelector:@selector(webContents:thumbnailReady:)]) {
      [observer webContents:_owner thumbnailReady:converted];
    }
  }
}
@end

ARC_EXPORT @interface ArcWebContentsPDFController : NSObject
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner __attribute__((objc_direct));
- (instancetype)initWithWebContentsPDFController:(const void*)controller;
- (BOOL)hasBookmarks:(void (^)(BOOL))callback error:(NSError**)error;
- (BOOL)getBookmarks:(void (^)(id))callback error:(NSError**)error;
- (BOOL)availablePages:(void (^)(int))callback error:(NSError**)error;
- (BOOL)visiblePages:(void (^)(id))callback error:(NSError**)error;
- (BOOL)getAvailableTextOn:(int)page callback:(void (^)(NSString*))callback error:(NSError**)error;
@end
@implementation ArcWebContentsPDFController {
  __weak ArcWebContents* _owner;
}
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) { _owner = owner; }
  return self;
}
- (instancetype)initWithWebContentsPDFController:(const void*)controller {
  return [self initWithArcWebContents:(__bridge ArcWebContents*)controller];
}
- (pdf::PDFDocumentHelper*)nativeHelper:(NSError**)error __attribute__((objc_direct)) {
  auto* contents = [_owner webContents];
  if (!contents) { CaptureFailure(error, @"The object is in detached state"); return nullptr; }
  auto* helper = pdf::PDFDocumentHelper::MaybeGetForFullPagePdf(*contents);
  if (!helper && error) { *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:10 userInfo:@{NSLocalizedDescriptionKey:@"Internal error: pdf_controller is null"}]; }
  return helper;
}
- (BOOL)hasBookmarks:(void (^)(BOOL))callback error:(NSError**)error {
  if (![self nativeHelper:error]) { return NO; }
  return CaptureFailure(error, @"PDF bookmark bridge is unavailable");
}
- (BOOL)getBookmarks:(void (^)(id))callback error:(NSError**)error {
  if (![self nativeHelper:error]) { return NO; }
  return CaptureFailure(error, @"PDF bookmark bridge is unavailable");
}
- (BOOL)availablePages:(void (^)(int))callback error:(NSError**)error {
  auto* helper = [self nativeHelper:error]; if (!helper) { return NO; }
  // A zero byte limit retrieves the real page count without copying the document.
  helper->GetPdfBytes(0, base::BindOnce(^(pdf::mojom::PdfListener::GetPdfBytesStatus status, const std::vector<uint8_t>& bytes, uint32_t count) {
    if (callback) { callback(status == pdf::mojom::PdfListener::GetPdfBytesStatus::kFailed ? 0 : static_cast<int>(count)); }
  }));
  return YES;
}
- (BOOL)visiblePages:(void (^)(id))callback error:(NSError**)error {
  if (![self nativeHelper:error]) { return NO; }
  return CaptureFailure(error, @"PDF visible page bridge is unavailable");
}
- (BOOL)getAvailableTextOn:(int)page callback:(void (^)(NSString*))callback error:(NSError**)error {
  auto* helper = [self nativeHelper:error]; if (!helper) { return NO; }
  if (page < 0) { return CaptureFailure(error, @"page index is negative"); }
  helper->GetPageText(page, base::BindOnce(^(const std::u16string& text) { if (callback) { callback(ArcNSString(text)); } }));
  return YES;
}
@end

@interface ArcWebContents (PDFInternal)
- (void)setPdfController:(id)controller;
- (id)uiDelegate;
@end
@protocol ArcPDFUI <NSObject>
- (void)webContents:(ArcWebContents*)contents didFinishPDFRendering:(ArcWebContentsPDFController*)controller;
@end
namespace {
WEB_CONTENTS_USER_DATA_KEY_IMPL(ArcPDFObserver);
ArcPDFObserver::ArcPDFObserver(content::WebContents* contents, ArcWebContents* owner)
    : content::WebContentsObserver(contents), content::WebContentsUserData<ArcPDFObserver>(*contents), owner_(owner) {
  // Install user data before a preloaded PDF can synchronously call back into the app.
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE,
      base::BindOnce(&ArcPDFObserver::WatchPDF, weak_factory_.GetWeakPtr()));
}
void ArcPDFObserver::DidFinishNavigation(content::NavigationHandle* navigation) {
  if (!navigation->IsInPrimaryMainFrame() || !navigation->HasCommitted() || navigation->IsSameDocument()) { return; }
  ++generation_;
  creation_subscription_ = {};
  if (has_controller_) {
    has_controller_ = false;
    auto weak = weak_factory_.GetWeakPtr();
    [owner_ setPdfController:nil];
    if (!weak) { return; }
  }
  WatchPDF();
}
void ArcPDFObserver::WatchPDF() {
  if (!web_contents()) { return; }
  auto* helper = pdf::PDFDocumentHelper::MaybeGetForWebContents(*web_contents());
  if (!helper) {
    creation_subscription_ = pdf::PDFDocumentHelper::RegisterForCreate(*web_contents(),
        base::BindOnce(&ArcPDFObserver::WatchPDF, weak_factory_.GetWeakPtr()));
    return;
  }
  if (helper != pdf::PDFDocumentHelper::MaybeGetForFullPagePdf(*web_contents())) { return; }
  helper->RegisterForDocumentLoadComplete(base::BindOnce(&ArcPDFObserver::PDFReady,
      weak_factory_.GetWeakPtr(), generation_));
}
void ArcPDFObserver::PDFReady(uint64_t generation) {
  if (generation != generation_ || has_controller_ || !web_contents()) { return; }
  auto* helper = pdf::PDFDocumentHelper::MaybeGetForFullPagePdf(*web_contents());
  if (!helper || !helper->IsDocumentLoadComplete()) { return; }
  ArcWebContents* owner = owner_;
  if (!owner) { return; }
  has_controller_ = true;
  ArcWebContentsPDFController* controller = [[ArcWebContentsPDFController alloc]
      initWithWebContentsPDFController:(__bridge const void*)owner];
  auto weak = weak_factory_.GetWeakPtr();
  [owner setPdfController:controller];
  if (!weak || generation != generation_) { return; }
  id<ArcPDFUI> delegate = [owner uiDelegate];
  if ([delegate respondsToSelector:@selector(webContents:didFinishPDFRendering:)]) {
    [delegate webContents:owner didFinishPDFRendering:controller];
  }
}
}
