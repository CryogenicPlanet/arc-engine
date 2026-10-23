// ==== 0x4404770  ArcImportSourceProfile::supportedCategories  (28 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::supportedCategories(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (ulong)*(ushort *)(*(long *)(param_1 + 8) + 0x50) & 0xfff;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440478c);
  (*pcVar1)();
}


