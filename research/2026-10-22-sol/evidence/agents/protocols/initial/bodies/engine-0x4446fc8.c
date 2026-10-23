// ==== 0x4446fc8  FUN_04446fc8  (104 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_04446fc8(long param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend(*(ID *)(param_1 + 0x10),"webContentsWantsOverscrollSwipeEvents:",
                        *(undefined8 *)(param_1 + 8));
  return IVar1;
}


