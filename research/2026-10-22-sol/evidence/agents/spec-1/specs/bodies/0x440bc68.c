// ==== 0x440bc68  ArcSiteSettingInfo::contentSettingsType  (36 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcSiteSettingInfo::contentSettingsType(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc88);
    (*pcVar1)();
  }
  if (**(uint **)(param_1 + 8) < 0x25) {
    return (ulong)**(uint **)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bc8c);
  (*pcVar1)();
}


