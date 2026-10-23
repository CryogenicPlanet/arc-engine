// ==== 0x43f0ba8  objc::Categories::ArcBrowserContext_Extensions_::removeBookmark:withParent:  (900 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::removeBookmark_withParent_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  bool bVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_88;
  long *local_80;
  ulong local_78;
  undefined1 auStack_70 [8];
  long *local_68;
  byte local_60;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  
  if ((param_3 == 0) || (_objc_msgSend(param_3,"node",&local_58), plVar1 = local_50, local_48 == 0))
  {
    return 0;
  }
  if (local_50 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0c48;
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
LAB_043f0c48:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f0f20);
    (*pcVar4)();
  }
  _objc_msgSend(param_3,"node",auStack_70);
  if ((local_60 & 1) == 0) {
LAB_043f0f28:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_4 == 0) {
    local_88 = 0;
    local_80 = (long *)0x0;
    local_78 = 0;
    goto LAB_043f0f28;
  }
  _objc_msgSend(param_4,"node",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043f0f28;
  bVar3 = FUN_042ca6d4(local_58,auStack_70,&local_88);
  if (((char)local_78 != '\0') && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_80[1];
    local_80[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar2 = local_80[2];
        local_80[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0ccc;
      }
      pcVar4 = *(code **)(*local_80 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar4)(local_80);
      }
    }
  }
LAB_043f0ccc:
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_68[1];
    local_68[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar2 = local_68[2];
        local_68[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0cec;
      }
      pcVar4 = *(code **)(*local_68 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar4)(local_68);
      }
    }
  }
LAB_043f0cec:
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
        if (lVar2 != 0) {
          return bVar3;
        }
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
  return bVar3;
}


