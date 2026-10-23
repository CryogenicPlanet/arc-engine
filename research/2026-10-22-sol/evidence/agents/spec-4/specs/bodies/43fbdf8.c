// ==== 0x43fbdf8  objc::Categories::ArcBrowserContextPreferences_Extensions_::setUserColor:  (316 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContextPreferences_Extensions_::setUserColor_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  code *pcVar2;
  undefined4 local_54;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  undefined4 local_34;
  
  _objc_msgSend(param_1,"browser_context_preferences",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43fbf34);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_54 = 0;
  }
  else {
    _objc_msgSend(param_3,"color",&local_54);
  }
  local_34 = local_54;
  FUN_042f36e8(local_50,&local_34);
  if ((local_40 != 0) && (local_48 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_48 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar2)(local_48);
      }
    }
  }
  return;
}


