// ==== 0x4294be4  FUN_04294be4  (420 bytes)

undefined4 * FUN_04294be4(undefined4 *param_1,undefined4 *param_2)

{
  undefined2 uVar1;
  code *pcVar2;
  undefined8 *puVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  
  *param_1 = *param_2;
  puVar3 = (undefined8 *)(param_1 + 2);
  *(undefined1 *)puVar3 = 0;
  *(undefined1 *)(param_1 + 8) = 0;
  if (*(char *)(param_2 + 8) != '\0') {
    if (*(char *)((long)param_2 + 0x1f) < '\0') {
      FUN_00060a60(puVar3,*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 4);
      uVar9 = *(undefined8 *)(param_2 + 2);
      *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 6);
      *(undefined8 *)(param_1 + 4) = uVar10;
      *puVar3 = uVar9;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
  puVar3 = (undefined8 *)(param_1 + 10);
  *(undefined1 *)puVar3 = 0;
  *(undefined1 *)(param_1 + 0x10) = 0;
  if (*(char *)(param_2 + 0x10) != '\0') {
    if (*(char *)((long)param_2 + 0x3f) < '\0') {
      FUN_00060a60(puVar3,*(undefined8 *)(param_2 + 10),*(undefined8 *)(param_2 + 0xc));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0xc);
      uVar9 = *(undefined8 *)(param_2 + 10);
      *(undefined8 *)(param_1 + 0xe) = *(undefined8 *)(param_2 + 0xe);
      *(undefined8 *)(param_1 + 0xc) = uVar10;
      *puVar3 = uVar9;
    }
    *(undefined1 *)(param_1 + 0x10) = 1;
  }
  puVar3 = (undefined8 *)(param_1 + 0x12);
  *(undefined1 *)puVar3 = 0;
  *(undefined1 *)(param_1 + 0x18) = 0;
  if (*(char *)(param_2 + 0x18) != '\0') {
    if (*(char *)((long)param_2 + 0x5f) < '\0') {
      FUN_00060a60(puVar3,*(undefined8 *)(param_2 + 0x12),*(undefined8 *)(param_2 + 0x14));
    }
    else {
      uVar10 = *(undefined8 *)(param_2 + 0x14);
      uVar9 = *(undefined8 *)(param_2 + 0x12);
      *(undefined8 *)(param_1 + 0x16) = *(undefined8 *)(param_2 + 0x16);
      *(undefined8 *)(param_1 + 0x14) = uVar10;
      *puVar3 = uVar9;
    }
    *(undefined1 *)(param_1 + 0x18) = 1;
  }
  *(long *)(param_1 + 0x1a) = 0;
  *(undefined8 *)(param_1 + 0x1c) = 0;
  *(undefined8 *)(param_1 + 0x1e) = 0;
  uVar8 = *(ulong *)(param_2 + 0x1c);
  if (uVar8 != 0) {
    if (0x38e38e38e38e38e < uVar8) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    lVar6 = *(long *)(param_2 + 0x1a);
    lVar7 = uVar8 * 0x48;
    lVar4 = FUN_00008790(lVar7);
    *(long *)(param_1 + 0x1a) = lVar4;
    *(undefined8 *)(param_1 + 0x1c) = 0;
    *(ulong *)(param_1 + 0x1e) = uVar8;
    lVar5 = lVar4;
    if (lVar7 != 0) {
      do {
        if (lVar4 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x4294d84);
          (*pcVar2)();
        }
        lVar4 = FUN_083694e8(lVar4,lVar6);
        lVar6 = lVar6 + 0x48;
        lVar4 = lVar4 + 0x48;
        lVar7 = lVar7 + -0x48;
      } while (lVar7 != 0);
      lVar5 = *(long *)(param_1 + 0x1a);
    }
    *(long *)(param_1 + 0x1c) = (lVar4 - lVar5 >> 3) * -0x71c71c71c71c71c7;
  }
  uVar1 = *(undefined2 *)(param_2 + 0x20);
  *(undefined1 *)((long)param_1 + 0x82) = *(undefined1 *)((long)param_2 + 0x82);
  *(undefined2 *)(param_1 + 0x20) = uVar1;
  return param_1;
}


