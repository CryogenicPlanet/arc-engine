// ==== 0x44065c8  ArcLocalState::getString:error:  (384 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcLocalState::getString_error_(ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  undefined8 ****ppppuVar1;
  ulong uVar2;
  long lVar3;
  code *pcVar4;
  bool bVar5;
  uint uVar6;
  ID IVar7;
  uint *puVar8;
  uint *local_80;
  undefined8 ***local_78;
  ulong uStack_70;
  undefined8 local_68;
  uint *local_60;
  ulong uStack_58;
  long lStack_50;
  undefined8 local_48;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    uStack_58 = 0;
    local_60 = (uint *)0x0;
    local_48 = 0;
    lStack_50 = 0;
    if (param_3 == 0) {
      uVar6 = 0;
      local_78 = (undefined8 ****)0x0;
      uStack_70 = 0;
      local_68 = 0;
    }
    else {
      FUN_01b356b8(&local_78,param_3);
      uVar6 = (uint)local_68._7_1_;
    }
    bVar5 = -1 < (char)uVar6;
    ppppuVar1 = (undefined8 ****)local_78;
    if (bVar5) {
      ppppuVar1 = &local_78;
    }
    uVar2 = uStack_70;
    if (bVar5) {
      uVar2 = (ulong)uVar6;
    }
    FUN_04325e5c(&local_60,*(undefined8 *)(param_1 + 8),ppppuVar1,uVar2);
    if ((long)local_68 < 0) {
      FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
    }
    if (local_60 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      uVar2 = uStack_58;
      if (-1 < (long)local_48._7_1_) {
        uVar2 = (ulong)&local_60 | 8;
      }
      lVar3 = lStack_50;
      if (-1 < (long)local_48) {
        lVar3 = (long)local_48._7_1_;
      }
      IVar7 = FUN_01c391bc(uVar2,lVar3);
    }
    else {
      local_80 = local_60;
      puVar8 = local_60;
      if (((ulong)local_60 & 1) == 0) {
        uVar6 = *local_60;
        *local_60 = uVar6 + 1;
        puVar8 = (uint *)(ulong)uVar6;
      }
      IVar7 = FUN_044025f4(puVar8,&local_80);
      *param_4 = IVar7;
      if (((ulong)local_80 & 1) == 0) {
        FUN_04b35898();
      }
      IVar7 = 0;
    }
    if (local_60 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      if ((long)local_48 < 0) {
        FUN_00005810(uStack_58,local_48 & 0x7fffffffffffffff);
      }
    }
    else if (((ulong)local_60 & 1) == 0) {
      FUN_04b35898();
    }
    return IVar7;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4406748);
  (*pcVar4)();
}


