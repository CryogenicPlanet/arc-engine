// ==== 0x43e8d34  ArcBrowserApplication::availableImportSources:  (196 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::availableImportSources_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 *puVar1;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar1 = *(undefined8 **)(param_1 + 0x20);
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  local_48 = FUN_043eb948;
  local_40 = &PTR_FUN_0df6b7a8;
  FUN_042b8d7c(*puVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


