// ==== 0x442b9ec  ArcFormControlInfo::initWithFormControlInfo:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcFormControlInfo::initWithFormControlInfo_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcFormControlInfo;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x98) != '\0') {
      FUN_04294a84(IVar1 + 0x10);
      *(undefined1 *)(IVar1 + 0x98) = 0;
    }
    FUN_04294be4(IVar1 + 0x10,param_3);
    *(undefined1 *)(IVar1 + 0x98) = 1;
  }
  return IVar1;
}


