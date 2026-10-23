// ==== 0x433410c  FUN_0433410c  (216 bytes)

void FUN_0433410c(long *param_1)

{
  long *plVar1;
  undefined8 uVar2;
  long *plVar3;
  long lVar4;
  code *UNRECOVERED_JUMPTABLE;
  
  uVar2 = *(undefined8 *)(*param_1 + 0x1e0);
  plVar3 = *(long **)(*param_1 + 0x1e8);
  if (plVar3 == (long *)0x0) {
    FUN_042c8400(uVar2);
    return;
  }
  plVar1 = plVar3 + 1;
  *plVar1 = *plVar1 + 1;
  FUN_042c8400(uVar2);
  LOAcquire();
  lVar4 = *plVar1;
  *plVar1 = lVar4 + -1;
  LORelease();
  if (lVar4 != 0) {
    return;
  }
  (**(code **)(*plVar3 + 0x10))(plVar3);
  if (plVar3[2] != 0) {
    LOAcquire();
    lVar4 = plVar3[2];
    plVar3[2] = lVar4 + -1;
    LORelease();
    if (lVar4 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043341e0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(plVar3);
    return;
  }
  FUN_00005810(plVar3,0x20);
  return;
}


