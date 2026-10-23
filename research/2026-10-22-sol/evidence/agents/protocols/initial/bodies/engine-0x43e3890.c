// ==== 0x43e3890  FUN_043e3890  (120 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043e3890(long param_1,undefined8 param_2,undefined8 param_3,uint param_4)

{
  code *pcVar1;
  
  if (param_4 < 3) {
    _objc_msgSend(*(ID *)(param_1 + 8),"browser:didChangeFullscreenState:initiatedBy:",
                  *(undefined8 *)(param_1 + 0x10),param_3,(ulong)param_4);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e3908);
  (*pcVar1)();
}


