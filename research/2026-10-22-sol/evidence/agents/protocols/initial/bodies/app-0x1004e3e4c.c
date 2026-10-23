// ==== 0x1004e3e4c  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:productConnectionCloseRequestedForID:completion:]  (336 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_productConnectionCloseRequestedForID_completion__
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               void *param_5)

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
  pvVar2 = __Block_copy(param_5);
  puVar1 = &DAT_104187b88;
  _swift_allocObject(&DAT_104187b88,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = param_3;
  *(undefined8 *)(puVar1 + 0x18) = param_4;
  *(void **)(puVar1 + 0x20) = pvVar2;
  *(undefined8 *)(puVar1 + 0x28) = param_1;
  TVar3 = Swift::TaskPriority::typeMetadataAccessor();
  (**(code **)(*(long *)(TVar3.unknown + -8) + 0x38))
            (&stack0xffffffffffffffc0 + -extraout_x8,1,1,TVar3.unknown);
  puVar4 = &DAT_104187bb0;
  _swift_allocObject(&DAT_104187bb0,0x30,7);
  *(undefined8 *)(puVar4 + 0x10) = 0;
  *(undefined8 *)(puVar4 + 0x18) = 0;
  *(ushort **)(puVar4 + 0x20) = &USHORT_10363b0e0;
  *(undefined **)(puVar4 + 0x28) = puVar1;
  puVar1 = &DAT_104187bd8;
  _swift_allocObject(&DAT_104187bd8,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  *(ushort **)(puVar1 + 0x20) = &USHORT_10363b0e8;
  *(undefined **)(puVar1 + 0x28) = puVar4;
  FUN_10055ef7c(0,0,&stack0xffffffffffffffc0 + -extraout_x8,&USHORT_10363b0f0,puVar1);
  _swift_release();
  return;
}


