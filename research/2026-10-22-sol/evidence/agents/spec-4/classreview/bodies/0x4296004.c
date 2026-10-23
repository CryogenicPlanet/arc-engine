// ==== 0x4296004  FUN_04296004  (136 bytes)

undefined8 * FUN_04296004(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(param_1,*param_2,param_2[1]);
  }
  else {
    uVar2 = param_2[1];
    uVar1 = *param_2;
    param_1[2] = param_2[2];
    param_1[1] = uVar2;
    *param_1 = uVar1;
  }
  if (*(char *)((long)param_2 + 0x2f) < '\0') {
    FUN_00060a60(param_1 + 3,param_2[3],param_2[4]);
  }
  else {
    uVar2 = param_2[4];
    uVar1 = param_2[3];
    param_1[5] = param_2[5];
    param_1[4] = uVar2;
    param_1[3] = uVar1;
  }
  param_1[6] = param_2[6];
  FUN_0429539c(param_1 + 7,param_2 + 7);
  return param_1;
}


