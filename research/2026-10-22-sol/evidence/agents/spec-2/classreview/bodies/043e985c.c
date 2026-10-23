// ==== 0x43e985c  ArcBrowserApplication::setOnWebContentsCloseDebugLogCallback:  (312 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setOnWebContentsCloseDebugLogCallback_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  ID *pIVar1;
  ID *local_58;
  undefined8 local_50;
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_0436ec80(&local_58,param_2,0,param_4);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58);
    }
  }
  else {
    pIVar1 = (ID *)FUN_00008790(8);
    *pIVar1 = param_3;
    local_50 = 0;
    local_48 = (code *)&LAB_043ebf28;
    local_40 = &PTR_FUN_0df6b828;
    local_58 = (ID *)FUN_00008790(8);
    *local_58 = param_3;
    FUN_0436ec80(&local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58);
    }
    if (pIVar1 != (ID *)0x0) {
      FUN_00005810(pIVar1,8);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


