// ==== 0x43f4590  objc::Categories::ArcBrowserContext_Extensions_::favicon:completion:  (516 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::favicon_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_d8;
  long *local_d0;
  byte local_c8;
  ID *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  ID *local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_b8 = 0;
  local_c0 = (ID *)0x0;
  FUN_043abdf8(&local_68,param_3);
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  uStack_b8 = uStack_60;
  local_c0 = local_68;
  local_b0 = (ulong)local_58;
  _objc_msgSend(param_1,"browser_context",&local_d8);
  if ((local_c8 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f4790);
    (*pcVar2)();
  }
  local_58 = FUN_043f8954;
  local_50 = &PTR_FUN_0df6ba08;
  local_68 = (ID *)FUN_00008790(8);
  *local_68 = param_4;
  FUN_042d5f00(local_d8,&local_c0,&local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68);
  }
  if ((local_c8 != 0) && (local_d0 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_d0[1];
    local_d0[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_d0 + 0x10))(local_d0);
      if (local_d0[2] != 0) {
        LOAcquire();
        lVar1 = local_d0[2];
        local_d0[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f46c0;
      }
      pcVar2 = *(code **)(*local_d0 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_d0,0x20);
      }
      else {
        (*pcVar2)(local_d0);
      }
    }
  }
LAB_043f46c0:
  FUN_041ab210(&local_c0);
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


