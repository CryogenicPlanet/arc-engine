// ==== 0x43ecb50  ArcBrowserContextAutocomplete::initWithAutocompleteController:  (176 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContextAutocomplete::initWithAutocompleteController_
             (ID param_1,SEL param_2,
             unique_ptr<arc::ArcAutocompleteController,_std::default_delete<arc::ArcAutocompleteController>>
             param_3)

{
  ID IVar1;
  long lVar2;
  long *plVar3;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBrowserContextAutocomplete;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 == 0) {
    if (param_3.field0_0x0 != (ArcAutocompleteController *)0x0) {
      lVar2 = *(long *)param_3.field0_0x0;
      *(undefined8 *)param_3.field0_0x0 = 0;
      if (lVar2 != 0) {
        FUN_042c33cc();
      }
      FUN_00005810(param_3.field0_0x0,8);
    }
  }
  else {
    plVar3 = *(long **)(IVar1 + 8);
    *(ArcAutocompleteController **)(IVar1 + 8) = param_3.field0_0x0;
    if (plVar3 != (long *)0x0) {
      lVar2 = *plVar3;
      *plVar3 = 0;
      if (lVar2 != 0) {
        FUN_042c33cc();
      }
      FUN_00005810(plVar3,8);
    }
  }
  return IVar1;
}


