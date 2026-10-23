// ==== 0x4401170  ArcBrowserSidePanel::requestClose  (52 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserSidePanel::requestClose(ID param_1,SEL param_2)

{
  long lVar1;
  
  if (((*(long *)(*(long *)(param_1 + 8) + 0x68) != 0) && (lVar1 = FUN_0031dab4(), lVar1 != 0)) &&
     (*(long *)(lVar1 + 0x278) != 0)) {
    FUN_043c3588();
    return;
  }
  return;
}


