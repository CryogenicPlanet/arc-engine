// ==== 0x4406140  ArcLocalState::addPrefChangeCallback:callback:error:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcLocalState::addPrefChangeCallback_callback_error_
             (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5,ID *param_6)

{
  undefined8 ****ppppuVar1;
  ulong uVar2;
  code *pcVar3;
  bool bVar4;
  uint uVar5;
  ID IVar6;
  SEL extraout_x1;
  uint *puVar7;
  undefined8 uVar8;
  uint *local_a8;
  undefined8 ***local_a0;
  ulong uStack_98;
  undefined8 local_90;
  uint *local_88;
  ulong local_80;
  ID *local_78 [2];
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4406338);
    (*pcVar3)();
  }
  local_88 = (uint *)0x0;
  local_80 = 0;
  if (param_3 == 0) {
    uVar5 = 0;
    local_a0 = (undefined8 ****)0x0;
    uStack_98 = 0;
    local_90 = 0;
  }
  else {
    FUN_01b356b8(&local_a0,param_3);
    uVar5 = (uint)local_90._7_1_;
    param_2 = extraout_x1;
  }
  bVar4 = -1 < (char)uVar5;
  ppppuVar1 = (undefined8 ****)local_a0;
  if (bVar4) {
    ppppuVar1 = &local_a0;
  }
  uVar2 = uStack_98;
  if (bVar4) {
    uVar2 = (ulong)uVar5;
  }
  uVar8 = *(undefined8 *)(param_1 + 8);
  local_68 = FUN_04407194;
  local_60 = &PTR_FUN_0df6bb90;
  local_78[0] = (ID *)FUN_00008790(8,param_2);
  *local_78[0] = param_4;
  FUN_04325ac8(&local_88,uVar8,ppppuVar1,uVar2,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_90 < 0) {
    FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
  }
  if (local_88 == (uint *)((long)&MACH_HEADER.magic + 1)) {
    IVar6 = NSNumber::numberWithUnsignedInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithUnsignedInt:",local_80 & 0xffffffff);
  }
  else {
    local_a8 = local_88;
    puVar7 = local_88;
    if (((ulong)local_88 & 1) == 0) {
      uVar5 = *local_88;
      *local_88 = uVar5 + 1;
      puVar7 = (uint *)(ulong)uVar5;
    }
    uVar8 = FUN_044025f4(puVar7,&local_a8);
    *(undefined8 *)(ulong)param_5 = uVar8;
    if (((ulong)local_a8 & 1) == 0) {
      FUN_04b35898();
    }
    IVar6 = 0;
  }
  if (((ulong)local_88 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return IVar6;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


