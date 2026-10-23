// ==== 0x4439580  ArcAutocompleteMatch::deletable  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::deletable(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x14);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4439598);
  (*pcVar1)();
}


