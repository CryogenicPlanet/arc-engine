// ==== 0x43f2ca8  objc::Categories::ArcBrowserContext_Extensions_::getAllCookies:completion:  (476 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAllCookies_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  code *pcVar2;
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
  _objc_msgSend(param_1,"browser_context",&local_80,param_3,param_4,(ulong)param_5);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2e80);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  local_58 = FUN_043e5ef8;
  local_50 = &PTR_FUN_0df6b9a8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_042cba00(local_80,local_98,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
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
        if (lVar1 != 0) goto LAB_043f2db8;
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
LAB_043f2db8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


