// ==== 0x100536f1c  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:showTranslateBubble:withTargetLanguage:]  (256 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_showTranslateBubble_withTargetLanguage__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  long lVar1;
  long lVar2;
  long lVar3;
  String SVar4;
  String SVar5;
  undefined1 auStack_78 [24];
  
  SVar4 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar5 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  lVar2 = param_1 + uiDelegate;
  _swift_beginAccess(lVar2,auStack_78,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x168))
              (param_1,&PTR_PTR_10418d648,SVar4.str,SVar4.bridgeObject,SVar5.str,SVar5.bridgeObject,
               lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  _swift_bridgeObjectRelease(SVar4.bridgeObject);
  _swift_bridgeObjectRelease(SVar5.bridgeObject);
  return;
}


