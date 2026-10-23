// ==== 0x4332524  FUN_04332524  (232 bytes)

long FUN_04332524(long param_1)

{
  undefined8 *puVar1;
  code *pcVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 *puVar6;
  
  if (*(char *)(param_1 + 0x57) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x40),*(ulong *)(param_1 + 0x50) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  lVar5 = *(long *)(param_1 + 8);
  if (lVar5 != 0) {
    lVar4 = *(long *)(param_1 + 0x10) * 0x18;
    lVar3 = lVar5;
    if (lVar4 != 0) {
      lVar3 = *(long *)(param_1 + 0x10) * -0x18;
      puVar1 = (undefined8 *)(lVar4 + lVar5);
      do {
        puVar6 = puVar1 + -3;
        if (puVar6 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x433260c);
          (*pcVar2)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar6,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar3 = lVar3 + 0x18;
        puVar1 = puVar6;
      } while (lVar3 != 0);
      lVar3 = *(long *)(param_1 + 8);
    }
    *(long *)(param_1 + 0x10) = (lVar5 - lVar3 >> 3) * -0x5555555555555555;
    FUN_00005810(lVar3,*(long *)(param_1 + 0x18) * 0x18);
  }
  return param_1;
}


