// ==== 0x100538880  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:didChangeStatusText:]  (148 bytes)

/* Function Stack Size: 0x20 bytes */

void _TtC3ADK18WebContentChromium::__WebContentChromium_webContents_didChangeStatusText__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined8 uVar3;
  String SVar4;
  String local_50;
  
  SVar4 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  uVar3 = *(undefined8 *)(param_1 + features);
  puVar1 = &DAT_103640508;
  _swift_getKeyPath(&DAT_103640508);
  puVar2 = &DAT_103640530;
  _swift_getKeyPath(&DAT_103640530);
  local_50 = SVar4;
  _swift_retain(uVar3);
  Combine::Published::_set_subscript((Published *)SVar4.str,&local_50,uVar3,puVar1,puVar2);
  return;
}


