// ==== 0x4434a8c  ArcCommandBarRankingCandidate::typedCount  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingCandidate::typedCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(int *)(param_1 + 0x88);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434aa0);
  (*pcVar1)();
}


