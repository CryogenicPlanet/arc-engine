// ==== 0x44081f0  objc::Categories::ArcMenuItemModel_Extensions_::accelerator  (296 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcMenuItemModel_Extensions_::accelerator(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auVar4 [16];
  undefined8 local_58;
  long *local_50;
  byte local_48;
  KeyboardAccelerator local_40;
  undefined4 local_38;
  
  local_38 = 0;
  _objc_msgSend(param_1,"menu_item_model",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4408318);
    (*pcVar3)();
  }
  auVar4 = FUN_0432bdc0(local_58);
  local_40 = auVar4._0_8_;
  local_38 = CONCAT31(local_38._1_3_,auVar4[8]);
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_50[1];
    local_50[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar1 = local_50[2];
        local_50[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04408254;
      }
      pcVar3 = *(code **)(*local_50 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar3)(local_50);
      }
    }
  }
LAB_04408254:
  if ((auVar4._8_8_ & 1) == 0) {
    IVar2 = 0;
  }
  else {
    IVar2 = _objc_alloc(&class_t::ArcKeyboardAccelerator);
    IVar2 = ArcKeyboardAccelerator::initWithAccelerator_(IVar2,"initWithAccelerator:",&local_40);
  }
  return IVar2;
}


