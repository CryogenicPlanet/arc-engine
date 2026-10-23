// ==== 0x43f35bc  objc::Categories::ArcBrowserContext_Extensions_::createWebContents:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createWebContents_
             (ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  undefined8 local_40;
  long *local_38;
  
  local_40 = 0;
  local_38 = (long *)0x0;
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f37b8);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    _objc_msgSend(param_3,"nativeWebContentsConfiguration",&local_70);
  }
  FUN_042d7064(&local_40,local_58,&local_70);
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
        if (lVar2 != 0) goto LAB_043f3648;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_043f3648:
  IVar3 = _objc_alloc(&class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",&local_40);
  plVar1 = local_38;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_38[1];
    local_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
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
  return IVar3;
}


