// ==== 0x1004fcb3c  _TtC3ADKP33_CC9CAABBB99C9291EF1375D1E90F27BC9EventShim::-[EventShim_name]  (64 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID _TtC3ADKP33_CC9CAABBB99C9291EF1375D1E90F27BC9EventShim::__EventShim_name_(ID param_1,SEL param_2)

{
  undefined8 uVar1;
  NSString *pNVar2;
  
  uVar1 = *(undefined8 *)(param_1 + 0x28);
  _swift_bridgeObjectRetain(uVar1);
  pNVar2 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  _swift_bridgeObjectRelease(uVar1);
  return (ID)pNVar2;
}


