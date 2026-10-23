// ==== 0x4442354  ArcWebContents::find:forward:matchCase:findMatch:completion:  (356 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x2c bytes */

void ArcWebContents::find_forward_matchCase_findMatch_completion_
               (ID param_1,SEL param_2,ID param_3,bool param_4,bool param_5,bool param_6,ID param_7,
               undefined4 param_8)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 uVar2;
  undefined8 local_90;
  undefined8 uStack_88;
  ulong local_80;
  ID *local_78 [2];
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44424b4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_90 = 0;
    uStack_88 = 0;
    local_80 = 0;
  }
  else {
    FUN_01b356b8(&local_90,param_3);
    param_2 = extraout_x1;
  }
  uVar2 = *(undefined8 *)(param_1 + 0x118);
  local_68 = FUN_0444510c;
  local_60 = &PTR_FUN_0df6c3d8;
  local_78[0] = (ID *)FUN_00008790(8,param_2);
  *local_78[0] = param_7;
  FUN_043770c0(uVar2,&local_90,param_4,param_5,param_6,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_80 < 0) {
    FUN_00005810(local_90,local_80 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


