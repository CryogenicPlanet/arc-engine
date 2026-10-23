// ==== 0x4409274  ArcNotification::progress  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcNotification::progress(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 0x10) + 0x288);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440928c);
  (*pcVar1)();
}


