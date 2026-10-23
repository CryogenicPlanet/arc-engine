// ==== 0x10053a528  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:onSendChatDataToFrameForChatId:frameId:data:completion:]  (368 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_onSendChatDataToFrameForChatId_frameId_data_completion__
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined4 param_5,undefined8 param_6,void *param_7)

{
  undefined *puVar1;
  void *pvVar2;
  TaskPriority TVar3;
  undefined *puVar4;
  long extraout_x8;
  
  puVar1 = &DAT_1044c8738;
  FUN_100005134(&DAT_1044c8738,&USHORT_10363b090);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  pvVar2 = __Block_copy(param_7);
  puVar1 = &DAT_10418d318;
  _swift_allocObject(&DAT_10418d318,0x40,7);
  *(undefined8 *)(puVar1 + 0x10) = param_3;
  *(undefined8 *)(puVar1 + 0x18) = param_4;
  *(undefined4 *)(puVar1 + 0x20) = param_5;
  *(undefined8 *)(puVar1 + 0x28) = param_6;
  *(void **)(puVar1 + 0x30) = pvVar2;
  *(undefined8 *)(puVar1 + 0x38) = param_1;
  TVar3 = Swift::TaskPriority::typeMetadataAccessor();
  (**(code **)(*(long *)(TVar3.unknown + -8) + 0x38))
            (&stack0xffffffffffffffb0 + -extraout_x8,1,1,TVar3.unknown);
  puVar4 = &DAT_10418d340;
  _swift_allocObject(&DAT_10418d340,0x30,7);
  *(undefined8 *)(puVar4 + 0x10) = 0;
  *(undefined8 *)(puVar4 + 0x18) = 0;
  *(undefined **)(puVar4 + 0x20) = &DAT_103640460;
  *(undefined **)(puVar4 + 0x28) = puVar1;
  puVar1 = &DAT_10418d368;
  _swift_allocObject(&DAT_10418d368,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  *(undefined **)(puVar1 + 0x20) = &DAT_103640468;
  *(undefined **)(puVar1 + 0x28) = puVar4;
  FUN_10055ef7c(0,0,&stack0xffffffffffffffb0 + -extraout_x8,&DAT_103640470,puVar1);
  _swift_release();
  return;
}


