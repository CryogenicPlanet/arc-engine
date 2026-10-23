// ==== 0x100535c64  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didFailProvisionalNavigation:withError:]  (252 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_didFailProvisionalNavigation_withError__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined8 uVar3;
  undefined1 local_80 [16];
  ID local_70;
  ID IStack_68;
  ID local_60;
  
  uVar3 = *(undefined8 *)(param_1 + features);
  puVar1 = &DAT_1036403c8;
  _swift_getKeyPath(&DAT_1036403c8);
  puVar2 = &DAT_1036403f0;
  _swift_getKeyPath(&DAT_1036403f0);
  local_80[0] = 1;
  _swift_retain(uVar3);
  Combine::Published::_set_subscript((Published *)param_4,local_80,uVar3,puVar1,puVar2);
  local_70 = param_1;
  IStack_68 = param_4;
  local_60 = param_5;
  FUN_10072b418(FUN_1005369f4,local_80);
  return;
}


