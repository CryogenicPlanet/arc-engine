// ==== 0x4407084  ArcLocalState::getBrowserPolicy:error:  (204 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcLocalState::getBrowserPolicy_error_
             (ID param_1,SEL param_2,unsigned_long_long param_3,ID *param_4)

{
  code *pcVar1;
  uint uVar2;
  ID IVar3;
  uint *puVar4;
  uint *local_58;
  uint *local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    local_30 = 0;
    uStack_48 = 0;
    local_50 = (uint *)0x0;
    uStack_38 = 0;
    uStack_40 = 0;
    FUN_0432631c(&local_50,*(undefined8 *)(param_1 + 8),param_3);
    if (local_50 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      IVar3 = FUN_04406e44((ulong)&local_50 | 8);
    }
    else {
      local_58 = local_50;
      puVar4 = local_50;
      if (((ulong)local_50 & 1) == 0) {
        uVar2 = *local_50;
        *local_50 = uVar2 + 1;
        puVar4 = (uint *)(ulong)uVar2;
      }
      IVar3 = FUN_044025f4(puVar4,&local_58);
      *param_4 = IVar3;
      if (((ulong)local_58 & 1) == 0) {
        FUN_04b35898();
      }
      IVar3 = 0;
    }
    if (local_50 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      FUN_0433b0ec((ulong)&local_50 | 8);
    }
    else if (((ulong)local_50 & 1) == 0) {
      FUN_04b35898();
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4407150);
  (*pcVar1)();
}


