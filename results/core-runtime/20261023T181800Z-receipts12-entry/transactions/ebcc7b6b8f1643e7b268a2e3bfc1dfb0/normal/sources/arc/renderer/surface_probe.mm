// Runtime surface only: no browser/profile creation and no API invocation.
#import <Foundation/Foundation.h>
#import <objc/runtime.h>

#include <dlfcn.h>
#include <iostream>

int main(int argc, const char* argv[]) {
  @autoreleasepool {
    if (argc != 2 || !dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)) {
      std::cerr << "Expected one loadable framework path\n";
      return 1;
    }
    for (NSString* name in @[@"ArcHSLAMatrix", @"ArcMetaTagEntry", @"ArcMetaTagSelector",
                             @"ArcHtmlToMarkdownResult", @"ArcContentNode",
                             @"ArcExtractedPageContent", @"ArcBrowserContextEffects",
                             @"ArcBrowserContextPageContentExtraction"]) {
      Class cls = NSClassFromString(name);
      if (!cls) {
        std::cerr << "Missing " << name.UTF8String << '\n';
        return 1;
      }
      std::cout << "C\t" << name.UTF8String << '\t'
                << class_getName(class_getSuperclass(cls)) << '\n';
      unsigned int count = 0;
      Method* methods = class_copyMethodList(cls, &count);
      for (unsigned int i = 0; i < count; ++i) {
        std::cout << "M\t" << name.UTF8String << '\t'
                  << sel_getName(method_getName(methods[i])) << '\t'
                  << method_getTypeEncoding(methods[i]) << '\n';
      }
      free(methods);
      objc_property_t* properties = class_copyPropertyList(cls, &count);
      for (unsigned int i = 0; i < count; ++i) {
        std::cout << "R\t" << name.UTF8String << '\t'
                  << property_getName(properties[i]) << '\t'
                  << property_getAttributes(properties[i]) << '\n';
      }
      free(properties);
    }
  }
}
