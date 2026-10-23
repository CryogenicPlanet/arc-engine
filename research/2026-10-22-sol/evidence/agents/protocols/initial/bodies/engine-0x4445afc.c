// ==== 0x4445afc  FUN_04445afc  (176 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04445afc(long param_1,undefined8 param_2,uint *param_3,undefined8 param_4)

{
  ID self;
  undefined8 uVar1;
  undefined8 uVar2;
  uint local_38;
  undefined1 local_34;
  
  self = *(ID *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  local_38 = local_38 & 0xffffff00;
  local_34 = (char)param_3[1] != '\0';
  if ((bool)local_34) {
    local_38 = *param_3;
  }
  uVar2 = FUN_04401bc0(&local_38);
  _objc_msgSend(self,"webContents:didSampleTopBandColor:shouldCrossfade:",uVar1,uVar2,param_4);
  return 0;
}


