// ==== 0x101360f8c  _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::-[BrowserExtensionsLifeCycleManager_browserContext:didFail:withRuntimeError:]  (204 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC13ADKClientLive33BrowserExtensionsLifeCycleManager::
     __BrowserExtensionsLifeCycleManager_browserContext_didFail_withRuntimeError__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  void *pvVar1;
  String local_68;
  ID local_58;
  undefined8 uStack_50;
  undefined1 local_48;
  
  FUN_101360d20(param_1);
  objc_stub::FUN_1035baea0(param_5);
  local_68 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  pvVar1 = local_68.bridgeObject;
  uStack_50 = 0;
  local_48 = 0x80;
  local_58 = param_5;
  Combine::PassthroughSubject::send((PassthroughSubject *)param_1,&local_68);
  _swift_bridgeObjectRelease(pvVar1);
  _swift_release(param_1);
  return;
}


