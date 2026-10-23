// ==== 0x42b8f50  FUN_042b8f50  (144 bytes)

void FUN_042b8f50(undefined8 param_1,undefined8 param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  uVar1 = FUN_043210c8();
  puVar2 = (undefined4 *)FUN_00008790(0x48);
  uVar4 = *param_3;
  uVar6 = param_3[3];
  uVar5 = param_3[2];
  *(undefined8 *)(puVar2 + 0xc) = param_3[1];
  *(undefined8 *)(puVar2 + 10) = uVar4;
  *puVar2 = 1;
  *(undefined **)(puVar2 + 2) = &DAT_042bfcd4;
  *(code **)(puVar2 + 4) = FUN_042a34c0;
  *(code **)(puVar2 + 6) = FUN_00b77080;
  lVar3 = param_3[3];
  *(undefined8 *)(puVar2 + 0x10) = uVar6;
  *(undefined8 *)(puVar2 + 0xe) = uVar5;
  if (*(long *)(lVar3 + 8) != 0) {
    param_3[2] = 0;
    param_3[3] = &DAT_0c4b3500;
  }
  FUN_043216bc(uVar1,param_2,puVar2);
  return;
}


