// ==== 0x42b1200  FUN_042b1200  (452 bytes)

void FUN_042b1200(long param_1)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  code *UNRECOVERED_JUMPTABLE;
  undefined1 local_40 [16];
  
  if ((*(byte *)(param_1 + 0x149) & 1) != 0) {
    return;
  }
  *(undefined1 *)(param_1 + 0x149) = 1;
  local_40 = FUN_042b17e8();
  plVar3 = local_40._8_8_;
  if (plVar3 == (long *)0x0) {
    FUN_042b13c4(param_1 + 0x68,"arc::BrowserApplicationDelegate::DidInitializeBrowserProcess",0x28,
                 1,local_40);
    return;
  }
  plVar1 = plVar3 + 1;
  lVar2 = *plVar1;
  *plVar1 = lVar2 + 1;
  FUN_042b13c4(lVar2,param_1 + 0x68,"arc::BrowserApplicationDelegate::DidInitializeBrowserProcess",
               0x28,1,local_40);
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
      if (lVar2 != 0) goto LAB_042b126c;
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
LAB_042b126c:
  LOAcquire();
  lVar2 = *plVar1;
  *plVar1 = lVar2 + -1;
  LORelease();
  if (lVar2 != 0) {
    return;
  }
  (**(code **)(*plVar3 + 0x10))(plVar3);
  if (plVar3[2] != 0) {
    LOAcquire();
    lVar2 = plVar3[2];
    plVar3[2] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x042b13c0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(plVar3);
    return;
  }
  FUN_00005810(plVar3,0x20);
  return;
}


