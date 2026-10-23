// ==== 0x43eff2c  objc::Categories::ArcBrowserContext_Extensions_::renameBookmark:withTitle:  (824 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::renameBookmark_withTitle_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  code *pcVar4;
  bool bVar5;
  long *plVar6;
  undefined8 local_88;
  long *local_80;
  byte local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  long local_60;
  long local_58;
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
      plVar6 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043effd8;
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
LAB_043effd8:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f0260);
    (*pcVar4)();
  }
  if (param_4 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    FUN_083b1228(&local_70,param_4);
  }
  _objc_msgSend(param_3,"node",&local_88);
  if ((local_78 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (*(long *)(local_58 + 0x88) == 0) {
LAB_043f004c:
    bVar5 = 0;
  }
  else {
    lVar2 = FUN_00881874();
    bVar5 = 0;
    if (lVar2 != 0) {
      if ((*(char *)(lVar2 + 0x21) == '\0') || (lVar3 = FUN_042affa0(local_88), lVar3 == 0))
      goto LAB_043f004c;
      FUN_09f4f5c4(lVar2,lVar3,&local_70,0);
      bVar5 = 1;
    }
  }
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
        if (lVar2 != 0) goto LAB_043f0070;
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
LAB_043f0070:
  if (local_60 < 0) {
    FUN_00005810(local_70,local_60 << 1);
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
          return bVar5;
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
  return bVar5;
}


