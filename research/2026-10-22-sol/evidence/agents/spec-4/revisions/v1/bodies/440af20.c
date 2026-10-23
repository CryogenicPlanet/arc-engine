// ==== 0x440af20  ArcSessionTabGroup::color  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSessionTabGroup::color(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 8) + 0x30);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440af38);
  (*pcVar1)();
}


