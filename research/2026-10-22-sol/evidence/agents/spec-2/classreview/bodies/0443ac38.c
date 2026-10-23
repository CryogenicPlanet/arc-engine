// ==== 0x443ac38  ArcAccessibilityTreeNode::children  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::children(ID param_1,SEL param_2)

{
  ID self;
  ID IVar1;
  long lVar2;
  uint *puVar3;
  
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                    *(undefined8 *)(param_1 + 0x98));
  lVar2 = *(long *)(param_1 + 0x98) << 2;
  if (lVar2 != 0) {
    puVar3 = *(uint **)(param_1 + 0x90);
    do {
      IVar1 = NSNumber::numberWithInt_((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",(ulong)*puVar3);
      NSMutableArray::addObject_(self,"addObject:",IVar1);
      lVar2 = lVar2 + -4;
      puVar3 = puVar3 + 1;
    } while (lVar2 != 0);
  }
  return self;
}


