// ==== 0x440b0a8  ArcSessionWindow::tabs  (456 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionWindow::tabs(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  ID self;
  ID IVar4;
  code *pcVar5;
  long *plVar6;
  long lVar7;
  const_void__ local_78;
  long local_70;
  long local_68;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    local_78 = (const_void__)0x0;
    local_70 = 0;
    local_68 = 0;
    FUN_04341268(&local_78,*(undefined8 *)(param_1 + 8));
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_70);
    pvVar1 = local_78;
    pvVar2 = local_78;
    for (lVar7 = local_70 << 4; local_78 = pvVar2, lVar7 != 0; lVar7 = lVar7 + -0x10) {
      IVar4 = _objc_alloc(&objc::class_t::ArcSessionTab);
      IVar4 = ArcSessionTab::initWithSessionTab_(IVar4,"initWithSessionTab:",pvVar1);
      NSMutableArray::addObject_(self,"addObject:",IVar4);
      pvVar1 = (const_void__)((long)pvVar1 + 0x10);
      pvVar2 = local_78;
    }
    if (pvVar2 != (const_void__)0x0) {
      if (local_70 * 0x10 != 0) {
        lVar7 = local_70 * -0x10;
        pvVar1 = (const_void__)(local_70 * 0x10 + (long)pvVar2);
        do {
          if ((const_void__)((long)pvVar1 + -0x10) == (const_void__)0x0) goto LAB_0440b26c;
          plVar6 = *(long **)((long)pvVar1 + -8);
          if (plVar6 != (long *)0x0) {
            LOAcquire();
            lVar3 = plVar6[1];
            plVar6[1] = lVar3 + -1;
            LORelease();
            if (lVar3 == 0) {
              (**(code **)(*plVar6 + 0x10))(plVar6);
              if (plVar6[2] != 0) {
                LOAcquire();
                lVar3 = plVar6[2];
                plVar6[2] = lVar3 + -1;
                LORelease();
                if (lVar3 != 0) goto LAB_0440b1f0;
              }
              pcVar5 = *(code **)(*plVar6 + 0x20);
              if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar6,0x20);
              }
              else {
                (*pcVar5)(plVar6);
              }
            }
          }
LAB_0440b1f0:
          lVar7 = lVar7 + 0x10;
          pvVar1 = (const_void__)((long)pvVar1 + -0x10);
        } while (lVar7 != 0);
      }
      local_70 = (long)pvVar2 - (long)local_78 >> 4;
      FUN_00005810(local_78,local_68 << 4);
    }
    return self;
  }
LAB_0440b26c:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x440b270);
  (*pcVar5)();
}


