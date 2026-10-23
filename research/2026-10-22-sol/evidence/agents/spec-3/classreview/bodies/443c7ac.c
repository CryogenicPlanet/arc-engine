// ==== 0x443c7ac  ArcPerformanceController::setArcPerformanceObserver:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcPerformanceController::setArcPerformanceObserver_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined8 local_48 [2];
  code *local_38;
  undefined **local_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    FUN_00008790(0x40);
    DAT_0e4a2f18 = FUN_0433d1bc();
    thunk_FUN_0c44b760(&DAT_0e4a2f20);
  }
  if (param_3 == 0) {
    local_38 = FUN_040490c0;
    local_30 = (undefined **)&DAT_0c4b3500;
    FUN_0433dcf0(DAT_0e4a2f18,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
  }
  else {
    _objc_initWeak(auStack_50,param_3);
    _objc_copyWeak(auStack_58,auStack_50);
    local_38 = FUN_0443c924;
    local_30 = &PTR_FUN_0df6c318;
    uVar2 = FUN_00008790(8);
    _objc_moveWeak(uVar2,auStack_58);
    local_48[0] = uVar2;
    _objc_destroyWeak(auStack_58);
    FUN_0433dcf0(DAT_0e4a2f18,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
    _objc_destroyWeak(auStack_50);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


