// ==== 0x440b074  ArcSessionWindow::windowType  (52 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSessionWindow::windowType(ID param_1,SEL param_2)

{
  long_long lVar1;
  uint uVar2;
  code *pcVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x440b0a4);
    (*pcVar3)();
  }
  uVar2 = *(uint *)(**(long **)(param_1 + 8) + 0x38);
  if (uVar2 < 5) {
    uVar2 = uVar2 - 1;
    lVar1 = 0;
    if (uVar2 < 4) {
      lVar1 = (ulong)uVar2 + 1;
    }
    return lVar1;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(0x550b,0x440b0a8);
  (*pcVar3)();
}


