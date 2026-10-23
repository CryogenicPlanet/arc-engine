// ==== 0x442efe0  ArcExtensionRuntimeError::severity  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcExtensionRuntimeError::severity(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x90) & 1) != 0) {
    return (ulong)*(uint *)(param_1 + 0x74);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442eff4);
  (*pcVar1)();
}


