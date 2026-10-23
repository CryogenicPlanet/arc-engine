// ==== 0x40d0e40  ArcWebContents::blockedResourceCount  (8 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcWebContents::blockedResourceCount(ID param_1,SEL param_2)

{
  return *(int *)(param_1 + 0x24);
}


