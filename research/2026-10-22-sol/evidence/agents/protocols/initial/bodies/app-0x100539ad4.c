// ==== 0x100539ad4  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didShowWebUI:]  (328 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didShowWebUI__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined *puVar1;
  URL UVar2;
  long lVar3;
  long lVar4;
  long extraout_x8;
  undefined1 *puVar5;
  long lVar6;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [24];
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  puVar5 = auStack_70 + -extraout_x8;
  if (param_4 == 0) {
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))(puVar5,param_4 == 0,1);
  lVar4 = param_1 + uiDelegate;
  _swift_beginAccess(lVar4,auStack_68,0,0);
  lVar3 = lVar4;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar3 != 0) {
    lVar6 = *(long *)(lVar4 + 8);
    lVar4 = lVar3;
    _swift_getObjectType();
    (**(code **)(lVar6 + 0x248))(param_1,&PTR_PTR_10418d648,puVar5,lVar4,lVar6);
    _swift_unknownObjectRelease(lVar3);
  }
  FUN_1005339f4(puVar5);
  return;
}


