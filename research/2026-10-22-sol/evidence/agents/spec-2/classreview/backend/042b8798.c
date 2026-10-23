// ==== 0x42b8798  FUN_042b8798  (304 bytes)

void FUN_042b8798(long param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  ulong uVar3;
  
  puVar2 = *(undefined8 **)(param_1 + 0x458);
  *(undefined8 *)(param_1 + 0x458) = 0;
  if (puVar2 != (undefined8 *)0x0) {
    *puVar2 = &PTR_LAB_0d8c3f10;
    uVar3 = puVar2[2];
    if (uVar3 == 0) {
      uVar3 = 0;
    }
    else {
      puVar2[2] = 0;
      if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(uVar3);
      }
      uVar1 = puVar2[3];
      puVar2[3] = 0;
      FUN_02c167a8(uVar3,uVar1);
      uVar3 = puVar2[2];
    }
    if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    FUN_00005810(puVar2,0x20);
  }
  puVar2 = *(undefined8 **)(param_1 + 0x450);
  *(undefined8 *)(param_1 + 0x450) = 0;
  if (puVar2 != (undefined8 *)0x0) {
    *puVar2 = &PTR_LAB_0d8c3f10;
    uVar3 = puVar2[2];
    if (uVar3 == 0) {
      uVar3 = 0;
    }
    else {
      puVar2[2] = 0;
      if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(uVar3);
      }
      uVar1 = puVar2[3];
      puVar2[3] = 0;
      FUN_02c167a8(uVar3,uVar1);
      uVar3 = puVar2[2];
    }
    if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    FUN_00005810(puVar2,0x20);
    return;
  }
  return;
}


