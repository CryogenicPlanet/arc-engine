// ==== 0x40439c4  objc::Categories::ArcBrowserContextHistory_Extensions_::setVisitObserver:  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::setVisitObserver_
               (ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  long *extraout_x1;
  long *extraout_x1_00;
  long *extraout_x1_01;
  long *plVar5;
  undefined1 auStack_68 [16];
  byte local_58;
  
  IVar3 = _objc_msgSend(param_1,"visitObserverBridge");
  if (param_3 == 0) {
    if (extraout_x1 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar2 = extraout_x1[1];
    extraout_x1[1] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
    (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
    plVar4 = extraout_x1;
  }
  else {
    if (extraout_x1 != (long *)0x0) {
      LOAcquire();
      lVar2 = extraout_x1[1];
      extraout_x1[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
        FUN_02527d18(extraout_x1);
      }
    }
    if (IVar3 == 0) {
      IVar3 = _objc_msgSend(param_1,"owner");
      plVar4 = (long *)FUN_00008790(0x30);
      plVar5 = plVar4 + 1;
      *plVar5 = 0;
      plVar4[2] = 0;
      *plVar4 = (long)&PTR_LAB_0dd2c070;
      if (plVar4 + 3 != (long *)0x0) {
        plVar4[3] = (long)&PTR_FUN_0dd2c0b0;
        _objc_initWeak(plVar4 + 4,param_3);
        _objc_initWeak(plVar4 + 5,IVar3);
        if (param_1 == 0) {
          if (plVar4 != (long *)0x0) {
            LOAcquire();
            lVar2 = *plVar5;
            *plVar5 = lVar2 + -1;
            LORelease();
            if (lVar2 == 0) {
              (**(code **)(*plVar4 + 0x10))(plVar4);
              FUN_02527d18(plVar4);
            }
          }
        }
        else {
          _objc_msgSend(param_1,"setVisitObserverBridge:",plVar4 + 3,plVar4);
        }
        _objc_msgSend(param_1,"browser_context_history",auStack_68);
        if ((local_58 & 1) != 0) {
          IVar3 = _objc_msgSend(param_1,"visitObserverBridge");
          if (extraout_x1_01 != (long *)0x0) {
            extraout_x1_01[2] = extraout_x1_01[2] + 1;
          }
          FUN_04332f08(auStack_68,IVar3,extraout_x1_01);
          if (extraout_x1_01 != (long *)0x0) {
            LOAcquire();
            lVar2 = extraout_x1_01[1];
            extraout_x1_01[1] = lVar2 + -1;
            LORelease();
            if (lVar2 == 0) {
              (**(code **)(*extraout_x1_01 + 0x10))(extraout_x1_01);
              FUN_02527d18(extraout_x1_01);
            }
          }
          if (local_58 == 0) {
            return;
          }
          FUN_04332750(auStack_68);
          return;
        }
      }
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4043c38);
      (*pcVar1)();
    }
    IVar3 = _objc_msgSend(param_1,"visitObserverBridge");
    *(ID *)(IVar3 + 8) = param_3;
    if (extraout_x1_00 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar2 = extraout_x1_00[1];
    extraout_x1_00[1] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
    (**(code **)(*extraout_x1_00 + 0x10))(extraout_x1_00);
    plVar4 = extraout_x1_00;
  }
  FUN_02527d18(plVar4);
  return;
}


