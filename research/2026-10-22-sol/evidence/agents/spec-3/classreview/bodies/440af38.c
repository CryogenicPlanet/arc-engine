// ==== 0x440af38  ArcSessionTabGroup::isCollapsed  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSessionTabGroup::isCollapsed(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x34);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440af50);
  (*pcVar1)();
}


