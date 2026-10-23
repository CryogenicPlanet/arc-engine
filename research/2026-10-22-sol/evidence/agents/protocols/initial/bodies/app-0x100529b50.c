// ==== 0x100529b50  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:didUpdateExtensionToolbarActions:]  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_didUpdateExtensionToolbarActions__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined1 auStack_68 [24];
  
  FUN_1000391c0(0,&DAT_1044cc018,&PTR__OBJC_CLASS___ArcExtensionToolbarAction_1044700d8);
  (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
  lVar2 = param_1 + extensionsPopUpDelegate;
  _swift_beginAccess(lVar2,auStack_68,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x10))(param_1,&PTR_PTR_10418c868,param_4,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  _swift_bridgeObjectRelease(param_4);
  return;
}


