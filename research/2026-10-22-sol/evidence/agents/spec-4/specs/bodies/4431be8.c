// ==== 0x4431be8  ArcCommandBarRankingTrainingResult::gradientNormMean  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingTrainingResult::gradientNormMean(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    return *(float *)(param_1 + 0x60);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431bfc);
  (*pcVar1)();
}


