// ==== 0x4434ac8  ArcCommandBarRankingCandidate::isBaseDomainMatch  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCommandBarRankingCandidate::isBaseDomainMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x91);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434adc);
  (*pcVar1)();
}


