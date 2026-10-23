// ==== 0x444811c  FUN_0444811c  (156 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0444811c(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  ID self;
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  uVar2 = FUN_043abd34(param_3);
  uVar3 = FUN_043abd34(param_4);
  _objc_msgSend(self,"webContents:didDestroyWebUI:newURL:",uVar1,uVar2,uVar3);
  return 0;
}


