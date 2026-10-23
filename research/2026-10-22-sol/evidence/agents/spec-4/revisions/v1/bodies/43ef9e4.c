// ==== 0x43ef9e4  objc::Categories::ArcBrowserContext_Extensions_::addBookmark:withUrl:withParent:withIndexInParent:withUuid:  (1084 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     addBookmark_withUrl_withParent_withIndexInParent_withUuid_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               )

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  long *plVar4;
  long *plVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  ulong local_e8;
  undefined1 auStack_e0 [8];
  long *local_d8;
  byte local_d0;
  undefined8 local_c8 [2];
  undefined7 local_b8;
  char cStack_b1;
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
  
  if (param_5 == 0) {
    return 0;
  }
  _objc_msgSend(param_5,"node",&local_80);
  plVar4 = local_78;
  if ((char)local_70 == '\0') {
    return 0;
  }
  if (local_78 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      plVar5 = local_78 + 2;
      (**(code **)(*local_78 + 0x10))(local_78);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043efb64;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_043efb64:
  if (param_7 == 0) {
    uVar6 = 0;
    plVar4 = (long *)0x0;
    local_e8 = 0;
    uVar8 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_7);
    uVar8 = local_70 >> 0x38;
    local_e8 = local_70 & 0xffffffffffffff;
    plVar4 = local_78;
    uVar6 = local_80;
  }
  _objc_msgSend(param_1,"browser_context",&local_98);
  if ((local_88 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43efe1c);
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
  FUN_043abdf8(local_c8,param_4);
  _objc_msgSend(param_5,"node",auStack_e0);
  if ((local_d0 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_7 == 0) {
    local_80 = local_80 & 0xffffffffffffff00;
    uVar7 = uVar6;
  }
  else {
    local_70 = local_e8 | uVar8 << 0x38;
    uVar8 = 0;
    local_e8 = 0;
    uVar7 = 0;
    local_80 = uVar6;
    local_78 = plVar4;
  }
  local_68 = param_7 != 0;
  bVar2 = FUN_042ca218(local_98,&local_b0,local_c8,auStack_e0,param_6,&local_80);
  if ((local_68 != false) && ((long)local_70 < 0)) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if ((local_d0 != 0) && (local_d8 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_d8[1];
    local_d8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_d8 + 0x10))(local_d8);
      if (local_d8[2] != 0) {
        LOAcquire();
        lVar1 = local_d8[2];
        local_d8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043efc80;
      }
      pcVar3 = *(code **)(*local_d8 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_d8,0x20);
      }
      else {
        (*pcVar3)(local_d8);
      }
    }
  }
LAB_043efc80:
  if (cStack_b1 < '\0') {
    FUN_00005810(local_c8[0],CONCAT17(cStack_b1,local_b8) & 0x7fffffffffffffff);
  }
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
        if (lVar1 != 0) goto LAB_043efcd0;
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
LAB_043efcd0:
  if (param_7 == 0) {
    return bVar2;
  }
  if ((uint)uVar8 >> 7 != 0) {
    FUN_00005810(uVar7,local_e8 | (uVar8 & 0x7f) << 0x38);
    return bVar2;
  }
  return bVar2;
}


