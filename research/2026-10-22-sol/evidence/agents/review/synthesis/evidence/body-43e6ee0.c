// ==== 0x43e6ee0  ArcBrowserApplication::tryToTerminate:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

unsigned_long_long
ArcBrowserApplication::tryToTerminate_(ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  code *pcVar1;
  uint uVar2;
  undefined8 *puVar3;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar3 = *(undefined8 **)(param_1 + 0x20);
  local_48 = &LAB_04044e30;
  local_40 = &PTR_FUN_0df6b728;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  uVar2 = FUN_042b2430(*puVar3,local_58);
  if (2 < uVar2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e6fb0);
    (*pcVar1)();
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return (ulong)uVar2;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


