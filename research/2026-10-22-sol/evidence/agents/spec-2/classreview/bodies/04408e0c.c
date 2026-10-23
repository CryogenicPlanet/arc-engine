// ==== 0x4408e0c  ArcNotification::browserContextLocalID  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::browserContextLocalID(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x10);
    if (*(char *)(lVar4 + 0x27) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x10),*(undefined8 *)(lVar4 + 0x18));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x18);
      local_40 = *(undefined1 **)(lVar4 + 0x10);
      local_30 = *(ulong *)(lVar4 + 0x20);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4408ea8);
  (*pcVar2)();
}


