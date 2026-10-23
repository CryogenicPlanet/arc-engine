// ==== 0x43eacb0  ArcBrowserApplication::setBrowserApplicationDelegate:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcBrowserApplication::setBrowserApplicationDelegate_
          (ArcBrowserApplication *this,ID param_1,SEL param_2,
          shared_ptr<(anonymous_namespace)::BrowserApplicationDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  BrowserApplicationDelegateBridge *pBVar2;
  long *plVar3;
  
  pBVar2 = param_3.field0_0x0;
  if (pBVar2 != (BrowserApplicationDelegateBridge *)0x0) {
    *(long *)(pBVar2 + 8) = *(long *)(pBVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0x38);
  *(SEL *)(this + 0x30) = param_2;
  *(BrowserApplicationDelegateBridge **)(this + 0x38) = pBVar2;
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
        if (lVar1 != 0) goto LAB_043eacf0;
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
LAB_043eacf0:
  if (pBVar2 != (BrowserApplicationDelegateBridge *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(pBVar2 + 8);
    *(long *)(pBVar2 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)pBVar2 + 0x10))(pBVar2,param_1);
      if (*(long *)(pBVar2 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(pBVar2 + 0x10);
        *(long *)(pBVar2 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(long *)pBVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043eae28. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(pBVar2);
        return;
      }
      FUN_00005810(pBVar2,0x20);
      return;
    }
  }
  return;
}


