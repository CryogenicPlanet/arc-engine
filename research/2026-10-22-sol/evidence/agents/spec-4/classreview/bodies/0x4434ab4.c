// ==== 0x4434ab4  ArcCommandBarRankingCandidate::isExactURLMatch  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCommandBarRankingCandidate::isExactURLMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x90);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434ac8);
  (*pcVar1)();
}


