// ==== 0x441d950  FUN_0441d950  (336 bytes)

undefined4 * FUN_0441d950(undefined4 *param_1,undefined4 *param_2)

{
  undefined2 uVar1;
  void *pvVar2;
  void *pvVar3;
  ulong uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  *param_1 = *param_2;
  if (*(char *)((long)param_2 + 0x1f) < '\0') {
    FUN_00060a60(param_1 + 2,*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4));
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 4);
    uVar5 = *(undefined8 *)(param_2 + 2);
    *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 6);
    *(undefined8 *)(param_1 + 4) = uVar6;
    *(undefined8 *)(param_1 + 2) = uVar5;
  }
  if (*(char *)((long)param_2 + 0x37) < '\0') {
    FUN_00060a60(param_1 + 8,*(undefined8 *)(param_2 + 8),*(undefined8 *)(param_2 + 10));
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 10);
    uVar5 = *(undefined8 *)(param_2 + 8);
    *(undefined8 *)(param_1 + 0xc) = *(undefined8 *)(param_2 + 0xc);
    *(undefined8 *)(param_1 + 10) = uVar6;
    *(undefined8 *)(param_1 + 8) = uVar5;
  }
  if (*(char *)((long)param_2 + 0x4f) < '\0') {
    FUN_00060a60(param_1 + 0xe,*(undefined8 *)(param_2 + 0xe),*(undefined8 *)(param_2 + 0x10));
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 0x10);
    uVar5 = *(undefined8 *)(param_2 + 0xe);
    *(undefined8 *)(param_1 + 0x12) = *(undefined8 *)(param_2 + 0x12);
    *(undefined8 *)(param_1 + 0x10) = uVar6;
    *(undefined8 *)(param_1 + 0xe) = uVar5;
  }
  if (*(char *)((long)param_2 + 0x67) < '\0') {
    FUN_00060a60(param_1 + 0x14,*(undefined8 *)(param_2 + 0x14),*(undefined8 *)(param_2 + 0x16));
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 0x16);
    uVar5 = *(undefined8 *)(param_2 + 0x14);
    *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(param_2 + 0x18);
    *(undefined8 *)(param_1 + 0x16) = uVar6;
    *(undefined8 *)(param_1 + 0x14) = uVar5;
  }
  uVar1 = *(undefined2 *)(param_2 + 0x1a);
  *(undefined1 *)((long)param_1 + 0x6a) = *(undefined1 *)((long)param_2 + 0x6a);
  *(undefined2 *)(param_1 + 0x1a) = uVar1;
  if (*(char *)((long)param_2 + 0x87) < '\0') {
    FUN_00060a60(param_1 + 0x1c,*(undefined8 *)(param_2 + 0x1c),*(undefined8 *)(param_2 + 0x1e));
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 0x1e);
    uVar5 = *(undefined8 *)(param_2 + 0x1c);
    *(undefined8 *)(param_1 + 0x20) = *(undefined8 *)(param_2 + 0x20);
    *(undefined8 *)(param_1 + 0x1e) = uVar6;
    *(undefined8 *)(param_1 + 0x1c) = uVar5;
  }
  *(undefined8 *)(param_1 + 0x22) = 0;
  *(undefined8 *)(param_1 + 0x24) = 0;
  *(undefined8 *)(param_1 + 0x26) = 0;
  uVar4 = *(ulong *)(param_2 + 0x24);
  if (uVar4 != 0) {
    if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    pvVar3 = *(void **)(param_2 + 0x22);
    pvVar2 = (void *)FUN_00008790(uVar4 << 2);
    *(void **)(param_1 + 0x22) = pvVar2;
    *(undefined8 *)(param_1 + 0x24) = 0;
    *(ulong *)(param_1 + 0x26) = uVar4;
    _memcpy(pvVar2,pvVar3,uVar4 << 2);
    *(long *)(param_1 + 0x24) = (long)(uVar4 << 2) >> 2;
  }
  return param_1;
}


