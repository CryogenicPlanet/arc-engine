// ==== 0x432631c  FUN_0432631c  (184 bytes)

void FUN_0432631c(undefined8 *param_1,long *param_2,undefined4 param_3)

{
  undefined8 uVar1;
  undefined4 local_68 [4];
  code *local_58;
  undefined *puStack_50;
  code *local_38;
  undefined *puStack_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  local_58 = FUN_04327f3c;
  puStack_50 = &DAT_0c9d6bc0;
  local_38 = FUN_0432801c;
  puStack_30 = &DAT_0c9d6be0;
  local_68[0] = param_3;
  if (*param_2 == 0) {
    uVar1 = FUN_04b37104(0x39,"This object is detached",0x17,0xe9,
                         "../../arc/browser/local_state_impl.cc");
    *param_1 = uVar1;
  }
  else {
    FUN_04327f3c(param_1,local_68);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


