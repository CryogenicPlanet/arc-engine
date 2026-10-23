// ==== 0x4432890  ArcCommandBarRankingResult::score  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingResult::score(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(float *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44328a4);
  (*pcVar1)();
}


