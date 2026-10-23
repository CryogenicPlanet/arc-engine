// ==== 0x43e1f6c  ArcBrowser::saveCookiesToDisk  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowser::saveCookiesToDisk(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    FUN_042fbc80(*(undefined8 *)(param_1 + 0x28));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e1f80);
  (*pcVar1)();
}


