// ==== 0x4345f38  FUN_04345f38  (184 bytes)

void FUN_04345f38(undefined8 param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_70 = *param_3;
  uVar3 = NEON_sqadd(local_70,param_3[1],4);
  uVar3 = NEON_sqsub(uVar3,local_70,4);
  uStack_68 = NEON_smax(uVar3,0,4);
  uVar3 = *param_2;
  puVar2 = (undefined8 *)param_4[3];
  uStack_58 = param_4[1];
  local_60 = *param_4;
  uStack_48 = param_4[3];
  uStack_50 = param_4[2];
  pcVar1 = (code *)*puVar2;
  if (pcVar1 != (code *)0x0) {
    local_60 = (*pcVar1)(*param_4);
  }
  FUN_04376da0(param_1,uVar3,&local_70,&local_60);
  pcVar1 = (code *)puVar2[1];
  if (pcVar1 != (code *)0x0) {
    (*pcVar1)(local_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


