// ==== 0x4416bd4  ArcNavigation::responseStatusCode  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcNavigation::responseStatusCode(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x48);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4416bec);
  (*pcVar1)();
}


