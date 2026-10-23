// ==== 0x43fc200  objc::Categories::ArcBrowserContextPreferences_Extensions_::historyDatabaseFilePath  (400 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContextPreferences_Extensions_::historyDatabaseFilePath
             (ID param_1,SEL param_2)

{
  ulong uVar1;
  long lVar2;
  undefined8 uVar3;
  ID IVar4;
  code *pcVar5;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  ulong local_40;
  ulong uStack_38;
  long lStack_30;
  undefined8 local_28;
  
  uStack_38 = 0;
  local_40 = 0;
  local_28 = 0;
  lStack_30 = 0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43fc390);
    (*pcVar5)();
  }
  FUN_042f35f0(&local_40,local_58);
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043fc25c;
      }
      pcVar5 = *(code **)(*local_50 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar5)(local_50);
      }
    }
  }
LAB_043fc25c:
  if (local_40 == 1) {
    uVar1 = uStack_38;
    if (-1 < (long)local_28._7_1_) {
      uVar1 = (ulong)&local_40 | 8;
    }
    lVar2 = lStack_30;
    if (-1 < (long)local_28) {
      lVar2 = (long)local_28._7_1_;
    }
    uVar3 = FUN_01c391bc(uVar1,lVar2);
    IVar4 = NSURL::fileURLWithPath_((ID)&_OBJC_CLASS___NSURL,"fileURLWithPath:",uVar3);
    if (local_40 == 1) {
      if (-1 < (long)local_28) {
        return IVar4;
      }
      FUN_00005810(uStack_38,local_28 & 0x7fffffffffffffff);
      return IVar4;
    }
  }
  else {
    IVar4 = 0;
  }
  if ((local_40 & 1) == 0) {
    FUN_04b35898();
  }
  return IVar4;
}


