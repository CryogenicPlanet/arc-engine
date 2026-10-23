// ==== 0x10052a1b8  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:didUpdateExtensionToolbarAction:]  (292 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_didUpdateExtensionToolbarAction__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  String SVar4;
  undefined1 auStack_78 [24];
  
  lVar2 = param_1 + extensionsPopUpDelegate;
  _swift_beginAccess(lVar2,auStack_78,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    objc_stub::id(param_4);
    SVar4 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
    (**(code **)(lVar3 + 0x28))
              (param_1,&PTR_PTR_10418c868,param_4,SVar4.str,SVar4.bridgeObject,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
    _swift_bridgeObjectRelease(SVar4.bridgeObject);
  }
  return;
}


