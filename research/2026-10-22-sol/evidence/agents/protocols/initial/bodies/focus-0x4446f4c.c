// ==== 0x4446f4c  FUN_04446f4c  (124 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04446f4c(long param_1,undefined8 param_2,int param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  
  if (param_3 == 0) {
    uVar2 = 0;
  }
  else {
    if (param_3 != 1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4446fc8);
      (*pcVar1)();
    }
    uVar2 = 1;
  }
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContents:willExternallyNavigate:",
                *(undefined8 *)(param_1 + 8),uVar2);
  return 0;
}


