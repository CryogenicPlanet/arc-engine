// ==== 0x442fdd8  ArcDownloadItem::cancel:  (96 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcDownloadItem::cancel_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fe38);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x10);
  if ((lVar3 != 0) && (lVar2 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442fe24. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(long **)(lVar3 + 0x18) + 0x180))(*(long **)(lVar3 + 0x18),param_3);
    return;
  }
  return;
}


