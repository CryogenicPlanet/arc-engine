// ==== 0x473dff4  FUN_0473dff4  (536 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0473dff4(ID *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  ID IVar4;
  ID IVar5;
  ID self_00;
  ulong uVar6;
  undefined8 extraout_d0;
  undefined8 uVar7;
  undefined8 uVar8;
  
  self_00 = *param_1;
  cVar3 = *(char *)((long)param_2 + 0x17);
  puVar1 = (undefined8 *)*param_2;
  if (-1 < (long)cVar3) {
    puVar1 = param_2;
  }
  lVar2 = param_2[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  self = FUN_01c391bc(puVar1,lVar2);
  IVar4 = _objc_msgSend(self,"length");
  if (IVar4 != 0) {
    uVar6 = 0;
    do {
      IVar4 = _objc_msgSend(self,"substringWithRange:",uVar6,1);
      uVar7 = *(undefined8 *)PTR__NSZeroPoint_0d825658;
      uVar8 = *(undefined8 *)(PTR__NSZeroPoint_0d825658 + 8);
      IVar5 = NSProcessInfo::processInfo((ID)&_OBJC_CLASS___NSProcessInfo,"processInfo");
      NSProcessInfo::systemUptime(IVar5,"systemUptime");
      IVar5 = _objc_msgSend(self_00,"window");
      IVar5 = _objc_msgSend(IVar5,"windowNumber");
      IVar4 = NSEvent::
              keyEventWithType_location_modifierFlags_timestamp_windowNumber_context_characters_charactersIgnoringModifiers_isARepeat_keyCode_
                        ((ID)&_OBJC_CLASS___NSEvent,
                         "keyEventWithType:location:modifierFlags:timestamp:windowNumber:context:characters:charactersIgnoringModifiers:isARepeat:keyCode:"
                         ,uVar7,uVar8,extraout_d0,10,0,IVar5,0,IVar4,IVar4,0);
      _objc_msgSend(self_00,"keyDown:",IVar4);
      uVar6 = uVar6 + 1;
      IVar4 = _objc_msgSend(self,"length");
    } while (uVar6 < IVar4);
  }
  return 0;
}


