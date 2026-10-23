// ==== 0x43f2aec  objc::Categories::ArcBrowserContext_Extensions_::getAllCookies:  (444 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAllCookies_
               (ID param_1,SEL param_2,ID param_3,uint param_4)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_70,param_3,(ulong)param_4);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2ca4);
    (*pcVar2)();
  }
  local_88 = 0;
  uStack_80 = 0;
  local_78 = 0;
  local_48 = FUN_043e5ef8;
  local_40 = &PTR_FUN_0df6b988;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_3;
  FUN_042cba00(local_70,&local_88,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((long)local_78 < 0) {
    FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
  }
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f2be8;
      }
      pcVar2 = *(code **)(*local_68 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar2)(local_68);
      }
    }
  }
LAB_043f2be8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


