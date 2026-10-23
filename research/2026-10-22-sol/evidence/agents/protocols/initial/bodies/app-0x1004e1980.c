// ==== 0x1004e1980  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:shouldAllowArtifactNavigationToURL:forSessionID:folderName:]  (292 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

bool _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_shouldAllowArtifactNavigationToURL_forSessionID_folderName__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6)

{
  URL UVar1;
  long extraout_x8;
  long lVar2;
  String SVar3;
  String SVar4;
  
  UVar1 = Foundation::URL::typeMetadataAccessor();
  lVar2 = *(long *)(UVar1.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar2 + 0x40));
  Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
  SVar3 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar4 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_1004e1774(param_3,&stack0xffffffffffffffa0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),
                SVar3.str,SVar3.bridgeObject,SVar4.str,SVar4.bridgeObject);
  _swift_bridgeObjectRelease(SVar3.bridgeObject);
  _swift_bridgeObjectRelease(SVar4.bridgeObject);
  (**(code **)(lVar2 + 8))
            (&stack0xffffffffffffffa0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),UVar1.unknown);
  return (uint)param_3 & 1;
}


