// ==== 0x4446ec4  FUN_04446ec4  (136 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_04446ec4(long param_1,undefined8 param_2,int param_3)

{
  code *pcVar1;
  ID IVar2;
  undefined8 uVar3;
  
  if (param_3 == 0) {
    uVar3 = 0;
  }
  else {
    if (param_3 != 1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4446f4c);
      (*pcVar1)();
    }
    uVar3 = 1;
  }
  IVar2 = _objc_msgSend(*(ID *)(param_1 + 0x10),"webContents:canExternallyNavigate:",
                        *(undefined8 *)(param_1 + 8),uVar3);
  return IVar2;
}


