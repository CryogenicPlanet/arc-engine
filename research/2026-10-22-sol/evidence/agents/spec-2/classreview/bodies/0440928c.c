// ==== 0x440928c  ArcNotification::renotify  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::renotify(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x2e0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44092a4);
  (*pcVar1)();
}


