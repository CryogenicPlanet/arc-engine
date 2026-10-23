// ==== 0x443c924  FUN_0443c924  (68 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0443c924(undefined8 *param_1,undefined8 param_2)

{
  if (*(ID *)*param_1 != 0) {
    _objc_msgSend(*(ID *)*param_1,"onBatterySaverModeChanged:",param_2);
  }
  return 0;
}


