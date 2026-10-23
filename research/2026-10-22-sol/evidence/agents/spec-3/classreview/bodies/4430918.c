// ==== 0x4430918  ArcCookie::setExpirationDate:  (24 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcCookie::setExpirationDate_(ID param_1,SEL param_2,double param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(double *)(*(long *)(param_1 + 8) + 0x68) = param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430930);
  (*pcVar1)();
}


