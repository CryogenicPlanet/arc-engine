// ==== 0x43f0330  objc::Categories::ArcBrowserContext_Extensions_::editBookmark:withNewURL:  (736 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::editBookmark_withNewURL_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  bool bVar3;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_88 [8];
  long *local_80;
  byte local_78;
  undefined8 local_70 [2];
  undefined7 local_60;
  char cStack_59;
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
        if (lVar2 != 0) goto LAB_043f03d0;
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
LAB_043f03d0:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f060c);
    (*pcVar4)();
  }
  FUN_043abdf8(local_70,param_4);
  _objc_msgSend(param_3,"node",auStack_88);
  if ((local_78 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  bVar3 = FUN_042ca078(local_58,local_70,auStack_88);
  if ((local_78 != 0) && (local_80 != (long *)0x0)) {
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
        if (lVar2 != 0) goto LAB_043f0444;
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
LAB_043f0444:
  if (cStack_59 < '\0') {
    FUN_00005810(local_70[0],CONCAT17(cStack_59,local_60) & 0x7fffffffffffffff);
  }
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


