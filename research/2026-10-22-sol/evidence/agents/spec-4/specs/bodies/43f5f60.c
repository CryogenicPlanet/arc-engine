// ==== 0x43f5f60  objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders:  (352 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders_
             (ID param_1,SEL param_2,unsigned_long_long param_3)

{
  long lVar1;
  undefined8 uVar2;
  unique_ptr<arc::ArcAutocompleteController,_std::default_delete<arc::ArcAutocompleteController>>
  uVar3;
  ID IVar4;
  code *pcVar5;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43f60c0);
    (*pcVar5)();
  }
  uVar2 = FUN_00008790(0x78);
  FUN_042c2f5c(uVar2,*(undefined8 *)(local_48 + 0x88),(uint)param_3 & 3);
  uVar3.field0_0x0 = (ArcAutocompleteController *)FUN_00008790(8);
  *(undefined8 *)uVar3.field0_0x0 = uVar2;
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f5fdc;
      }
      pcVar5 = *(code **)(*local_40 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar5)(local_40);
      }
    }
  }
LAB_043f5fdc:
  IVar4 = _objc_alloc(&class_t::ArcBrowserContextAutocomplete);
  if (IVar4 == 0) {
    if (uVar3.field0_0x0 != (ArcAutocompleteController *)0x0) {
      lVar1 = *(long *)uVar3.field0_0x0;
      *(undefined8 *)uVar3.field0_0x0 = 0;
      if (lVar1 != 0) {
        FUN_042c33cc();
      }
      FUN_00005810(uVar3.field0_0x0,8);
    }
    IVar4 = 0;
  }
  else {
    IVar4 = ArcBrowserContextAutocomplete::initWithAutocompleteController_
                      (IVar4,"initWithAutocompleteController:",uVar3);
  }
  return IVar4;
}


