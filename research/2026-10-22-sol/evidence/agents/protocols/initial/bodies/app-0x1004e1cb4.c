// ==== 0x1004e1cb4  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:allowedArtifactImageSourcesForSessionID:folderName:]  (208 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

ID _TtC3ADK28BrowserContextDelegateBridge::
   __BrowserContextDelegateBridge_browserContext_allowedArtifactImageSourcesForSessionID_folderName__
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  NSArray *pNVar1;
  String SVar2;
  String SVar3;
  
  SVar2 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar3 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_1004e1aa4(param_3,SVar2.str,SVar2.bridgeObject,SVar3.str,SVar3.bridgeObject);
  _swift_bridgeObjectRelease(SVar2.bridgeObject);
  _swift_bridgeObjectRelease(SVar3.bridgeObject);
  pNVar1 = (extension_Foundation)::Swift::Array<undefined>::_bridgeToObjectiveC();
  _swift_bridgeObjectRelease(param_3);
  return (ID)pNVar1;
}


