// ==== 0x43310bc  FUN_043310bc  (328 bytes)

undefined8
FUN_043310bc(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined4 *param_4,
            undefined8 *param_5,undefined8 *param_6)

{
  undefined8 uVar1;
  undefined4 local_b0 [2];
  undefined8 local_a8;
  undefined8 uStack_a0;
  ulong local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  long local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  long local_58;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *param_1;
  local_b0[0] = *param_4;
  if (*(char *)((long)param_4 + 0x1f) < '\0') {
    FUN_00060a60(&local_a8,*(undefined8 *)(param_4 + 2),*(undefined8 *)(param_4 + 4));
  }
  else {
    uStack_a0 = *(undefined8 *)(param_4 + 4);
    local_a8 = *(undefined8 *)(param_4 + 2);
    local_98 = *(ulong *)(param_4 + 6);
  }
  uStack_68 = param_5[1];
  local_70 = *param_5;
  local_58 = param_5[3];
  uStack_60 = param_5[2];
  if (*(code **)param_5[3] != (code *)0x0) {
    local_70 = (**(code **)param_5[3])(*param_5);
  }
  uStack_88 = param_6[1];
  local_90 = *param_6;
  local_78 = param_6[3];
  uStack_80 = param_6[2];
  if (*(code **)param_6[3] != (code *)0x0) {
    local_90 = (**(code **)param_6[3])(*param_6);
  }
  uVar1 = FUN_0429fa38(uVar1,param_2,param_3,local_b0,&local_70,&local_90);
  if (*(code **)(local_78 + 8) != (code *)0x0) {
    (**(code **)(local_78 + 8))(local_90);
  }
  if (*(code **)(local_58 + 8) != (code *)0x0) {
    (**(code **)(local_58 + 8))(local_70);
  }
  if ((long)local_98 < 0) {
    FUN_00005810(local_a8,local_98 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


