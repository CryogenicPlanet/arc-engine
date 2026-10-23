// ==== 0x441b638  FUN_0441b638  (84 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441b638(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"webContentsDidClickPipSettingsButton:",
                *(undefined8 *)(param_1 + 0x10));
  return 0;
}


