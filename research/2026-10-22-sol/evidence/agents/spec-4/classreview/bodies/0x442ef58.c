// ==== 0x442ef58  ArcExtensionRuntimeError::contextURL  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionRuntimeError::contextURL(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x90) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 0x28);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442ef6c);
  (*pcVar1)();
}


