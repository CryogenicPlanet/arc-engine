// ==== 0x444510c  FUN_0444510c  (88 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0444510c(undefined8 *param_1,const_FindResult__ param_2)

{
  ID IVar1;
  long lVar2;
  
  lVar2 = *(long *)*param_1;
  IVar1 = _objc_alloc(&objc::class_t::ArcFindResult);
  IVar1 = ArcFindResult::initWithFindResult_(IVar1,"initWithFindResult:",param_2);
  (**(code **)(lVar2 + 0x10))(lVar2,IVar1);
  return 0;
}


