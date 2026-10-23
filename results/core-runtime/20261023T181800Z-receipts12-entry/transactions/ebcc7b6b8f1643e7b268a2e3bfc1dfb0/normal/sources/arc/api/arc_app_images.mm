// Copyright 2026 ArcMod. Image values backed by Chromium image representations.
#include "arc/api/arc_internal.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <vector>

#include "base/apple/foundation_util.h"
#include "base/containers/span.h"
#include "base/functional/bind.h"
#include "base/numerics/safe_conversions.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "services/data_decoder/public/cpp/decode_image.h"
#include "services/media_session/public/cpp/media_image.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/codec/jpeg_codec.h"
#include "ui/gfx/codec/png_codec.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_rep.h"

namespace {
base::span<const uint8_t> ImageBytes(NSData* data) {
  return base::apple::NSDataToSpan(data);
}

SkBitmap DecodeImageSynchronously(NSData* data) {
  // These codecs are deliberately synchronous, as this selector advertises.
  SkBitmap bitmap = gfx::PNGCodec::Decode(ImageBytes(data));
  if (bitmap.isNull()) bitmap = gfx::JPEGCodec::Decode(ImageBytes(data));
  return bitmap;
}

void DecodeImage(NSData* data, NSInteger width, NSInteger height,
                 NSUInteger maximum, data_decoder::DecodeImageCallback callback) {
  // NSData may be mutable and the caller may release it after returning.
  NSData* bytes = [data copy];
  auto decode = base::BindOnce(^(data_decoder::DecodeImageCallback reply) {
    data_decoder::DecodeImageIsolated(
        ImageBytes(bytes), data_decoder::mojom::ImageCodec::kDefault, false,
        maximum ? maximum : data_decoder::kDefaultMaxSizeInBytes,
        gfx::Size(base::saturated_cast<int>(std::max<NSInteger>(0, width)),
                  base::saturated_cast<int>(std::max<NSInteger>(0, height))),
        std::move(reply));
  }, std::move(callback));
  if (content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    std::move(decode).Run();
  } else {
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, std::move(decode));
  }
}
}  // namespace

ARC_EXPORT @interface ArcImageRep : NSObject {
  gfx::ImageSkiaRep _representation;
}
- (instancetype)initWithArcImageRep:(const void*)representation;
@property(nonatomic, readonly) BOOL isNull;
@property(nonatomic, readonly) NSInteger pixelWidth;
@property(nonatomic, readonly) NSInteger pixelHeight;
- (NSData*)copyPixelDataBGRA8888;
- (NSImage*)toNSImage;
- (NSData*)encodeToPNGWithMethod:(NSUInteger)method;
- (NSData*)encodeToJPEGWithQuality:(float)quality;
+ (instancetype)createFromPixelDataBGRA8888:(NSInteger)width
                                   height:(NSInteger)height
                                    scale:(double)scale
                                 rowBytes:(NSUInteger)rowBytes
                                pixelData:(NSData*)data;
+ (instancetype)dangerouslyDecodeFromData:(NSData*)data scale:(float)scale;
+ (void)decodeFromData:(NSData*)data scale:(float)scale
           completion:(void (^)(ArcImageRep*))completion;
+ (void)decodeFromData:(NSData*)data scale:(float)scale
         desiredWidth:(NSInteger)width desiredHeight:(NSInteger)height
      maxDecodedBytes:(NSUInteger)maximum completion:(void (^)(ArcImageRep*))completion;
@end

