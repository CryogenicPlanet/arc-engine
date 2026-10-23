// ==== 0x4416bbc  ArcNavigation::didRedirectToOtherTab  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNavigation::didRedirectToOtherTab(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x18);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4416bd4);
  (*pcVar1)();
}


