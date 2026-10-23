// ==== 0x43e18cc  ArcBrowser::getAllCookies:  (428 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::getAllCookies_(ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  undefined8 local_68;
  long *local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43e1a74);
    (*pcVar2)();
  }
  FUN_0433410c(&local_68,param_1 + 0x28,param_2,param_3,param_4);
  local_80 = 0;
  uStack_78 = 0;
  local_70 = 0;
  local_48 = FUN_043e5ef8;
  local_40 = &PTR_FUN_0df6b628;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_3;
  FUN_042cba00(local_68,&local_80,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (local_60 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e19b8;
      }
      pcVar2 = *(code **)(*local_60 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar2)(local_60);
      }
    }
  }
LAB_043e19b8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


