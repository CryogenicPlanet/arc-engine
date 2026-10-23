// ==== 0x43fa828  objc::Categories::ArcBrowserContextPreferences_Extensions_::setDefaultSearchEngine:  (700 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContextPreferences_Extensions_::setDefaultSearchEngine_
               (ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  ulong uVar2;
  long lVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_80;
  long *plStack_78;
  ulong local_70;
  undefined8 local_60;
  long *plStack_58;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  ulong local_38;
  
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43faad8);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    plStack_78 = (long *)0x0;
    local_70 = 0;
LAB_043faae0:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  _objc_msgSend(param_3,"search_engine_template_url",&local_80);
  if ((local_70 & 1) == 0) goto LAB_043faae0;
  plStack_58 = plStack_78;
  local_60 = local_80;
  if (plStack_78 != (long *)0x0) {
    plStack_78[1] = plStack_78[1] + 1;
  }
  FUN_042f2a04(&local_38,local_50,&local_60);
  plVar1 = plStack_58;
  if (plStack_58 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_58[1];
    plStack_58[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar5 = plStack_58 + 2;
      (**(code **)(*plStack_58 + 0x10))(plStack_58);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fa8d0;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043fa8d0:
  if (((char)local_70 != '\0') && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (plStack_78[2] != 0) {
        LOAcquire();
        lVar3 = plStack_78[2];
        plStack_78[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fa8f0;
      }
      pcVar4 = *(code **)(*plStack_78 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plStack_78,0x20);
      }
      else {
        (*pcVar4)(plStack_78);
      }
    }
  }
LAB_043fa8f0:
  if ((local_40 != 0) && (local_48 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_48[1];
    local_48[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar3 = local_48[2];
        local_48[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fa910;
      }
      pcVar4 = *(code **)(*local_48 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar4)(local_48);
      }
    }
  }
LAB_043fa910:
  uVar2 = local_38;
  if ((local_38 & 1) == 0) {
    FUN_04b35898(local_38);
  }
  return (bool)(uVar2 == 1);
}


