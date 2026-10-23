// ==== 0x42b7db8  FUN_042b7db8  (132 bytes)

void FUN_042b7db8(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  long local_28;
  long local_18;
  
  local_18 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *(undefined8 *)(param_1 + 0x310);
  uStack_38 = param_2[1];
  local_40 = *param_2;
  local_28 = param_2[3];
  uStack_30 = param_2[2];
  if (*(long *)(param_2[3] + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  FUN_043282e0(uVar1,&local_40);
  if (*(code **)(local_28 + 8) != (code *)0x0) {
    (**(code **)(local_28 + 8))(local_40);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


