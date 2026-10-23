// ==== 0x440bc8c  ArcSiteSettingInfo::setContentSettingsType:  (36 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setContentSettingsType_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bcac);
    (*pcVar1)();
  }
  if (param_3 < 0x25) {
    **(undefined4 **)(param_1 + 8) = (int)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bcb0);
  (*pcVar1)();
}


