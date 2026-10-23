// ==== 0x42b85e8  FUN_042b85e8  (208 bytes)

void FUN_042b85e8(long param_1)

{
  code *pcVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  ulong uVar4;
  
  if (*(long *)(param_1 + 0x458) == 0) {
    if (DAT_0e495268 == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x42b86b0);
      (*pcVar1)();
    }
    uVar2 = FUN_029d4b50(DAT_0e495268,0xf);
    puVar3 = *(undefined8 **)(param_1 + 0x458);
    *(undefined8 *)(param_1 + 0x458) = uVar2;
    if (puVar3 != (undefined8 *)0x0) {
      *puVar3 = &PTR_LAB_0d8c3f10;
      uVar4 = puVar3[2];
      if (uVar4 == 0) {
        uVar4 = 0;
      }
      else {
        puVar3[2] = 0;
        if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0001fee4(uVar4);
        }
        uVar2 = puVar3[3];
        puVar3[3] = 0;
        FUN_02c167a8(uVar4,uVar2);
        uVar4 = puVar3[2];
      }
      if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4();
      }
      FUN_00005810(puVar3,0x20);
      return;
    }
  }
  return;
}


