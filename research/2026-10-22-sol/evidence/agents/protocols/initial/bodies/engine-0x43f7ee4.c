// ==== 0x43f7ee4  FUN_043f7ee4  (84 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f7ee4(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"didDestroy:",*(undefined8 *)(param_1 + 0x10));
  return 0;
}


