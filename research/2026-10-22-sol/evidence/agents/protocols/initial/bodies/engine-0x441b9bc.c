// ==== 0x441b9bc  FUN_0441b9bc  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441b9bc(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  undefined8 uVar1;
  ID IVar2;
  
  self = *(ID *)(param_1 + 8);
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  IVar2 = _objc_alloc(&objc::class_t::ArcMediaPosition);
  IVar2 = ArcMediaPosition::initWithMediaPosition_(IVar2,"initWithMediaPosition:",param_3);
  _objc_msgSend(self,"webContents:pipMediaPositionChanged:",uVar1,IVar2);
  return 0;
}


