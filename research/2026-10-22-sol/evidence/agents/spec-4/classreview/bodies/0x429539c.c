// ==== 0x429539c  FUN_0429539c  (716 bytes)

undefined4 * FUN_0429539c(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  code *pcVar2;
  long lVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  undefined8 uVar10;
  
  *param_1 = *param_2;
  *(long *)(param_1 + 2) = 0;
  *(undefined8 *)(param_1 + 4) = 0;
  *(undefined8 *)(param_1 + 6) = 0;
  uVar9 = *(ulong *)(param_2 + 4);
  if (uVar9 != 0) {
    if (0x9d89d89d89d89d < uVar9) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    lVar7 = *(long *)(param_2 + 2);
    lVar8 = uVar9 * 0x1a0;
    lVar3 = FUN_00008790(lVar8);
    *(long *)(param_1 + 2) = lVar3;
    *(undefined8 *)(param_1 + 4) = 0;
    *(ulong *)(param_1 + 6) = uVar9;
    lVar5 = lVar3;
    if (lVar8 != 0) {
      do {
        if (lVar3 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x4295664);
          (*pcVar2)();
        }
        lVar3 = FUN_0429539c(lVar3,lVar7);
        lVar7 = lVar7 + 0x1a0;
        lVar3 = lVar3 + 0x1a0;
        lVar8 = lVar8 + -0x1a0;
      } while (lVar8 != 0);
      lVar5 = *(long *)(param_1 + 2);
    }
    *(long *)(param_1 + 4) = (lVar3 - lVar5 >> 5) * 0x4ec4ec4ec4ec4ec5;
  }
  puVar4 = (undefined8 *)(param_1 + 8);
  *(undefined1 *)puVar4 = 0;
  *(undefined1 *)(param_1 + 0xe) = 0;
  if (*(char *)(param_2 + 0xe) != '\0') {
    if (*(char *)((long)param_2 + 0x37) < '\0') {
      FUN_00060a60(puVar4,*(undefined8 *)(param_2 + 8),*(undefined8 *)(param_2 + 10));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 10);
      uVar6 = *(undefined8 *)(param_2 + 8);
      *(undefined8 *)(param_1 + 0xc) = *(undefined8 *)(param_2 + 0xc);
      *(undefined8 *)(param_1 + 10) = uVar10;
      *puVar4 = uVar6;
    }
    *(undefined1 *)(param_1 + 0xe) = 1;
  }
  uVar6 = *(undefined8 *)(param_2 + 0x10);
  uVar1 = param_2[0x12];
  puVar4 = (undefined8 *)(param_1 + 0x14);
  *(undefined1 *)puVar4 = 0;
  param_1[0x12] = uVar1;
  *(undefined8 *)(param_1 + 0x10) = uVar6;
  *(undefined1 *)(param_1 + 0x1a) = 0;
  if (*(char *)(param_2 + 0x1a) != '\0') {
    if (*(char *)((long)param_2 + 0x67) < '\0') {
      FUN_00060a60(puVar4,*(undefined8 *)(param_2 + 0x14),*(undefined8 *)(param_2 + 0x16));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x16);
      uVar6 = *(undefined8 *)(param_2 + 0x14);
      *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(param_2 + 0x18);
      *(undefined8 *)(param_1 + 0x16) = uVar10;
      *puVar4 = uVar6;
    }
    *(undefined1 *)(param_1 + 0x1a) = 1;
  }
  puVar4 = (undefined8 *)(param_1 + 0x1c);
  *(undefined1 *)puVar4 = 0;
  *(undefined1 *)(param_1 + 0x22) = 0;
  if (*(char *)(param_2 + 0x22) != '\0') {
    if (*(char *)((long)param_2 + 0x87) < '\0') {
      FUN_00060a60(puVar4,*(undefined8 *)(param_2 + 0x1c),*(undefined8 *)(param_2 + 0x1e));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x1e);
      uVar6 = *(undefined8 *)(param_2 + 0x1c);
      *(undefined8 *)(param_1 + 0x20) = *(undefined8 *)(param_2 + 0x20);
      *(undefined8 *)(param_1 + 0x1e) = uVar10;
      *puVar4 = uVar6;
    }
    *(undefined1 *)(param_1 + 0x22) = 1;
  }
  puVar4 = (undefined8 *)(param_1 + 0x24);
  *(undefined1 *)puVar4 = 0;
  *(undefined1 *)(param_1 + 0x2a) = 0;
  if (*(char *)(param_2 + 0x2a) != '\0') {
    if (*(char *)((long)param_2 + 0xa7) < '\0') {
      FUN_00060a60(puVar4,*(undefined8 *)(param_2 + 0x24),*(undefined8 *)(param_2 + 0x26));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x26);
      uVar6 = *(undefined8 *)(param_2 + 0x24);
      *(undefined8 *)(param_1 + 0x28) = *(undefined8 *)(param_2 + 0x28);
      *(undefined8 *)(param_1 + 0x26) = uVar10;
      *puVar4 = uVar6;
    }
    *(undefined1 *)(param_1 + 0x2a) = 1;
  }
  *(undefined1 *)(param_1 + 0x2c) = 0;
  *(undefined1 *)(param_1 + 0x4e) = 0;
  if (*(char *)(param_2 + 0x4e) != '\0') {
    FUN_04294be4(param_1 + 0x2c,param_2 + 0x2c);
    *(undefined1 *)(param_1 + 0x4e) = 1;
  }
  *(undefined1 *)(param_1 + 0x50) = 0;
  *(undefined1 *)(param_1 + 0x56) = 0;
  if (*(char *)(param_2 + 0x56) != '\0') {
    if (*(char *)((long)param_2 + 0x157) < '\0') {
      FUN_00060a60(param_1 + 0x50,*(undefined8 *)(param_2 + 0x50),*(undefined8 *)(param_2 + 0x52));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x52);
      uVar6 = *(undefined8 *)(param_2 + 0x50);
      *(undefined8 *)(param_1 + 0x54) = *(undefined8 *)(param_2 + 0x54);
      *(undefined8 *)(param_1 + 0x52) = uVar10;
      *(undefined8 *)(param_1 + 0x50) = uVar6;
    }
    *(undefined1 *)(param_1 + 0x56) = 1;
  }
  *(undefined1 *)(param_1 + 0x58) = 0;
  *(undefined1 *)(param_1 + 0x5e) = 0;
  if (*(char *)(param_2 + 0x5e) != '\0') {
    if (*(char *)((long)param_2 + 0x177) < '\0') {
      FUN_00060a60(param_1 + 0x58,*(undefined8 *)(param_2 + 0x58),*(undefined8 *)(param_2 + 0x5a));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x5a);
      uVar6 = *(undefined8 *)(param_2 + 0x58);
      *(undefined8 *)(param_1 + 0x5c) = *(undefined8 *)(param_2 + 0x5c);
      *(undefined8 *)(param_1 + 0x5a) = uVar10;
      *(undefined8 *)(param_1 + 0x58) = uVar6;
    }
    *(undefined1 *)(param_1 + 0x5e) = 1;
  }
  *(undefined1 *)(param_1 + 0x60) = 0;
  *(undefined1 *)(param_1 + 0x66) = 0;
  if (*(char *)(param_2 + 0x66) != '\0') {
    if (*(char *)((long)param_2 + 0x197) < '\0') {
      FUN_00060a60(param_1 + 0x60,*(undefined8 *)(param_2 + 0x60),*(undefined8 *)(param_2 + 0x62));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x62);
      uVar6 = *(undefined8 *)(param_2 + 0x60);
      *(undefined8 *)(param_1 + 100) = *(undefined8 *)(param_2 + 100);
      *(undefined8 *)(param_1 + 0x62) = uVar10;
      *(undefined8 *)(param_1 + 0x60) = uVar6;
    }
    *(undefined1 *)(param_1 + 0x66) = 1;
  }
  return param_1;
}


