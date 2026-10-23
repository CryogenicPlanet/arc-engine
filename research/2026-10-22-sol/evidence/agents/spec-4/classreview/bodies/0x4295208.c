// ==== 0x4295208  FUN_04295208  (404 bytes)

long FUN_04295208(long param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if ((*(char *)(param_1 + 0x198) != '\0') && (*(char *)(param_1 + 0x197) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x180),*(ulong *)(param_1 + 400) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x178) != '\0') && (*(char *)(param_1 + 0x177) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x160),*(ulong *)(param_1 + 0x170) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x158) != '\0') && (*(char *)(param_1 + 0x157) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x140),*(ulong *)(param_1 + 0x150) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x138) != '\0') {
    FUN_04294a84(param_1 + 0xb0);
  }
  if ((*(char *)(param_1 + 0xa8) != '\0') && (*(char *)(param_1 + 0xa7) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x90),*(ulong *)(param_1 + 0xa0) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x88) != '\0') && (*(char *)(param_1 + 0x87) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x70),*(ulong *)(param_1 + 0x80) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x68) != '\0') && (*(char *)(param_1 + 0x67) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x50),*(ulong *)(param_1 + 0x60) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x38) != '\0') && (*(char *)(param_1 + 0x37) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  lVar3 = *(long *)(param_1 + 8);
  if (lVar3 != 0) {
    lVar2 = *(long *)(param_1 + 0x10) * 0x1a0;
    lVar4 = lVar3;
    if (lVar2 != 0) {
      lVar2 = lVar2 + lVar3;
      lVar4 = *(long *)(param_1 + 0x10) * -0x1a0;
      do {
        if (lVar2 == 0x1a0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x429539c);
          (*pcVar1)();
        }
        lVar2 = FUN_04295208();
        lVar4 = lVar4 + 0x1a0;
      } while (lVar4 != 0);
      lVar4 = *(long *)(param_1 + 8);
    }
    *(long *)(param_1 + 0x10) = (lVar3 - lVar4 >> 5) * 0x4ec4ec4ec4ec4ec5;
    FUN_00005810(lVar4,*(long *)(param_1 + 0x18) * 0x1a0);
  }
  return param_1;
}


