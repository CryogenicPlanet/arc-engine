// ==== 0x43e8e0c  ArcBrowserApplication::loadImportSourceProfiles:completion:  (204 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserApplication::loadImportSourceProfiles_completion_
               (ID param_1,SEL param_2,long_long param_3,ID param_4,undefined4 param_5)

{
  undefined8 *puVar1;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar1 = *(undefined8 **)(param_1 + 0x20);
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  local_48 = &LAB_043eba34;
  local_40 = &PTR_FUN_0df6b7c8;
  FUN_042b8f50(*puVar1,param_3,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


