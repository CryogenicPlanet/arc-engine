// ==== 0x42a3ea8  FUN_042a3ea8  (332 bytes)

undefined8 * FUN_042a3ea8(undefined8 *param_1,undefined8 *param_2)

{
  void *pvVar1;
  void *pvVar2;
  ulong uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(param_1,*param_2,param_2[1]);
  }
  else {
    uVar5 = param_2[1];
    uVar4 = *param_2;
    param_1[2] = param_2[2];
    param_1[1] = uVar5;
    *param_1 = uVar4;
  }
  if (*(char *)((long)param_2 + 0x2f) < '\0') {
    FUN_00060a60(param_1 + 3,param_2[3],param_2[4]);
  }
  else {
    uVar5 = param_2[4];
    uVar4 = param_2[3];
    param_1[5] = param_2[5];
    param_1[4] = uVar5;
    param_1[3] = uVar4;
  }
  if (*(char *)((long)param_2 + 0x47) < '\0') {
    FUN_00060a60(param_1 + 6,param_2[6],param_2[7]);
  }
  else {
    uVar5 = param_2[7];
    uVar4 = param_2[6];
    param_1[8] = param_2[8];
    param_1[7] = uVar5;
    param_1[6] = uVar4;
  }
  if (*(char *)((long)param_2 + 0x5f) < '\0') {
    FUN_00060a60(param_1 + 9,param_2[9],param_2[10]);
  }
  else {
    uVar5 = param_2[10];
    uVar4 = param_2[9];
    param_1[0xb] = param_2[0xb];
    param_1[10] = uVar5;
    param_1[9] = uVar4;
  }
  if (*(char *)((long)param_2 + 0x77) < '\0') {
    FUN_00060a60(param_1 + 0xc,param_2[0xc],param_2[0xd]);
  }
  else {
    uVar5 = param_2[0xd];
    uVar4 = param_2[0xc];
    param_1[0xe] = param_2[0xe];
    param_1[0xd] = uVar5;
    param_1[0xc] = uVar4;
  }
  uVar5 = param_2[0x10];
  uVar4 = param_2[0xf];
  *(undefined4 *)((long)param_1 + 0x87) = *(undefined4 *)((long)param_2 + 0x87);
  param_1[0x10] = uVar5;
  param_1[0xf] = uVar4;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  param_1[0x12] = 0;
  uVar3 = param_2[0x13];
  if (uVar3 != 0) {
    if (uVar3 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    pvVar2 = (void *)param_2[0x12];
    pvVar1 = (void *)FUN_00008790(uVar3 << 2);
    param_1[0x12] = pvVar1;
    param_1[0x13] = 0;
    param_1[0x14] = uVar3;
    _memcpy(pvVar1,pvVar2,uVar3 << 2);
    param_1[0x13] = (long)(uVar3 << 2) >> 2;
  }
  return param_1;
}


