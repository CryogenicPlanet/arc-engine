// ==== 0x4294a84  FUN_04294a84  (352 bytes)

/* WARNING: Removing unreachable block (ram,0x04294b04) */
/* WARNING: Removing unreachable block (ram,0x04294ac0) */

long FUN_04294a84(long param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar2 = *(long *)(param_1 + 0x68);
  if (lVar2 != 0) {
    lVar3 = *(long *)(param_1 + 0x70) * 0x48;
    lVar4 = lVar2;
    if (lVar3 != 0) {
      lVar4 = 0;
      lVar5 = lVar2;
      do {
        if (lVar5 + lVar3 == 0x48) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x4294be4);
          (*pcVar1)();
        }
        lVar5 = lVar5 + -0x48;
        lVar4 = lVar4 + 0x48;
      } while (lVar3 - lVar4 != 0);
      lVar4 = *(long *)(param_1 + 0x68);
    }
    *(long *)(param_1 + 0x70) = (lVar2 - lVar4 >> 3) * -0x71c71c71c71c71c7;
    FUN_00005810(lVar4,*(long *)(param_1 + 0x78) * 0x48);
  }
  if ((*(char *)(param_1 + 0x60) != '\0') && (*(char *)(param_1 + 0x5f) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x48),*(ulong *)(param_1 + 0x58) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x40) != '\0') && (*(char *)(param_1 + 0x3f) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
  }
  if ((*(char *)(param_1 + 0x20) != '\0') && (*(char *)(param_1 + 0x1f) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
  }
  return param_1;
}


