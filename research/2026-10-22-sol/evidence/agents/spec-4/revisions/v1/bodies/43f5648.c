// ==== 0x43f5648  objc::Categories::ArcBrowserContext_Extensions_::metrics  (512 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::metrics(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"metrics");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&class_t::ArcMetrics);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5848);
    (*pcVar3)();
  }
  FUN_04298600(auStack_50,*(undefined8 *)(local_68 + 0x120));
  IVar2 = ArcMetrics::initWithBrowserContextMetrics_
                    (IVar2,"initWithBrowserContextMetrics:",auStack_50);
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
        if (lVar1 != 0) goto LAB_043f56ec;
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
LAB_043f56ec:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f570c;
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
LAB_043f570c:
  _objc_setAssociatedObject(param_1,"metrics",IVar2,1);
  return IVar2;
}


