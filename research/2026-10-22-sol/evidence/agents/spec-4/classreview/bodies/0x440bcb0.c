// ==== 0x440bcb0  ArcSiteSettingInfo::siteSetting  (72 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcSiteSettingInfo::siteSetting(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bcf4);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 8);
  if ((*(char *)(lVar2 + 0x18) != '\0') && (*(char *)(lVar2 + 0x14) == '\0')) {
    if (*(uint *)(lVar2 + 0xc) < 6) {
      return (ulong)(byte)(&DAT_0c9d8188)[*(uint *)(lVar2 + 0xc)];
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bcf8);
    (*pcVar1)();
  }
  return 0;
}


