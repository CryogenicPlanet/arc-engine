// ==== 0x442f724  ArcDownloadItem::currentSpeed  (100 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::currentSpeed(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f788);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x10);
  if (lVar3 != 0) {
    lVar2 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))();
    if (lVar2 == 0) {
      return 0;
    }
    lVar3 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))();
    if ((*(byte *)(lVar3 + 0x3a4) & 1) == 0) {
      return *(long_long *)(lVar3 + 0x430);
    }
  }
  return 0;
}


