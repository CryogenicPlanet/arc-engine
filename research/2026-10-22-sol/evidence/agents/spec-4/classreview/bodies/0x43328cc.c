// ==== 0x43328cc  FUN_043328cc  (188 bytes)

void FUN_043328cc(undefined8 *param_1,long *param_2,undefined8 param_3)

{
  undefined8 uVar1;
  long local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined *puStack_50;
  code *local_38;
  undefined *puStack_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  local_68 = *param_2;
  local_58 = FUN_0836ba70;
  puStack_50 = &DAT_0c9d6788;
  local_38 = FUN_042e1750;
  puStack_30 = &DAT_0c9d6768;
  uStack_60 = param_3;
  if (*(long *)(local_68 + 8) == 0) {
    uVar1 = FUN_04b37104(0x39,"This object is detached",0x17,0x3b,
                         "../../arc/browser/browser_context/downloads_impl.cc");
    *param_1 = uVar1;
  }
  else {
    FUN_0836ba70(param_1,&local_68);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


