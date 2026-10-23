// ==== 0x443953c  ArcAutocompleteMatch::hasTabMatch  (44 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::hasTabMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    if (*(char *)(*(long *)(param_1 + 0x18) + 0x419) == '\0') {
      uVar2 = 0;
    }
    else {
      uVar2 = (uint)*(byte *)(*(long *)(param_1 + 0x18) + 0x418);
    }
    return uVar2 & 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4439568);
  (*pcVar1)();
}


