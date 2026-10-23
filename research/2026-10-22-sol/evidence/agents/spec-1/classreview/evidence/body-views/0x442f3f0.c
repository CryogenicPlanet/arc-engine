// ==== 0x442f3f0  ArcExtensionInfo::icon  (44 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::icon(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f41c);
    (*pcVar1)();
  }
  if ((**(long **)(param_1 + 0x88) != 0) && (*(long *)(**(long **)(param_1 + 0x88) + 0x18) != 0)) {
    IVar2 = FUN_02a02ccc();
    return IVar2;
  }
  return 0;
}


