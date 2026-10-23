// ==== 0x1004ad0a0  ADK.ADKApplication.method_instance  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined1  [16] ADK_ADKApplication_method_instance(undefined8 param_1,char param_2,long param_3)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 *self;
  NSString *pNVar3;
  long unaff_x20;
  undefined8 uVar4;
  undefined1 auVar5 [16];
  String SVar6;
  ID local_e0 [7];
  undefined1 auStack_a8 [24];
  ID local_90;
  undefined1 auStack_88 [8];
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  
  if (*(char *)(unaff_x20 + _TtC3ADK14ADKApplication::features + 8) != '\x02') {
    ADK_ADKApplication_method_instance(local_e0,param_3);
    if (local_e0[0] == 0) {
      FUN_10000733c(local_e0,&DAT_1044c82d8,&DAT_103638e58);
      uStack_78 = *(undefined8 *)(param_3 + 0x18);
      local_80 = *(undefined8 *)(param_3 + 0x10);
      uStack_68 = *(undefined8 *)(param_3 + 0x28);
      uStack_70 = *(undefined8 *)(param_3 + 0x20);
      local_60 = *(undefined8 *)(param_3 + 0x30);
      puVar2 = &local_80;
      ADK_ADKApplication_method_instance();
      self = puVar2;
      ADK_ADKApplication_arcBrowserApplication_getter();
      uVar4 = *(undefined8 *)(unaff_x20 + _TtC3ADK14ADKApplication::contextHistoryDelegateBridge);
      if (param_2 == '\0') {
        pNVar3 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
        local_e0[0] = objc_stub::createBrowser_withDelegate_forWindow_((ID)self,puVar2,0,pNVar3);
      }
      else if (param_2 == '\x01') {
        local_e0[0] = objc_stub::createPopupBrowser_withDelegate_((ID)self,puVar2,0);
      }
      else {
        pNVar3 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
        local_e0[0] = objc_stub::createBrowser_withDelegate_forWindow_((ID)self,puVar2,0,pNVar3);
      }
      FUN_10052de00(0);
      _objc_allocWithZone();
      FUN_100525930(local_e0[0],uVar4);
      objc_stub::setWindow_(*(ID *)(local_e0[0] + _TtC3ADK15ChromiumBrowser::browser),param_1);
      _swift_unknownObjectWeakInit(auStack_88,0);
      local_90 = local_e0[0];
      _swift_unknownObjectWeakAssign(auStack_88,param_1);
      _swift_beginAccess(unaff_x20 + _TtC3ADK14ADKApplication::browsers,auStack_a8,0x21,0);
      FUN_1004a3cc0(param_3,local_e0);
      FUN_1004ab554(&local_90,param_3);
      _swift_endAccess(auStack_a8);
    }
    else {
      FUN_1004c0c14(local_e0);
    }
    auVar5._8_8_ = &PTR_PTR_10418c868;
    auVar5._0_8_ = local_e0[0];
    return auVar5;
  }
  SVar6.bridgeObject = &DAT_00000002;
  SVar6.str = &SUB_0000000b;
  Swift::_assertionFailure
            ((StaticString)0x103921730,SVar6,(StaticString)0xd000000000000034,0x800000010392f640,
             0x392f60c);
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x1004ad314);
  (*pcVar1)();
}


