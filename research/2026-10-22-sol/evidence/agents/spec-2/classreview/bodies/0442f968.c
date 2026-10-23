// ==== 0x442f968  ArcDownloadItem::id  (148 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::id(ID param_1,SEL param_2)

{
  undefined1 *puVar1;
  code *pcVar2;
  bool bVar3;
  ID IVar4;
  uint uVar5;
  undefined1 local_38;
  undefined7 uStack_37;
  ulong uStack_30;
  undefined7 local_28;
  byte bStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (*(long *)(param_1 + 0x10) == 0) {
      uVar5 = 0;
      bStack_21 = 0;
      local_38 = 0;
    }
    else {
      FUN_0430e358(&local_38);
      uVar5 = (uint)bStack_21;
    }
    bVar3 = -1 < (char)uVar5;
    puVar1 = (undefined1 *)CONCAT71(uStack_37,local_38);
    if (bVar3) {
      puVar1 = &local_38;
    }
    if (bVar3) {
      uStack_30 = (ulong)uVar5;
    }
    IVar4 = FUN_01c391bc(puVar1,uStack_30);
    if ((char)bStack_21 < '\0') {
      FUN_00005810(CONCAT71(uStack_37,local_38),CONCAT17(bStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442f9fc);
  (*pcVar2)();
}


