// ==== 0x43e3908  FUN_043e3908  (132 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043e3908(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  ID self;
  undefined8 uVar1;
  undefined8 uVar2;
  
  self = *(ID *)(param_1 + 8);
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  uVar2 = FUN_01d9fb90(param_3,param_4);
  _objc_msgSend(self,"browser:didAskAbout:",uVar1,uVar2);
  return 0;
}


