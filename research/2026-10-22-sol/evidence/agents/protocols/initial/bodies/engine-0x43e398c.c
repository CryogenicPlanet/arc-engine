// ==== 0x43e398c  FUN_043e398c  (124 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043e398c(long param_1,undefined8 param_2,undefined8 param_3)

{
  long lVar1;
  
  lVar1 = FUN_043abd34(param_3);
  if (lVar1 != 0) {
    _objc_msgSend(*(ID *)(param_1 + 8),"browser:didAskAboutImage:",*(undefined8 *)(param_1 + 0x10),
                  lVar1);
  }
  return 0;
}


