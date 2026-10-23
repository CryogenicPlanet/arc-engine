// ==== 0x442bd6c  ArcFormControlInfo::isRedacted  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcFormControlInfo::isRedacted(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x92);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442bd80);
  (*pcVar1)();
}


