// ==== 0x43e35d4  FUN_043e35d4  (168 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043e35d4(long param_1,undefined8 param_2,undefined8 *param_3,int param_4,int param_5)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
  
  self = *(ID *)(param_1 + 8);
  uVar4 = *(undefined8 *)(param_1 + 0x10);
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
  _objc_msgSend(self,"browser:didResizeExtensionPopup:size:",(double)param_4,(double)param_5,uVar4,
                uVar5);
  return 0;
}


