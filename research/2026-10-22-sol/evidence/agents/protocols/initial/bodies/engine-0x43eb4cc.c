// ==== 0x43eb4cc  FUN_043eb4cc  (104 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb4cc(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  ID IVar1;
  
  self = *(ID *)(param_1 + 8);
  IVar1 = _objc_alloc(&objc::class_t::ArcDevToolsConnectionDialog);
  IVar1 = ArcDevToolsConnectionDialog::initWithDevToolsConnectionDialog_
                    (IVar1,"initWithDevToolsConnectionDialog:",param_3);
  _objc_msgSend(self,"didRequestDevToolsDebugging:",IVar1);
  return 0;
}


