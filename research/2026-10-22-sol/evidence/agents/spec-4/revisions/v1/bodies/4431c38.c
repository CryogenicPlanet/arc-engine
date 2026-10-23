// ==== 0x4431c38  ArcCommandBarRankingTrainingResult::trainingDurationMs  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingTrainingResult::trainingDurationMs(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    return *(int *)(param_1 + 0x70);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431c4c);
  (*pcVar1)();
}


