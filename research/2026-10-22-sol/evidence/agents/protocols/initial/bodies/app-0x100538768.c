// ==== 0x100538768  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didCreateNewWebContent:targetURL:disposition:]  (280 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_didCreateNewWebContent_targetURL_disposition__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6)

{
  undefined *puVar1;
  URL UVar2;
  long extraout_x8;
  undefined1 *puVar3;
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  puVar3 = &stack0xffffffffffffffc0 + -extraout_x8;
  if (param_5 == 0) {
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_5);
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))(puVar3,param_5 == 0,1);
  FUN_10053afd4(param_4,puVar3,param_6);
  FUN_1005339f4(puVar3);
  return;
}


