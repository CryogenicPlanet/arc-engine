// ==== 0x44447e4  ArcWebContents::setCanGoBack:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setCanGoBack_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x16) = (char)param_3;
  return;
}


