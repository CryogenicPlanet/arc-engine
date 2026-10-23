// ==== 0x43f06dc  objc::Categories::ArcBrowserContext_Extensions_::addBookmarkFolder:withParent:withIndexInParent:withUuid:  (992 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     addBookmarkFolder_withParent_withIndexInParent_withUuid_
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5,ID param_6)

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  long *plVar7;
  ulong uVar8;
  long *plVar9;
  undefined1 auStack_c8 [8];
  long *local_c0;
  byte local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  long local_a0;
  undefined8 local_98;
  long *local_90;
  byte local_88;
  ulong local_80;
  long *local_78;
  ulong local_70;
  bool local_68;
  
  if (param_4 == 0) {
    return 0;
  }
  _objc_msgSend(param_4,"node",&local_80);
  plVar9 = local_78;
  if ((char)local_70 == '\0') {
    return 0;
  }
  if (local_78 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      plVar7 = local_78 + 2;
      (**(code **)(*local_78 + 0x10))(local_78);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar1 = plVar9[2];
        plVar9[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f07c4;
      }
      pcVar3 = *(code **)(*plVar9 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar9,0x20);
      }
      else {
        (*pcVar3)(plVar9);
      }
    }
  }
LAB_043f07c4:
  if (param_6 == 0) {
    uVar4 = 0;
    plVar9 = (long *)0x0;
    uVar6 = 0;
    uVar8 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_6);
    uVar8 = local_70 >> 0x38;
    uVar6 = local_70 & 0xffffffffffffff;
    uVar4 = local_80;
    plVar9 = local_78;
  }
  _objc_msgSend(param_1,"browser_context",&local_98);
  if ((local_88 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f0ab8);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_083b1228(&local_b0,param_3);
  }
  _objc_msgSend(param_4,"node",auStack_c8);
  if ((local_b8 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_6 == 0) {
    local_80 = local_80 & 0xffffffffffffff00;
    uVar5 = uVar4;
  }
  else {
    local_70 = uVar6 | uVar8 << 0x38;
    uVar8 = 0;
    uVar6 = 0;
    uVar5 = 0;
    local_80 = uVar4;
    local_78 = plVar9;
  }
  local_68 = param_6 != 0;
  bVar2 = FUN_042ca500(local_98,&local_b0,auStack_c8,param_5,&local_80);
  if ((local_68 != false) && ((long)local_70 < 0)) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if ((local_b8 != 0) && (local_c0 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_c0[1];
    local_c0[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_c0 + 0x10))(local_c0);
      if (local_c0[2] != 0) {
        LOAcquire();
        lVar1 = local_c0[2];
        local_c0[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f08c4;
      }
      pcVar3 = *(code **)(*local_c0 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_c0,0x20);
      }
      else {
        (*pcVar3)(local_c0);
      }
    }
  }
LAB_043f08c4:
  if (local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 << 1);
  }
  if ((local_88 != 0) && (local_90 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_90[1];
    local_90[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_90 + 0x10))(local_90);
      if (local_90[2] != 0) {
        LOAcquire();
        lVar1 = local_90[2];
        local_90[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f08fc;
      }
      pcVar3 = *(code **)(*local_90 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_90,0x20);
      }
      else {
        (*pcVar3)(local_90);
      }
    }
  }
LAB_043f08fc:
  if (param_6 == 0) {
    return bVar2;
  }
  if ((uint)uVar8 >> 7 != 0) {
    FUN_00005810(uVar5,uVar6 | (uVar8 & 0x7f) << 0x38);
    return bVar2;
  }
  return bVar2;
}


