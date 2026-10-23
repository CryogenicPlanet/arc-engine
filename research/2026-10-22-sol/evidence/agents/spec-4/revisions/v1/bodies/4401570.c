// ==== 0x4401570  ArcBrowserSidePanel::getEntryInfoForTabWithSessionId:  (348 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserSidePanel::getEntryInfoForTabWithSessionId_(ID param_1,SEL param_2,long_long param_3)

{
  ulong uVar1;
  long lVar2;
  ID IVar3;
  ID IVar4;
  const_void__ pvVar5;
  uint uVar6;
  undefined8 local_60;
  ulong uStack_58;
  long local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  const_void__ local_38;
  
  IVar3 = _objc_alloc_init(&objc::class_t::ArcBrowserSidePanelEntryInfo);
  uStack_48 = 0;
  local_50 = 0;
  local_38 = (const_void__)0x0;
  local_40 = 0;
  uStack_58 = 0;
  local_60 = 0;
  if (*(long *)(param_1 + 8) == 0) {
    uVar6 = 0;
    uStack_58 = 0;
    local_50 = 0;
    local_40 = 0;
    uStack_48 = 0;
  }
  else {
    FUN_04346bd8(&local_60,*(long *)(param_1 + 8),param_3);
    uVar6 = (uint)(byte)local_60;
  }
  AXAnnouncementSpec::setPolite_(IVar3,"setHasActiveEntry:",uVar6 & 1);
  uVar1 = uStack_58;
  if (-1 < (long)uStack_48._7_1_) {
    uVar1 = (ulong)&local_60 | 8;
  }
  lVar2 = local_50;
  if (-1 < (long)uStack_48) {
    lVar2 = (long)uStack_48._7_1_;
  }
  IVar4 = FUN_01c391bc(uVar1,lVar2);
  AXAnnouncementSpec::setAnnouncement_(IVar3,"setExtensionId:",IVar4);
  if (local_38 == (const_void__)0x0) {
    pvVar5 = (const_void__)0x0;
  }
  else {
    IVar4 = _objc_alloc(&objc::class_t::ArcWebContents);
    IVar4 = ArcWebContents::initWithWebContents_(IVar4,"initWithWebContents:",local_38);
    AXAnnouncementSpec::setWindow_(IVar3,"setCachedWebContents:",IVar4);
    pvVar5 = local_38;
  }
  if (((ulong)pvVar5 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  local_38 = (const_void__)0x0;
  if ((long)uStack_48 < 0) {
    FUN_00005810(uStack_58,uStack_48 & 0x7fffffffffffffff);
  }
  return IVar3;
}


