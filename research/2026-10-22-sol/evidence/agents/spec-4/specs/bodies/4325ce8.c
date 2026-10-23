// ==== 0x4325ce8  FUN_04325ce8  (188 bytes)

void FUN_04325ce8(undefined8 *param_1,long *param_2,ulong param_3)

{
  undefined8 uVar1;
  long *local_68;
  ulong uStack_60;
  code *local_58;
  undefined *puStack_50;
  code *local_38;
  undefined *puStack_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_60 = param_3 & 0xffffffff;
  local_58 = FUN_0432714c;
  puStack_50 = &DAT_0c9d6a40;
  local_38 = FUN_04327750;
  puStack_30 = &DAT_0c9d6a60;
  local_68 = param_2;
  if (*param_2 == 0) {
    uVar1 = FUN_04b37104(0x39,"This object is detached",0x17,0x95,
                         "../../arc/browser/local_state_impl.cc");
    *param_1 = uVar1;
  }
  else {
    FUN_0432714c(param_1,&local_68);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


