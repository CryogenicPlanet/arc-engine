// ==== 0x1004e2088  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:chatDataStreamOpenRequestedForChatId:update:completion:]  (364 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_chatDataStreamOpenRequestedForChatId_update_completion__
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               void *param_5,void *param_6)

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
  pvVar2 = __Block_copy(param_5);
  pvVar3 = __Block_copy(param_6);
  puVar1 = &DAT_104187d90;
  _swift_allocObject(&DAT_104187d90,0x38,7);
  *(undefined8 *)(puVar1 + 0x10) = param_3;
  *(undefined8 *)(puVar1 + 0x18) = param_4;
  *(void **)(puVar1 + 0x20) = pvVar2;
  *(void **)(puVar1 + 0x28) = pvVar3;
  *(undefined8 *)(puVar1 + 0x30) = param_1;
  TVar4 = Swift::TaskPriority::typeMetadataAccessor();
  (**(code **)(*(long *)(TVar4.unknown + -8) + 0x38))
            (&stack0xffffffffffffffb0 + -extraout_x8,1,1,TVar4.unknown);
  puVar5 = &DAT_104187db8;
  _swift_allocObject(&DAT_104187db8,0x30,7);
  *(undefined8 *)(puVar5 + 0x10) = 0;
  *(undefined8 *)(puVar5 + 0x18) = 0;
  *(ushort **)(puVar5 + 0x20) = &USHORT_10363b160;
  *(undefined **)(puVar5 + 0x28) = puVar1;
  puVar1 = &DAT_104187de0;
  _swift_allocObject(&DAT_104187de0,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  *(ushort **)(puVar1 + 0x20) = &USHORT_10363b168;
  *(undefined **)(puVar1 + 0x28) = puVar5;
  FUN_10055ef7c(0,0,&stack0xffffffffffffffb0 + -extraout_x8,&USHORT_10363b170,puVar1);
  _swift_release();
  return;
}


