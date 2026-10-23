// ==== 0x100539c1c  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didDestroyWebUI:newURL:]  (436 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didDestroyWebUI_newURL__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  undefined *puVar1;
  URL UVar2;
  long lVar3;
  long lVar4;
  long extraout_x8;
  long extraout_x12;
  undefined1 *puVar5;
  long lVar6;
  long lVar7;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [24];
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(puVar1 + -8) + 0x40));
  puVar5 = auStack_70 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0);
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar6 = (long)puVar5 - extraout_x12;
  if (param_4 == 0) {
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))(lVar6,param_4 == 0,1);
  if (param_5 != 0) {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_5);
  }
  UVar2 = Foundation::URL::typeMetadataAccessor();
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))(puVar5,param_5 == 0,1,UVar2.unknown);
  lVar4 = param_1 + uiDelegate;
  _swift_beginAccess(lVar4,auStack_68,0,0);
  lVar3 = lVar4;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar3 != 0) {
    lVar7 = *(long *)(lVar4 + 8);
    lVar4 = lVar3;
    _swift_getObjectType();
    (**(code **)(lVar7 + 0x250))(param_1,&PTR_PTR_10418d648,lVar6,puVar5,lVar4,lVar7);
    _swift_unknownObjectRelease(lVar3);
  }
  FUN_1005339f4(puVar5);
  FUN_1005339f4(lVar6);
  return;
}


