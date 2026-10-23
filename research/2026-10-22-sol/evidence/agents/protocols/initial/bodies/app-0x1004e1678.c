// ==== 0x1004e1678  _TtC3ADK28BrowserContextDelegateBridge::-[BrowserContextDelegateBridge_browserContext:extensionPermissionPrompt:extensionInfo:withMessages:completion:]  (252 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void _TtC3ADK28BrowserContextDelegateBridge::
     __BrowserContextDelegateBridge_browserContext_extensionPermissionPrompt_extensionInfo_withMessages_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
               undefined4 param_8)

{
  void *pvVar1;
  undefined *puVar2;
  
  pvVar1 = __Block_copy((void *)param_7);
  (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
  puVar2 = &DAT_104187e30;
  _swift_allocObject(&DAT_104187e30,0x18,7);
  *(void **)(puVar2 + 0x10) = pvVar1;
  FUN_1004e149c(param_3,param_4,param_5,param_6,FUN_1004e80e0,puVar2);
  _swift_bridgeObjectRelease(param_6);
  _swift_release(puVar2);
  return;
}


