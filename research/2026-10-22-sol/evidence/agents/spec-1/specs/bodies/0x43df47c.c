// ==== 0x43df47c  ArcBackForwardListItem::url  (132 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBackForwardListItem::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SEL extraout_x1;
  long lVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x2f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x18),*(undefined8 *)(lVar3 + 0x20));
      param_2 = extraout_x1;
    }
    else {
      uStack_38 = *(undefined8 *)(lVar3 + 0x20);
      local_40 = *(undefined8 *)(lVar3 + 0x18);
      local_30 = *(ulong *)(lVar3 + 0x28);
    }
    IVar2 = FUN_043abd34(&local_40,param_2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43df500);
  (*pcVar1)();
}


