// Copyright 2026 ArcMod. Values recovered from ArcCore's Objective-C metadata
// and the reviewed local initializer/getter bodies in specs/.
#import "arc/common/renderer_values.h"

#include "base/check.h"
#include "base/compiler_specific.h"
#include "base/containers/span.h"

@implementation ArcHSLAMatrix
@synthesize hslaMatrix = _hslaMatrix;
- (instancetype)initWithMatrix:(const float[20])matrix {
  CHECK(matrix);
  // Arc's [20f] ABI supplies exactly twenty floats. Bound the foreign pointer
  // once, then use checked spans for every access.
  auto values = UNSAFE_BUFFERS(base::span<const float, 20>(matrix, size_t{20}));
  if ((self = [super init])) {
    _hslaMatrix = {values[0], values[1], values[2], values[3], values[4],
                   values[5], values[6], values[7], values[8], values[9],
                   values[10], values[11], values[12], values[13], values[14],
                   values[15], values[16], values[17], values[18], values[19]};
  }
  return self;
}
- (float)m0 { return _hslaMatrix.m0; }
- (float)m1 { return _hslaMatrix.m1; }
- (float)m2 { return _hslaMatrix.m2; }
- (float)m3 { return _hslaMatrix.m3; }
- (float)m4 { return _hslaMatrix.m4; }
- (float)m5 { return _hslaMatrix.m5; }
- (float)m6 { return _hslaMatrix.m6; }
- (float)m7 { return _hslaMatrix.m7; }
- (float)m8 { return _hslaMatrix.m8; }
- (float)m9 { return _hslaMatrix.m9; }
- (float)m10 { return _hslaMatrix.m10; }
- (float)m11 { return _hslaMatrix.m11; }
- (float)m12 { return _hslaMatrix.m12; }
- (float)m13 { return _hslaMatrix.m13; }
- (float)m14 { return _hslaMatrix.m14; }
- (float)m15 { return _hslaMatrix.m15; }
- (float)m16 { return _hslaMatrix.m16; }
- (float)m17 { return _hslaMatrix.m17; }
- (float)m18 { return _hslaMatrix.m18; }
- (float)m19 { return _hslaMatrix.m19; }
@end

@implementation ArcMetaTagEntry {
  NSString* _key;
  NSString* _value;
}
- (instancetype)initWithKey:(NSString*)key value:(NSString*)value {
  if ((self = [super init])) {
    _key = [key copy];
    _value = [value copy];
  }
  return self;
}
- (NSString*)key { return _key; }
- (NSString*)value { return _value; }
@end

@implementation ArcMetaTagSelector {
  NSInteger _kind;
  NSString* _value;
}
- (instancetype)initWithKind:(NSInteger)kind value:(NSString*)value {
  if ((self = [super init])) {
    _kind = kind;
    _value = [value copy];
  }
  return self;
}
- (NSInteger)kind { return _kind; }
- (NSString*)value { return _value; }
@end

@implementation ArcHtmlToMarkdownResult {
  NSString* _body;
  NSString* _title;
  NSArray* _references;
  NSArray* _selectorsApplied;
  double _selectorsContentPct;
  NSArray* _metaTags;
}
- (instancetype)initWithBody:(NSString*)body
                       title:(NSString*)title
                  references:(NSArray*)references
            selectorsApplied:(NSArray*)selectorsApplied
         selectorsContentPct:(double)selectorsContentPct
                    metaTags:(NSArray*)metaTags {
  if ((self = [super init])) {
    _body = [body copy];
    _title = [title copy];
    _references = [references copy];
    _selectorsApplied = [selectorsApplied copy];
    _selectorsContentPct = selectorsContentPct;
    _metaTags = [metaTags copy];
  }
  return self;
}
- (NSString*)body { return _body; }
- (NSString*)title { return _title; }
- (NSArray*)references { return _references; }
- (NSArray*)selectorsApplied { return _selectorsApplied; }
- (double)selectorsContentPct { return _selectorsContentPct; }
- (NSArray*)metaTags { return _metaTags; }
@end
