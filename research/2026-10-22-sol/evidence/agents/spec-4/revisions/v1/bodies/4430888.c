// ==== 0x4430888  ArcCookie::setHttpOnly:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setHttpOnly_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 0x60) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44308a0);
  (*pcVar1)();
}


