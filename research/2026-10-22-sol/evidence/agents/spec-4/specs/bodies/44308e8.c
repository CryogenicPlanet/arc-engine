// ==== 0x44308e8  ArcCookie::setSession:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setSession_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 99) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430900);
  (*pcVar1)();
}


