// ==== 0x440b430  ArcSessionWindow::showState  (52 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSessionWindow::showState(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x440b460);
    (*pcVar2)();
  }
  uVar1 = *(uint *)(**(long **)(param_1 + 8) + 0x90);
  if (uVar1 < 7) {
    return (ulong)(byte)(&DAT_0c9d8180)[uVar1];
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x440b464);
  (*pcVar2)();
}


