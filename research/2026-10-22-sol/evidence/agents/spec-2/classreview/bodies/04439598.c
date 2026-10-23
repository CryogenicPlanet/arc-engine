// ==== 0x4439598  ArcAutocompleteMatch::swapContentsAndDescription  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::swapContentsAndDescription(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x330);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44395b0);
  (*pcVar1)();
}


