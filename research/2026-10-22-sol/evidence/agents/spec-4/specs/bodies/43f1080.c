// ==== 0x43f1080  objc::Categories::ArcBrowserContext_Extensions_::moveBookmark:newParent:atIndex:  (1296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::moveBookmark_newParent_atIndex_
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5)

{
  bool bVar1;
  long *plVar2;
  long lVar3;
  bool bVar4;
  code *pcVar5;
  long *plVar6;
  undefined8 local_a8;
  long *local_a0;
  ulong local_98;
  undefined8 local_90;
  long *plStack_88;
  undefined8 local_80;
  long *plStack_78;
  byte local_70;
  undefined1 auStack_68 [8];
  long *local_60;
  byte local_58;
  
  if (param_3 == 0) {
    return 0;
  }
  _objc_msgSend(param_3,"node",auStack_68);
  if (local_58 == 0) {
    return 0;
  }
  if ((param_4 == 0) ||
     (_objc_msgSend(param_4,"node",&local_80), plVar2 = plStack_78, local_70 == 0)) {
    bVar1 = true;
  }
  else {
    if (plStack_78 != (long *)0x0) {
      LOAcquire();
      lVar3 = plStack_78[1];
      plStack_78[1] = lVar3 + -1;
      LORelease();
      if (lVar3 == 0) {
        plVar6 = plStack_78 + 2;
        (**(code **)(*plStack_78 + 0x10))(plStack_78);
        if (*plVar6 != 0) {
          LOAcquire();
          lVar3 = plVar2[2];
          plVar2[2] = lVar3 + -1;
          LORelease();
          if (lVar3 != 0) goto LAB_043f1148;
        }
        pcVar5 = *(code **)(*plVar2 + 0x20);
        if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
          FUN_00005810(plVar2,0x20);
        }
        else {
          (*pcVar5)(plVar2);
        }
      }
    }
LAB_043f1148:
    bVar1 = false;
  }
  plVar2 = local_60;
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto joined_r0x043f1174;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
joined_r0x043f1174:
  if (bVar1) {
    return 0;
  }
  _objc_msgSend(param_1,"browser_context",auStack_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43f1584);
    (*pcVar5)();
  }
  _objc_msgSend(param_3,"node",&local_80);
  if ((local_70 & 1) == 0) {
LAB_043f158c:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  plStack_88 = plStack_78;
  local_90 = local_80;
  local_80 = 0;
  plStack_78 = (long *)0x0;
  if (param_4 == 0) {
    local_a8 = 0;
    local_a0 = (long *)0x0;
    local_98 = 0;
    goto LAB_043f158c;
  }
  _objc_msgSend(param_4,"node",&local_a8);
  if ((local_98 & 1) == 0) goto LAB_043f158c;
  bVar4 = FUN_04333ad8(auStack_68,&local_90,&local_a8,param_5);
  if (((char)local_98 != '\0') && (local_a0 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_a0[1];
    local_a0[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_a0 + 0x10))(local_a0);
      if (local_a0[2] != 0) {
        LOAcquire();
        lVar3 = local_a0[2];
        local_a0[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f120c;
      }
      pcVar5 = *(code **)(*local_a0 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_a0,0x20);
      }
      else {
        (*pcVar5)(local_a0);
      }
    }
  }
LAB_043f120c:
  plVar2 = plStack_88;
  if (plStack_88 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_88[1];
    plStack_88[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = plStack_88 + 2;
      (**(code **)(*plStack_88 + 0x10))(plStack_88);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f1224;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
LAB_043f1224:
  plVar2 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f1244;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
LAB_043f1244:
  if (local_58 == 0) {
    return bVar4;
  }
  if (local_60 == (long *)0x0) {
    return bVar4;
  }
  LOAcquire();
  lVar3 = local_60[1];
  local_60[1] = lVar3 + -1;
  LORelease();
  if (lVar3 != 0) {
    return bVar4;
  }
  (**(code **)(*local_60 + 0x10))(local_60);
  if (local_60[2] != 0) {
    LOAcquire();
    lVar3 = local_60[2];
    local_60[2] = lVar3 + -1;
    LORelease();
    if (lVar3 != 0) {
      return bVar4;
    }
  }
  pcVar5 = *(code **)(*local_60 + 0x20);
  if (((pcVar5 != FUN_00f01bc0) && (pcVar5 != FUN_00f01bc0)) && (pcVar5 != FUN_013efb40)) {
    (*pcVar5)(local_60);
    return bVar4;
  }
  FUN_00005810(local_60,0x20);
  return bVar4;
}


