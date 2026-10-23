// ==== 0x8371bec  ArcBrowserApplication::devToolsWebSocketDebugURL  (152 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::devToolsWebSocketDebugURL(ID param_1,SEL param_2)

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
  FUN_05533b24(&local_40,**(long **)(param_1 + 0x20) + 0x128);
  if ((char)local_28 == '\0') {
    IVar3 = 0;
  }
  else {
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
      thunk_FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}


