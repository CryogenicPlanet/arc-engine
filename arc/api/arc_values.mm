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

// An RGBA color with float components (Arc's Color4f; same layout as SkColor4f), e.g. a page's root
// background color. Encodings match Arc's: {Color4f=ffff}.
struct Color4f {
  float r;
  float g;
  float b;
  float a;
};

ARC_EXPORT @interface ArcColor4f : NSObject
- (instancetype)initWithR:(float)r g:(float)g b:(float)b a:(float)a;
@end

@implementation ArcColor4f {
  Color4f _color4f;
}

- (instancetype)initWithR:(float)r g:(float)g b:(float)b a:(float)a {
  if ((self = [super init])) {
    _color4f = {r, g, b, a};
  }
  return self;
}

- (instancetype)initWithColor4f:(const Color4f*)color {
  if ((self = [super init])) {
    if (color) {
      _color4f = *color;
    }
  }
  return self;
}

- (float)r {
  return _color4f.r;
}
- (float)g {
  return _color4f.g;
}
- (float)b {
  return _color4f.b;
}
- (float)a {
  return _color4f.a;
}
- (Color4f)color4f {
  return _color4f;
}
- (void)setColor4f:(Color4f)color4f {
  _color4f = color4f;
}

@end
