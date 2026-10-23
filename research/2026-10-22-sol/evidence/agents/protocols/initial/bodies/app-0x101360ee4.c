// ==== 0x101360ee4  _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::-[BrowserExtensionsLifeCycleManager_browserContext:didFailLoadUnpacked:withManifestError:]  (168 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::
     __BrowserExtensionsLifeCycleManager_browserContext_didFailLoadUnpacked_withManifestError__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  String SVar1;
  String SVar2;
  String local_68;
  String local_58;
  undefined1 local_48;
  
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar2 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_101360d20(param_1);
  local_48 = 0x83;
  local_68 = SVar1;
  local_58 = SVar2;
  Combine::PassthroughSubject::send((PassthroughSubject *)param_1,&local_68);
  _swift_bridgeObjectRelease(SVar2.bridgeObject);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  _swift_release(param_1);
  return;
}


