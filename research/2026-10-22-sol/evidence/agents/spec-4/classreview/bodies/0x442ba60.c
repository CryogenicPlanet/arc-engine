// ==== 0x442ba60  ArcFormControlInfo::controlType  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcFormControlInfo::controlType(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    return (long)*(int *)(param_1 + 0x10);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442ba74);
  (*pcVar1)();
}


