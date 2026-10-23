// ==== 0x43e2e9c  ArcBrowser::isJavaScriptFromAppleEventsAllowed  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowser::isJavaScriptFromAppleEventsAllowed(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    bVar2 = FUN_042fb370(*(undefined8 *)(param_1 + 0x28));
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e2eb0);
  (*pcVar1)();
}


