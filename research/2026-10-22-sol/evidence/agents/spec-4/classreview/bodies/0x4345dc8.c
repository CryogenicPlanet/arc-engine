// ==== 0x4345dc8  FUN_04345dc8  (184 bytes)

void FUN_04345dc8(undefined8 *param_1,undefined8 param_2,long *param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  code *UNRECOVERED_JUMPTABLE;
  
  uVar3 = *param_1;
  if (param_3 == (long *)0x0) {
    FUN_043761ec(uVar3,param_2,0);
    return;
  }
  plVar1 = param_3 + 2;
  lVar2 = *plVar1;
  *plVar1 = lVar2 + 1;
  FUN_043761ec(lVar2,uVar3,param_2,param_3);
  if (param_3[2] != 0) {
    LOAcquire();
    lVar2 = *plVar1;
    *plVar1 = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x04345e7c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}


