// ==== 0x4434aa0  ArcCommandBarRankingCandidate::siteEngagement  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingCandidate::siteEngagement(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(float *)(param_1 + 0x8c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434ab4);
  (*pcVar1)();
}


