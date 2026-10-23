// ==== 0x100536c38  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContentsWantsOverscrollSwipeEvents:]  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool _TtC3ADK18WebContentChromium::__WebContentChromium_webContentsWantsOverscrollSwipeEvents__
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  uint uVar3;
  long lVar4;
  undefined1 auStack_58 [24];
  
  lVar2 = param_1 + uiDelegate;
  _swift_beginAccess(lVar2,auStack_58,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 == 0) {
    uVar3 = 0;
  }
  else {
    lVar4 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    lVar4 = *(long *)(lVar4 + 8);
    (**(code **)(lVar4 + 8))(param_1,&PTR_PTR_10418d648,lVar2,lVar4);
    uVar3 = (uint)param_1;
    _swift_unknownObjectRelease(lVar1);
  }
  return uVar3 & 1;
}


