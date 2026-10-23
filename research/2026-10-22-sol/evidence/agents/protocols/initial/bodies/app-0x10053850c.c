// ==== 0x10053850c  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:willShowContextMenu:withElement:]  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

bool _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_willShowContextMenu_withElement__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  long lVar1;
  long lVar2;
  uint uVar3;
  long lVar4;
  undefined1 auStack_68 [24];
  
  lVar2 = param_1 + uiDelegate;
  _swift_beginAccess(lVar2,auStack_68,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 == 0) {
    uVar3 = 0;
  }
  else {
    lVar4 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar4 + 0x20))(param_1,&PTR_PTR_10418d648,param_4,param_5,lVar2,lVar4);
    uVar3 = (uint)param_1;
    _swift_unknownObjectRelease(lVar1);
  }
  return uVar3 & 1;
}


