// ==== 0x4406798  ArcLocalState::getInteger:error:  (332 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcLocalState::getInteger_error_(ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  undefined8 ****ppppuVar1;
  ulong uVar2;
  code *pcVar3;
  bool bVar4;
  uint uVar5;
  ID IVar6;
  uint *puVar7;
  uint *local_70;
  undefined8 ***local_68;
  ulong uStack_60;
  undefined8 local_58;
  uint *local_50;
  ulong local_48;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    local_50 = (uint *)0x0;
    local_48 = 0;
    if (param_3 == 0) {
      uVar5 = 0;
      local_68 = (undefined8 ****)0x0;
      uStack_60 = 0;
      local_58 = 0;
    }
    else {
      FUN_01b356b8(&local_68,param_3);
      uVar5 = (uint)local_58._7_1_;
    }
    bVar4 = -1 < (char)uVar5;
    ppppuVar1 = (undefined8 ****)local_68;
    if (bVar4) {
      ppppuVar1 = &local_68;
    }
    uVar2 = uStack_60;
    if (bVar4) {
      uVar2 = (ulong)uVar5;
    }
    FUN_04325f14(&local_50,*(undefined8 *)(param_1 + 8),ppppuVar1,uVar2);
    if ((long)local_58 < 0) {
      FUN_00005810(local_68,local_58 & 0x7fffffffffffffff);
    }
    if (local_50 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      IVar6 = NSNumber::numberWithInt_
                        ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",local_48 & 0xffffffff);
    }
    else {
      local_70 = local_50;
      puVar7 = local_50;
      if (((ulong)local_50 & 1) == 0) {
        uVar5 = *local_50;
        *local_50 = uVar5 + 1;
        puVar7 = (uint *)(ulong)uVar5;
      }
      IVar6 = FUN_044025f4(puVar7,&local_70);
      *param_4 = IVar6;
      if (((ulong)local_70 & 1) == 0) {
        FUN_04b35898();
      }
      IVar6 = 0;
    }
    if (((ulong)local_50 & 1) == 0) {
      FUN_04b35898();
    }
    return IVar6;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x44068e4);
  (*pcVar3)();
}


