// ==== 0x440a1e4  ArcNotification::didClickSettings  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcNotification::didClickSettings(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433ef10(param_1 + 0x10);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a1f8);
  (*pcVar1)();
}


