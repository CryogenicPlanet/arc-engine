// ==== 0x42b7974  FUN_042b7974  (820 bytes)

void FUN_042b7974(long param_1,undefined8 param_2,long *param_3)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  code *UNRECOVERED_JUMPTABLE;
  undefined1 local_50 [16];
  
  if (param_3 != (long *)0x0) {
    plVar3 = param_3 + 2;
    *plVar3 = *plVar3 + 1;
    *plVar3 = *plVar3 + 1;
  }
  plVar3 = *(long **)(param_1 + 0x70);
  *(undefined8 *)(param_1 + 0x68) = param_2;
  *(long **)(param_1 + 0x70) = param_3;
  if (plVar3 != (long *)0x0) {
    if (plVar3[2] != 0) {
      LOAcquire();
      lVar2 = plVar3[2];
      plVar3[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) goto LAB_042b79dc;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(plVar3,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)();
    }
  }
LAB_042b79dc:
  if (param_3 != (long *)0x0) {
    if (param_3[2] != 0) {
      LOAcquire();
      lVar2 = param_3[2];
      param_3[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) goto LAB_042b7a0c;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(param_3,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)(param_3);
    }
  }
LAB_042b7a0c:
  if (*(char *)(param_1 + 0x149) == '\0') goto LAB_042b7b50;
  local_50 = FUN_042b17e8();
  plVar3 = local_50._8_8_;
  if (plVar3 == (long *)0x0) {
    FUN_042b13c4(param_1 + 0x68,"arc::BrowserApplicationDelegate::DidInitializeBrowserProcess",0x28,
                 1,local_50);
    goto LAB_042b7b50;
  }
  plVar1 = plVar3 + 1;
  lVar2 = *plVar1;
  *plVar1 = lVar2 + 1;
  FUN_042b13c4(lVar2,param_1 + 0x68,"arc::BrowserApplicationDelegate::DidInitializeBrowserProcess",
               0x28,1,local_50);
  LOAcquire();
  lVar2 = *plVar1;
  *plVar1 = lVar2 + -1;
  LORelease();
  if (lVar2 == 0) {
    (**(code **)(*plVar3 + 0x10))(plVar3);
    if (plVar3[2] != 0) {
      LOAcquire();
      lVar2 = plVar3[2];
      plVar3[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) goto LAB_042b7a58;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(plVar3,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)(plVar3);
    }
  }
LAB_042b7a58:
  LOAcquire();
  lVar2 = *plVar1;
  *plVar1 = lVar2 + -1;
  LORelease();
  if (lVar2 == 0) {
    (**(code **)(*plVar3 + 0x10))(plVar3);
    if (plVar3[2] != 0) {
      LOAcquire();
      lVar2 = plVar3[2];
      plVar3[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) goto LAB_042b7b50;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(plVar3,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)(plVar3);
    }
  }
LAB_042b7b50:
  if (param_3 == (long *)0x0) {
    return;
  }
  if (param_3[2] != 0) {
    LOAcquire();
    lVar2 = param_3[2];
    param_3[2] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x042b7bf4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}


