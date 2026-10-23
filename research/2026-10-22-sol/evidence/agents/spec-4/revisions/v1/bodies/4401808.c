// ==== 0x4401808  ArcBrowserSidePanel::.cxx_destruct  (364 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserSidePanel::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  SEL extraout_x1_01;
  long *plVar2;
  
  plVar2 = *(long **)(param_1 + 0x20);
  if (plVar2 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar2[1];
    plVar2[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar2 + 0x10))(plVar2);
      if (plVar2[2] != 0) {
        LOAcquire();
        lVar1 = plVar2[2];
        plVar2[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_04401834;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar2);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_04401834:
  plVar2 = *(long **)(param_1 + 0x10);
  if (plVar2 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar2[1];
    plVar2[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar2 + 0x10))(plVar2,param_2);
      if (plVar2[2] != 0) {
        LOAcquire();
        lVar1 = plVar2[2];
        plVar2[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x04401970. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(plVar2);
        return;
      }
      FUN_00005810(plVar2,0x20);
      return;
    }
  }
  return;
}


