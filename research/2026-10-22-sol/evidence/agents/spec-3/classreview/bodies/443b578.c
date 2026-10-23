// ==== 0x443b578  ArcCrashKeysReporter::setArcCrashKeysObserver:  (440 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCrashKeysReporter::setArcCrashKeysObserver_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  int *piVar5;
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined8 local_48 [2];
  code *local_38;
  undefined **local_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((DAT_0e53bde0 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e53bde0), iVar2 != 0)) {
    puVar4 = (undefined8 *)FUN_00008790(0x48);
    puVar4[5] = 0;
    puVar4[4] = 0;
    puVar4[7] = 0;
    puVar4[6] = 0;
    puVar4[1] = 0;
    *puVar4 = 0;
    puVar4[3] = 0;
    puVar4[2] = 0;
    puVar4[8] = 0;
    puVar4[2] = FUN_040490c0;
    puVar4[3] = &DAT_0c4b3500;
    puVar4[4] = 0;
    puVar4[5] = 0;
    puVar4[6] = 0;
    piVar5 = (int *)FUN_00008790(8);
    *piVar5 = 0;
    *(undefined1 *)(piVar5 + 1) = 0;
    if ((piVar5 != (int *)0x0) && (iVar2 = *piVar5, *piVar5 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x443b784);
      (*pcVar1)();
    }
    puVar4[7] = piVar5;
    puVar4[8] = puVar4;
    DAT_0e53bdd8 = puVar4;
    thunk_FUN_0c44b760(&DAT_0e53bde0);
  }
  if (param_3 == 0) {
    local_38 = FUN_040490c0;
    local_30 = (undefined **)&DAT_0c4b3500;
    FUN_045559f4(&DAT_0e53bdd8,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
  }
  else {
    _objc_initWeak(auStack_50,param_3);
    _objc_copyWeak(auStack_58,auStack_50);
    local_38 = FUN_0443b78c;
    local_30 = &PTR_FUN_0df6c2f8;
    uVar3 = FUN_00008790(8);
    _objc_moveWeak(uVar3,auStack_58);
    local_48[0] = uVar3;
    _objc_destroyWeak(auStack_58);
    FUN_045559f4(&DAT_0e53bdd8,local_48);
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


