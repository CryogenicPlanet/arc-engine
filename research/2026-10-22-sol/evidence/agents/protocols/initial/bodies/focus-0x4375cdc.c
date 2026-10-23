// ==== 0x4375cdc  FUN_04375cdc  (412 bytes)

void FUN_04375cdc(long param_1,undefined8 param_2,long *param_3)

{
  long lVar1;
  long *plVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_3 == (long *)0x0) {
    *(undefined8 *)(param_1 + 0x348) = param_2;
    param_3 = *(long **)(param_1 + 0x350);
    *(undefined8 *)(param_1 + 0x350) = 0;
    if (param_3 == (long *)0x0) {
      return;
    }
    goto LAB_04375d68;
  }
  plVar2 = param_3 + 2;
  *plVar2 = *plVar2 + 1;
  *plVar2 = *plVar2 + 1;
  *(undefined8 *)(param_1 + 0x348) = param_2;
  plVar2 = *(long **)(param_1 + 0x350);
  *(long **)(param_1 + 0x350) = param_3;
  if (plVar2 != (long *)0x0) {
    if (plVar2[2] != 0) {
      LOAcquire();
      lVar1 = plVar2[2];
      plVar2[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_04375d3c;
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
LAB_04375d3c:
  if (param_3[2] != 0) {
    LOAcquire();
    lVar1 = param_3[2];
    param_3[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) goto LAB_04375d68;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
     (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
    FUN_00005810(param_3,0x20);
  }
  else {
    (*UNRECOVERED_JUMPTABLE)(param_3);
  }
LAB_04375d68:
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
                    /* WARNING: Could not recover jumptable at 0x04375e20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}


