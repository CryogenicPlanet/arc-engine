// ==== 0x4332dc0  FUN_04332dc0  (176 bytes)

void FUN_04332dc0(undefined8 param_1,undefined8 param_2,undefined8 *param_3,undefined8 param_4,
                 undefined8 *param_5)

{
  undefined8 uVar1;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long local_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *param_3;
  uStack_58 = param_5[1];
  local_60 = *param_5;
  local_48 = param_5[3];
  uStack_50 = param_5[2];
  if (*(code **)param_5[3] != (code *)0x0) {
    local_60 = (**(code **)param_5[3])(*param_5);
  }
  FUN_042eda00(param_1,param_2,uVar1,param_4,&local_60);
  if (*(code **)(local_48 + 8) != (code *)0x0) {
    (**(code **)(local_48 + 8))(local_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


