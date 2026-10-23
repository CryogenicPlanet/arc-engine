// ==== 0x43f60ec  FUN_043f60ec  (104 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f60ec(long param_1,const_void__ param_2)

{
  ID self;
  ID IVar1;
  
  self = *(ID *)(param_1 + 8);
  IVar1 = _objc_alloc(&objc::class_t::ArcImportSourceProfile);
  IVar1 = ArcImportSourceProfile::initWithImportSourceProfile_
                    (IVar1,"initWithImportSourceProfile:",param_2);
  _objc_msgSend(self,"didImportInitialize:",IVar1);
  return 0;
}


