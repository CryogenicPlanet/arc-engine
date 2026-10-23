// ==== 0x10052ae08  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:addTabsToGroupWithId:sessionIds:completion:]  (224 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void _TtC3ADK15ChromiumBrowser::
     __ChromiumBrowser_browser_addTabsToGroupWithId_sessionIds_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,
               undefined4 param_7)

{
  void *aBlock;
  String SVar1;
  
  aBlock = __Block_copy((void *)param_6);
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  FUN_1000391c0(0,&DAT_1044c85a8,&PTR__OBJC_CLASS___NSNumber_10446fcc8);
  (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
  __Block_copy(aBlock);
  FUN_10052eef4(SVar1.str,SVar1.bridgeObject,param_5,param_1,aBlock);
  __Block_release(aBlock);
  __Block_release(aBlock);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  _swift_bridgeObjectRelease(param_5);
  return;
}


