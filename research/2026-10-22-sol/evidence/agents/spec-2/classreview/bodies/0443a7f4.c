// ==== 0x443a7f4  ArcAIDistilBertClassifier::classifierHeadFilePath  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::classifierHeadFilePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x67);
    plVar1 = (long *)*(long *)(param_1 + 0x50);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x50);
    }
    lVar2 = *(long *)(param_1 + 0x58);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a824);
  (*pcVar4)();
}


