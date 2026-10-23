// ==== 0x1005381c8  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didChangeVisibleSecurityState:]  (192 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didChangeVisibleSecurityState__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined1 uVar3;
  Published *pPVar4;
  undefined1 local_41;
  
  uVar3 = (undefined1)param_4;
  pPVar4 = *(Published **)(param_1 + features);
  _swift_retain(pPVar4);
  FUN_10053ad78();
  puVar1 = &DAT_103640550;
  _swift_getKeyPath(&DAT_103640550);
  puVar2 = &DAT_103640578;
  _swift_getKeyPath(&DAT_103640578);
  local_41 = uVar3;
  Combine::Published::_set_subscript(pPVar4,&local_41,pPVar4,puVar1,puVar2);
  return;
}


