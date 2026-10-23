// ==== 0x44480a0  FUN_044480a0  (124 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_044480a0(long param_1,undefined8 param_2,undefined8 param_3)

{
  ID self;
  undefined8 uVar1;
  undefined8 uVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  uVar2 = FUN_043abd34(param_3);
  _objc_msgSend(self,"webContents:didShowWebUI:",uVar1,uVar2);
  return 0;
}


