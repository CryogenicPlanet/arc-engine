// ==== 0x44120fc  objc::Categories::ArcWebContents_Extensions_::isEqual:  (568 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcWebContents_Extensions_::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  bool bVar5;
  long *plVar6;
  long local_60;
  long *local_58;
  undefined8 local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  IVar3 = _objc_msgSend((ID)&class_t::ArcWebContents,"class");
  IVar3 = _objc_msgSend(param_3,"isKindOfClass:",IVar3);
  if ((int)IVar3 == 0) {
    return 0;
  }
  _objc_msgSend(param_1,"web_contents",&local_48);
  if (param_3 == 0) {
    local_50._0_1_ = 0;
    local_60 = 0;
    local_58 = (long *)0x0;
    local_50 = 0;
  }
  else {
    _objc_msgSend(param_3,"web_contents",&local_60);
  }
  plVar1 = local_58;
  if ((local_38 & (byte)local_50) == 0) {
    bVar5 = (bool)(local_38 == (byte)local_50);
    if ((byte)local_50 == 0) goto LAB_044121bc;
  }
  else {
    if (((byte)local_50 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x4412334);
      (*pcVar4)();
    }
    bVar5 = (bool)(local_48 == local_60);
  }
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_58[1];
    local_58[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar6 = local_58 + 2;
      (**(code **)(*local_58 + 0x10))(local_58);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_044121bc;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_044121bc:
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_40[1];
    local_40[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar2 = local_40[2];
        local_40[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return bVar5;
        }
      }
      pcVar4 = *(code **)(*local_40 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar4)(local_40);
      }
    }
  }
  return bVar5;
}


