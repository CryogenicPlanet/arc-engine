// ==== 0x441cf40  ArcWebDialog::title  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcWebDialog::title(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x47);
  plVar1 = (long *)*(long *)(param_1 + 0x30);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x30);
  }
  lVar2 = *(long *)(param_1 + 0x38);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01d9fb90(plVar1,lVar2);
  return IVar4;
}


