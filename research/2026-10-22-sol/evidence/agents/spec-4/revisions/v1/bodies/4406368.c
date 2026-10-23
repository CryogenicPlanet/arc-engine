// ==== 0x4406368  ArcLocalState::removePrefChangeCallback:error:  (200 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcLocalState::removePrefChangeCallback_error_(ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  int *piVar1;
  code *pcVar2;
  ID IVar3;
  int *local_40;
  int *local_38;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    local_38 = (int *)0x0;
    IVar3 = _objc_msgSend(param_3,"unsignedIntValue");
    FUN_04325ce8(&local_38,*(undefined8 *)(param_1 + 8),IVar3);
    piVar1 = local_38;
    if (local_38 != (int *)((long)&MACH_HEADER.magic + 1)) {
      local_40 = local_38;
      if (((ulong)local_38 & 1) == 0) {
        *local_38 = *local_38 + 1;
      }
      IVar3 = FUN_044025f4(&local_40);
      *param_4 = IVar3;
      if (((ulong)local_40 & 1) == 0) {
        FUN_04b35898();
      }
    }
    if (((ulong)local_38 & 1) == 0) {
      FUN_04b35898();
    }
    return (bool)(piVar1 == (int *)((long)&MACH_HEADER.magic + 1));
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4406430);
  (*pcVar2)();
}


