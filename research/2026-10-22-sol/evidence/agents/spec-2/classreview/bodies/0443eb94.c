// ==== 0x443eb94  ArcFreezingVoteToken::clearVote  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcFreezingVoteToken::clearVote(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x10) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443ebb0);
    (*pcVar1)();
  }
  if (*(long *)(param_1 + 8) != 0) {
    FUN_04335e08();
    return;
  }
  return;
}


