// ==== 0x43dd3c0  ArcReference::initWithReferenceText:minimumCharacterCount:  (144 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcReference::initWithReferenceText_minimumCharacterCount_
             (ID param_1,SEL param_2,ID param_3,long_long param_4)

{
  ID IVar1;
  ID IVar2;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcReference;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 != 0) {
    IVar2 = _objc_msgSend(param_3,"copy");
    *(ID *)(IVar1 + 8) = IVar2;
    *(long_long *)(IVar1 + 0x10) = param_4;
  }
  return IVar1;
}


