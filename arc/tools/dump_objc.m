#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#include <dlfcn.h>
static void methods(Class c, const char *kind, FILE *f) {
  unsigned n = 0; Method *ms = class_copyMethodList(c, &n);
  for (unsigned i = 0; i < n; i++) fprintf(f, "M\t%s\t%s\t%s\t%s\n", class_getName(c), kind, sel_getName(method_getName(ms[i])), method_getTypeEncoding(ms[i]) ?: "");
  free(ms);
}
int main(int argc, char **argv) {
  @autoreleasepool {
    void *h = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!h) { fprintf(stderr, "%s\n", dlerror()); return 1; }
    unsigned n = 0; const char **names = objc_copyClassNamesForImage(argv[1], &n);
    FILE *f = fopen(argv[2], "w");
    for (unsigned i = 0; i < n; i++) {
      if (strncmp(names[i], "Arc", 3) != 0) continue;
      Class c = objc_getClass(names[i]);
      fprintf(f, "C\t%s\t%s\t%zu\n", names[i], class_getName(class_getSuperclass(c)), class_getInstanceSize(c));
      unsigned pn = 0; Protocol * __unsafe_unretained *ps = class_copyProtocolList(c, &pn);
      for (unsigned j = 0; j < pn; j++) fprintf(f, "P\t%s\t%s\n", names[i], protocol_getName(ps[j]));
      free(ps);
      unsigned qn = 0; objc_property_t *props = class_copyPropertyList(c, &qn);
      for (unsigned j = 0; j < qn; j++) fprintf(f, "R\t%s\t%s\t%s\n", names[i], property_getName(props[j]), property_getAttributes(props[j]));
      free(props);
      unsigned vn = 0; Ivar *iv = class_copyIvarList(c, &vn);
      for (unsigned j = 0; j < vn; j++) fprintf(f, "I\t%s\t%s\t%s\t%td\n", names[i], ivar_getName(iv[j]), ivar_getTypeEncoding(iv[j]) ?: "", ivar_getOffset(iv[j]));
      free(iv);
      methods(c, "-", f); methods(object_getClass(c), "+", f);
    }
    fclose(f);
  }
  return 0;
}
