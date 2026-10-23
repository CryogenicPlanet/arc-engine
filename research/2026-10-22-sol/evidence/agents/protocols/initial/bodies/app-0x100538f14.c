// ==== 0x100538f14  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:willRunJavaScriptTextInputPanel:withMessage:withDefaultText:completion:]  (396 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_willRunJavaScriptTextInputPanel_withMessage_withDefaultText_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
               undefined4 param_8)

{
  void *pvVar1;
  undefined *puVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  String SVar6;
  String SVar7;
  String SVar8;
  undefined1 auStack_78 [24];
  
  pvVar1 = __Block_copy((void *)param_7);
  SVar6 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar7 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  SVar8 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  puVar2 = &DAT_10418d4f8;
  _swift_allocObject(&DAT_10418d4f8,0x18,7);
  *(void **)(puVar2 + 0x10) = pvVar1;
  lVar4 = param_1 + uiDelegate;
  _swift_beginAccess(lVar4,auStack_78,0,0);
  lVar3 = lVar4;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar3 == 0) {
    _swift_release(puVar2);
  }
  else {
    lVar5 = *(long *)(lVar4 + 8);
    lVar4 = lVar3;
    _swift_getObjectType();
    (**(code **)(lVar5 + 0x220))
              (param_1,&PTR_PTR_10418d648,SVar6.str,SVar6.bridgeObject,SVar7.str,SVar7.bridgeObject,
               SVar8.str,SVar8.bridgeObject,FUN_10053b67c,puVar2,lVar4,lVar5);
    _swift_unknownObjectRelease(lVar3);
    _swift_release(puVar2);
  }
  _swift_bridgeObjectRelease(SVar6.bridgeObject);
  _swift_bridgeObjectRelease(SVar7.bridgeObject);
  _swift_bridgeObjectRelease(SVar8.bridgeObject);
  return;
}


