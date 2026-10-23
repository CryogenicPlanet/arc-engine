// ==== 0x442c440  ArcCommandBarRankingEvaluation::numExamplesEvaluated  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingEvaluation::numExamplesEvaluated(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(int *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442c454);
  (*pcVar1)();
}


