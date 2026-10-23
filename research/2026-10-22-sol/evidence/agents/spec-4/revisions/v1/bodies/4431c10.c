// ==== 0x4431c10  ArcCommandBarRankingTrainingResult::deltaNorm  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingTrainingResult::deltaNorm(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    return *(float *)(param_1 + 0x68);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431c24);
  (*pcVar1)();
}


