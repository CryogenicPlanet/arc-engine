// ==== 0x1004aa758  _TtC3ADK21AccessibilityObserver::-[AccessibilityObserver_accessibilityModeChanged:]  (112 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtC3ADK21AccessibilityObserver::__AccessibilityObserver_accessibilityModeChanged__
               (ID param_1,SEL param_2,ID param_3)

{
  String SVar1;
  
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  (**(code **)(param_1 + closure))(SVar1.str,SVar1.bridgeObject);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  return;
}


