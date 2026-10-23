// Copyright 2026 ArcMod. Standalone ABI/copy-semantics probe, no profile access.
// xcrun clang++ -fobjc-arc -framework Foundation value_probe.mm -o /tmp/arc-value-probe
// /tmp/arc-value-probe /path/to/ArcCore.framework/Versions/A/ArcCore
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>

#include <array>
#include <cassert>
#include <dlfcn.h>
#include <iostream>

struct HSLAMatrix {
  float m0, m1, m2, m3, m4, m5, m6, m7, m8, m9;
  float m10, m11, m12, m13, m14, m15, m16, m17, m18, m19;
};

@interface NSObject (ArcRendererValueProbe)
- (instancetype)initWithMatrix:(const float[20])matrix;
- (HSLAMatrix)hslaMatrix;
- (void)setHslaMatrix:(HSLAMatrix)matrix;
- (instancetype)initWithKey:(NSString*)key value:(NSString*)value;
- (instancetype)initWithKind:(NSInteger)kind value:(NSString*)value;
- (NSString*)key;
- (NSString*)value;
- (NSInteger)kind;
- (instancetype)initWithBody:(NSString*)body title:(NSString*)title
                  references:(NSArray*)references selectorsApplied:(NSArray*)selectors
         selectorsContentPct:(double)percentage metaTags:(NSArray*)tags;
- (NSString*)body;
- (NSString*)title;
- (NSArray*)references;
- (NSArray*)selectorsApplied;
- (double)selectorsContentPct;
- (NSArray*)metaTags;
@end

int main(int argc, const char* argv[]) {
  @autoreleasepool {
    assert(argc == 2);
    void* library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!library) {
      std::cerr << dlerror() << '\n';
      return 1;
    }
    auto alloc = ^id(NSString* name) {
      Class cls = NSClassFromString(name);
      assert(cls);
      return [cls alloc];
    };
    std::array<float, 20> values;
    for (size_t i = 0; i < values.size(); ++i) {
      values[i] = static_cast<float>(i) + 0.25f;
    }
    id matrix = [alloc(@"ArcHSLAMatrix") initWithMatrix:values.data()];
    HSLAMatrix aggregate = [matrix hslaMatrix];
    assert(aggregate.m0 == 0.25f && aggregate.m9 == 9.25f && aggregate.m19 == 19.25f);
    values[0] = -1;
    assert([matrix hslaMatrix].m0 == 0.25f);
    aggregate.m0 = -2.5f;
    aggregate.m19 = 77.25f;
    [matrix setHslaMatrix:aggregate];
    for (size_t i = 0; i < values.size(); ++i) {
      SEL getter = NSSelectorFromString([NSString stringWithFormat:@"m%zu", i]);
      float value = ((float (*)(id, SEL))objc_msgSend)(matrix, getter);
      assert(value == (i == 0 ? -2.5f : i == 19 ? 77.25f : static_cast<float>(i) + 0.25f));
    }

    NSMutableString* key = [@"key" mutableCopy];
    NSMutableString* value = [@"value" mutableCopy];
    id entry = [alloc(@"ArcMetaTagEntry") initWithKey:key value:value];
    NSObject* selector = [alloc(@"ArcMetaTagSelector") initWithKind:-7 value:value];
    NSMutableArray* references = [NSMutableArray arrayWithObject:@"reference"];
    id markdown = [alloc(@"ArcHtmlToMarkdownResult")
        initWithBody:value title:key references:references selectorsApplied:references
        selectorsContentPct:0.375 metaTags:references];
    [key appendString:@"-changed"];
    [value appendString:@"-changed"];
    [references removeAllObjects];
    assert([[entry key] isEqual:@"key"] && [[entry value] isEqual:@"value"]);
    assert([selector kind] == -7 && [[selector value] isEqual:@"value"]);
    assert([[markdown body] isEqual:@"value"] && [[markdown title] isEqual:@"key"]);
    assert([[markdown references] count] == 1 && [[markdown selectorsApplied] count] == 1);
    assert([[markdown metaTags] count] == 1 && [markdown selectorsContentPct] == 0.375);
    id nilEntry = [alloc(@"ArcMetaTagEntry") initWithKey:nil value:nil];
    assert([nilEntry key] == nil && [nilEntry value] == nil);

    for (NSString* name in @[@"ArcHSLAMatrix", @"ArcMetaTagEntry", @"ArcMetaTagSelector",
                             @"ArcHtmlToMarkdownResult"]) {
      Class cls = NSClassFromString(name);
      unsigned int count = 0;
      Method* methods = class_copyMethodList(cls, &count);
      for (unsigned int i = 0; i < count; ++i) {
        std::cout << name.UTF8String << '\t' << sel_getName(method_getName(methods[i]))
                  << '\t' << method_getTypeEncoding(methods[i]) << '\n';
      }
      free(methods);
      objc_property_t* properties = class_copyPropertyList(cls, &count);
      for (unsigned int i = 0; i < count; ++i) {
        std::cout << "PROPERTY:" << name.UTF8String << '\t'
                  << property_getName(properties[i]) << '\t'
                  << property_getAttributes(properties[i]) << '\n';
      }
      free(properties);
    }
    std::cout << "PASS: matrix ABI/getters, copy semantics, nil values\n";
  }
}
