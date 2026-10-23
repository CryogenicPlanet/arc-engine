// ==== 0x441604c  FUN_0441604c  (108 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441604c(long param_1,undefined8 param_2,int param_3)

{
  long lVar1;
  
  lVar1 = 0;
  if (param_3 - 1U < 4) {
    lVar1 = (ulong)(param_3 - 1U) + 1;
  }
  _objc_msgSend(*(ID *)(param_1 + 8),"webContentsDevTools:inspectModeDidChange:",
                *(undefined8 *)(param_1 + 0x10),lVar1);
  return 0;
}


