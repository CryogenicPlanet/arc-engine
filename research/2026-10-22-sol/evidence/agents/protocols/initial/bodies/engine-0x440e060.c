// ==== 0x440e060  FUN_0440e060  (88 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0440e060(long param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 uVar4;
  ID self;
  
  self = *(ID *)(param_1 + 8);
  cVar3 = *(char *)((long)param_2 + 0x17);
  puVar1 = (undefined8 *)*param_2;
  if (-1 < (long)cVar3) {
    puVar1 = param_2;
  }
  lVar2 = param_2[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar4 = FUN_01c391bc(puVar1,lVar2);
  _objc_msgSend(self,"didFinish:",uVar4);
  return 0;
}


