// ==== 0x42a43ac  FUN_042a43ac  (252 bytes)

long FUN_042a43ac(long param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = *(long *)(param_1 + 0x40);
  if (lVar2 != 0) {
    lVar4 = *(long *)(param_1 + 0x48) * 4;
    if (lVar4 != 0) {
      lVar3 = *(long *)(param_1 + 0x48) * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) goto LAB_042a44a4;
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    *(undefined8 *)(param_1 + 0x48) = 0;
    FUN_00005810(lVar2,*(long *)(param_1 + 0x50) << 2);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  lVar2 = *(long *)(param_1 + 8);
  if (lVar2 != 0) {
    lVar4 = *(long *)(param_1 + 0x10) * 0x48;
    lVar3 = lVar2;
    if (lVar4 != 0) {
      lVar4 = lVar4 + lVar2;
      lVar3 = *(long *)(param_1 + 0x10) * -0x48;
      do {
        if (lVar4 == 0x48) {
LAB_042a44a4:
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x42a44a8);
          (*pcVar1)();
        }
        lVar4 = FUN_042a44a8();
        lVar3 = lVar3 + 0x48;
      } while (lVar3 != 0);
      lVar3 = *(long *)(param_1 + 8);
    }
    *(long *)(param_1 + 0x10) = (lVar2 - lVar3 >> 3) * -0x71c71c71c71c71c7;
    FUN_00005810(lVar3,*(long *)(param_1 + 0x18) * 0x48);
  }
  return param_1;
}


