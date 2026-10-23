// ==== 0x4447bf4  FUN_04447bf4  (152 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_04447bf4(long param_1,undefined8 param_2,const_void__ param_3)

{
  undefined8 uVar1;
  ID IVar2;
  ID IVar3;
  
  IVar3 = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcHungRendererDialog);
  IVar2 = ArcHungRendererDialog::initWithHungRendererDialog_
                    (IVar2,"initWithHungRendererDialog:",param_3);
  IVar3 = _objc_msgSend(IVar3,"webContents:showHungRendererDialog:",uVar1,IVar2);
  return IVar3;
}


