// ==== 0x440949c  ArcNotification::showSettingsButton  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::showSettingsButton(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)(*(int *)(*(long *)(param_1 + 0x10) + 0x310) != 0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44094bc);
  (*pcVar1)();
}


