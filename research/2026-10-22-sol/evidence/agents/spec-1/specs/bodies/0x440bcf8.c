// ==== 0x440bcf8  ArcSiteSettingInfo::setSiteSetting:  (36 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setSiteSetting_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bd18);
    (*pcVar1)();
  }
  if (param_3 < 5) {
    FUN_04341964(*(undefined8 *)(param_1 + 8),param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bd1c);
  (*pcVar1)();
}


