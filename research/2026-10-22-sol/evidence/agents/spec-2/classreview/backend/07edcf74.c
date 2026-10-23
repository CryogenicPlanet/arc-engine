// ==== 0x7edcf74  FUN_07edcf74  (232 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ulong FUN_07edcf74(long *param_1)

{
  uint uVar1;
  uint uVar2;
  ID IVar3;
  ulong uVar4;
  long *plVar5;
  undefined8 uVar6;
  
  IVar3 = _objc_msgSend(*(ID *)PTR__NSApp_0d8240e8,"currentEvent");
  IVar3 = _objc_msgSend(IVar3,"CGEvent");
  uVar4 = 0;
  if (IVar3 != 0) {
    uVar1 = _CGEventGetIntegerValueField(IVar3,0x29);
    if (uVar1 != 0) {
      uVar2 = _getpid();
      if (uVar2 != uVar1) {
        return (ulong)uVar2;
      }
    }
    uVar4 = _CGEventGetIntegerValueField(IVar3,0x2d);
    if ((int)uVar4 == 1) {
      plVar5 = (long *)(**(code **)(*param_1 + 0x38))(param_1);
      uVar6 = (**(code **)(*plVar5 + 0x168))();
      uVar1 = FUN_003bc2f4(uVar6,"browser.allow_javascript_apple_events",0x25);
      uVar4 = FUN_0167c9e8(uVar6,"browser.allow_javascript_apple_events",0x25,uVar1 ^ 1);
      return uVar4;
    }
  }
  return uVar4;
}


