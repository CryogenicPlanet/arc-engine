// ==== 0x4431aa8  ArcCommandBarRankingTrainingResult::outcome  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcCommandBarRankingTrainingResult::outcome(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    return (long)*(int *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431abc);
  (*pcVar1)();
}


