// ==== 0x4442a30  ArcWebContents::isFullscreen  (52 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::isFullscreen(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long *plVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442a64);
    (*pcVar1)();
  }
  lVar4 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
  if ((lVar4 != 0) && (plVar3 = *(long **)(lVar4 + 0xe0), plVar3 != (long *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x04442a54. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar2 = (**(code **)(*plVar3 + 0x1d0))();
    return bVar2;
  }
  return 0;
}


