// ==== 0x442ff44  ArcDownloadItem::canResume  (92 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcDownloadItem::canResume(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442ffa0);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar3 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442ff88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x1a0))();
    return bVar2;
  }
  return 0;
}


