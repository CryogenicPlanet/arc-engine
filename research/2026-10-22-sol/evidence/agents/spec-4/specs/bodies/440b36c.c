// ==== 0x440b36c  ArcSessionWindow::selectedTabIndex  (28 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcSessionWindow::selectedTabIndex(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(int *)(**(long **)(param_1 + 8) + 0x34);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440b388);
  (*pcVar1)();
}


