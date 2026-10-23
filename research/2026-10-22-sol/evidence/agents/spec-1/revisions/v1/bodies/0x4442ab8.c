// ==== 0x4442ab8  ArcWebContents::captureViewport:  (272 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::captureViewport_(ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  code *pcVar1;
  undefined8 uVar2;
  ulong local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442bc4);
    (*pcVar1)();
  }
  local_60 = 0;
  uVar2 = *(undefined8 *)(param_1 + 0x118);
  local_48 = FUN_043f8954;
  local_40 = &PTR_FUN_0df6c418;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  FUN_04376d10(&local_60,uVar2,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (local_60 != 1) {
    (**(code **)(param_3 + 0x10))(param_3,0);
    if ((local_60 & 1) == 0) {
      FUN_04b35898();
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


