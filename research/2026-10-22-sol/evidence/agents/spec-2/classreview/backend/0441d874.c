// ==== 0x441d874  FUN_0441d874  (220 bytes)

long FUN_0441d874(long param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = *(long *)(param_1 + 0x88);
  if (lVar2 != 0) {
    lVar4 = *(long *)(param_1 + 0x90) * 4;
    if (lVar4 != 0) {
      lVar3 = *(long *)(param_1 + 0x90) * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x441d950);
          (*pcVar1)();
        }
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    *(undefined8 *)(param_1 + 0x90) = 0;
    FUN_00005810(lVar2,*(long *)(param_1 + 0x98) << 2);
  }
  if (*(char *)(param_1 + 0x87) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x70),*(ulong *)(param_1 + 0x80) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x67) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x50),*(ulong *)(param_1 + 0x60) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x4f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x38),*(ulong *)(param_1 + 0x48) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x1f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
  }
  return param_1;
}


