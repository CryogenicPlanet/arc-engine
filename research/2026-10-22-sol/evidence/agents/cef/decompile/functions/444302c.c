// ==== 0x444302c  ArcWebContents::executeJavascript:reason:completion:  (396 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcWebContents::executeJavascript_reason_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44431b4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_58 = FUN_04445338;
  local_50 = &PTR_FUN_0df6c498;
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  if (param_4 == 0) {
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
  }
  else {
    FUN_01b356b8(&local_98,param_4);
  }
  FUN_04377d70(*(undefined8 *)(param_1 + 0x118),&local_80,local_68,&local_98);
  if ((long)local_88 < 0) {
    FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
  }
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


