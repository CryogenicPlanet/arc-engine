// ==== 0x100529600  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_sidePanelWillUnhide:]  (192 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_sidePanelWillUnhide__
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined1 auStack_58 [24];
  
  lVar2 = param_1 + sidePanelDelegate;
  _swift_beginAccess(lVar2,auStack_58,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x10))(param_1,&PTR_PTR_10418c868,param_3,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  return;
}


