// ==== 0x42dee8c  FUN_042dee8c  (556 bytes)

void FUN_042dee8c(long param_1,long param_2,long *param_3)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  code *UNRECOVERED_JUMPTABLE;
  ulong local_80;
  long *local_78 [2];
  code *local_68;
  undefined **ppuStack_60;
  code *local_48;
  undefined *puStack_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 != (long *)0x0) {
    param_3[2] = param_3[2] + 1;
  }
  local_68 = FUN_042e1378;
  ppuStack_60 = &PTR_FUN_0df6a900;
  plVar2 = (long *)FUN_00008790(0x18);
  *plVar2 = param_1;
  plVar2[1] = param_2;
  plVar2[2] = (long)param_3;
  local_48 = FUN_042e1750;
  puStack_40 = &DAT_0c9d6768;
  local_78[0] = plVar2;
  if (*(long *)(param_1 + 8) == 0) {
    local_80 = FUN_04b37104(0x39,"This object is detached",0x17,0x3b,
                            "../../arc/browser/browser_context/downloads_impl.cc");
  }
  else {
    FUN_042e1378(&local_80,local_78);
  }
  if ((local_80 & 1) == 0) {
    FUN_04b35898();
  }
  if (plVar2 != (long *)0x0) {
    plVar3 = (long *)plVar2[2];
    if (plVar3 != (long *)0x0) {
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_042def60;
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
LAB_042def60:
    FUN_00005810(plVar2,0x18);
  }
  if (param_3 == (long *)0x0) {
LAB_042defe8:
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
      return;
    }
  }
  else {
    if (param_3[2] != 0) {
      LOAcquire();
      lVar1 = param_3[2];
      param_3[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_042defe8;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
        FUN_00005810(param_3,0x20);
        return;
      }
    }
    else if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
                    /* WARNING: Could not recover jumptable at 0x042df0b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(param_3);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


