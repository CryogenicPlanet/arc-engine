// ==== 0x43fbfe0  objc::Categories::ArcBrowserContextPreferences_Extensions_::setDownloadFilePath:  (372 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContextPreferences_Extensions_::setDownloadFilePath_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  IVar2 = _objc_msgSend(param_3,"path");
  _objc_msgSend(param_1,"browser_context_preferences",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43fc154);
    (*pcVar3)();
  }
  if (IVar2 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,IVar2);
  }
  FUN_042f350c(local_48,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar3)(local_40);
      }
    }
  }
  return;
}


