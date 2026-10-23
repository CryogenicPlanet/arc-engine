// ==== 0x43f7ab8  FUN_043f7ab8  (256 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_043f7ab8(long param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4,
               undefined8 param_5)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ID IVar8;
  
  lVar4 = FUN_043abd34(param_5);
  if (lVar4 == 0) {
    IVar8 = 0;
  }
  else {
    IVar8 = *(ID *)(param_1 + 8);
    uVar5 = *(undefined8 *)(param_1 + 0x10);
    cVar3 = *(char *)((long)param_3 + 0x17);
    puVar1 = (undefined8 *)*param_3;
    if (-1 < (long)cVar3) {
      puVar1 = param_3;
    }
    lVar2 = param_3[1];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar6 = FUN_01c391bc(puVar1,lVar2);
    cVar3 = *(char *)((long)param_4 + 0x17);
    puVar1 = (undefined8 *)*param_4;
    if (-1 < (long)cVar3) {
      puVar1 = param_4;
    }
    lVar2 = param_4[1];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar7 = FUN_01c391bc(puVar1,lVar2);
    IVar8 = _objc_msgSend(IVar8,
                          "browserContext:shouldAllowArtifactNavigationToURL:forSessionID:folderName:"
                          ,uVar5,lVar4,uVar6,uVar7);
  }
  return IVar8;
}


