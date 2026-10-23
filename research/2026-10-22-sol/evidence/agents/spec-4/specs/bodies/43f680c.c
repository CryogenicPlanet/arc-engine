// ==== 0x43f680c  FUN_043f680c  (192 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f680c(long param_1,undefined8 param_2,undefined8 *param_3,const_void__ param_4)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  ID IVar5;
  undefined8 uVar6;
  
  self = *(ID *)(param_1 + 8);
  uVar4 = *(undefined8 *)(param_1 + 0x10);
  IVar5 = _objc_alloc(&objc::class_t::ArcBrowserContextDevTools);
  IVar5 = ArcBrowserContextDevTools::initWithBrowserContextDevTools_
                    (IVar5,"initWithBrowserContextDevTools:",param_4);
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
  _objc_msgSend(self,"browserContext:showDevTools:for:",uVar4,IVar5,uVar6);
  return 0;
}


