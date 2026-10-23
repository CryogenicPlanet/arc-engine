// ==== 0x443bb24  ArcExtensionCommand::shortcut  (180 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionCommand::shortcut(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  ID IVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  lStack_38 = 0;
  local_40 = (undefined1 *)0x0;
  local_28 = 0;
  local_30 = 0;
  if (*(char *)(param_1 + 0x68) == '\0') {
    IVar3 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x67) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(param_1 + 0x50),*(undefined8 *)(param_1 + 0x58));
    }
    else {
      lStack_38 = *(long *)(param_1 + 0x58);
      local_40 = *(undefined1 **)(param_1 + 0x50);
      local_30 = *(ulong *)(param_1 + 0x60);
    }
    local_28 = CONCAT71(local_28._1_7_,1);
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar2);
    if (((char)local_28 != '\0') && ((long)local_30 < 0)) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}


