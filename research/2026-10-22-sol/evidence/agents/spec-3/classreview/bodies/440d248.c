// ==== 0x440d248  FUN_0440d248  (84 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0440d248(long param_1)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"tracingControllerDidStartTracing:",
                *(undefined8 *)(param_1 + 0x10));
  return 0;
}


