// ==== 0x4430900  ArcCookie::expirationDate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcCookie::expirationDate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(double *)(*(long *)(param_1 + 8) + 0x68);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430918);
  (*pcVar1)();
}


