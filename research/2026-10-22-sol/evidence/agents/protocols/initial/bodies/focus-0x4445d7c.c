// ==== 0x4445d7c  FUN_04445d7c  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04445d7c(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID self;
  undefined8 uVar1;
  ID IVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcCreditCardInfoSaveRequest);
  IVar2 = ArcCreditCardInfoSaveRequest::initWithCreditCardInfoSaveRequest_
                    (IVar2,"initWithCreditCardInfoSaveRequest:",param_3);
  _objc_msgSend(self,"webContents:didSubmitCreditCardInfo:",uVar1,IVar2);
  return 0;
}


