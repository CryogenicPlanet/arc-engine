// ==== 0x43eb430  FUN_043eb430  (52 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb430(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"didQuitRunLoop");
  return 0;
}


