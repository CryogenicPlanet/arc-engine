// ==== 0x44374c0  ArcBrowserContext::preferences  (332 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::preferences(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"preferences");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContextPreferences);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x443760c);
    (*pcVar3)();
  }
  FUN_042c8400(auStack_50,*(undefined8 *)(*(long *)(param_1 + 0x30) + 0x140));
  IVar2 = ArcBrowserContextPreferences::initWithBrowserContextPreferences_forBrowserContext_
                    (IVar2,"initWithBrowserContextPreferences:forBrowserContext:",auStack_50,param_1
                    );
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437554;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04437554:
  _objc_setAssociatedObject(param_1,"preferences",IVar2,1);
  return IVar2;
}


