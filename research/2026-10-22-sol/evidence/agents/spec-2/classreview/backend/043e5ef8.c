// ==== 0x43e5ef8  FUN_043e5ef8  (220 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043e5ef8(undefined8 *param_1,undefined8 *param_2)

{
  ID IVar1;
  ID IVar2;
  const_void__ pvVar3;
  long *plVar4;
  long lVar5;
  
  plVar4 = (long *)*param_1;
  IVar1 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
  lVar5 = param_2[1] << 4;
  if (lVar5 != 0) {
    pvVar3 = (const_void__)*param_2;
    do {
      IVar2 = _objc_alloc(&objc::class_t::ArcCookie);
      IVar2 = ArcCookie::initWithCookie_(IVar2,"initWithCookie:",pvVar3);
      NSMutableArray::addObject_(IVar1,"addObject:",IVar2);
      pvVar3 = (const_void__)((long)pvVar3 + 0x10);
      lVar5 = lVar5 + -0x10;
    } while (lVar5 != 0);
  }
  lVar5 = *plVar4;
  IVar1 = NSMutableArray::copy(IVar1,"copy");
  (**(code **)(lVar5 + 0x10))(lVar5,IVar1);
  return 0;
}


