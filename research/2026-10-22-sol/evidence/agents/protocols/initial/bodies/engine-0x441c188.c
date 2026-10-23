// ==== 0x441c188  FUN_0441c188  (112 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441c188(long param_1,undefined8 param_2,uint param_3)

{
  code *pcVar1;
  
  if (param_3 < 3) {
    _objc_msgSend(*(ID *)(param_1 + 8),"webContents:didChangeMicrophoneState:",
                  *(undefined8 *)(param_1 + 0x10),(ulong)param_3);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x441c1f8);
  (*pcVar1)();
}


