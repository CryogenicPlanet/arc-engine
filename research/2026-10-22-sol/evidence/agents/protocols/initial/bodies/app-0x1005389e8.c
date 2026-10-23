// ==== 0x1005389e8  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContentsDidRequestEnterFullscreen:withDisplayId:requestingOrigin:]  (280 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContentsDidRequestEnterFullscreen_withDisplayId_requestingOrigin__
               (ID param_1,SEL param_2,ID param_3,long_long param_4,ID param_5)

{
  long_long lVar1;
  long lVar2;
  long lVar3;
  void *pvVar4;
  long lVar5;
  String SVar6;
  undefined1 auStack_78 [24];
  
  SVar6 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  pvVar4 = SVar6.bridgeObject;
  lVar5 = param_1 + uiDelegate;
  _swift_beginAccess(lVar5,auStack_78,0,0);
  lVar2 = lVar5;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar2 == 0) {
    _swift_bridgeObjectRelease(pvVar4);
  }
  else {
    lVar5 = *(long *)(lVar5 + 8);
    lVar1 = 0;
    if (param_4 != 0xffffffffffffffff) {
      lVar1 = param_4;
    }
    lVar3 = lVar2;
    _swift_getObjectType();
    (**(code **)(lVar5 + 0x30))
              (param_1,&PTR_PTR_10418d648,lVar1,param_4 == 0xffffffffffffffff,SVar6.str,pvVar4,lVar3
               ,lVar5);
    _swift_bridgeObjectRelease(pvVar4);
    _swift_unknownObjectRelease(lVar2);
  }
  return;
}


