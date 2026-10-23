// ==== 0x44447fc  ArcWebContents::setBlockedResourceCount:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setBlockedResourceCount_(ID param_1,SEL param_2,int param_3)

{
  *(int *)(param_1 + 0x24) = param_3;
  return;
}


