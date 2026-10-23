// ==== 0x43f3234  objc::Categories::ArcBrowserContext_Extensions_::setLocalStorage:key:value:completion:  (668 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x30 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::setLocalStorage_key_value_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,
               undefined4 param_7)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_d8;
  undefined8 uStack_d0;
  ulong local_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f34cc);
    (*pcVar2)();
  }
  FUN_043abdf8(local_a8,param_3);
  if (param_4 == 0) {
    local_c0 = 0;
    uStack_b8 = 0;
    local_b0 = 0;
  }
  else {
    FUN_01b356b8(&local_c0,param_4);
  }
  if (param_5 == 0) {
    local_d8 = 0;
    uStack_d0 = 0;
    local_c8 = 0;
  }
  else {
    FUN_01b356b8(&local_d8,param_5);
  }
  local_68 = &LAB_04045100;
  local_60 = &PTR_FUN_0df6b9e8;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_6;
  FUN_042ccef8(local_90,local_a8,&local_c0,&local_d8,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_c8 < 0) {
    FUN_00005810(local_d8,local_c8 & 0x7fffffffffffffff);
  }
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_88[1];
    local_88[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar1 = local_88[2];
        local_88[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f33f0;
      }
      pcVar2 = *(code **)(*local_88 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar2)(local_88);
      }
    }
  }
LAB_043f33f0:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


