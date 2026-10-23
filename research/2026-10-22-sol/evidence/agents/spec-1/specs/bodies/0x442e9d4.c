// ==== 0x442e9d4  ArcExtractedPageContent::initWithExtractedPageContent:  (164 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtractedPageContent::initWithExtractedPageContent_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtractedPageContent;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x1e8) != '\0') {
      FUN_04295208(IVar1 + 0x48);
      if (*(char *)(IVar1 + 0x3f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x28),*(ulong *)(IVar1 + 0x38) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar1 + 0x27) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x10),*(ulong *)(IVar1 + 0x20) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x1e8) = 0;
    }
    FUN_04296004(IVar1 + 0x10,param_3);
    *(undefined1 *)(IVar1 + 0x1e8) = 1;
  }
  return IVar1;
}


