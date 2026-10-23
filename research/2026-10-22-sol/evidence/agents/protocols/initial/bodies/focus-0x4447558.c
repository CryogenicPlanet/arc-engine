// ==== 0x4447558  FUN_04447558  (100 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04447558(long param_1,undefined8 param_2,undefined8 *param_3)

{
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContents:showsAuxiliaryWindow:",
                *(undefined8 *)(param_1 + 8),*param_3);
  return 0;
}


