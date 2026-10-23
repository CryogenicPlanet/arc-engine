// ==== 0x1004af380  ADK.ADKApplication.method_instance  (612 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

undefined *
ADK_ADKApplication_method_instance
          (char *param_1,void *param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  char *self;
  NSString *pNVar2;
  undefined *puVar3;
  undefined *puVar4;
  void *aBlock;
  ID IVar5;
  StaticString SVar6;
  void *pvVar7;
  String SVar8;
  String SVar9;
  String SVar10;
  String SVar11;
  String SVar12;
  String SVar13;
  String SVar14;
  String SVar15;
  String SVar16;
  undefined *local_98 [2];
  StaticString local_88;
  __uint64 local_80;
  code *local_78;
  undefined *puStack_70;
  code *local_68;
  undefined *local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_10415f368;
  self = param_1;
  ADK_ADKApplication_arcBrowserApplication_getter();
  pNVar2 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  puVar3 = &DAT_1041848e0;
  _swift_allocObject(&DAT_1041848e0,0x18,7);
  _swift_unknownObjectWeakInit(puVar3 + 0x10);
  puVar4 = &DAT_1041849d0;
  _swift_allocObject(&DAT_1041849d0,0x28,7);
  *(undefined **)(puVar4 + 0x10) = puVar3;
  *(undefined8 *)(puVar4 + 0x18) = param_3;
  *(undefined8 *)(puVar4 + 0x20) = param_4;
  local_68 = FUN_1004c1d58;
  local_88.unknown = PTR___NSConcreteStackBlock_10415f2f0;
  local_80 = 0x42000000;
  local_78 = thunk_FUN_1004b6398;
  puStack_70 = &DAT_1041849e8;
  local_60 = puVar4;
  aBlock = __Block_copy(&local_88);
  puVar3 = local_60;
  _swift_retain(param_4);
  _swift_release(puVar3);
  local_88.unknown = (undefined *)0x0;
  pvVar7 = (void *)0x0;
  IVar5 = objc_stub::FUN_1035b6a60((ID)self,pNVar2,0,0,aBlock,&local_88);
  __Block_release(aBlock);
  if ((int)IVar5 == 0) {
    SVar6.unknown = local_88.unknown;
    Foundation::__convertNSErrorToError();
    _swift_willThrow(0);
    local_88.unknown = (undefined *)0x0;
    local_80 = 0xe000000000000000;
    _swift_errorRetain(SVar6.unknown);
    SVar8.bridgeObject = (void *)0x800000010392f810;
    SVar8.str = (char *)0xd000000000000030;
    SVar13.bridgeObject = pvVar7;
    SVar13.str = (char *)pNVar2;
    Swift::String::append(SVar8,SVar13);
    SVar9.bridgeObject = &DAT_e100000000000000;
    SVar9.str = &DAT_00000028;
    SVar14.bridgeObject = pvVar7;
    SVar14.str = (char *)pNVar2;
    Swift::String::append(SVar9,SVar14);
    SVar10.bridgeObject = param_2;
    SVar10.str = param_1;
    SVar15.bridgeObject = pvVar7;
    SVar15.str = (char *)pNVar2;
    Swift::String::append(SVar10,SVar15);
    SVar11.bridgeObject = (void *)0xe300000000000000;
    SVar11.str = (char *)0x203a29;
    SVar16.bridgeObject = pvVar7;
    SVar16.str = (char *)pNVar2;
    Swift::String::append(SVar11,SVar16);
    puVar3 = &DAT_1044c8388;
    local_98[0] = SVar6.unknown;
    FUN_100005134(&DAT_1044c8388,&DAT_103638ee0);
    Swift::__print_unlocked
              (local_98,&local_88,puVar3,
               PTR___type_metadata_for_Swift_DefaultStringInterpolation_104166b60,
               PTR___protocol_witness_table_for_Swift_DefaultStringInterpolation___Swift_TextOutputStream_in_Swift_104166b70
              );
    SVar12.bridgeObject = &DAT_00000002;
    SVar12.str = &SUB_0000000b;
    Swift::_assertionFailure((StaticString)0x103921730,SVar12,local_88,local_80,0x392f60c);
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x1004af5e4);
    (*pcVar1)();
  }
  if (*(long *)PTR____stack_chk_guard_10415f368 == local_58) {
    return local_88.unknown;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


