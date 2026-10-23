// ==== 0x1004f2470  _TtC3ADKP33_20D2BB56A1E5673B080299A2FAA53E4D11CommandShim::-[CommandShim_name]  (64 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID _TtC3ADKP33_20D2BB56A1E5673B080299A2FAA53E4D11CommandShim::__CommandShim_name_
             (ID param_1,SEL param_2)

{
  undefined8 uVar1;
  NSString *pNVar2;
  
  uVar1 = *(undefined8 *)(param_1 + 0x28);
  _swift_bridgeObjectRetain(uVar1);
  pNVar2 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  _swift_bridgeObjectRelease(uVar1);
  return (ID)pNVar2;
}


