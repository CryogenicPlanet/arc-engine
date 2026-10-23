// ==== 0x4443cec  ArcWebContents::isScaleFactorOne  (56 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::isScaleFactorOne(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4443d24);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 0x118) + 0x130) != 0) {
    lVar3 = FUN_00882ba0();
    if (lVar3 != 0) {
      bVar2 = FUN_00882fd0();
      return bVar2;
    }
  }
  return 1;
}


