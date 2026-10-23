// ==== 0x4408318  objc::Categories::ArcMenuItemModel_Extensions_::icon  (460 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcMenuItemModel_Extensions_::icon(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined **local_48;
  long *local_40;
  long *local_38;
  
  _objc_msgSend(param_1,"menu_item_model",&local_60);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44084e4);
    (*pcVar3)();
  }
  FUN_0432be28(&local_48,local_60);
  if ((*local_40 == 0) || (*(long *)(*local_40 + 0x18) == 0)) {
    IVar2 = 0;
  }
  else {
    IVar2 = FUN_02a02ccc();
  }
  local_48 = &PTR_FUN_0da7eed0;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04408398;
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
LAB_04408398:
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_58 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar3)(local_58);
      }
    }
  }
  return IVar2;
}


