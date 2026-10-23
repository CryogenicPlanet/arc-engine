// ==== 0x4438f84  ArcAutocompleteMatch::relevance  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAutocompleteMatch::relevance(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 0x18) + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4438f9c);
  (*pcVar1)();
}


