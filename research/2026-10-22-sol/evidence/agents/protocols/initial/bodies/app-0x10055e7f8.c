// ==== 0x10055e7f8  _TtC3ADK28TablessWebContentsUIDelegate::-[TablessWebContentsUIDelegate_webContents:onFrameToChatAssociationRequestForChatId:frameId:updateCallback:completion:]  (372 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void _TtC3ADK28TablessWebContentsUIDelegate::
     __TablessWebContentsUIDelegate_webContents_onFrameToChatAssociationRequestForChatId_frameId_updateCallback_completion__
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined4 param_5,void *param_6,void *param_7)

{
  undefined *puVar1;
  void *pvVar2;
  void *pvVar3;
  TaskPriority TVar4;
  undefined *puVar5;
  long extraout_x8;
  
  puVar1 = &DAT_1044c8738;
  FUN_100005134(&DAT_1044c8738,&USHORT_10363b090);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  pvVar2 = __Block_copy(param_6);
  pvVar3 = __Block_copy(param_7);
  puVar1 = &DAT_10418f940;
  _swift_allocObject(&DAT_10418f940,0x40,7);
  *(undefined8 *)(puVar1 + 0x10) = param_3;
  *(undefined8 *)(puVar1 + 0x18) = param_4;
  *(undefined4 *)(puVar1 + 0x20) = param_5;
  *(void **)(puVar1 + 0x28) = pvVar2;
  *(void **)(puVar1 + 0x30) = pvVar3;
  *(undefined8 *)(puVar1 + 0x38) = param_1;
  TVar4 = Swift::TaskPriority::typeMetadataAccessor();
  (**(code **)(*(long *)(TVar4.unknown + -8) + 0x38))
            (&stack0xffffffffffffffb0 + -extraout_x8,1,1,TVar4.unknown);
  puVar5 = &DAT_10418f968;
  _swift_allocObject(&DAT_10418f968,0x30,7);
  *(undefined8 *)(puVar5 + 0x10) = 0;
  *(undefined8 *)(puVar5 + 0x18) = 0;
  *(undefined **)(puVar5 + 0x20) = &DAT_103642ee8;
  *(undefined **)(puVar5 + 0x28) = puVar1;
  puVar1 = &DAT_10418f990;
  _swift_allocObject(&DAT_10418f990,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  *(undefined **)(puVar1 + 0x20) = &DAT_103642ef0;
  *(undefined **)(puVar1 + 0x28) = puVar5;
  FUN_10055ef7c(0,0,&stack0xffffffffffffffb0 + -extraout_x8,&DAT_103642ef8,puVar1);
  _swift_release();
  return;
}


