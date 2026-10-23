// ==== 0x442bd58  ArcFormControlInfo::isRequired  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcFormControlInfo::isRequired(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x91);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442bd6c);
  (*pcVar1)();
}


