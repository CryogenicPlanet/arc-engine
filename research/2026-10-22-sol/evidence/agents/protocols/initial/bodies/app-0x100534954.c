// ==== 0x100534954  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContentsDevTools:needsConfirmation:completion:]  (296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContentsDevTools_needsConfirmation_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  void *pvVar1;
  undefined *puVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  String SVar6;
  undefined1 auStack_78 [24];
  
  pvVar1 = __Block_copy((void *)param_5);
  SVar6 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  puVar2 = &DAT_10418d2e0;
  _swift_allocObject(&DAT_10418d2e0,0x18,7);
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
    (**(code **)(lVar5 + 0xd8))
              (param_1,&PTR_PTR_10418d648,SVar6.str,SVar6.bridgeObject,FUN_1004e80e0,puVar2,lVar4,
               lVar5);
    _swift_unknownObjectRelease(lVar3);
    _swift_release(puVar2);
  }
  _swift_bridgeObjectRelease(SVar6.bridgeObject);
  return;
}


