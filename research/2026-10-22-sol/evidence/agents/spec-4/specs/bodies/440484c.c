// ==== 0x440484c  ArcImportSourceProfile::constraints  (32 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::constraints(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (ulong)(*(int *)(*(long *)(param_1 + 8) + 0x18) == 3);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440486c);
  (*pcVar1)();
}


