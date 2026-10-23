// ==== 0x100535388  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didChangeAvailableMediaActions:]  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didChangeAvailableMediaActions__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  IndexSet IVar1;
  long extraout_x8;
  long lVar2;
  
  IVar1 = Foundation::IndexSet::typeMetadataAccessor();
  lVar2 = *(long *)(IVar1.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar2 + 0x40));
  Foundation::IndexSet::__unconditionallyBridgeFromObjectiveC((IndexSet)param_4);
  FUN_100535894(&stack0xffffffffffffffc0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0));
  (**(code **)(lVar2 + 8))
            (&stack0xffffffffffffffc0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),IVar1.unknown);
  return;
}


