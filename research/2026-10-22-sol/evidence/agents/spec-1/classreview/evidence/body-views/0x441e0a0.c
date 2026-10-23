// ==== 0x441e0a0  FUN_0441e0a0  (216 bytes)

undefined8 * FUN_0441e0a0(undefined8 *param_1,undefined8 *param_2)

{
  ulong uVar1;
  byte bVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  if (param_1 != param_2) {
    bVar2 = *(byte *)((long)param_2 + 0x17);
    if (*(char *)((long)param_1 + 0x17) < '\0') {
      uVar1 = param_2[1];
      puVar3 = (undefined8 *)*param_2;
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar3 = param_2;
      }
      FUN_00359ad8(param_1,puVar3,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1,*param_2,param_2[1]);
    }
    else {
      uVar5 = param_2[1];
      uVar4 = *param_2;
      param_1[2] = param_2[2];
      param_1[1] = uVar5;
      *param_1 = uVar4;
    }
    bVar2 = *(byte *)((long)param_2 + 0x2f);
    if (*(char *)((long)param_1 + 0x2f) < '\0') {
      uVar1 = param_2[4];
      puVar3 = (undefined8 *)param_2[3];
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar3 = param_2 + 3;
      }
      FUN_00359ad8(param_1 + 3,puVar3,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1 + 3,param_2[3],param_2[4]);
    }
    else {
      uVar5 = param_2[4];
      uVar4 = param_2[3];
      param_1[5] = param_2[5];
      param_1[4] = uVar5;
      param_1[3] = uVar4;
    }
  }
  *(undefined4 *)(param_1 + 6) = *(undefined4 *)(param_2 + 6);
  return param_1;
}


