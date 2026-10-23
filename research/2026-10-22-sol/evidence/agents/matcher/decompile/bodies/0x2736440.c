// ==== 0x2736440  FUN_02736440  (492 bytes)

void FUN_02736440(long param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  code *pcVar2;
  bool bVar3;
  bool bVar4;
  uint uVar5;
  long lVar6;
  undefined8 uVar7;
  byte bVar8;
  int iVar9;
  long local_180 [2];
  undefined1 auStack_170 [292];
  undefined4 local_4c;
  undefined4 *local_48;
  
  local_4c = param_2;
  if (*(char *)(param_1 + 0x58) != '\0') {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x2736614);
    (*pcVar2)();
  }
  bVar8 = *(int *)(param_1 + 0x50) < 1;
  bVar3 = *(int *)(param_1 + 0x50) == *(int *)(param_1 + 0x54);
  if (bVar3 && !(bool)bVar8) {
    bVar8 = *(byte *)(param_1 + 0x59);
  }
  local_48 = &local_4c;
  local_180[0] = param_1;
  lVar6 = FUN_039d52c0(local_180,&local_4c,&local_48);
  if (lVar6 == 0) {
LAB_02736618:
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x273661c);
    (*pcVar2)();
  }
  *(int *)(lVar6 + 0x14) = *(int *)(lVar6 + 0x14) + 1;
  iVar1 = *(int *)(param_1 + 0x50) + 1;
  *(int *)(param_1 + 0x50) = iVar1;
  if (param_3 == 1) {
    local_180[0] = param_1 + 0x28;
    local_48 = &local_4c;
    lVar6 = FUN_039d52c0(local_180,&local_4c,&local_48);
    if (lVar6 == 0) goto LAB_02736618;
    *(int *)(lVar6 + 0x14) = *(int *)(lVar6 + 0x14) + 1;
    iVar1 = *(int *)(param_1 + 0x50);
    iVar9 = *(int *)(param_1 + 0x54) + 1;
    *(int *)(param_1 + 0x54) = iVar9;
  }
  else {
    iVar9 = *(int *)(param_1 + 0x54);
  }
  bVar4 = iVar1 == iVar9;
  if (iVar1 < 1) {
    if ((bVar8 & 1) == 0) {
      bVar8 = 0;
      goto LAB_02736570;
    }
LAB_02736578:
    if (bVar3 == bVar4) goto LAB_02736588;
  }
  else if (iVar1 == iVar9) {
    if ((bVar8 & 1) != *(byte *)(param_1 + 0x59)) {
      bVar8 = *(byte *)(param_1 + 0x59) ^ 1;
      bVar4 = true;
      goto LAB_02736570;
    }
    if (bVar3) goto LAB_02736588;
  }
  else {
    if ((bVar8 & 1) != 0) {
      bVar4 = false;
      bVar8 = 1;
LAB_02736570:
      FUN_02dc4194(param_1,bVar8);
      goto LAB_02736578;
    }
    if (!bVar3) goto LAB_02736588;
  }
  FUN_039d4e00(param_1);
LAB_02736588:
  if (DAT_0e4a3090 == 0) {
    uVar5 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
  }
  else {
    uVar5 = FUN_083a6f28(DAT_0e4a3090,"../../components/keep_alive_registry/keep_alive_registry.cc",
                         0x3b);
  }
  if (3 < (int)uVar5) {
    FUN_00f195d8(local_180,"../../components/keep_alive_registry/keep_alive_registry.cc",0xa2,
                 0xfffffffc);
    uVar7 = FUN_00040060(auStack_170,"New state of the KeepAliveRegistry: ",0x24);
    FUN_02fd12b4(uVar7,param_1);
    FUN_00f1950c(local_180);
  }
  FUN_02fd0ef0(param_1);
  return;
}


