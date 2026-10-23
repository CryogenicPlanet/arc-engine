// ==== 0x443b028  ArcBrowserContextAttributes::userColor  (84 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextAttributes::userColor(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = *(ID *)(param_1 + 8);
  if (IVar1 == 0) {
    IVar1 = _objc_alloc(&objc::class_t::ArcColor);
    IVar1 = ArcColor::initWithColor_(IVar1,"initWithColor:",(const_Color__)(param_1 + 0x40));
    *(ID *)(param_1 + 8) = IVar1;
    IVar1 = *(ID *)(param_1 + 8);
  }
  return IVar1;
}


