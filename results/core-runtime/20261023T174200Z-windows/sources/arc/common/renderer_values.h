// Copyright 2026 ArcMod. Objective-C value types shared by renderer bridges.
#ifndef ARC_COMMON_RENDERER_VALUES_H_
#define ARC_COMMON_RENDERER_VALUES_H_

#import "arc/api/arc_internal.h"

// Keep the named fields: @encode(HSLAMatrix) is part of Arc's public ABI.
struct HSLAMatrix {
  float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9;
  float m10, m11, m12, m13, m14, m15, m16, m17, m18, m19;
};
static_assert(sizeof(HSLAMatrix) == 20 * sizeof(float));

ARC_EXPORT @interface ArcHSLAMatrix : NSObject
- (instancetype)initWithMatrix:(const float[20])matrix;
@property(nonatomic) HSLAMatrix hslaMatrix;
@property(nonatomic, readonly) float m0;
@property(nonatomic, readonly) float m1;
@property(nonatomic, readonly) float m2;
@property(nonatomic, readonly) float m3;
@property(nonatomic, readonly) float m4;
@property(nonatomic, readonly) float m5;
@property(nonatomic, readonly) float m6;
@property(nonatomic, readonly) float m7;
@property(nonatomic, readonly) float m8;
@property(nonatomic, readonly) float m9;
@property(nonatomic, readonly) float m10;
@property(nonatomic, readonly) float m11;
@property(nonatomic, readonly) float m12;
@property(nonatomic, readonly) float m13;
@property(nonatomic, readonly) float m14;
@property(nonatomic, readonly) float m15;
@property(nonatomic, readonly) float m16;
@property(nonatomic, readonly) float m17;
@property(nonatomic, readonly) float m18;
@property(nonatomic, readonly) float m19;
@end

ARC_EXPORT @interface ArcMetaTagEntry : NSObject
- (instancetype)initWithKey:(NSString*)key value:(NSString*)value;
@property(nonatomic, readonly, copy) NSString* key;
@property(nonatomic, readonly, copy) NSString* value;
@end

ARC_EXPORT @interface ArcMetaTagSelector : NSObject
- (instancetype)initWithKind:(NSInteger)kind value:(NSString*)value;
@property(nonatomic, readonly) NSInteger kind;
@property(nonatomic, readonly, copy) NSString* value;
@end

ARC_EXPORT @interface ArcHtmlToMarkdownResult : NSObject
- (instancetype)initWithBody:(NSString*)body
                       title:(NSString*)title
                  references:(NSArray*)references
            selectorsApplied:(NSArray*)selectorsApplied
         selectorsContentPct:(double)selectorsContentPct
                    metaTags:(NSArray*)metaTags;
@property(nonatomic, readonly, copy) NSString* body;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly, copy) NSArray* references;
@property(nonatomic, readonly, copy) NSArray* selectorsApplied;
@property(nonatomic, readonly) double selectorsContentPct;
@property(nonatomic, readonly, copy) NSArray* metaTags;
@end

#endif  // ARC_COMMON_RENDERER_VALUES_H_
