// ==== 0x441066c  objc::Categories::ArcWebContents_Extensions_::setUserActivityDelegate:  (924 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::setUserActivityDelegate_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  long *plVar3;
  long *extraout_x1;
  long *extraout_x1_00;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_58 [8];
  long *local_50;
  byte local_48;
  
  IVar2 = _objc_msgSend(param_1,"userActivityObserver");
  if (extraout_x1 != (long *)0x0) {
    LOAcquire();
    lVar1 = extraout_x1[1];
    extraout_x1[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
      if (extraout_x1[2] != 0) {
        LOAcquire();
        lVar1 = extraout_x1[2];
        extraout_x1[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044106c4;
      }
      pcVar4 = *(code **)(*extraout_x1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(extraout_x1,0x20);
      }
      else {
        (*pcVar4)(extraout_x1);
      }
    }
  }
LAB_044106c4:
  if (IVar2 != 0) goto LAB_044106c8;
  plVar3 = (long *)FUN_00008790(0x28);
  plVar5 = plVar3 + 1;
  *plVar5 = 0;
  plVar3[2] = 0;
  *plVar3 = (long)&PTR_viewWillStartLiveResize_0da88080;
  if (plVar3 + 3 == (long *)0x0) goto LAB_04410a04;
  plVar3[3] = (long)&PTR_FUN_0da880b8;
  _objc_initWeak(plVar3 + 4,param_1);
  if (param_1 == 0) {
    if (plVar3 != (long *)0x0) {
      LOAcquire();
      lVar1 = *plVar5;
      *plVar5 = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*plVar3 + 0x10))(plVar3);
        if (plVar3[2] != 0) {
          LOAcquire();
          lVar1 = plVar3[2];
          plVar3[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) goto LAB_04410860;
        }
        pcVar4 = *(code **)(*plVar3 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(plVar3,0x20);
        }
        else {
          (*pcVar4)(plVar3);
        }
      }
    }
  }
  else {
    _objc_msgSend(param_1,"setUserActivityObserver:",plVar3 + 3,plVar3);
  }
LAB_04410860:
  _objc_msgSend(param_1,"web_contents",auStack_58);
  if ((local_48 & 1) == 0) {
LAB_04410a04:
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4410a08);
    (*pcVar4)();
  }
  IVar2 = _objc_msgSend(param_1,"userActivityObserver");
  if (extraout_x1_00 == (long *)0x0) {
    FUN_04345dc8(auStack_58,IVar2,0);
  }
  else {
    lVar1 = extraout_x1_00[2];
    extraout_x1_00[2] = lVar1 + 1;
    FUN_04345dc8(lVar1,auStack_58,IVar2,extraout_x1_00);
    LOAcquire();
    lVar1 = extraout_x1_00[1];
    extraout_x1_00[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*extraout_x1_00 + 0x10))(extraout_x1_00);
      if (extraout_x1_00[2] != 0) {
        LOAcquire();
        lVar1 = extraout_x1_00[2];
        extraout_x1_00[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0441079c;
      }
      pcVar4 = *(code **)(*extraout_x1_00 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(extraout_x1_00,0x20);
      }
      else {
        (*pcVar4)(extraout_x1_00);
      }
    }
  }
LAB_0441079c:
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
        if (lVar1 != 0) goto LAB_044106c8;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_044106c8:
  *(ID *)(param_1 + 8) = param_3;
  return;
}


