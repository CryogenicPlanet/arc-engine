// ==== 0x4042484  FUN_04042484  (248 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */

undefined8 FUN_04042484(long param_1,undefined8 param_2,undefined8 *param_3)

{
  ID self;
  ID IVar1;
  const_void__ pvVar2;
  long lVar3;
  
  self = NSMutableArray::array((ID)&_OBJC_CLASS___NSMutableArray,"array");
  lVar3 = param_3[1] << 4;
  if (lVar3 != 0) {
    pvVar2 = (const_void__)*param_3;
    do {
      IVar1 = _objc_alloc(&objc::class_t::ArcDownloadItem);
      IVar1 = ArcDownloadItem::initWithDownloadItem_(IVar1,"initWithDownloadItem:",pvVar2);
      NSMutableArray::addObject_(self,"addObject:",IVar1);
      pvVar2 = (const_void__)((long)pvVar2 + 0x10);
      lVar3 = lVar3 + -0x10;
    } while (lVar3 != 0);
  }
  _objc_msgSend(*(ID *)(param_1 + 8),"browserContext:downloadManagerInitializedWithItems:",
                *(undefined8 *)(param_1 + 0x10),self);
  return 0;
}


