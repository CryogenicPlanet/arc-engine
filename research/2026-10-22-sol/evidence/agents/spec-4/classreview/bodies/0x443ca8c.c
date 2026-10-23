// ==== 0x443ca8c  ArcSessionNavigation::title  (52 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSessionNavigation::title(ID param_1,SEL param_2)

{
  long lVar1;
  char cVar2;
  code *pcVar3;
  ID IVar4;
  long lVar5;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar5 = **(long **)(param_1 + 8);
    cVar2 = *(char *)(lVar5 + 0x117);
    lVar1 = *(long *)(lVar5 + 0x100);
    if (-1 < (long)cVar2) {
      lVar1 = lVar5 + 0x100;
    }
    lVar5 = *(long *)(lVar5 + 0x108);
    if (-1 < cVar2) {
      lVar5 = (long)cVar2;
    }
    IVar4 = FUN_01d9fb90(lVar1,lVar5);
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x443cac0);
  (*pcVar3)();
}


