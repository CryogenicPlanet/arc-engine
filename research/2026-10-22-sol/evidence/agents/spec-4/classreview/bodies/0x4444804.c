// ==== 0x4444804  ArcWebContents::setHadFormOrUserEdits:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setHadFormOrUserEdits_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x18) = (char)param_3;
  return;
}


