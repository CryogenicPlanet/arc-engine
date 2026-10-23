// ==== 0x100538050  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:willAskForPermission:withRequests:withQuietUIReason:completion:]  (208 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_willAskForPermission_withRequests_withQuietUIReason_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,unsigned_long_long param_6,
               ID param_7,undefined4 param_8)

{
  void *aBlock;
  
  aBlock = __Block_copy((void *)param_7);
  FUN_10053b6a0(0);
  (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
  __Block_copy(aBlock);
  FUN_10053ab4c(param_4,param_5,param_6,param_1,aBlock);
  __Block_release(aBlock);
  __Block_release(aBlock);
  _swift_bridgeObjectRelease(param_5);
  return;
}


