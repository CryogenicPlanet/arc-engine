// ==== 0x43f6154  FUN_043f6154  (104 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f6154(long param_1,const_void__ param_2)

{
  ID self;
  ID IVar1;
  
  self = *(ID *)(param_1 + 8);
  IVar1 = _objc_alloc(&objc::class_t::ArcImportSourceProfile);
  IVar1 = ArcImportSourceProfile::initWithImportSourceProfile_
                    (IVar1,"initWithImportSourceProfile:",param_2);
  _objc_msgSend(self,"didImportStart:",IVar1);
  return 0;
}


