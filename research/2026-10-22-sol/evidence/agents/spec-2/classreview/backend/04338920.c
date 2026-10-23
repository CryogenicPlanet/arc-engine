// ==== 0x4338920  FUN_04338920  (300 bytes)

void FUN_04338920(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 *local_98 [2];
  code *local_88;
  undefined **local_80;
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = *param_4;
  uVar2 = param_4[1];
  puVar3 = (undefined8 *)param_4[3];
  uVar5 = param_4[3];
  uVar4 = param_4[2];
  if (puVar3[1] != 0) {
    param_4[2] = 0;
    param_4[3] = &DAT_0c4b3500;
  }
  local_88 = FUN_0433ab74;
  local_80 = &PTR_FUN_0df6af40;
  local_98[0] = (undefined8 *)FUN_00008790(0x20);
  *local_98[0] = uVar1;
  local_98[0][1] = uVar2;
  local_98[0][3] = uVar5;
  local_98[0][2] = uVar4;
  if (puVar3[1] != 0) {
    puVar3 = &DAT_0c4b3500;
  }
  FUN_04337e00(param_1,param_2,param_3,local_98,param_5,param_6,param_7);
  if ((code *)local_80[1] != (code *)0x0) {
    (*(code *)local_80[1])(local_98[0]);
  }
  if ((code *)puVar3[1] != (code *)0x0) {
    (*(code *)puVar3[1])(uVar1);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


