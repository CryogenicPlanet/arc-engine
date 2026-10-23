// ==== 0x8371cb0  objc::Categories::ArcBrowserContextPreferences_Extensions_::downloadFilePath  (428 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x08371dac) */
/* WARNING: Removing unreachable block (ram,0x08371dc0) */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContextPreferences_Extensions_::downloadFilePath
             (ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  long lVar2;
  undefined8 uVar3;
  ID IVar4;
  code *pcVar5;
  long local_68;
  long *local_60;
  byte local_58;
  undefined8 **local_50;
  long lStack_48;
  undefined8 local_40;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  undefined1 uStack_21;
  
  local_50 = (undefined8 **)0x0;
  lStack_48 = 0;
  local_40 = 0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x8371e5c);
    (*pcVar5)();
  }
  FUN_07dab804(*(undefined8 *)(local_68 + 0x70));
  FUN_07dab8a0(&local_38);
  lStack_48 = lStack_30;
  local_50 = local_38;
  local_40 = CONCAT17(uStack_21,local_28);
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_60[1];
    local_60[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar2 = local_60[2];
        local_60[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_08371d2c;
      }
      pcVar5 = *(code **)(*local_60 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        thunk_FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar5)(local_60);
      }
    }
  }
LAB_08371d2c:
  pppuVar1 = (undefined8 ***)local_50;
  if (-1 < (long)local_40._7_1_) {
    pppuVar1 = &local_50;
  }
  lVar2 = lStack_48;
  if (-1 < (long)local_40) {
    lVar2 = (long)local_40._7_1_;
  }
  uVar3 = FUN_01c391bc(pppuVar1,lVar2);
  IVar4 = NSURL::fileURLWithPath_((ID)&_OBJC_CLASS___NSURL,"fileURLWithPath:",uVar3);
  if ((long)local_40 < 0) {
    thunk_FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  return IVar4;
}


