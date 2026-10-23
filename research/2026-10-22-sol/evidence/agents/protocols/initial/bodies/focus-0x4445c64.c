// ==== 0x4445c64  FUN_04445c64  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04445c64(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  undefined8 uVar1;
  ID IVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcAddressEntrySaveRequest);
  IVar2 = ArcAddressEntrySaveRequest::initWithAddressEntrySaveRequest_
                    (IVar2,"initWithAddressEntrySaveRequest:",param_3);
  _objc_msgSend(self,"webContents:didSubmitAddressEntry:",uVar1,IVar2);
  return 0;
}


