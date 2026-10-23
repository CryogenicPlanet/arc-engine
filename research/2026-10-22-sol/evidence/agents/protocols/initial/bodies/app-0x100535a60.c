// ==== 0x100535a60  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didCommitNavigation:]  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didCommitNavigation__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined1 auStack_90 [16];
  ID local_80;
  ID IStack_78;
  undefined1 auStack_68 [24];
  
  lVar2 = param_1 + navigationDelegate;
  _swift_beginAccess(lVar2,auStack_68,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar3 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    (**(code **)(lVar3 + 0x18))(param_1,&PTR_PTR_10418d648,lVar2,lVar3);
    _swift_unknownObjectRelease(lVar1);
  }
  local_80 = param_1;
  IStack_78 = param_4;
  FUN_10072b418(FUN_100536b54,auStack_90);
  return;
}


