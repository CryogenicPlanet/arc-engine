// ==== 0x43fea84  FUN_043fea84  (356 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */

undefined8 FUN_043fea84(long param_1,undefined8 param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  undefined8 uVar5;
  ID self;
  long lVar6;
  
  IVar4 = _objc_msgSend(*(ID *)(param_1 + 8),"respondsToSelector:",
                        "browserContext:didChangePinnedExtensionOrder:");
  if ((int)IVar4 == 0) {
    return 0;
  }
  IVar4 = NSMutableArray::arrayWithCapacity_
                    ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",param_3[1]);
  lVar6 = param_3[1] * 0x18;
  if (lVar6 != 0) {
    param_3 = (undefined8 *)*param_3;
    do {
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
      NSMutableArray::addObject_(IVar4,"addObject:",uVar5);
      param_3 = param_3 + 3;
      lVar6 = lVar6 + -0x18;
    } while (lVar6 != 0);
  }
  self = *(ID *)(param_1 + 8);
  uVar5 = *(undefined8 *)(param_1 + 0x10);
  IVar4 = NSMutableArray::copy(IVar4,"copy");
  _objc_msgSend(self,"browserContext:didChangePinnedExtensionOrder:",uVar5,IVar4);
  return 0;
}


