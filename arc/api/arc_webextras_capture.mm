// Copyright 2026 ArcMod. Thumbnail and PDF controllers backed by Chrome helpers.
#import "arc/api/arc_internal.h"

#include "base/functional/bind.h"
#include "chrome/browser/ui/thumbnails/thumbnail_tab_helper.h"
#include "chrome/browser/ui/thumbnails/thumbnail_image.h"
#include "components/tabs/public/tab_interface.h"
#include "components/pdf/browser/pdf_document_helper.h"
#include "content/public/browser/web_contents.h"
#include "ui/gfx/image/image_skia_util_mac.h"

namespace {
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
  if (!observer) { return; }
  [_weakObservers addObject:observer];
  [self subscribeIfAvailable];
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
  if (![_owner webContents]) { return CaptureFailure(error, @"managed webcontents is null"); }
  [self subscribeIfAvailable];
  if (!_weakObservers.count) { return YES; }
  if (![self nativeHelper]) { return CaptureFailure(error, @"thumbnail helper is unavailable"); }
  if (!_subscription) { return YES; }
  _thumbnail->RequestThumbnailImage();
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
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
- (instancetype)initWithWebContentsPDFController:(const void*)controller;
- (BOOL)hasBookmarks:(void (^)(BOOL))callback error:(NSError**)error;
- (BOOL)getBookmarks:(void (^)(id))callback error:(NSError**)error;
- (BOOL)availablePages:(void (^)(id))callback error:(NSError**)error;
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
- (BOOL)availablePages:(void (^)(id))callback error:(NSError**)error {
  auto* helper = [self nativeHelper:error]; if (!helper) { return NO; }
  helper->GetPdfBytes(UINT32_MAX, base::BindOnce(^(pdf::mojom::PdfListener::GetPdfBytesStatus status, const std::vector<uint8_t>& bytes, uint32_t count) {
    NSMutableIndexSet* pages = [NSMutableIndexSet indexSet];
    if (status == pdf::mojom::PdfListener::GetPdfBytesStatus::kSuccess) { [pages addIndexesInRange:NSMakeRange(0, count)]; }
    if (callback) { callback(pages); }
  }));
  return YES;
}
- (BOOL)visiblePages:(void (^)(id))callback error:(NSError**)error {
  auto* helper = [self nativeHelper:error]; if (!helper) { return NO; }
  helper->GetMostVisiblePageIndex(base::BindOnce(^(std::optional<uint32_t> page) {
    if (callback) { callback(page ? [NSIndexSet indexSetWithIndex:*page] : [NSIndexSet indexSet]); }
  }));
  return YES;
}
- (BOOL)getAvailableTextOn:(int)page callback:(void (^)(NSString*))callback error:(NSError**)error {
  auto* helper = [self nativeHelper:error]; if (!helper) { return NO; }
  if (page < 0) { return CaptureFailure(error, @"page index is negative"); }
  helper->GetPageText(page, base::BindOnce(^(const std::u16string& text) { if (callback) { callback(ArcNSString(text)); } }));
  return YES;
}
@end
