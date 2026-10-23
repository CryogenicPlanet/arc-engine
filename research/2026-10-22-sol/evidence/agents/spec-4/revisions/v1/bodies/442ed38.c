// ==== 0x442ed38  ArcExtensionRuntimeErrorStackFrame::lineNumber  (20 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_int ArcExtensionRuntimeErrorStackFrame::lineNumber(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    return *(unsigned_int *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442ed4c);
  (*pcVar1)();
}


