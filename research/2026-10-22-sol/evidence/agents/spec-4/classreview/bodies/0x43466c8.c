// ==== 0x43466c8  FUN_043466c8  (156 bytes)

undefined8 * FUN_043466c8(undefined8 *param_1,undefined8 *param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00054420(param_1,*param_2,param_2[1]);
  }
  else {
    uVar3 = param_2[1];
    uVar2 = *param_2;
    param_1[2] = param_2[2];
    param_1[1] = uVar3;
    *param_1 = uVar2;
  }
  uVar2 = param_2[3];
  param_1[4] = param_2[4];
  param_1[3] = uVar2;
  if (*(char *)((long)param_2 + 0x3f) < '\0') {
    FUN_00054420(param_1 + 5,param_2[5],param_2[6]);
  }
  else {
    uVar3 = param_2[6];
    uVar2 = param_2[5];
    param_1[7] = param_2[7];
    param_1[6] = uVar3;
    param_1[5] = uVar2;
  }
  param_1[8] = param_2[8];
  lVar1 = param_2[9];
  param_1[9] = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 0x10) = *(long *)(lVar1 + 0x10) + 1;
  }
  return param_1;
}


