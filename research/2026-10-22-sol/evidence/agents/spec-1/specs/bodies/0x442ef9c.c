// ==== 0x442ef9c  ArcExtensionRuntimeError::source  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionRuntimeError::source(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x90) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x6f);
    plVar1 = (long *)*(long *)(param_1 + 0x58);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x58);
    }
    lVar2 = *(long *)(param_1 + 0x60);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01d9fb90(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442efcc);
  (*pcVar4)();
}


