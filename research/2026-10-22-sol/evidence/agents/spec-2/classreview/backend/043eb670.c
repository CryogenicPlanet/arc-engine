// ==== 0x43eb670  FUN_043eb670  (88 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb670(undefined8 *param_1,const_void__ param_2)

{
  ID IVar1;
  long lVar2;
  
  lVar2 = *(long *)*param_1;
  IVar1 = _objc_alloc(&objc::class_t::ArcBreadcrumb);
  IVar1 = ArcBreadcrumb::initWithBreadcrumb_(IVar1,"initWithBreadcrumb:",param_2);
  (**(code **)(lVar2 + 0x10))(lVar2,IVar1);
  return 0;
}


