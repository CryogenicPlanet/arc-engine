// ==== 0x43e2ec4  ArcBrowser::toggleJavaScriptFromAppleEventsAllowed  (76 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowser::toggleJavaScriptFromAppleEventsAllowed(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43e2f10);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 0x28);
  if (((*(long *)(lVar2 + 0xf0) != 0) && (*(char *)(*(long *)(lVar2 + 0xf0) + 4) == '\0')) &&
     (*(long *)(lVar2 + 0xf8) != 0)) {
    if ((*(long *)(lVar2 + 0xf0) != 0) && (*(char *)(*(long *)(lVar2 + 0xf0) + 4) == '\0')) {
      FUN_07edcf74(*(undefined8 *)(lVar2 + 0xf8));
      return;
    }
    FUN_07edcf74(0);
    return;
  }
  return;
}


