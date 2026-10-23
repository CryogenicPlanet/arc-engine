// ==== 0x44080f8  objc::Categories::ArcMenuItemModel_Extensions_::hasAccelerator  (248 bytes)

/* Function Stack Size: 0x10 bytes */

bool objc::Categories::ArcMenuItemModel_Extensions_::hasAccelerator(ID param_1,SEL param_2)

{
  long lVar1;
  uint extraout_w1;
  code *pcVar2;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"menu_item_model",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x44081f0);
    (*pcVar2)();
  }
  FUN_0432bdc0(local_48);
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04408150;
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
LAB_04408150:
  return extraout_w1 & 1;
}


