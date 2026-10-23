// ==== 0x1005351e4  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContentsDidClickPipCloseButton:]  (168 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContentsDidClickPipCloseButton__
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined1 auStack_58 [24];
  
  lVar2 = param_1 + uiDelegate;
  _swift_beginAccess(lVar2,auStack_58,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x158))(param_1,&PTR_PTR_10418d648,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  return;
}


