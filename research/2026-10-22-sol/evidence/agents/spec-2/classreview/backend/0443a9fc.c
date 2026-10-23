// ==== 0x443a9fc  FUN_0443a9fc  (204 bytes)

undefined8 * FUN_0443a9fc(undefined8 *param_1,undefined8 *param_2)

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
  if (*(char *)((long)param_2 + 0x47) < '\0') {
    FUN_00060a60(param_1 + 6,param_2[6],param_2[7]);
  }
  else {
    uVar2 = param_2[7];
    uVar1 = param_2[6];
    param_1[8] = param_2[8];
    param_1[7] = uVar2;
    param_1[6] = uVar1;
  }
  if (*(char *)((long)param_2 + 0x5f) < '\0') {
    FUN_00060a60(param_1 + 9,param_2[9],param_2[10]);
  }
  else {
    uVar2 = param_2[10];
    uVar1 = param_2[9];
    param_1[0xb] = param_2[0xb];
    param_1[10] = uVar2;
    param_1[9] = uVar1;
  }
  *(undefined2 *)(param_1 + 0xc) = *(undefined2 *)(param_2 + 0xc);
  return param_1;
}


