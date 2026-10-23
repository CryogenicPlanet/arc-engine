// ==== 0x442efcc  ArcExtensionRuntimeError::fromIncognito  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionRuntimeError::fromIncognito(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x90) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x70);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442efe0);
  (*pcVar1)();
}


