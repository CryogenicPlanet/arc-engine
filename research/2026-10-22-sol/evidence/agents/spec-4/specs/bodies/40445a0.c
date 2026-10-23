// ==== 0x40445a0  objc::Categories::ArcBrowserContextHistory_Extensions_::deleteByURL:endTime:completion:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::deleteByURL_endTime_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  undefined8 extraout_d0;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined1 auStack_80 [16];
  byte local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context_history",auStack_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x40446f8);
    (*pcVar1)();
  }
  FUN_043abdf8(local_98,param_3);
  _objc_msgSend(param_4,"timeIntervalSince1970");
  local_58 = &LAB_04044e30;
  local_50 = &PTR_FUN_0da87c88;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_5;
  FUN_04333228(extraout_d0,auStack_80,local_98,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if (local_70 != 0) {
    FUN_04332750(auStack_80);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


