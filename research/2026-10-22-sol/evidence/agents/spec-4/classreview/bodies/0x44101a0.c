// ==== 0x44101a0  objc::Categories::ArcWebContents_Extensions_::setGestureHandler:forGestures:  (1056 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcWebContents_Extensions_::setGestureHandler_forGestures_
               (ID param_1,SEL param_2,ID param_3,long_long param_4)

{
  long lVar1;
  long *plVar2;
  ID IVar3;
  long *extraout_x1;
  long *extraout_x1_00;
  code *pcVar4;
  uint uVar5;
  long *plVar6;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  
  uVar5 = (uint)param_4;
  if (param_3 != 0) {
    plVar2 = (long *)FUN_00008790(0x30);
    plVar6 = plVar2 + 1;
    *plVar6 = 0;
    plVar2[2] = 0;
    *plVar2 = (long)&PTR_viewWillStartLiveResize_0da88048;
    if (plVar2 + 3 == (long *)0x0) goto LAB_044105bc;
    _objc_initWeak(&local_48,param_1);
    _objc_initWeak(&local_60,local_48);
    plVar2[3] = (long)&PTR_FUN_0da88f80;
    _objc_copyWeak(plVar2 + 4,&local_60);
    _objc_initWeak(plVar2 + 5,param_3);
    _objc_destroyWeak(&local_60);
    _objc_destroyWeak(&local_48);
    if (param_1 == 0) {
      if (plVar2 != (long *)0x0) {
        LOAcquire();
        lVar1 = *plVar6;
        *plVar6 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*plVar2 + 0x10))(plVar2);
          if (plVar2[2] != 0) {
            LOAcquire();
            lVar1 = plVar2[2];
            plVar2[2] = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_04410284;
          }
          pcVar4 = *(code **)(*plVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(plVar2,0x20);
          }
          else {
            (*pcVar4)(plVar2);
          }
        }
      }
    }
    else {
      _objc_msgSend(param_1,"setWeb_contents_gesture_handler_bridge:",plVar2 + 3,plVar2);
    }
LAB_04410284:
    _objc_msgSend(param_1,"web_contents",&local_60);
    if ((local_50 & 1) == 0) goto LAB_044105bc;
    IVar3 = _objc_msgSend(param_1,"web_contents_gesture_handler_bridge");
    if (extraout_x1 == (long *)0x0) {
      FUN_04345c58(&local_60,IVar3,0,uVar5 & 0x1f);
    }
    else {
      lVar1 = extraout_x1[2];
      extraout_x1[2] = lVar1 + 1;
      FUN_04345c58(lVar1,&local_60,IVar3,extraout_x1,uVar5 & 0x1f);
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
          if (lVar1 != 0) goto LAB_0441041c;
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
    goto LAB_0441041c;
  }
  _objc_msgSend(param_1,"web_contents_gesture_handler_bridge");
  if (extraout_x1_00 != (long *)0x0) {
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
        if (lVar1 != 0) goto LAB_044103ec;
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
LAB_044103ec:
  _objc_msgSend(param_1,"web_contents",&local_60);
  if ((local_50 & 1) == 0) {
LAB_044105bc:
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x44105c0);
    (*pcVar4)();
  }
  FUN_04375e78(local_60,0,0,uVar5 & 0x1f);
LAB_0441041c:
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
          return;
        }
      }
      pcVar4 = *(code **)(*local_58 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar4)(local_58);
      }
    }
  }
  return;
}


