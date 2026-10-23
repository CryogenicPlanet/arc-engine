// ==== 0x4445a58  FUN_04445a58  (164 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_04445a58(long param_1,undefined8 param_2,const_void__ param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  ID IVar3;
  ID IVar4;
  
  IVar4 = *(ID *)(param_1 + 0x10);
  uVar2 = *(undefined8 *)(param_1 + 8);
  IVar3 = _objc_alloc(&objc::class_t::ArcNavigationAction);
  IVar3 = ArcNavigationAction::initWithNavigationAction_(IVar3,"initWithNavigationAction:",param_3);
  IVar4 = _objc_msgSend(IVar4,"webContents:decidePolicyForNavigationAction:",uVar2,IVar3);
  if (IVar4 < 0xc) {
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4445afc);
  (*pcVar1)();
}


