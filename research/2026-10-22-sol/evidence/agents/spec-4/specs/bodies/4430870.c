// ==== 0x4430870  ArcCookie::httpOnly  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCookie::httpOnly(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x60);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430888);
  (*pcVar1)();
}


