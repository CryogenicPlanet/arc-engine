// ==== 0x4447ce0  FUN_04447ce0  (84 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04447ce0(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContentsProcessGone:",*(undefined8 *)(param_1 + 8));
  return 0;
}


