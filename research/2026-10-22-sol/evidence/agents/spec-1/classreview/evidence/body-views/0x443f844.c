// ==== 0x443f844  ArcContextMenuElementInfo::inputType  (72 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcContextMenuElementInfo::inputType(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443f88c);
    (*pcVar2)();
  }
  if (*(char *)(*(long *)(param_1 + 8) + 0x344) == '\0') {
    return 0;
  }
  uVar1 = *(int *)(*(long *)(param_1 + 8) + 0x340) - 0xf;
  if (8 < uVar1) {
    return 5;
  }
  return (ulong)(byte)(&DAT_0c9d8198)[uVar1];
}


