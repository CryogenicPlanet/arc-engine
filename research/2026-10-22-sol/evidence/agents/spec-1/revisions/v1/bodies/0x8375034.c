// ==== 0x8375034  ArcWebContents::accessibilityTreeForTreeId:  (456 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcWebContents::accessibilityTreeForTreeId_(ID param_1,SEL param_2,ID param_3)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  code *pcVar3;
  ID self;
  ID IVar4;
  undefined1 *puVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 local_b8;
  undefined8 uStack_b0;
  ulong local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  const_void__ local_88;
  long local_80;
  long local_78;
  undefined1 auStack_70 [32];
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    if (param_3 == 0) {
      local_b8 = 0;
      uStack_b0 = 0;
      local_a8 = 0;
    }
    else {
      FUN_01b356b8(&local_b8,param_3);
    }
    uStack_98 = uStack_b0;
    local_a0 = local_b8;
    local_90 = local_a8;
    uStack_b0 = 0;
    local_a8 = 0;
    local_b8 = 0;
    uVar6 = *(undefined8 *)(param_1 + 0x118);
    FUN_09273058(auStack_70,&local_a0);
    FUN_0439d2c0(&local_88,uVar6,auStack_70);
    if ((long)local_90 < 0) {
      thunk_FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
    }
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_80);
    pvVar1 = local_88;
    pvVar2 = local_88;
    for (lVar7 = local_80 * 0xa0; local_88 = pvVar2, lVar7 != 0; lVar7 = lVar7 + -0xa0) {
      IVar4 = _objc_alloc(&objc::class_t::ArcAccessibilityTreeNode);
      IVar4 = ArcAccessibilityTreeNode::initWithAccessibilityTreeNode_
                        (IVar4,"initWithAccessibilityTreeNode:",pvVar1);
      NSMutableArray::addObject_(self,"addObject:",IVar4);
      pvVar1 = (const_void__)((long)pvVar1 + 0xa0);
      pvVar2 = local_88;
    }
    if (pvVar2 != (const_void__)0x0) {
      if (local_80 * 0xa0 != 0) {
        puVar5 = (undefined1 *)(local_80 * 0xa0 + (long)pvVar2);
        lVar7 = local_80 * -0xa0;
        do {
          if ((dword *)puVar5 == &section_00000068.reloff) goto LAB_0837520c;
          puVar5 = (undefined1 *)FUN_0441d874();
          lVar7 = lVar7 + 0xa0;
        } while (lVar7 != 0);
      }
      local_80 = ((long)pvVar2 - (long)local_88 >> 5) * -0x3333333333333333;
      thunk_FUN_00005810(local_88,local_78 * 0xa0);
    }
    return self;
  }
LAB_0837520c:
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x8375210);
  (*pcVar3)();
}


