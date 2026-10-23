// ==== 0x1004e9da8  FUN_1004e9da8  (56 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_1004e9da8(void)

{
  NSString *pNVar1;
  long lVar2;
  long unaff_x20;
  
  lVar2 = *(long *)(unaff_x20 + 0x10);
  pNVar1 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  (**(code **)(lVar2 + 0x10))(lVar2,pNVar1);
  return 0;
}


