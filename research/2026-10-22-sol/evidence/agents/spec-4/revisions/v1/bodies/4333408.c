// ==== 0x4333408  FUN_04333408  (192 bytes)

void FUN_04333408(undefined8 param_1,undefined8 param_2,undefined8 *param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 *param_6)

{
  undefined8 uVar1;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  long local_58;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *param_3;
  uStack_68 = param_6[1];
  local_70 = *param_6;
  local_58 = param_6[3];
  uStack_60 = param_6[2];
  if (*(code **)param_6[3] != (code *)0x0) {
    local_70 = (**(code **)param_6[3])(*param_6);
  }
  FUN_042eeb5c(param_1,param_2,uVar1,param_4,param_5,&local_70);
  if (*(code **)(local_58 + 8) != (code *)0x0) {
    (**(code **)(local_58 + 8))(local_70);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


