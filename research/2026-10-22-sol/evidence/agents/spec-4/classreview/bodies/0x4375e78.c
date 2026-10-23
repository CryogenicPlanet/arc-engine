// ==== 0x4375e78  FUN_04375e78  (472 bytes)

void FUN_04375e78(long param_1,undefined8 param_2,long *param_3,undefined4 param_4)

{
  long lVar1;
  long *plVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  *(undefined4 *)(param_1 + 0x408) = param_4;
  if ((*(long *)(param_1 + 0x130) != 0) &&
     (lVar1 = FUN_000805e0(*(long *)(param_1 + 0x130) + 8,&DAT_0c9d710c), lVar1 != 0)) {
    *(undefined4 *)(lVar1 + 0x18) = param_4;
  }
  if (param_3 == (long *)0x0) {
    *(undefined8 *)(param_1 + 0x3d8) = param_2;
    param_3 = *(long **)(param_1 + 0x3e0);
    *(undefined8 *)(param_1 + 0x3e0) = 0;
    if (param_3 == (long *)0x0) {
      return;
    }
    goto LAB_04375f34;
  }
  plVar2 = param_3 + 2;
  *plVar2 = *plVar2 + 1;
  *plVar2 = *plVar2 + 1;
  *(undefined8 *)(param_1 + 0x3d8) = param_2;
  plVar2 = *(long **)(param_1 + 0x3e0);
  *(long **)(param_1 + 0x3e0) = param_3;
  if (plVar2 != (long *)0x0) {
    if (plVar2[2] != 0) {
      LOAcquire();
      lVar1 = plVar2[2];
      plVar2[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_04375f08;
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
LAB_04375f08:
  if (param_3[2] != 0) {
    LOAcquire();
    lVar1 = param_3[2];
    param_3[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) goto LAB_04375f34;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
     (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
    FUN_00005810(param_3,0x20);
  }
  else {
    (*UNRECOVERED_JUMPTABLE)(param_3);
  }
LAB_04375f34:
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
                    /* WARNING: Could not recover jumptable at 0x04375ff8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}


