// ==== 0x4431b14  ArcCommandBarRankingTrainingResult::numSamples  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingTrainingResult::numSamples(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    return *(int *)(param_1 + 0x44);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431b28);
  (*pcVar1)();
}


