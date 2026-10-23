// ==== 0x101360db8  _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::-[BrowserExtensionsLifeCycleManager_browserContext:didInstallExtension:]  (140 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::
     __BrowserExtensionsLifeCycleManager_browserContext_didInstallExtension__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  String SVar1;
  String local_58;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined1 local_38;
  
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_101360d20(param_1);
  local_48 = 0;
  uStack_40 = 0;
  local_38 = 0;
  local_58 = SVar1;
  _swift_bridgeObjectRetain(SVar1.bridgeObject);
  Combine::PassthroughSubject::send((PassthroughSubject *)param_1,&local_58);
  _swift_release(param_1);
  _swift_bridgeObjectRelease_n(SVar1.bridgeObject,2);
  return;
}


