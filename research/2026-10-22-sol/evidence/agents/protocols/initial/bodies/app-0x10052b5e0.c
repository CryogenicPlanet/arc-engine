// ==== 0x10052b5e0  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:moveTabGroupWithId:toNewIndex:completion:]  (176 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_moveTabGroupWithId_toNewIndex_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5,ID param_6,
               undefined4 param_7)

{
  void *aBlock;
  String SVar1;
  
  aBlock = __Block_copy((void *)param_6);
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  __Block_copy(aBlock);
  FUN_10052f684(SVar1.str,SVar1.bridgeObject,param_5,param_1,aBlock);
  __Block_release(aBlock);
  __Block_release(aBlock);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  return;
}


