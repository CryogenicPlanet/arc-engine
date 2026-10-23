// ==== 0x442de14  ArcMetrics::getAllSiteEngagementDetails  (320 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcMetrics::getAllSiteEngagementDetails(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  undefined8 *puVar2;
  const_void__ pvVar3;
  code *pcVar4;
  ID self;
  ID IVar5;
  undefined8 *puVar6;
  long lVar7;
  const_void__ local_68;
  long local_60;
  long local_58;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_042efb64(&local_68,*(undefined8 *)(param_1 + 8));
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_60);
    pvVar1 = local_68;
    pvVar3 = local_68;
    for (lVar7 = local_60 << 5; local_68 = pvVar3, lVar7 != 0; lVar7 = lVar7 + -0x20) {
      IVar5 = _objc_alloc(&objc::class_t::ArcSiteEngagementDetails);
      IVar5 = ArcSiteEngagementDetails::initWithSiteEngagementDetails_
                        (IVar5,"initWithSiteEngagementDetails:",pvVar1);
      NSMutableArray::addObject_(self,"addObject:",IVar5);
      pvVar1 = (const_void__)((long)pvVar1 + 0x20);
      pvVar3 = local_68;
    }
    if (pvVar3 != (const_void__)0x0) {
      if (local_60 * 0x20 != 0) {
        lVar7 = local_60 * -0x20;
        puVar2 = (undefined8 *)(local_60 * 0x20 + (long)pvVar3);
        do {
          puVar6 = puVar2 + -4;
          if (puVar6 == (undefined8 *)0x0) goto LAB_0442df7c;
          if (*(char *)((long)puVar2 + -9) < '\0') {
            FUN_00005810(*puVar6,puVar2[-2] & 0x7fffffffffffffff);
          }
          lVar7 = lVar7 + 0x20;
          puVar2 = puVar6;
        } while (lVar7 != 0);
      }
      local_60 = (long)pvVar3 - (long)local_68 >> 5;
      FUN_00005810(local_68,local_58 << 5);
    }
    return self;
  }
LAB_0442df7c:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442df80);
  (*pcVar4)();
}


