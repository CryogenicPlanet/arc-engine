// ==== 0x4442eb8  ArcWebContents::executeJavascript:completion:  (328 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcWebContents::executeJavascript_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined1 local_88;
  undefined7 uStack_87;
  undefined7 local_78;
  char cStack_71;
  undefined8 local_70;
  undefined8 uStack_68;
  ulong local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442ffc);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    FUN_01b356b8(&local_70,param_3);
    param_2 = extraout_x1;
  }
  local_48 = FUN_04445338;
  local_40 = &PTR_FUN_0df6c478;
  local_58[0] = (ID *)FUN_00008790(8,param_2);
  *local_58[0] = param_4;
  cStack_71 = '\0';
  local_88 = 0;
  FUN_04377d70(*(undefined8 *)(param_1 + 0x118),&local_70,local_58,&local_88);
  if (cStack_71 < '\0') {
    FUN_00005810(CONCAT71(uStack_87,local_88),CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff);
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


