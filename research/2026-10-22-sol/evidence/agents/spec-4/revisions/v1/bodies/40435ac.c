// ==== 0x40435ac  objc::Categories::ArcBrowserContextHistory_Extensions_::expireHistoryBetween:forBeginTime:forEndTime:completion:  (460 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::
     expireHistoryBetween_forBeginTime_forEndTime_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,
               undefined4 param_7)

{
  code *pcVar1;
  undefined8 extraout_d0;
  undefined8 extraout_d0_00;
  undefined8 **local_d0 [2];
  byte local_c0;
  undefined *local_b8;
  undefined8 uStack_b0;
  undefined1 *local_a8;
  undefined *puStack_a0;
  undefined8 **local_98;
  undefined8 *local_90;
  undefined8 local_88;
  undefined8 local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  local_98 = &local_90;
  local_90 = &local_88;
  local_88 = 0;
  local_b8 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_b0 = 0xd0800000;
  local_80 = 0;
  local_a8 = &LAB_040451a0;
  puStack_a0 = &DAT_0da87b48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_b8);
  _objc_msgSend(param_1,"browser_context_history",local_d0);
  if ((local_c0 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4043774);
    (*pcVar1)();
  }
  _objc_msgSend(param_4,"timeIntervalSince1970");
  _objc_msgSend(param_5,"timeIntervalSince1970");
  local_68 = &LAB_04044e30;
  local_60 = &PTR_FUN_0da87bc8;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_6;
  FUN_04332dc0(extraout_d0,extraout_d0_00,local_d0,&local_90,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if (local_c0 != 0) {
    FUN_04332750(local_d0);
  }
  local_d0[0] = &local_90;
  FUN_0063242c(local_d0,local_88);
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


