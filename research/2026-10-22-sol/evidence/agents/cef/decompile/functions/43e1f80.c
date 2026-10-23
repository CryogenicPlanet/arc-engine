// ==== 0x43e1f80  ArcBrowser::autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:  (532 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x3c bytes */

bool ArcBrowser::
     autocomplete_cursorPosition_preventInlineAutocomplete_startFresh_keyword_error_completion_
               (ID param_1,SEL param_2,ID param_3,int param_4,bool param_5,bool param_6,ID param_7,
               ID *param_8,ID param_9,undefined4 param_10)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  SEL extraout_x1;
  uint *puVar4;
  undefined8 uVar5;
  uint *local_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  ulong local_98;
  uint *local_90;
  ID *local_88 [2];
  undefined1 *local_78;
  undefined **local_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43e2190);
    (*pcVar1)();
  }
  local_90 = (uint *)0x0;
  if (param_3 == 0) {
    local_a8 = 0;
    uStack_a0 = 0;
    local_98 = 0;
  }
  else {
    FUN_01b356b8(&local_a8,param_3);
    param_2 = extraout_x1;
  }
  if (param_7 == 0) {
    local_c0 = 0;
    uStack_b8 = 0;
    local_b0 = 0;
  }
  else {
    FUN_01b356b8(&local_c0,param_7,param_2);
  }
  uVar5 = *(undefined8 *)(param_1 + 0x28);
  local_78 = &LAB_043e5fd4;
  local_70 = &PTR_FUN_0df6b668;
  local_88[0] = (ID *)FUN_00008790(8);
  *local_88[0] = param_9;
  FUN_042fbd80(&local_90,uVar5,&local_a8,param_4,param_5,param_6,&local_c0,local_88);
  if ((code *)local_70[1] != (code *)0x0) {
    (*(code *)local_70[1])(local_88[0]);
  }
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if ((long)local_98 < 0) {
    FUN_00005810(local_a8,local_98 & 0x7fffffffffffffff);
  }
  local_c8 = local_90;
  puVar4 = local_90;
  if (((ulong)local_90 & 1) == 0) {
    uVar2 = *local_90;
    *local_90 = uVar2 + 1;
    puVar4 = (uint *)(ulong)uVar2;
  }
  bVar3 = FUN_04402548(puVar4,&local_c8,param_8);
  if (((ulong)local_c8 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_90 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_68) {
    return bVar3;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


