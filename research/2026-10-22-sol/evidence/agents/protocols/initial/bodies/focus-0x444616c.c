// ==== 0x444616c  FUN_0444616c  (208 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_0444616c(long param_1,undefined8 param_2,const_void__ param_3,const_void__ param_4)

{
  undefined8 uVar1;
  ID IVar2;
  ID IVar3;
  ID IVar4;
  
  IVar4 = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcMenuModel);
  IVar2 = ArcMenuModel::initWithMenuModel_(IVar2,"initWithMenuModel:",param_3);
  IVar3 = _objc_alloc(&objc::class_t::ArcContextMenuElementInfo);
  IVar3 = ArcContextMenuElementInfo::initWithContextMenuElementInfo_
                    (IVar3,"initWithContextMenuElementInfo:",param_4);
  IVar4 = _objc_msgSend(IVar4,"webContents:willShowContextMenu:withElement:",uVar1,IVar2,IVar3);
  return IVar4;
}


