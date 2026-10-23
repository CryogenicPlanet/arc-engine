// ==== 0x42c35e4  FUN_042c35e4  (348 bytes)

void FUN_042c35e4(long param_1,undefined8 *param_2,int param_3,undefined1 param_4,
                 undefined8 *param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined **local_4e8;
  ulong local_4e0;
  undefined8 local_4d8 [2];
  undefined7 local_4c8;
  char cStack_4c1;
  undefined1 auStack_4c0 [432];
  undefined1 local_310;
  undefined4 local_30c;
  
  if (*(long *)(param_1 + 0x70) != 0) {
    lVar1 = *(long *)(param_1 + 0x50);
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 **)(param_1 + 0x50) = &DAT_0c4b3500;
    pcVar2 = *(code **)(lVar1 + 8);
    if (pcVar2 != (code *)0x0) {
      (*pcVar2)(*(undefined8 *)(param_1 + 0x38));
    }
    uVar4 = *param_5;
    *(undefined8 *)(param_1 + 0x40) = param_5[1];
    *(undefined8 *)(param_1 + 0x38) = uVar4;
    *(undefined8 *)(param_1 + 0x48) = param_5[2];
    *(undefined8 *)(param_1 + 0x50) = param_5[3];
    param_5[2] = 0;
    param_5[3] = &DAT_0c4b3500;
    _bzero(auStack_4c0,0x470);
    lVar1 = (long)*(char *)((long)param_2 + 0x17);
    puVar3 = param_2;
    if (lVar1 < 0) {
      puVar3 = (undefined8 *)*param_2;
      lVar1 = param_2[1];
    }
    FUN_007cf8e0(local_4d8,puVar3,lVar1);
    local_4e0 = *(ulong *)(param_1 + 0x70);
    local_4e8 = &PTR_FUN_0da51808;
    if ((local_4e0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(local_4e0);
    }
    FUN_01c8f2bc(auStack_4c0,local_4d8,(long)param_3,4,&local_4e8,0,0,0);
    local_4e8 = &PTR_FUN_0da51808;
    if ((local_4e0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    if (cStack_4c1 < '\0') {
      FUN_00005810(local_4d8[0],CONCAT17(cStack_4c1,local_4c8) << 1);
    }
    local_30c = 0;
    local_310 = param_4;
    FUN_020cd850(*(undefined8 *)(param_1 + 0x30),auStack_4c0);
    FUN_01cd230c(auStack_4c0);
  }
  return;
}


