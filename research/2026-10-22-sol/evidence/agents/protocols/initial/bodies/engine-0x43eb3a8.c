// ==== 0x43eb3a8  FUN_043eb3a8  (76 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb3a8(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"didReceiveGPUCrashNotification:withFallbackCount:",param_3,
                param_4);
  return 0;
}


