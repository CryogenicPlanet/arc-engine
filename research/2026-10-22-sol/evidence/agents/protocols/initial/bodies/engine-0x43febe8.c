// ==== 0x43febe8  FUN_043febe8  (156 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043febe8(long param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend(*(ID *)(param_1 + 8),"respondsToSelector:",
                        "browserContextDidExtensionSystemReady:");
  if ((int)IVar1 == 0) {
    return 0;
  }
  _objc_msgSend(*(ID *)(param_1 + 8),"browserContextDidExtensionSystemReady:",
                *(undefined8 *)(param_1 + 0x10));
  return 0;
}


