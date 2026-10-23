// ==== 0x100537180  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:showTranslateComplete:]  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_showTranslateComplete__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  String SVar4;
  undefined1 auStack_68 [24];
  
  SVar4 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  lVar2 = param_1 + uiDelegate;
  _swift_beginAccess(lVar2,auStack_68,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x180))
              (param_1,&PTR_PTR_10418d648,SVar4.str,SVar4.bridgeObject,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  _swift_bridgeObjectRelease(SVar4.bridgeObject);
  return;
}


