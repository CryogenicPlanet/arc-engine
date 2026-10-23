// ==== 0x442cd04  ArcSearchEngine::status  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSearchEngine::status(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 8) + 0x3ac);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442cd1c);
  (*pcVar1)();
}


