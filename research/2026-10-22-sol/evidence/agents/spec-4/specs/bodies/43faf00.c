// ==== 0x43faf00  objc::Categories::ArcBrowserContextPreferences_Extensions_::addSearchEngine:error:  (520 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContextPreferences_Extensions_::addSearchEngine_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  undefined8 local_68;
  long *local_60;
  ulong local_58;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  ulong local_38;
  
  _objc_msgSend(param_1,"browser_context_preferences",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43fb0fc);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_68 = 0;
    local_60 = (long *)0x0;
    local_58 = 0;
  }
  else {
    _objc_msgSend(param_3,"search_engine_template_url",&local_68);
    if ((local_58 & 1) != 0) {
      FUN_042f3024(&local_38,local_50,&local_68);
      bVar2 = FUN_04402548(&local_38,param_4);
      if ((local_38 & 1) == 0) {
        FUN_04b35898();
      }
      if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
        LOAcquire();
        lVar1 = local_60[1];
        local_60[1] = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*local_60 + 0x10))(local_60);
          if (local_60[2] != 0) {
            LOAcquire();
            lVar1 = local_60[2];
            local_60[2] = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_043fafb0;
          }
          pcVar3 = *(code **)(*local_60 + 0x20);
          if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
            FUN_00005810(local_60,0x20);
          }
          else {
            (*pcVar3)(local_60);
          }
        }
      }
LAB_043fafb0:
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
              return bVar2;
            }
          }
          pcVar3 = *(code **)(*local_48 + 0x20);
          if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
            FUN_00005810(local_48,0x20);
          }
          else {
            (*pcVar3)(local_48);
          }
        }
      }
      return bVar2;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


