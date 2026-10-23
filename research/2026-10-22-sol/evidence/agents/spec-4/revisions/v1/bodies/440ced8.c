// ==== 0x440ced8  ArcTracingController::.cxx_destruct  (372 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcTracingController::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  long *plVar2;
  
  if ((*(char *)(param_1 + 0x30) != '\0') &&
     (plVar2 = *(long **)(param_1 + 0x28), plVar2 != (long *)0x0)) {
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
        if (lVar1 != 0) goto LAB_0440cf0c;
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
LAB_0440cf0c:
  plVar2 = *(long **)(param_1 + 0x18);
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
                    /* WARNING: Could not recover jumptable at 0x0440d01c. Too many branches */
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


