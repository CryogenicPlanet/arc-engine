// ==== 0x432bf9c  FUN_0432bf9c  (404 bytes)

void FUN_0432bf9c(long param_1,undefined8 param_2,long *param_3)

{
  long lVar1;
  long *plVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_3 == (long *)0x0) {
    param_3 = *(long **)(param_1 + 0x28);
    *(undefined8 *)(param_1 + 0x20) = param_2;
    *(undefined8 *)(param_1 + 0x28) = 0;
    if (param_3 == (long *)0x0) {
      return;
    }
    goto LAB_0432c024;
  }
  plVar2 = param_3 + 2;
  *plVar2 = *plVar2 + 1;
  *plVar2 = *plVar2 + 1;
  plVar2 = *(long **)(param_1 + 0x28);
  *(undefined8 *)(param_1 + 0x20) = param_2;
  *(long **)(param_1 + 0x28) = param_3;
  if (plVar2 != (long *)0x0) {
    if (plVar2[2] != 0) {
      LOAcquire();
      lVar1 = plVar2[2];
      plVar2[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_0432bff8;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(plVar2,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)();
    }
  }
LAB_0432bff8:
  if (param_3[2] != 0) {
    LOAcquire();
    lVar1 = param_3[2];
    param_3[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) goto LAB_0432c024;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
     (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
    FUN_00005810(param_3,0x20);
  }
  else {
    (*UNRECOVERED_JUMPTABLE)(param_3);
  }
LAB_0432c024:
  if (param_3[2] != 0) {
    LOAcquire();
    lVar1 = param_3[2];
    param_3[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x0432c0d8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}


