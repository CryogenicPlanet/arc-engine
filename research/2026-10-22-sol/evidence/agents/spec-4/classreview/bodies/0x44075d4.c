// ==== 0x44075d4  ArcMediaImage::type  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcMediaImage::type(ID param_1,SEL param_2)

{
  long *plVar1;
  char cVar2;
  ID IVar3;
  long lVar4;
  
  lVar4 = *(long *)(param_1 + 0x10);
  cVar2 = *(char *)(lVar4 + 0x8f);
  plVar1 = (long *)*(long *)(lVar4 + 0x78);
  if (-1 < (long)cVar2) {
    plVar1 = (long *)(lVar4 + 0x78);
  }
  lVar4 = *(long *)(lVar4 + 0x80);
  if (-1 < cVar2) {
    lVar4 = (long)cVar2;
  }
  IVar3 = FUN_01d9fb90(plVar1,lVar4);
  return IVar3;
}


