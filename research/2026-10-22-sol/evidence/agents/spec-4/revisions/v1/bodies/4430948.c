// ==== 0x4430948  ArcCookie::setSameSite:  (24 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcCookie::setSameSite_(ID param_1,SEL param_2,long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(int *)(*(long *)(param_1 + 8) + 0x70) = (int)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430960);
  (*pcVar1)();
}


