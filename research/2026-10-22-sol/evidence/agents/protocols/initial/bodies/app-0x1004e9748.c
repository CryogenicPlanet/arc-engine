// ==== 0x1004e9748  _TtC3ADK36BrowserContextDownloadDelegateBridge::-[BrowserContextDownloadDelegateBridge_browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:]  (200 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void _TtC3ADK36BrowserContextDownloadDelegateBridge::
     __BrowserContextDownloadDelegateBridge_browserContext_onDeterminingFilename_defaultFilename_completionTimeout_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,double param_6,ID param_7,
               undefined4 param_8)

{
  void *aBlock;
  String SVar1;
  
  aBlock = __Block_copy((void *)param_7);
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  __Block_copy(aBlock);
  FUN_1004e9ac4(param_3,param_4,SVar1.str,SVar1.bridgeObject,param_1,aBlock);
  __Block_release(aBlock);
  __Block_release(aBlock);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  return;
}


