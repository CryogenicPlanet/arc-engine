// ==== 0x43e3798  FUN_043e3798  (248 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */

undefined8 FUN_043e3798(long param_1,undefined8 param_2,undefined8 *param_3)

{
  ID self;
  ID IVar1;
  const_void__ pvVar2;
  long lVar3;
  
  self = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
  lVar3 = param_3[1] * 0x70;
  if (lVar3 != 0) {
    pvVar2 = (const_void__)*param_3;
    do {
      IVar1 = _objc_alloc(&objc::class_t::ArcExtensionToolbarAction);
      IVar1 = ArcExtensionToolbarAction::initWithExtensionToolbarAction_
                        (IVar1,"initWithExtensionToolbarAction:",pvVar2);
      NSMutableArray::addObject_(self,"addObject:",IVar1);
      pvVar2 = (const_void__)((long)pvVar2 + 0x70);
      lVar3 = lVar3 + -0x70;
    } while (lVar3 != 0);
  }
  _objc_msgSend(*(ID *)(param_1 + 8),"browser:didUpdateExtensionToolbarActions:",
                *(undefined8 *)(param_1 + 0x10),self);
  return 0;
}


