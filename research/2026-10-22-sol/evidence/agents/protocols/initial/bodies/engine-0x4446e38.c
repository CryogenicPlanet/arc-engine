// ==== 0x4446e38  FUN_04446e38  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04446e38(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  undefined8 uVar1;
  ID IVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcSecureContext);
  IVar2 = ArcSecureContext::initWithSecureContext_(IVar2,"initWithSecureContext:",param_3);
  _objc_msgSend(self,"webContents:didChangeVisibleSecurityState:",uVar1,IVar2);
  return 0;
}


