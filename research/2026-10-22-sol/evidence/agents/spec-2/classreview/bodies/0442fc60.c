// ==== 0x442fc60  ArcDownloadItem::currentState  (104 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::currentState(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  long lVar3;
  long_long lVar4;
  long lVar5;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fcc4);
    (*pcVar1)();
  }
  lVar5 = *(long *)(param_1 + 0x10);
  if (lVar5 == 0) {
    lVar4 = 0;
  }
  else {
    lVar3 = (**(code **)(**(long **)(lVar5 + 0x18) + 0xf8))();
    lVar4 = 0;
    if (lVar3 != 0) {
      uVar2 = (**(code **)(**(long **)(lVar5 + 0x18) + 0x118))();
      if (3 < uVar2) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x442fcc8);
        (*pcVar1)();
      }
      lVar4 = (long_long)(uVar2 + 1);
    }
  }
  return lVar4;
}


