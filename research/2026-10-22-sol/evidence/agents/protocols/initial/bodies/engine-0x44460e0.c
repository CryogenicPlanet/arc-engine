// ==== 0x44460e0  FUN_044460e0  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_044460e0(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  undefined8 uVar1;
  ID IVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcLoginCredentialsSaveRequest);
  IVar2 = ArcLoginCredentialsSaveRequest::initWithLoginCredentialsSaveRequest_
                    (IVar2,"initWithLoginCredentialsSaveRequest:",param_3);
  _objc_msgSend(self,"webContents:didSubmitCredentials:",uVar1,IVar2);
  return 0;
}


