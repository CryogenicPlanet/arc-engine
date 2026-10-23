// ==== 0x4445e08  FUN_04445e08  (152 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04445e08(long param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
  
  self = *(ID *)(param_1 + 0x10);
  uVar4 = *(undefined8 *)(param_1 + 8);
  cVar3 = *(char *)((long)param_4 + 0x17);
  puVar1 = (undefined8 *)*param_4;
  if (-1 < (long)cVar3) {
    puVar1 = param_4;
  }
  lVar2 = param_4[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar5 = FUN_01c391bc(puVar1,lVar2);
  _objc_msgSend(self,"webContentsDidRequestEnterFullscreen:withDisplayId:requestingOrigin:",uVar4,
                param_3,uVar5);
  return 0;
}


