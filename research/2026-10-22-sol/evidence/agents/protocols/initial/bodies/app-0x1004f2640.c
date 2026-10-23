// ==== 0x1004f2640  FUN_1004f2640  (136 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_1004f2640(long param_1,long param_2,undefined8 param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  String SVar3;
  
  pcVar1 = *(code **)(param_1 + 0x20);
  uVar2 = *(undefined8 *)(param_1 + 0x28);
  if (param_2 == 0) {
    SVar3 = (String)ZEXT816(0);
  }
  else {
    SVar3 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  }
  _swift_retain(uVar2);
  (*pcVar1)(SVar3.str,SVar3.bridgeObject,param_3);
  _swift_release(uVar2);
  _swift_bridgeObjectRelease(SVar3.bridgeObject);
  return;
}


