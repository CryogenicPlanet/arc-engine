// ==== 0x4325fcc  FUN_04325fcc  (184 bytes)

void FUN_04325fcc(undefined8 *param_1,long *param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined *puStack_50;
  code *local_38;
  undefined *puStack_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  local_58 = FUN_04327a38;
  puStack_50 = &DAT_0c9d6b40;
  local_38 = FUN_0836d96c;
  puStack_30 = &DAT_0c9d6b60;
  local_68 = param_3;
  uStack_60 = param_4;
  if (*param_2 == 0) {
    uVar1 = FUN_04b37104(0x39,"This object is detached",0x17,0xc0,
                         "../../arc/browser/local_state_impl.cc");
    *param_1 = uVar1;
  }
  else {
    FUN_04327a38(param_1,&local_68);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