@implementation ArcImageRep
- (instancetype)init { return [super init]; }
- (instancetype)initWithArcImageRep:(const void*)representation {
  if ((self = [super init]) && representation)
    _representation = *static_cast<const gfx::ImageSkiaRep*>(representation);
  return self;
}
- (BOOL)isNull { return _representation.is_null(); }
- (NSInteger)pixelWidth { return _representation.pixel_width(); }
- (NSInteger)pixelHeight { return _representation.pixel_height(); }
- (NSData*)copyPixelDataBGRA8888 {
  if (_representation.is_null()) return nil;
  const SkBitmap& bitmap = _representation.GetBitmap();
  const SkImageInfo info = SkImageInfo::Make(bitmap.width(), bitmap.height(),
      kBGRA_8888_SkColorType, kPremul_SkAlphaType);
  const size_t rowBytes = info.minRowBytes();
  const size_t size = info.computeByteSize(rowBytes);
  if (SkImageInfo::ByteSizeOverflowed(size)) return nil;
  NSMutableData* data = [NSMutableData dataWithLength:size];
  return bitmap.readPixels(info, data.mutableBytes, rowBytes, 0, 0) ? data : nil;
}
- (NSImage*)toNSImage {
  if (_representation.is_null()) return nil;
  return gfx::Image(gfx::ImageSkia(_representation)).AsNSImage();
}
- (NSData*)encodeToPNGWithMethod:(NSUInteger)method {
  if (_representation.is_null()) return nil;
  const auto bytes = method == 1
      ? gfx::PNGCodec::FastEncodeBGRASkBitmap(_representation.GetBitmap(), false)
      : gfx::PNGCodec::EncodeBGRASkBitmap(_representation.GetBitmap(), false);
  return bytes ? [NSData dataWithBytes:bytes->data() length:bytes->size()] : nil;
}
- (NSData*)encodeToJPEGWithQuality:(float)quality {
  if (_representation.is_null() || !std::isfinite(quality)) return nil;
  const auto bytes = gfx::JPEGCodec::Encode(_representation.GetBitmap(),
      static_cast<int>(std::clamp(quality, 0.0f, 1.0f) * 100));
  return bytes ? [NSData dataWithBytes:bytes->data() length:bytes->size()] : nil;
}
+ (instancetype)createFromPixelDataBGRA8888:(NSInteger)width
                                   height:(NSInteger)height
                                    scale:(double)scale
                                 rowBytes:(NSUInteger)rowBytes
                                pixelData:(NSData*)data {
  gfx::ImageSkiaRep representation;
  const float representationScale = static_cast<float>(scale);
  if (width > 0 && height > 0 && width <= std::numeric_limits<int>::max() &&
      height <= std::numeric_limits<int>::max() && std::isfinite(scale) &&
      representationScale > 0 && std::isfinite(representationScale)) {
    const SkImageInfo info = SkImageInfo::Make(static_cast<int>(width),
        static_cast<int>(height), kBGRA_8888_SkColorType, kPremul_SkAlphaType);
    const size_t required = info.computeByteSize(rowBytes);
    if (info.validRowBytes(rowBytes) && !SkImageInfo::ByteSizeOverflowed(required) &&
        data.length >= required) {
      SkBitmap bitmap;
      if (bitmap.tryAllocPixels(info) && bitmap.writePixels(SkPixmap(info, data.bytes, rowBytes)))
        representation = gfx::ImageSkiaRep(bitmap, representationScale);
    }
  }
  return [[self alloc] initWithArcImageRep:&representation];
}
+ (instancetype)dangerouslyDecodeFromData:(NSData*)data scale:(float)scale {
  if (!data.length || !(scale > 0) || !std::isfinite(scale)) return nil;
  const SkBitmap bitmap = DecodeImageSynchronously(data);
  if (bitmap.isNull()) return nil;
  const gfx::ImageSkiaRep representation(bitmap, scale);
  return [[self alloc] initWithArcImageRep:&representation];
}
+ (void)decodeFromData:(NSData*)data scale:(float)scale
           completion:(void (^)(ArcImageRep*))completion {
  [self decodeFromData:data scale:scale desiredWidth:0 desiredHeight:0
      maxDecodedBytes:0 completion:completion];
}
+ (void)decodeFromData:(NSData*)data scale:(float)scale
         desiredWidth:(NSInteger)width desiredHeight:(NSInteger)height
      maxDecodedBytes:(NSUInteger)maximum completion:(void (^)(ArcImageRep*))completion {
  if (!completion) return;
  if (!data.length || !(scale > 0) || !std::isfinite(scale)) { completion(nil); return; }
  DecodeImage(data, width, height, maximum, base::BindOnce(^(const SkBitmap& bitmap) {
    if (bitmap.isNull()) { completion(nil); return; }
    const gfx::ImageSkiaRep representation(bitmap, scale);
    completion([[self alloc] initWithArcImageRep:&representation]);
  }));
}
@end

