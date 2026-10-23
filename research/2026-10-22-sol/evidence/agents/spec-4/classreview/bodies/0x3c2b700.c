// ==== 0x3c2b700  FUN_03c2b700  (624 bytes)

undefined4 FUN_03c2b700(long *param_1)

{
  uint *puVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  byte bVar5;
  char cVar6;
  undefined4 uVar7;
  ulong uVar8;
  long lVar9;
  ulong uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  
  if ((int)param_1[0xf] != 0) {
    return 5;
  }
  if ((*(char *)((long)param_1 + 0x12f) != '\0') && ((*(byte *)((long)param_1 + 0x12a) & 1) != 0)) {
    return 6;
  }
  if ((char)param_1[0x20] == '\0') {
    return 0;
  }
  uVar11 = *(uint *)((long)param_1 + 0x24);
  uVar8 = (ulong)uVar11;
  if (0 < (int)uVar11) {
    bVar5 = *(byte *)((long)param_1 + 0x17);
    uVar2 = param_1[1];
    if (-1 < (char)bVar5) {
      uVar2 = (ulong)bVar5;
    }
    uVar10 = (ulong)(int)param_1[4];
    if (uVar2 < uVar10) goto LAB_03c2b8e8;
    plVar3 = (long *)*param_1;
    if (-1 < (char)bVar5) {
      plVar3 = param_1;
    }
    uVar4 = uVar2 - uVar10;
    if (uVar8 <= uVar2 - uVar10) {
      uVar4 = uVar8;
    }
    puVar1 = (uint *)((long)plVar3 + uVar10);
    if (uVar4 == 5) {
      uVar12 = *puVar1;
      uVar13 = 0x70747468;
      cVar6 = (char)puVar1[1];
LAB_03c2b84c:
      if (((uVar12 == uVar13 && cVar6 == 's') && (param_1[0x21] != 0)) &&
         ((*(uint *)(param_1 + 0x23) & 0xff00ffff) != 0)) {
        return 5;
      }
    }
    else if (uVar4 == 3) {
      uVar12 = (uint)(ushort)*puVar1;
      uVar13 = 0x7377;
      cVar6 = *(char *)((long)puVar1 + 2);
      goto LAB_03c2b84c;
    }
    if ((uVar4 == 4) && (*puVar1 == 0x61746164)) {
      return 6;
    }
  }
  if ((*(byte *)((long)param_1 + 300) & 1) != 0) {
    return 0;
  }
  if (((*(byte *)((long)param_1 + 0x126) & 1) != 0) || ((*(byte *)(param_1 + 0x25) & 1) != 0)) {
    return 5;
  }
  if (0 < (int)uVar11) {
    bVar5 = *(byte *)((long)param_1 + 0x17);
    uVar2 = param_1[1];
    if (-1 < (char)bVar5) {
      uVar2 = (ulong)bVar5;
    }
    uVar10 = (ulong)(int)param_1[4];
    if (uVar2 < uVar10) {
LAB_03c2b8e8:
                    /* WARNING: Subroutine does not return */
      FUN_04b085b0("string_view::substr");
    }
    plVar3 = (long *)*param_1;
    if (-1 < (char)bVar5) {
      plVar3 = param_1;
    }
    uVar4 = uVar2 - uVar10;
    if (uVar8 <= uVar2 - uVar10) {
      uVar4 = uVar8;
    }
    puVar1 = (uint *)((long)plVar3 + uVar10);
    if (uVar4 == 5) {
      uVar11 = *puVar1;
      uVar12 = 0x70747468;
      cVar6 = (char)puVar1[1];
    }
    else {
      if (uVar4 != 3) goto LAB_03c2b7f8;
      uVar11 = (uint)(ushort)*puVar1;
      uVar12 = 0x7377;
      cVar6 = *(char *)((long)puVar1 + 2);
    }
    if ((uVar11 == uVar12 && cVar6 == 's') && (param_1[0x21] != 0)) {
      if ((*(byte *)((long)param_1 + 0x11a) >> 3 & 1) != 0) {
        return 0;
      }
      if ((*(byte *)((long)param_1 + 0x124) & 1) != 0) {
        return 6;
      }
      if ((*(char *)((long)param_1 + 0x125) != '\0') && (*(char *)((long)param_1 + 0x12e) == '\0'))
      {
        return 0;
      }
      if ((*(byte *)((long)param_1 + 0x127) & 1) != 0) {
        return 0;
      }
      uVar7 = 3;
      lVar9 = 299;
      goto LAB_03c2b8c8;
    }
  }
LAB_03c2b7f8:
  if ((((*(byte *)((long)param_1 + 0x12a) & 1) != 0) ||
      (uVar8 = FUN_004b78d0(param_1), (uVar8 & 1) != 0)) ||
     ((uVar8 = FUN_00786fb0(param_1), (uVar8 & 1) == 0 &&
      (uVar8 = FUN_00786b34(param_1,&DAT_0c6d5f28,4), (uVar8 & 1) == 0)))) {
    return 0;
  }
  uVar7 = 6;
  lVar9 = 0x12d;
LAB_03c2b8c8:
  if (*(char *)((long)param_1 + lVar9) == '\0') {
    return uVar7;
  }
  return 0;
}


