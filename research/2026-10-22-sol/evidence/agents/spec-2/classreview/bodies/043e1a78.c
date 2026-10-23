// ==== 0x43e1a78  ArcBrowser::getAllCookies:completion:  (460 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowser::getAllCookies_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_90 [2];
  undefined7 local_80;
  char cStack_79;
  undefined8 local_78;
  long *local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43e1c40);
    (*pcVar2)();
  }
  FUN_0433410c(&local_78,param_1 + 0x28);
  FUN_043abdf8(local_90,param_3);
  local_58 = FUN_043e5ef8;
  local_50 = &PTR_FUN_0df6b648;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_042cba00(local_78,local_90,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (cStack_79 < '\0') {
    FUN_00005810(local_90[0],CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
  }
  if (local_70 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_70[1];
    local_70[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_70 + 0x10))(local_70);
      if (local_70[2] != 0) {
        LOAcquire();
        lVar1 = local_70[2];
        local_70[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e1b78;
      }
      pcVar2 = *(code **)(*local_70 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_70,0x20);
      }
      else {
        (*pcVar2)(local_70);
      }
    }
  }
LAB_043e1b78:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


