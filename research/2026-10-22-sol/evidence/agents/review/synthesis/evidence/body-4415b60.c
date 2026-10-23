// ==== 0x4415b60  ArcWebContentsDevTools::openProtocolConnection:error:  (344 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcWebContentsDevTools::openProtocolConnection_error_
             (ID param_1,SEL param_2,ID param_3,undefined4 param_4,ID *param_5)

{
  code *pcVar1;
  uint uVar2;
  ulong uVar3;
  ID IVar4;
  uint *puVar5;
  uint *local_70;
  uint *local_68;
  ulong local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4415cac);
    (*pcVar1)();
  }
  local_58[0] = (ID *)FUN_00008790(8);
  local_68 = (uint *)0x0;
  local_60 = 0;
  *local_58[0] = param_3;
  local_48 = &LAB_04416690;
  local_40 = &PTR_FUN_0df6bc88;
  FUN_04355df8(&local_68,*(undefined8 *)(param_1 + 0x20),local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  local_70 = local_68;
  puVar5 = local_68;
  if (((ulong)local_68 & 1) == 0) {
    uVar2 = *local_68;
    *local_68 = uVar2 + 1;
    puVar5 = (uint *)(ulong)uVar2;
  }
  uVar3 = FUN_04402548(puVar5,&local_70,param_4);
  if (((ulong)local_70 & 1) == 0) {
    FUN_04b35898();
  }
  if ((uVar3 & 1) == 0) {
    IVar4 = 0;
  }
  else {
    if (local_68 != (uint *)((long)&MACH_HEADER.magic + 1)) {
                    /* WARNING: Subroutine does not return */
      FUN_04b37e18(&local_68);
    }
    IVar4 = NSNumber::numberWithInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",local_60 & 0xffffffff);
  }
  if (((ulong)local_68 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return IVar4;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


