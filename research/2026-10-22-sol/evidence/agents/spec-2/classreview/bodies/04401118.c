// ==== 0x4401118  ArcBrowserCreateParams::initialOriginSpecified  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserCreateParams::initialOriginSpecified(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)(*(int *)(*(long *)(param_1 + 8) + 0x58) == 1);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4401138);
  (*pcVar1)();
}


