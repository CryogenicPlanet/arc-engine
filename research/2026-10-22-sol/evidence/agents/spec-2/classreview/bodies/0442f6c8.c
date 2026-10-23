// ==== 0x442f6c8  ArcDownloadItem::totalBytes  (92 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::totalBytes(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long_long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f724);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442f70c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x30))();
    return lVar3;
  }
  return 0;
}


