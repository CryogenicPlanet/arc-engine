// Copyright 2026 ArcMod. Value classes in Arc's interface that wrap a Chromium data type.

#import <Foundation/Foundation.h>

#include "base/strings/sys_string_conversions.h"
#include "components/search_engines/template_url_data.h"
#include "url/gurl.h"

#define ARC_EXPORT __attribute__((visibility("default")))

// A search engine (Chromium's TemplateURLData): display name, keyword, URL template with {searchTerms}.
ARC_EXPORT @interface ArcSearchEngine : NSObject
- (instancetype)initWithTemplateURLData:(const TemplateURLData&)data;
- (const TemplateURLData&)templateURLData;
@end

@implementation ArcSearchEngine {
  TemplateURLData _data;
}

- (instancetype)init:(NSString*)name keyword:(NSString*)keyword url:(NSString*)url faviconUrl:(NSString*)faviconUrl {
  if ((self = [super init])) {
    _data.SetShortName(base::SysNSStringToUTF16(name ?: @""));
    _data.SetKeyword(base::SysNSStringToUTF16(keyword ?: @""));
    _data.SetURL(base::SysNSStringToUTF8(url ?: @""));
    _data.favicon_url = GURL(base::SysNSStringToUTF8(faviconUrl ?: @""));
  }
  return self;
}

- (instancetype)initWithTemplateURLData:(const TemplateURLData&)data {
  if ((self = [super init])) {
    _data = data;
  }
  return self;
}

- (const TemplateURLData&)templateURLData {
  return _data;
}

- (NSString*)name {
  return base::SysUTF16ToNSString(_data.short_name());
}
- (NSString*)keyword {
  return base::SysUTF16ToNSString(_data.keyword());
}
// Canonical form, as Arc's engine returns it ({searchTerms} percent-encoded).
- (NSString*)url {
  GURL url(_data.url());
  return base::SysUTF8ToNSString(url.is_valid() ? url.spec() : _data.url());
}
- (NSString*)faviconUrl {
  return base::SysUTF8ToNSString(_data.favicon_url.spec());
}
// TemplateURLData::ActiveStatus: 0 unspecified, 1 active, 2 inactive.
- (NSInteger)status {
  return static_cast<NSInteger>(_data.is_active);
}
- (BOOL)isPrepopulated {
  return _data.prepopulate_id > 0;
}

- (BOOL)isEqual:(id)other {
  if (![other isKindOfClass:[ArcSearchEngine class]]) {
    return NO;
  }
  const TemplateURLData& o = [other templateURLData];
  return _data.keyword() == o.keyword() && _data.url() == o.url() && _data.short_name() == o.short_name();
}
- (NSUInteger)hash {
  return self.keyword.hash ^ self.url.hash;
}

@end

// An RGBA color with float components (Arc's Color4f), e.g. a page's root background color. Only the
// ObjC-typed surface is implemented: Arc's C++-typed initWithColor4f:/color4f/setColor4f: are engine-internal
// (ARCHITECTURE.md), and reproducing them would need Arc's exact non-trivial Color4f return ABI.
ARC_EXPORT @interface ArcColor4f : NSObject
- (instancetype)initWithR:(float)r g:(float)g b:(float)b a:(float)a;
@property(nonatomic, readonly) float r;
@property(nonatomic, readonly) float g;
@property(nonatomic, readonly) float b;
@property(nonatomic, readonly) float a;
@end

@implementation ArcColor4f {
  float _r, _g, _b, _a;
}

- (instancetype)initWithR:(float)r g:(float)g b:(float)b a:(float)a {
  if ((self = [super init])) {
    _r = r;
    _g = g;
    _b = b;
    _a = a;
  }
  return self;
}

- (float)r {
  return _r;
}
- (float)g {
  return _g;
}
- (float)b {
  return _b;
}
- (float)a {
  return _a;
}

@end
