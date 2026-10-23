// ==== 0x441b76c  FUN_0441b76c  (108 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441b76c(long param_1,undefined8 param_2,int *param_3)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"webContents:pipVideoResolutionChanged:",(double)*param_3,
                (double)param_3[1],*(undefined8 *)(param_1 + 0x10));
  return 0;
}


