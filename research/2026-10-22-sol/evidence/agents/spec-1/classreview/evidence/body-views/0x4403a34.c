// ==== 0x4403a34  ArcHistoryQueryOptions::duplicateHandlingPolicy  (44 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcHistoryQueryOptions::duplicateHandlingPolicy(ID param_1,SEL param_2)

{
  long_long lVar1;
  uint uVar2;
  code *pcVar3;
  
  uVar2 = *(uint *)(*(long *)(param_1 + 8) + 0x30);
  if (uVar2 < 3) {
    lVar1 = 2;
    if (uVar2 != 2) {
      lVar1 = (ulong)(uVar2 == 1);
    }
    return lVar1;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(0x550b,0x4403a60);
  (*pcVar3)();
}


