// ==== 0x4405a94  ArcImageRep::isNull  (44 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcImageRep::isNull(ID param_1,SEL param_2)

{
  uint uVar1;
  long lVar2;
  
  lVar2 = *(long *)(param_1 + 0x10);
  if (*(int *)(lVar2 + 0x1c0) == 0) {
    uVar1 = (uint)(*(long *)(lVar2 + 0x1d0) == 0);
  }
  else {
    uVar1 = *(byte *)(lVar2 + 0x1b8) ^ 1;
  }
  return uVar1 & 1;
}


