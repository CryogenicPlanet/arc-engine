// ==== 0x440bb94  ArcSiteSettingInfo::setOriginUrl:  (148 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setOriginUrl_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    if (param_3 == 0) {
      local_38 = 0;
      uStack_30 = 0;
      local_28 = 0;
    }
    else {
      FUN_01b356b8(&local_38,param_3);
    }
    FUN_04341874(*(undefined8 *)(param_1 + 8),&local_38);
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc28);
  (*pcVar1)();
}


