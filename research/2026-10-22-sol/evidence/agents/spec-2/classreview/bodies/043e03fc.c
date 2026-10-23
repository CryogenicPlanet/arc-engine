// ==== 0x43e03fc  ArcBookmarkTreeNode::faviconUrl  (240 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::faviconUrl(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  long lVar3;
  undefined8 local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0xcf) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0xb8),*(undefined8 *)(lVar3 + 0xc0));
      param_2 = extraout_x1;
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0xc0);
      local_40 = *(undefined8 *)(lVar3 + 0xb8);
      local_30 = *(ulong *)(lVar3 + 200);
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    if ((long)local_30._7_1_ < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      param_2 = extraout_x1_00;
    }
    if (lVar3 != 0) {
      if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x43e04ec);
        (*pcVar1)();
      }
      lVar3 = *(long *)(param_1 + 8);
      if (*(char *)(lVar3 + 0xcf) < '\0') {
        FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0xb8),*(undefined8 *)(lVar3 + 0xc0));
        param_2 = extraout_x1_01;
      }
      else {
        lStack_38 = *(long *)(lVar3 + 0xc0);
        local_40 = *(undefined8 *)(lVar3 + 0xb8);
        local_30 = *(ulong *)(lVar3 + 200);
      }
      IVar2 = FUN_043abd34(&local_40,param_2);
      if (-1 < (long)local_30) {
        return IVar2;
      }
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      return IVar2;
    }
  }
  return 0;
}


