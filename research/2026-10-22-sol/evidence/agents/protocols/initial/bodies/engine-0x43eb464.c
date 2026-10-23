// ==== 0x43eb464  FUN_043eb464  (104 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb464(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID IVar1;
  
  IVar1 = _objc_alloc(&objc::class_t::ArcBrowserContextAttributes);
  IVar1 = ArcBrowserContextAttributes::initWithBrowserContextAttributes_
                    (IVar1,"initWithBrowserContextAttributes:",param_3);
  _objc_msgSend(*(ID *)(param_1 + 8),"didChangeBrowserContextAttributes:",IVar1);
  return 0;
}


