// ==== 0x43f2f30  objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage:key:completion:  (568 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage_key_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f3164);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  if (param_4 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_01b356b8(&local_b0,param_4);
  }
  local_58 = FUN_043f88dc;
  local_50 = &PTR_FUN_0df6b9c8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_5;
  FUN_042cc3f0(local_80,local_98,&local_b0,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f3094;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f3094:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


