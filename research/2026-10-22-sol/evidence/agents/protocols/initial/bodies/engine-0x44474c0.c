// ==== 0x44474c0  FUN_044474c0  (152 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_044474c0(long param_1,undefined8 param_2,long param_3)

{
  ID self;
  undefined8 uVar1;
  undefined8 uVar2;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  if ((**(long **)(param_3 + 8) == 0) || (*(long *)(**(long **)(param_3 + 8) + 0x18) == 0)) {
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_02a02ccc();
  }
  _objc_msgSend(self,"webContents:didCaptureImage:",uVar1,uVar2);
  return 0;
}


