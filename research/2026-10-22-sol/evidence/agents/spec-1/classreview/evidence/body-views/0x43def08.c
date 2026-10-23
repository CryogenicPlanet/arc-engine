// ==== 0x43def08  ArcKeyEventData::initWithEventType:domCode:key:windowsVirtualKeyCode:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x2c bytes */

ID ArcKeyEventData::initWithEventType_domCode_key_windowsVirtualKeyCode_
             (ID param_1,SEL param_2,unsigned_long_long param_3,ID param_4,ID param_5,int param_6)

{
  ID IVar1;
  ID IVar2;
  undefined8 uVar3;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcKeyEventData;
  local_50 = param_1;
  IVar1 = NSObject::init((ID)&local_50,"init");
  if (IVar1 != 0) {
    *(unsigned_long_long *)(IVar1 + 0x10) = param_3;
    IVar2 = _objc_msgSend(param_4,"copy");
    uVar3 = *(undefined8 *)(IVar1 + 0x18);
    *(ID *)(IVar1 + 0x18) = IVar2;
    IVar2 = _objc_msgSend(param_5,"copy",uVar3);
    *(ID *)(IVar1 + 0x20) = IVar2;
    *(int *)(IVar1 + 8) = param_6;
  }
  return IVar1;
}


