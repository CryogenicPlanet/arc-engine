// ==== 0x1004b7178  _TtC3ADK14ADKApplication::-[ADKApplication_didCloseBrowser:forBrowserContextID:ofType:withRestorationSessionID:]  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void _TtC3ADK14ADKApplication::
     __ADKApplication_didCloseBrowser_forBrowserContextID_ofType_withRestorationSessionID__
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5,ID param_6)

{
  String SVar1;
  
  if (param_3 == 0) {
    SVar1 = (String)ZEXT816(0);
  }
  else {
    SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  }
  FUN_1004b6e64(SVar1.str,SVar1.bridgeObject,param_4,param_5,param_6);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  return;
}


