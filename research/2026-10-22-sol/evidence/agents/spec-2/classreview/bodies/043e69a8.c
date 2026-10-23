// ==== 0x43e69a8  ArcBrowserApplication::setHistogramSamplesDelegate:  (440 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setHistogramSamplesDelegate_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined8 local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_042b7ca8(**(undefined8 **)(param_1 + 0x20),local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_042b7d30(**(undefined8 **)(param_1 + 0x20),local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
  }
  else {
    _objc_initWeak(auStack_60,param_3);
    puVar2 = *(undefined8 **)(param_1 + 0x20);
    _objc_copyWeak(auStack_68,auStack_60);
    uVar1 = FUN_00008790(8);
    _objc_moveWeak(uVar1,auStack_68);
    _objc_destroyWeak(auStack_68);
    local_48 = FUN_043eb534;
    local_40 = &PTR_FUN_0df6b688;
    local_58[0] = uVar1;
    FUN_042b7ca8(*puVar2,local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    puVar2 = *(undefined8 **)(param_1 + 0x20);
    _objc_copyWeak(auStack_70,auStack_60);
    uVar1 = FUN_00008790(8);
    _objc_moveWeak(uVar1,auStack_70);
    _objc_destroyWeak(auStack_70);
    local_48 = FUN_043eb604;
    local_40 = &PTR_FUN_0df6b6a8;
    local_58[0] = uVar1;
    FUN_042b7d30(*puVar2,local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    _objc_destroyWeak(auStack_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


