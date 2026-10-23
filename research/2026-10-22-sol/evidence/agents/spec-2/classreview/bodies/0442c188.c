// ==== 0x442c188  ArcSelectOption::isDisabled  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSelectOption::isDisabled(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x50) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x49);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442c19c);
  (*pcVar1)();
}


