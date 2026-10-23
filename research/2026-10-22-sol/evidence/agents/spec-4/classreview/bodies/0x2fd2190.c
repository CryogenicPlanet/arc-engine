// ==== 0x2fd2190  FUN_02fd2190  (176 bytes)

long FUN_02fd2190(long *param_1)

{
  ulong uVar1;
  code *pcVar2;
  long lVar3;
  long lVar4;
  ulong uVar5;
  
  uVar5 = param_1[1];
  if (uVar5 * 0x260 != 0) {
    lVar4 = param_1[3];
    lVar3 = *param_1;
    for (uVar1 = uVar5 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      if (*(long *)(lVar3 + 600) == lVar4) {
        return lVar3;
      }
      lVar3 = lVar3 + 0x260;
    }
    if (2 < (uVar5 - 1 & 0x7ffffffffffffff)) {
      do {
        if (*(long *)(lVar3 + 600) == lVar4) {
          return lVar3;
        }
        if (*(long *)(lVar3 + 0x4b8) == lVar4) {
          return lVar3 + 0x260;
        }
        if (*(long *)(lVar3 + 0x718) == lVar4) {
          return lVar3 + 0x4c0;
        }
        if (*(long *)(lVar3 + 0x978) == lVar4) {
          return lVar3 + 0x720;
        }
        lVar3 = lVar3 + 0x980;
      } while (lVar3 != *param_1 + uVar5 * 0x260);
    }
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0,0x2fd2240);
  (*pcVar2)();
}


