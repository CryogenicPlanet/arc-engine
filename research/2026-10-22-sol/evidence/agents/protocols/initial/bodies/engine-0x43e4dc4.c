// ==== 0x43e4dc4  FUN_043e4dc4  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_043e4dc4(long param_1,undefined8 param_2,int param_3)

{
  ID IVar1;
  
  if (*(long *)(param_1 + 8) == 0) {
    IVar1 = 0xffffffff;
  }
  else {
    IVar1 = _objc_msgSend(*(ID *)(param_1 + 8),"browser:extensionGroupIdForTabWithSessionId:",
                          *(undefined8 *)(param_1 + 0x10),(long)param_3);
  }
  return IVar1;
}


