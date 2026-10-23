// ==== 0x1004e4dac  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:diaResourceRequestedForType:resourceID:completion:]  (360 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_diaResourceRequestedForType_resourceID_completion__
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,void *param_6)

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
  pvVar2 = __Block_copy(param_6);
  puVar1 = &DAT_104187a70;
  _swift_allocObject(&DAT_104187a70,0x38,7);
  *(undefined8 *)(puVar1 + 0x10) = param_3;
  *(undefined8 *)(puVar1 + 0x18) = param_4;
  *(undefined8 *)(puVar1 + 0x20) = param_5;
  *(void **)(puVar1 + 0x28) = pvVar2;
  *(undefined8 *)(puVar1 + 0x30) = param_1;
  TVar3 = Swift::TaskPriority::typeMetadataAccessor();
  (**(code **)(*(long *)(TVar3.unknown + -8) + 0x38))
            (&stack0xffffffffffffffb0 + -extraout_x8,1,1,TVar3.unknown);
  puVar4 = &DAT_104187a98;
  _swift_allocObject(&DAT_104187a98,0x30,7);
  *(undefined8 *)(puVar4 + 0x10) = 0;
  *(undefined8 *)(puVar4 + 0x18) = 0;
  *(ushort **)(puVar4 + 0x20) = &USHORT_10363b0a0;
  *(undefined **)(puVar4 + 0x28) = puVar1;
  puVar1 = &DAT_104187ac0;
  _swift_allocObject(&DAT_104187ac0,0x30,7);
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  *(ushort **)(puVar1 + 0x20) = &USHORT_10363b0a8;
  *(undefined **)(puVar1 + 0x28) = puVar4;
  FUN_10055ef7c(0,0,&stack0xffffffffffffffb0 + -extraout_x8,&USHORT_10363b0b0,puVar1);
  _swift_release();
  return;
}


