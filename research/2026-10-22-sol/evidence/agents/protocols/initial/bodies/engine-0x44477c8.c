// ==== 0x44477c8  FUN_044477c8  (220 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8
FUN_044477c8(long param_1,undefined8 param_2,undefined8 *param_3,undefined8 param_4,
            undefined8 *param_5,undefined8 param_6)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  self = *(ID *)(param_1 + 0x10);
  uVar4 = *(undefined8 *)(param_1 + 8);
  cVar3 = *(char *)((long)param_3 + 0x17);
  puVar1 = (undefined8 *)*param_3;
  if (-1 < (long)cVar3) {
    puVar1 = param_3;
  }
  lVar2 = param_3[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar5 = FUN_01c391bc(puVar1,lVar2);
  cVar3 = *(char *)((long)param_5 + 0x17);
  puVar1 = (undefined8 *)*param_5;
  if (-1 < (long)cVar3) {
    puVar1 = param_5;
  }
  lVar2 = param_5[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar6 = FUN_01c391bc(puVar1,lVar2);
  _objc_msgSend(self,
                "webContents:didDetermineSourceLanguage:isTranslateEnabled:targetLanguage:isTranslationPending:"
                ,uVar4,uVar5,param_4,uVar6,param_6);
  return 0;
}


