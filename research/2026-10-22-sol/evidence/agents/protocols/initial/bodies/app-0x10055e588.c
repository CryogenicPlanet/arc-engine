// ==== 0x10055e588  _TtC3ADK28TablessWebContentsUIDelegate::-[TablessWebContentsUIDelegate_webContents:didDestroyWebUI:newURL:]  (324 bytes)

/* Function Stack Size: 0x28 bytes */

void _TtC3ADK28TablessWebContentsUIDelegate::
     __TablessWebContentsUIDelegate_webContents_didDestroyWebUI_newURL__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  undefined *puVar1;
  URL UVar2;
  long extraout_x8;
  long extraout_x12;
  undefined1 *puVar3;
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(puVar1 + -8) + 0x40));
  puVar3 = &stack0xffffffffffffffc0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0);
  (*(code *)PTR____chkstk_darwin_10415f308)();
  if (param_4 == 0) {
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))((long)puVar3 - extraout_x12,param_4 == 0,1);
  if (param_5 != 0) {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_5);
  }
  FUN_10000733c((long)puVar3 - extraout_x12,&DAT_1044c87d0,&DAT_10363af50);
  UVar2 = Foundation::URL::typeMetadataAccessor();
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))(puVar3,param_5 == 0,1,UVar2.unknown);
  FUN_10000733c(puVar3,&DAT_1044c87d0,&DAT_10363af50);
  return;
}


