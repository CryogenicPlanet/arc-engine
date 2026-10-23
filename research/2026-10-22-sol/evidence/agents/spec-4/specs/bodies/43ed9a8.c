// ==== 0x43ed9a8  objc::Categories::ArcBrowserContext_Extensions_::setDelegate:  (1116 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::setDelegate_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  long *plVar3;
  long *extraout_x1;
  long *extraout_x1_00;
  long *extraout_x1_01;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_58 [8];
  long *local_50;
  byte local_48;
  
  IVar2 = _objc_msgSend(param_1,"contextDelegate");
  if (param_3 == 0) {
    if (extraout_x1 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar1 = extraout_x1[1];
    extraout_x1[1] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
    (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
    if (extraout_x1[2] != 0) {
      LOAcquire();
      lVar1 = extraout_x1[2];
      extraout_x1[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
    }
    pcVar4 = *(code **)(*extraout_x1 + 0x20);
    local_50 = extraout_x1;
    if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40))
    goto LAB_043edda4;
    goto LAB_043eda70;
  }
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
        if (lVar1 != 0) goto LAB_043eda04;
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
LAB_043eda04:
  if (IVar2 == 0) {
    plVar3 = (long *)FUN_00008790(0x30);
    plVar5 = plVar3 + 1;
    *plVar5 = 0;
    plVar3[2] = 0;
    *plVar3 = (long)&PTR_viewWillStartLiveResize_0da879b0;
    if (plVar3 + 3 == (long *)0x0) goto LAB_043ede00;
    plVar3[3] = (long)&PTR_FUN_0da879e8;
    _objc_initWeak(plVar3 + 4,param_3);
    _objc_initWeak(plVar3 + 5,param_1);
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
            if (lVar1 != 0) goto LAB_043edc34;
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
      _objc_msgSend(param_1,"setContextDelegate:",plVar3 + 3,plVar3);
    }
LAB_043edc34:
    _objc_msgSend(param_1,"browser_context",auStack_58);
    if ((local_48 & 1) == 0) {
LAB_043ede00:
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x43ede04);
      (*pcVar4)();
    }
    IVar2 = _objc_msgSend(param_1,"contextDelegate");
    if (extraout_x1_01 == (long *)0x0) {
      FUN_04333868(auStack_58,IVar2,0);
    }
    else {
      lVar1 = extraout_x1_01[2];
      extraout_x1_01[2] = lVar1 + 1;
      FUN_04333868(lVar1,auStack_58,IVar2,extraout_x1_01);
      LOAcquire();
      lVar1 = extraout_x1_01[1];
      extraout_x1_01[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*extraout_x1_01 + 0x10))(extraout_x1_01);
        if (extraout_x1_01[2] != 0) {
          LOAcquire();
          lVar1 = extraout_x1_01[2];
          extraout_x1_01[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) goto LAB_043edba4;
        }
        pcVar4 = *(code **)(*extraout_x1_01 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(extraout_x1_01,0x20);
        }
        else {
          (*pcVar4)(extraout_x1_01);
        }
      }
    }
LAB_043edba4:
    if (local_48 == 0) {
      return;
    }
  }
  else {
    IVar2 = _objc_msgSend(param_1,"contextDelegate");
    *(ID *)(IVar2 + 8) = param_3;
    local_50 = extraout_x1_00;
  }
  if (local_50 == (long *)0x0) {
    return;
  }
  LOAcquire();
  lVar1 = local_50[1];
  local_50[1] = lVar1 + -1;
  LORelease();
  if (lVar1 != 0) {
    return;
  }
  (**(code **)(*local_50 + 0x10))(local_50);
  if (local_50[2] != 0) {
    LOAcquire();
    lVar1 = local_50[2];
    local_50[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
  }
  pcVar4 = *(code **)(*local_50 + 0x20);
  if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
LAB_043edda4:
    (*pcVar4)(local_50);
    return;
  }
LAB_043eda70:
  FUN_00005810(local_50,0x20);
  return;
}


