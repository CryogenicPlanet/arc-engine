// ==== 0x44395c8  ArcAutocompleteMatch::isFromKeyword  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::isFromKeyword(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x450);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44395e0);
  (*pcVar1)();
}


