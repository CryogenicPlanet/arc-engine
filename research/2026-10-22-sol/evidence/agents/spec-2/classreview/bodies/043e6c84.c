// ==== 0x43e6c84  ArcBrowserApplication::setDumpWithoutCrashingCallback:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setDumpWithoutCrashingCallback_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    uVar1 = *puVar2;
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
  }
  else {
    local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
    *local_58[0] = param_3;
    uVar1 = *puVar2;
    local_48 = FUN_043eb6c8;
    local_40 = &PTR_FUN_0df6b6e8;
  }
  FUN_042b7ea0(uVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


