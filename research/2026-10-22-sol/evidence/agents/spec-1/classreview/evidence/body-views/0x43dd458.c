// ==== 0x43dd458  ArcMetaTagSelector::initWithKind:value:  (144 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcMetaTagSelector::initWithKind_value_(ID param_1,SEL param_2,long_long param_3,ID param_4)

{
  ID IVar1;
  ID IVar2;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcMetaTagSelector;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 != 0) {
    *(long_long *)(IVar1 + 8) = param_3;
    IVar2 = _objc_msgSend(param_4,"copy");
    *(ID *)(IVar1 + 0x10) = IVar2;
  }
  return IVar1;
}


