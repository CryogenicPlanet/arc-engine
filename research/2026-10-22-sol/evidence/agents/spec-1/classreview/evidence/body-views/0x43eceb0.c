// ==== 0x43eceb0  FUN_043eceb0  (220 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eceb0(undefined8 *param_1,undefined8 *param_2,undefined8 param_3)

{
  ID self;
  ID IVar1;
  const_void__ pvVar2;
  long *plVar3;
  long lVar4;
  
  plVar3 = (long *)*param_1;
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",param_2[1]);
  lVar4 = param_2[1] << 4;
  if (lVar4 != 0) {
    pvVar2 = (const_void__)*param_2;
    do {
      IVar1 = _objc_alloc(&objc::class_t::ArcAutocompleteMatch);
      IVar1 = ArcAutocompleteMatch::initWithAutocompleteMatch_
                        (IVar1,"initWithAutocompleteMatch:",pvVar2);
      NSMutableArray::addObject_(self,"addObject:",IVar1);
      pvVar2 = (const_void__)((long)pvVar2 + 0x10);
      lVar4 = lVar4 + -0x10;
    } while (lVar4 != 0);
  }
  (**(code **)(*plVar3 + 0x10))(*plVar3,self,param_3);
  return 0;
}


