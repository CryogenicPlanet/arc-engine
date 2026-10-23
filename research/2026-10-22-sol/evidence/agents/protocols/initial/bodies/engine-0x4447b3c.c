// ==== 0x4447b3c  FUN_04447b3c  (84 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04447b3c(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContentsDidCloseWebLoginDialog:",
                *(undefined8 *)(param_1 + 8));
  return 0;
}


