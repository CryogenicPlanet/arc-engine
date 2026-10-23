// ==== 0x10052a548  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:didAskAboutImage:]  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_didAskAboutImage__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  URL UVar1;
  long lVar2;
  long lVar3;
  long extraout_x8;
  long lVar4;
  long lVar5;
  undefined1 auStack_80 [8];
  undefined1 auStack_78 [24];
  
  UVar1 = Foundation::URL::typeMetadataAccessor();
  lVar5 = *(long *)(UVar1.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar5 + 0x40));
  Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
  lVar3 = param_1 + browserDelegate;
  _swift_beginAccess(lVar3,auStack_78,0,0);
  lVar2 = lVar3;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar2 != 0) {
    lVar4 = *(long *)(lVar3 + 8);
    lVar3 = lVar2;
    _swift_getObjectType();
    (**(code **)(lVar4 + 0x30))
              (param_1,&PTR_PTR_10418c868,auStack_80 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),
               lVar3,lVar4);
    _swift_unknownObjectRelease(lVar2);
  }
  (**(code **)(lVar5 + 8))(auStack_80 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),UVar1.unknown);
  return;
}


