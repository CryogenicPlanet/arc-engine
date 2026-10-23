// ==== 0x4430930  ArcCookie::sameSite  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcCookie::sameSite(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 8) + 0x70);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430948);
  (*pcVar1)();
}


