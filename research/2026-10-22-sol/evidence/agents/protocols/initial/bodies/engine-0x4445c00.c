// ==== 0x4445c00  FUN_04445c00  (100 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04445c00(long param_1,undefined8 param_2,undefined8 param_3)

{
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContentsSelect:shouldActivate:",
                *(undefined8 *)(param_1 + 8),param_3);
  return 0;
}


