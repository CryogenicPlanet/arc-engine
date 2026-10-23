// ==== 0x4335ae8  FUN_04335ae8  (244 bytes)

undefined8 * FUN_04335ae8(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 uVar5;
  
  uVar2 = *param_2;
  uVar1 = param_2[1];
  uVar5 = param_2[3];
  uVar3 = param_2[2];
  if (*(code **)param_2[3] != (code *)0x0) {
    uVar2 = (**(code **)param_2[3])();
  }
  lVar4 = param_1[3];
  param_1[3] = uVar5;
  param_1[2] = uVar3;
  uVar3 = *param_1;
  *param_1 = uVar2;
  param_1[1] = uVar1;
  if (*(code **)(lVar4 + 8) != (code *)0x0) {
    (**(code **)(lVar4 + 8))(uVar3);
  }
  uVar2 = param_2[4];
  uVar1 = param_2[5];
  uVar5 = param_2[7];
  uVar3 = param_2[6];
  if (*(code **)param_2[7] != (code *)0x0) {
    uVar2 = (**(code **)param_2[7])();
  }
  lVar4 = param_1[7];
  param_1[7] = uVar5;
  param_1[6] = uVar3;
  uVar3 = param_1[4];
  param_1[4] = uVar2;
  param_1[5] = uVar1;
  if (*(code **)(lVar4 + 8) != (code *)0x0) {
    (**(code **)(lVar4 + 8))(uVar3);
  }
  uVar2 = param_2[8];
  uVar1 = param_2[9];
  uVar5 = param_2[0xb];
  uVar3 = param_2[10];
  if (*(code **)param_2[0xb] != (code *)0x0) {
    uVar2 = (**(code **)param_2[0xb])();
  }
  lVar4 = param_1[0xb];
  param_1[0xb] = uVar5;
  param_1[10] = uVar3;
  uVar3 = param_1[8];
  param_1[8] = uVar2;
  param_1[9] = uVar1;
  if (*(code **)(lVar4 + 8) != (code *)0x0) {
    (**(code **)(lVar4 + 8))(uVar3);
  }
  return param_1;
}


