// ==== 0x44045a8  ArcImportSourceProfile::type  (64 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::type(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x44045e4);
    (*pcVar2)();
  }
  uVar1 = *(int *)(*(long *)(param_1 + 8) + 0x18) - 1;
  if ((uVar1 < 0xd) && ((0x1fc7U >> (ulong)(uVar1 & 0x1f) & 1) != 0)) {
    return (ulong)(byte)(&DAT_0c9d8168)[uVar1];
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0,0x44045e8);
  (*pcVar2)();
}


