// ==== 0x443a824  ArcAIDistilBertClassifier::numClasses  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAIDistilBertClassifier::numClasses(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    return (int)*(short *)(param_1 + 0x68);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a838);
  (*pcVar1)();
}


