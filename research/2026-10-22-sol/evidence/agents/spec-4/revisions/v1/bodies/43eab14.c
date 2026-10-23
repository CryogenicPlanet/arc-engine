// ==== 0x43eab14  ArcWebContentsMediaSession::setBridge:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcWebContentsMediaSession::setBridge_
          (ArcWebContentsMediaSession *this,ID param_1,SEL param_2,
          shared_ptr<(anonymous_namespace)::WebContentsMediaSessionDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  WebContentsMediaSessionDelegateBridge *pWVar2;
  long *plVar3;
  
  pWVar2 = param_3.field0_0x0;
  if (pWVar2 != (WebContentsMediaSessionDelegateBridge *)0x0) {
    *(long *)(pWVar2 + 8) = *(long *)(pWVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0x28);
  *(SEL *)(this + 0x20) = param_2;
  *(WebContentsMediaSessionDelegateBridge **)(this + 0x28) = pWVar2;
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_1 = extraout_x1;
        if (lVar1 != 0) goto LAB_043eab54;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_1 = extraout_x1_00;
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar3);
        param_1 = extraout_x1_01;
      }
    }
  }
LAB_043eab54:
  if (pWVar2 != (WebContentsMediaSessionDelegateBridge *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(pWVar2 + 8);
    *(long *)(pWVar2 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)pWVar2 + 0x10))(pWVar2,param_1);
      if (*(long *)(pWVar2 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(pWVar2 + 0x10);
        *(long *)(pWVar2 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(long *)pWVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043eac8c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(pWVar2);
        return;
      }
      FUN_00005810(pWVar2,0x20);
      return;
    }
  }
  return;
}


