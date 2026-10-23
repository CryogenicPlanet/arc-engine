// ==== 0x1004e49d8  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:featureFlagsSubscribe:]  (144 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_featureFlagsSubscribe__
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  void *pvVar1;
  undefined *puVar2;
  
  pvVar1 = __Block_copy((void *)param_4);
  puVar2 = &DAT_104187ae8;
  _swift_allocObject(&DAT_104187ae8,0x18,7);
  *(void **)(puVar2 + 0x10) = pvVar1;
  FUN_1004e4834(param_3,FUN_1004e7bfc,puVar2);
  _swift_release(puVar2);
  return;
}


