// ==== 0x43dd4e8  ArcMetaTagEntry::initWithKey:value:  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcMetaTagEntry::initWithKey_value_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  ID IVar1;
  ID IVar2;
  undefined8 uVar3;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcMetaTagEntry;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 != 0) {
    IVar2 = _objc_msgSend(param_3,"copy");
    uVar3 = *(undefined8 *)(IVar1 + 8);
    *(ID *)(IVar1 + 8) = IVar2;
    IVar2 = _objc_msgSend(param_4,"copy",uVar3);
    *(ID *)(IVar1 + 0x10) = IVar2;
  }
  return IVar1;
}


