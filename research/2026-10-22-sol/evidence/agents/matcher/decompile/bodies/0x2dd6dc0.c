// ==== 0x2dd6dc0  FUN_02dd6dc0  (396 bytes)

void FUN_02dd6dc0(long param_1,undefined8 param_2,int param_3)

{
  int iVar1;
  bool bVar2;
  bool bVar3;
  uint uVar4;
  undefined8 uVar5;
  byte bVar6;
  undefined1 auStack_170 [16];
  undefined1 auStack_160 [288];
  
  iVar1 = *(int *)(param_1 + 0x50);
  bVar6 = iVar1 < 1;
  bVar2 = iVar1 == *(int *)(param_1 + 0x54);
  if (bVar2 && !(bool)bVar6) {
    bVar6 = *(byte *)(param_1 + 0x59);
  }
  *(int *)(param_1 + 0x50) = iVar1 + -1;
  FUN_039d4edc(param_2,param_1);
  if (param_3 == 1) {
    *(int *)(param_1 + 0x54) = *(int *)(param_1 + 0x54) + -1;
    FUN_039d4edc(param_2,param_1 + 0x28);
  }
  iVar1 = *(int *)(param_1 + 0x50);
  bVar3 = iVar1 == *(int *)(param_1 + 0x54);
  if (iVar1 < 1) {
    if ((bVar6 & 1) == 0) {
      bVar6 = 0;
      goto LAB_02dd6e98;
    }
LAB_02dd6ea0:
    if (bVar2 == bVar3) goto LAB_02dd6eb0;
  }
  else if (iVar1 == *(int *)(param_1 + 0x54)) {
    if ((bVar6 & 1) != *(byte *)(param_1 + 0x59)) {
      bVar6 = *(byte *)(param_1 + 0x59) ^ 1;
      bVar3 = true;
      goto LAB_02dd6e98;
    }
    if (bVar2) goto LAB_02dd6eb0;
  }
  else {
    if ((bVar6 & 1) != 0) {
      bVar3 = false;
      bVar6 = 1;
LAB_02dd6e98:
      FUN_02dc4194(param_1,bVar6);
      goto LAB_02dd6ea0;
    }
    if (!bVar2) goto LAB_02dd6eb0;
  }
  FUN_039d4e00(param_1);
LAB_02dd6eb0:
  if (DAT_0e4a3090 == 0) {
    uVar4 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
  }
  else {
    uVar4 = FUN_083a6f28(DAT_0e4a3090,"../../components/keep_alive_registry/keep_alive_registry.cc",
                         0x3b);
  }
  if (3 < (int)uVar4) {
    FUN_00f195d8(auStack_170,"../../components/keep_alive_registry/keep_alive_registry.cc",0xc3,
                 0xfffffffc);
    uVar5 = FUN_00040060(auStack_160,"New state of the KeepAliveRegistry: ",0x24);
    FUN_02fd12b4(uVar5,param_1);
    FUN_00f1950c(auStack_170);
  }
  FUN_02fd0ef0(param_1);
  return;
}


