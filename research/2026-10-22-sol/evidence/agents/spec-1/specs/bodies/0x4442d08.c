// ==== 0x4442d08  ArcWebContents::captureRegion:completion:  (304 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void ArcWebContents::captureRegion_completion_
               (ID param_1,SEL param_2,CGRect param_3,ID param_4,undefined4 param_5)

{
  code *pcVar1;
  int local_90;
  int iStack_8c;
  int local_88;
  int iStack_84;
  ulong local_80;
  ID *local_78 [2];
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442e34);
    (*pcVar1)();
  }
  local_90 = (int)param_3.field0_0x0.field0_0x0;
  iStack_8c = (int)param_3.field0_0x0.field1_0x8;
  local_88 = (int)param_3.field1_0x10.field0_0x0;
  iStack_84 = (int)param_3.field1_0x10.field1_0x8;
  local_68 = FUN_043f8954;
  local_60 = &PTR_FUN_0df6c458;
  local_78[0] = (ID *)FUN_00008790(8,param_2,param_4,param_5);
  local_80 = 0;
  *local_78[0] = param_4;
  FUN_04345f38(&local_80,param_1 + 0x118,&local_90,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if (local_80 != 1) {
    (**(code **)(param_4 + 0x10))(param_4,0);
    if ((local_80 & 1) == 0) {
      FUN_04b35898();
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


