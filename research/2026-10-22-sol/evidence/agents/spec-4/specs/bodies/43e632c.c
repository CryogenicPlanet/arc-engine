// ==== 0x43e632c  ArcNetworkLoggingConfiguration::filePath  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcNetworkLoggingConfiguration::filePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x1f);
  plVar1 = (long *)*(long *)(param_1 + 8);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 8);
  }
  lVar2 = *(long *)(param_1 + 0x10);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}


