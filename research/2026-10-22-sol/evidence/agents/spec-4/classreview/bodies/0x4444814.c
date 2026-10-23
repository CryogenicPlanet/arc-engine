// ==== 0x4444814  ArcWebContents::setLoadProgress:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setLoadProgress_(ID param_1,SEL param_2,double param_3)

{
  *(double *)(param_1 + 0x80) = param_3;
  return;
}


