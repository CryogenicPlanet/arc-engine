// ==== 0x43fe83c  FUN_043fe83c  (192 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043fe83c(long param_1,undefined8 param_2,undefined8 *param_3,const_void__ param_4)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
  ID IVar6;
  
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
  IVar6 = _objc_alloc(&objc::class_t::ArcExtensionRuntimeError);
  IVar6 = ArcExtensionRuntimeError::initWithExtensionRuntimeError_
                    (IVar6,"initWithExtensionRuntimeError:",param_4);
  _objc_msgSend(self,"browserContext:didFail:withRuntimeError:",uVar4,uVar5,IVar6);
  return 0;
}


