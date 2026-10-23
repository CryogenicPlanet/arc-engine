// ==== 0x100535040  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didChangeMediaPosition:]  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didChangeMediaPosition__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  code *pcVar3;
  long lVar4;
  long lVar5;
  undefined1 auStack_70 [32];
  
  pcVar3 = (code *)auStack_70;
  lVar5 = *(long *)(param_1 + features);
  puVar1 = &DAT_103640380;
  _swift_getKeyPath(&DAT_103640380);
  puVar2 = &DAT_1036403a8;
  _swift_getKeyPath(&DAT_1036403a8);
  _swift_retain(lVar5);
  lVar4 = lVar5;
  Combine::Published::_modify_subscript((Published *)param_1,auStack_70,lVar5,puVar1,puVar2);
  *(ID *)(lVar4 + 0x10) = param_4;
  (*pcVar3)(auStack_70,0);
  _swift_release(lVar5);
  _swift_release(puVar1);
  _swift_release(puVar2);
  return;
}


