// ==== 0x43332d0  FUN_043332d0  (168 bytes)

void FUN_043332d0(undefined8 param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)

{
  undefined8 uVar1;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long local_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *param_3;
  uStack_58 = param_4[1];
  local_60 = *param_4;
  local_48 = param_4[3];
  uStack_50 = param_4[2];
  if (*(code **)param_4[3] != (code *)0x0) {
    local_60 = (**(code **)param_4[3])(*param_4);
  }
  FUN_042ee818(param_1,param_2,uVar1,&local_60);
  if (*(code **)(local_48 + 8) != (code *)0x0) {
    (**(code **)(local_48 + 8))(local_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


