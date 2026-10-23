// ==== 0x43dfed8  ArcBookmarkTreeNode::nodeID  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::nodeID(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  cfstringStruct *pcVar2;
  long lVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    pcVar2 = &cf___;
  }
  else {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x5f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x48),*(undefined8 *)(lVar3 + 0x50));
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0x50);
      local_40 = *(undefined1 **)(lVar3 + 0x48);
      local_30 = *(ulong *)(lVar3 + 0x58);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    pcVar2 = (cfstringStruct *)FUN_01c391bc(ppuVar1,lVar3);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return (ID)pcVar2;
}


