// ==== 0x1004f1f78  _TtC3ADK23CrashKeysObserverBridge::-[CrashKeysObserverBridge_onCrashKeyEmitted:value:]  (132 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK23CrashKeysObserverBridge::__CrashKeysObserverBridge_onCrashKeyEmitted_value__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  String SVar1;
  String SVar2;
  
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar2 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_1004f1db8(SVar1.str,SVar1.bridgeObject,SVar2.str,SVar2.bridgeObject);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  _swift_bridgeObjectRelease(SVar2.bridgeObject);
  return;
}