ARC_EXPORT @interface ArcImage : NSObject {
  gfx::Image _nativeImage;
}
- (instancetype)initWithArcImage:(const void*)image;
@property(nonatomic, readonly) NSImage* nsImage;
@property(nonatomic, readonly) BOOL isNull;
@property(nonatomic, readonly) NSInteger logicalWidth;
@property(nonatomic, readonly) NSInteger logicalHeight;
+ (instancetype)dangerouslyDecodeFromData:(NSData*)data scale:(float)scale;
+ (void)decodeFromData:(NSData*)data scale:(float)scale completion:(void (^)(ArcImage*))completion;
+ (void)decodeFromData:(NSData*)data scale:(float)scale
         desiredWidth:(NSInteger)width desiredHeight:(NSInteger)height
      maxDecodedBytes:(NSUInteger)maximum completion:(void (^)(ArcImage*))completion;
@end

@implementation ArcImage
- (instancetype)init { return [super init]; }
- (instancetype)initWithArcImage:(const void*)image {
  if ((self = [super init]) && image) _nativeImage = *static_cast<const gfx::Image*>(image);
  return self;
}
- (NSImage*)nsImage { return _nativeImage.AsNSImage(); }
- (BOOL)isNull { return _nativeImage.IsEmpty(); }
- (NSInteger)logicalWidth { return _nativeImage.IsEmpty() ? 0 : _nativeImage.Width(); }
- (NSInteger)logicalHeight { return _nativeImage.IsEmpty() ? 0 : _nativeImage.Height(); }
+ (instancetype)dangerouslyDecodeFromData:(NSData*)data scale:(float)scale {
  if (!data.length || !(scale > 0) || !std::isfinite(scale)) return nil;
  const SkBitmap bitmap = DecodeImageSynchronously(data);
  if (bitmap.isNull()) return nil;
  const gfx::Image image{gfx::ImageSkia(gfx::ImageSkiaRep(bitmap, scale))};
  return [[self alloc] initWithArcImage:&image];
}
+ (void)decodeFromData:(NSData*)data scale:(float)scale completion:(void (^)(ArcImage*))completion {
  [self decodeFromData:data scale:scale desiredWidth:0 desiredHeight:0
      maxDecodedBytes:0 completion:completion];
}
+ (void)decodeFromData:(NSData*)data scale:(float)scale
         desiredWidth:(NSInteger)width desiredHeight:(NSInteger)height
      maxDecodedBytes:(NSUInteger)maximum completion:(void (^)(ArcImage*))completion {
  if (!completion) return;
  if (!data.length || !(scale > 0) || !std::isfinite(scale)) { completion(nil); return; }
  DecodeImage(data, width, height, maximum, base::BindOnce(^(const SkBitmap& bitmap) {
    if (bitmap.isNull()) { completion(nil); return; }
    const gfx::Image image{gfx::ImageSkia(gfx::ImageSkiaRep(bitmap, scale))};
    completion([[self alloc] initWithArcImage:&image]);
  }));
}
@end

ARC_EXPORT @interface ArcMediaImage : NSObject {
  std::optional<media_session::MediaImage> _mediaImage;
  std::vector<CGSize> _imageSizes;
}
// Chromium-only initializer; the engine's C++-typed initializer is not app ABI.
- (instancetype)initWithChromiumMediaImage:(const media_session::MediaImage&)image;
@property(nonatomic, readonly) NSString* src;
@property(nonatomic, readonly) NSString* type;
@property(nonatomic, readonly) CGSize* sizes;
@end
@implementation ArcMediaImage
- (instancetype)initWithChromiumMediaImage:(const media_session::MediaImage&)image {
  if ((self = [super init])) {
    _mediaImage = image;
    _imageSizes.reserve(image.sizes.size());
    for (const auto& size : image.sizes)
      _imageSizes.push_back(CGSizeMake(size.width(), size.height()));
  }
  return self;
}
- (NSString*)src { return _mediaImage ? ArcNSString(_mediaImage->src.spec()) : @""; }
- (NSString*)type { return _mediaImage ? ArcNSString(_mediaImage->type) : @""; }
- (CGSize*)sizes { return _imageSizes.empty() ? nullptr : _imageSizes.data(); }
@end
