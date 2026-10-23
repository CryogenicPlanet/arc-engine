// ==== 0x43f4b48  objc::Categories::ArcBrowserContext_Extensions_::setFavicon:forPage:iconURL:iconType:completion:  (752 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setFavicon_forPage_iconURL_iconType_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               ,undefined4 param_8)

{
  long *plVar1;
  long lVar2;
  code *pcVar3;
  long *plVar4;
  undefined **local_c8;
  undefined8 uStack_c0;
  long *local_b8;
  undefined8 local_b0 [2];
  undefined7 local_a0;
  char cStack_99;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f4e34);
    (*pcVar3)();
  }
  FUN_043abdf8(local_98,param_4);
  FUN_043abdf8(local_b0,param_5);
  if (param_3 == 0) {
    local_c8 = (undefined **)0x0;
    uStack_c0 = 0;
    local_b8 = (long *)0x0;
  }
  else {
    _objc_msgSend(param_3,"image",&local_c8);
  }
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_7;
  local_58 = &LAB_04045100;
  local_50 = &PTR_FUN_0df6ba48;
  FUN_042d6b6c(local_80,local_98,local_b0,(uint)param_6 & 0x1f,&local_c8,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  plVar1 = local_b8;
  local_c8 = &PTR_FUN_0da7eed0;
  if (local_b8 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_b8[1];
    local_b8[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar4 = local_b8 + 2;
      (**(code **)(*local_b8 + 0x10))(local_b8);
      if (*plVar4 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f4c7c;
      }
      pcVar3 = *(code **)(*plVar1 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar3)(plVar1);
      }
    }
  }
LAB_043f4c7c:
  if (cStack_99 < '\0') {
    FUN_00005810(local_b0[0],CONCAT17(cStack_99,local_a0) & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_78[1];
    local_78[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar2 = local_78[2];
        local_78[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f4ccc;
      }
      pcVar3 = *(code **)(*local_78 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar3)(local_78);
      }
    }
  }
LAB_043f4ccc:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


