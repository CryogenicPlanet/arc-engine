// ==== 0x441daa0  FUN_0441daa0  (520 bytes)

undefined4 * FUN_0441daa0(undefined4 *param_1,undefined4 *param_2)

{
  ulong uVar1;
  byte bVar2;
  undefined2 uVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  puVar5 = (undefined8 *)(param_1 + 2);
  *param_1 = *param_2;
  if (param_1 == param_2) {
    uVar3 = *(undefined2 *)(param_2 + 0x1a);
    *(undefined1 *)((long)param_1 + 0x6a) = *(undefined1 *)((long)param_2 + 0x6a);
    *(undefined2 *)(param_1 + 0x1a) = uVar3;
  }
  else {
    bVar2 = *(byte *)((long)param_2 + 0x1f);
    if (*(char *)((long)param_1 + 0x1f) < '\0') {
      uVar1 = *(ulong *)(param_2 + 4);
      puVar4 = *(undefined8 **)(param_2 + 2);
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar4 = (undefined8 *)(param_2 + 2);
      }
      FUN_00359ad8(puVar5,puVar4,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(puVar5,*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4));
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 4);
      uVar6 = *(undefined8 *)(param_2 + 2);
      *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 6);
      *(undefined8 *)(param_1 + 4) = uVar7;
      *puVar5 = uVar6;
    }
    bVar2 = *(byte *)((long)param_2 + 0x37);
    if (*(char *)((long)param_1 + 0x37) < '\0') {
      uVar1 = *(ulong *)(param_2 + 10);
      puVar5 = *(undefined8 **)(param_2 + 8);
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar5 = (undefined8 *)(param_2 + 8);
      }
      FUN_00359ad8(param_1 + 8,puVar5,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1 + 8,*(undefined8 *)(param_2 + 8),*(undefined8 *)(param_2 + 10));
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 10);
      uVar6 = *(undefined8 *)(param_2 + 8);
      *(undefined8 *)(param_1 + 0xc) = *(undefined8 *)(param_2 + 0xc);
      *(undefined8 *)(param_1 + 10) = uVar7;
      *(undefined8 *)(param_1 + 8) = uVar6;
    }
    bVar2 = *(byte *)((long)param_2 + 0x4f);
    if (*(char *)((long)param_1 + 0x4f) < '\0') {
      uVar1 = *(ulong *)(param_2 + 0x10);
      puVar5 = *(undefined8 **)(param_2 + 0xe);
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar5 = (undefined8 *)(param_2 + 0xe);
      }
      FUN_00359ad8(param_1 + 0xe,puVar5,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1 + 0xe,*(undefined8 *)(param_2 + 0xe),*(undefined8 *)(param_2 + 0x10));
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 0x10);
      uVar6 = *(undefined8 *)(param_2 + 0xe);
      *(undefined8 *)(param_1 + 0x12) = *(undefined8 *)(param_2 + 0x12);
      *(undefined8 *)(param_1 + 0x10) = uVar7;
      *(undefined8 *)(param_1 + 0xe) = uVar6;
    }
    bVar2 = *(byte *)((long)param_2 + 0x67);
    if (*(char *)((long)param_1 + 0x67) < '\0') {
      uVar1 = *(ulong *)(param_2 + 0x16);
      puVar5 = *(undefined8 **)(param_2 + 0x14);
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar5 = (undefined8 *)(param_2 + 0x14);
      }
      FUN_00359ad8(param_1 + 0x14,puVar5,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1 + 0x14,*(undefined8 *)(param_2 + 0x14),*(undefined8 *)(param_2 + 0x16));
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 0x16);
      uVar6 = *(undefined8 *)(param_2 + 0x14);
      *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(param_2 + 0x18);
      *(undefined8 *)(param_1 + 0x16) = uVar7;
      *(undefined8 *)(param_1 + 0x14) = uVar6;
    }
    uVar3 = *(undefined2 *)(param_2 + 0x1a);
    *(undefined1 *)((long)param_1 + 0x6a) = *(undefined1 *)((long)param_2 + 0x6a);
    *(undefined2 *)(param_1 + 0x1a) = uVar3;
    bVar2 = *(byte *)((long)param_2 + 0x87);
    if (*(char *)((long)param_1 + 0x87) < '\0') {
      uVar1 = *(ulong *)(param_2 + 0x1e);
      puVar5 = *(undefined8 **)(param_2 + 0x1c);
      if (-1 < (char)bVar2) {
        uVar1 = (ulong)bVar2;
        puVar5 = (undefined8 *)(param_2 + 0x1c);
      }
      FUN_00359ad8(param_1 + 0x1c,puVar5,uVar1);
    }
    else if ((char)bVar2 < '\0') {
      FUN_00359998(param_1 + 0x1c,*(undefined8 *)(param_2 + 0x1c),*(undefined8 *)(param_2 + 0x1e));
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 0x1e);
      uVar6 = *(undefined8 *)(param_2 + 0x1c);
      *(undefined8 *)(param_1 + 0x20) = *(undefined8 *)(param_2 + 0x20);
      *(undefined8 *)(param_1 + 0x1e) = uVar7;
      *(undefined8 *)(param_1 + 0x1c) = uVar6;
    }
    FUN_00ce2724(param_1 + 0x22,*(long *)(param_2 + 0x22),
                 *(long *)(param_2 + 0x22) + *(long *)(param_2 + 0x24) * 4,
                 (*(long *)(param_2 + 0x24) << 2) >> 2);
  }
  return param_1;
}


