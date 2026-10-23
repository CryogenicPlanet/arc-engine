// ==== 0x43f9fd4  objc::Categories::ArcBrowserContextPreferences_Extensions_::getDefaultSearchEngine  (520 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContextPreferences_Extensions_::getDefaultSearchEngine
             (ID param_1,SEL param_2)

{
  long *plVar1;
  uint uVar2;
  long lVar3;
  ID IVar4;
  code *pcVar5;
  uint *puVar6;
  long *plVar7;
  uint *local_60;
  long *local_58;
  byte local_50;
  uint *local_48;
  undefined8 uStack_40;
  long *local_38;
  
  local_48 = (uint *)0x0;
  uStack_40 = 0;
  local_38 = (long *)0x0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_60);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43fa1c4);
    (*pcVar5)();
  }
  FUN_042f26b4(&local_48,local_60);
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_58[1];
    local_58[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar3 = local_58[2];
        local_58[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fa034;
      }
      pcVar5 = *(code **)(*local_58 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar5)(local_58);
      }
    }
  }
LAB_043fa034:
  if (local_48 == (uint *)((long)&MACH_HEADER.magic + 1)) {
    IVar4 = _objc_alloc(&class_t::ArcSearchEngine);
    if (local_48 != (uint *)((long)&MACH_HEADER.magic + 1)) {
      local_60 = local_48;
      puVar6 = local_48;
      if (((ulong)local_48 & 1) == 0) {
        uVar2 = *local_48;
        *local_48 = uVar2 + 1;
        puVar6 = (uint *)(ulong)uVar2;
      }
                    /* WARNING: Subroutine does not return */
      FUN_04b37ee8(puVar6,&local_60);
    }
    IVar4 = ArcSearchEngine::initWithSearchEngineTemplateURL_
                      (IVar4,"initWithSearchEngineTemplateURL:",&uStack_40);
    plVar1 = local_38;
    if (local_48 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      if (local_38 == (long *)0x0) {
        return IVar4;
      }
      LOAcquire();
      lVar3 = local_38[1];
      local_38[1] = lVar3 + -1;
      LORelease();
      if (lVar3 != 0) {
        return IVar4;
      }
      plVar7 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) {
          return IVar4;
        }
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 != FUN_00f01bc0) && (pcVar5 != FUN_00f01bc0)) && (pcVar5 != FUN_013efb40)) {
        (*pcVar5)(plVar1);
        return IVar4;
      }
      FUN_00005810(plVar1,0x20);
      return IVar4;
    }
  }
  else {
    IVar4 = 0;
  }
  if (((ulong)local_48 & 1) == 0) {
    FUN_04b35898();
  }
  return IVar4;
}


