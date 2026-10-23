// ==== 0x4443d24  ArcWebContents::isScrollOffsetAtTop  (76 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::isScrollOffsetAtTop(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar3 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    uVar2 = 0;
    if (lVar3 != 0) {
      if ((*(long *)(lVar3 + 0x8d8) == 0) || (lVar3 = FUN_010f9b60(), lVar3 == 0)) {
        uVar2 = 0;
      }
      else {
        uVar2 = (uint)*(byte *)(*(long *)(lVar3 + 0x170) + 0x458);
      }
    }
    return uVar2 & 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443d70);
  (*pcVar1)();
}


