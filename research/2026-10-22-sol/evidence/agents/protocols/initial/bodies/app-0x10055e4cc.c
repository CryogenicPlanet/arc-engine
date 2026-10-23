// ==== 0x10055e4cc  _TtC3ADK28TablessWebContentsUIDelegate::-[TablessWebContentsUIDelegate_webContents:didShowWebUI:]  (188 bytes)

/* Function Stack Size: 0x20 bytes */

void _TtC3ADK28TablessWebContentsUIDelegate::
     __TablessWebContentsUIDelegate_webContents_didShowWebUI__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined *puVar1;
  URL UVar2;
  long extraout_x8;
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  if (param_4 == 0) {
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
    UVar2 = Foundation::URL::typeMetadataAccessor();
  }
  (**(code **)(*(long *)(UVar2.unknown + -8) + 0x38))
            (&stack0xffffffffffffffe0 + -extraout_x8,param_4 == 0,1);
  FUN_10000733c(&stack0xffffffffffffffe0 + -extraout_x8,&DAT_1044c87d0,&DAT_10363af50);
  return;
}


