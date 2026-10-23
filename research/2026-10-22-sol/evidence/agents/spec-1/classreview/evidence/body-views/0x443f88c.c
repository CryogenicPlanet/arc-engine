// ==== 0x443f88c  ArcContextMenuElementInfo::nodeType  (48 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcContextMenuElementInfo::nodeType(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f8b8);
    (*pcVar1)();
  }
  if (**(uint **)(param_1 + 8) < 7) {
    return (ulong)(byte)(&DAT_0c9d81a8)[**(uint **)(param_1 + 8)];
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x443f8bc);
  (*pcVar1)();
}


