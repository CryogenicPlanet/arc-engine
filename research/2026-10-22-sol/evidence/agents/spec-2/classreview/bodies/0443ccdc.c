// ==== 0x443ccdc  ArcSessionTab::currentNavigationIndex  (28 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcSessionTab::currentNavigationIndex(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return *(int *)(**(long **)(param_1 + 0x10) + 0xc);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ccf8);
  (*pcVar1)();
}


