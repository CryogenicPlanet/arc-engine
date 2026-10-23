// ==== 0x43e0510  ArcBookmarkTreeNode::dateAdded  (84 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::dateAdded(ID param_1,SEL param_2)

{
  ID IVar1;
  undefined8 uVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined8 *)(*(long *)(param_1 + 8) + 0xd0);
  }
  IVar1 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",uVar2);
  return IVar1;
}


