// ==== 0x10053605c  FUN_10053605c  (1472 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_10053605c(ID param_1)

{
  DefaultStringInterpolation stringInterpolation;
  undefined *puVar1;
  long lVar2;
  URL UVar3;
  URL UVar4;
  long lVar5;
  ID IVar6;
  undefined8 extraout_x1;
  undefined8 uVar7;
  long *plVar8;
  undefined *puVar9;
  undefined **ppuVar10;
  undefined8 uVar11;
  char *pcVar12;
  long lVar13;
  void *pvVar14;
  ID IVar15;
  long extraout_x8;
  long extraout_x8_00;
  long extraout_x8_01;
  long extraout_x12;
  long extraout_x12_00;
  ID unaff_x20;
  long lVar16;
  long lVar17;
  code *pcVar18;
  long lVar19;
  long lVar20;
  URL url;
  String SVar21;
  String SVar22;
  long lStack_240;
  undefined4 auStack_238 [2];
  void *local_230;
  undefined8 local_228;
  undefined8 local_220;
  ID local_218;
  undefined1 auStack_1e0 [96];
  undefined1 auStack_180 [24];
  undefined8 local_168;
  long lStack_160;
  undefined1 local_158 [8];
  undefined *local_150;
  undefined *puStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  String local_120;
  String local_110;
  undefined8 local_100;
  undefined1 auStack_f8 [24];
  undefined1 auStack_e0 [24];
  DefaultStringInterpolation local_c8;
  undefined *local_c0;
  undefined *puStack_b8;
  undefined8 local_b0;
  long lStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  String local_90;
  String local_80;
  undefined8 local_70;
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  lVar13 = (long)&local_230 - extraout_x8;
  lVar2 = 0;
  FUN_100551100();
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(lVar2 + -8) + 0x40));
  lVar20 = lVar13 - (extraout_x8_00 + 0xfU & 0xfffffffffffffff0);
  UVar3 = Foundation::URL::typeMetadataAccessor();
  lVar19 = *(long *)(UVar3.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar19 + 0x40));
  url.unknown = (undefined *)(lVar20 - (extraout_x8_01 + 0xfU & 0xfffffffffffffff0));
  (*(code *)PTR____chkstk_darwin_10415f308)();
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar16 = ((long)url.unknown - extraout_x12) - extraout_x12_00;
  UVar4.unknown = (undefined *)objc_stub::url(param_1);
  if (UVar4.unknown == (undefined *)0x0) {
    unaff_x20 = 0;
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC(UVar4);
    (**(code **)(lVar19 + 0x20))(lVar16,(long)url.unknown - extraout_x12,UVar3.unknown);
    lVar17 = unaff_x20 + _TtC3ADK18WebContentChromium::navigationDelegate;
    uVar11 = 0;
    pvVar14 = (void *)0x0;
    _swift_beginAccess(lVar17,auStack_e0);
    lVar5 = lVar17;
    _swift_unknownObjectWeakLoadStrong();
    if (lVar5 == 0) {
      pcVar12 = &DAT_00000002;
      SVar21 = Swift::StaticString::get_description((StaticString)0x103932230);
      plVar8 = SVar21.bridgeObject;
      local_c8.unknown = &SUB_d000000000000013;
      local_c0 = (undefined *)0x8000000103918510;
      SVar22.bridgeObject = pvVar14;
      SVar22.str = pcVar12;
      Swift::String::append(SVar21,SVar22);
      _swift_bridgeObjectRelease();
      puVar1 = local_c0;
      stringInterpolation.unknown = local_c8.unknown;
      FUN_100734924();
      lVar20 = *plVar8;
      SVar21 = Swift::StaticString::get_description((StaticString)0x103932200);
      puVar9 = SVar21.bridgeObject;
      SVar22 = Swift::StaticString::get_description((StaticString)0x103932290);
      pvVar14 = SVar22.bridgeObject;
      _swift_beginAccess(lVar20 + 0x20,auStack_f8,0,0);
      FUN_100012c10(lVar20 + 0x20,&local_c8);
      lVar2 = lStack_a8;
      uVar11 = local_b0;
      FUN_100012c54(&local_c8,local_b0);
      (**(code **)(lVar2 + 0x70))(uVar11,lVar2);
      FUN_100012e70(&local_c8);
      if (((uint)uVar11 & 0xff) < 6) {
        FUN_100012c10(lVar20 + 0x20,auStack_180);
        FUN_100012c54(auStack_180,local_168);
        FUN_1004c0f68();
        _swift_bridgeObjectRetain(puVar1);
        (extension_Swift)::Swift::ExpressibleByStringInterpolation::_init(stringInterpolation);
        puStack_b8 = local_c0;
        local_c0 = local_c8.unknown;
        local_c8.unknown._0_1_ = 5;
        lStack_a8 = 0;
        local_b0 = 0;
        uStack_98 = 0;
        local_a0 = 0;
        local_70 = 0x11;
        local_158[0] = 5;
        local_150 = local_c0;
        puStack_148 = puStack_b8;
        uStack_138 = 0;
        local_140 = 0;
        uStack_128 = 0;
        local_130 = 0;
        local_100 = 0x11;
        local_120 = SVar21;
        local_110 = SVar22;
        local_90 = SVar21;
        local_80 = SVar22;
        _swift_bridgeObjectRetain(puVar9);
        _swift_bridgeObjectRetain(pvVar14);
        FUN_1004c0fa8(&local_c8,auStack_1e0);
        FUN_100012cf4(local_158);
        (**(code **)(lStack_160 + 0x28))(&local_c8,local_168,lStack_160);
        _swift_bridgeObjectRelease(pvVar14);
        _swift_bridgeObjectRelease(puVar9);
        FUN_100012cf4(&local_c8);
        FUN_100012e70(auStack_180);
      }
      else {
        _swift_bridgeObjectRelease(puVar1);
        _swift_bridgeObjectRelease(pvVar14);
        puVar1 = puVar9;
      }
      _swift_bridgeObjectRelease(puVar1);
      unaff_x20 = objc_stub::FUN_1035c6500(param_1);
      (**(code **)(lVar19 + 8))(lVar16,UVar3.unknown);
    }
    else {
      lVar17 = *(long *)(lVar17 + 8);
      IVar6 = objc_stub::FUN_1035c6500(param_1);
      uVar7 = extraout_x1;
      FUN_100535eb8();
      local_230 = pvVar14;
      local_228 = uVar11;
      local_220 = uVar7;
      local_218 = IVar6;
      UVar4.unknown = (undefined *)objc_stub::FUN_1035ca260(param_1);
      if (UVar4.unknown != (undefined *)0x0) {
        Foundation::URL::__unconditionallyBridgeFromObjectiveC(UVar4);
      }
      pcVar18 = *(code **)(lVar19 + 0x38);
      (*pcVar18)(lVar13,UVar4.unknown == (undefined *)0x0,1,UVar3.unknown);
      IVar6 = objc_stub::navigationType(param_1);
      if (4 < IVar6) {
        *(undefined4 *)(lVar16 + -8) = 0;
        *(undefined8 *)(lVar16 + -0x10) = 0xce;
        SVar21.bridgeObject = &DAT_00000002;
        SVar21.str = &SUB_0000000b;
        Swift::_assertionFailure
                  ((StaticString)0x103921730,SVar21,(StaticString)0x0,0xe000000000000000,0x3932200);
                    /* WARNING: Does not return */
        pcVar18 = (code *)SoftwareBreakpoint(1,0x10053661c);
        (*pcVar18)();
      }
      (**(code **)(lVar19 + 0x10))(url.unknown,lVar16,UVar3.unknown);
      Foundation::URLRequest::init(url,(NSURLRequestCachePolicy)0x0,60.0);
      (*pcVar18)(lVar20,1,1,UVar3.unknown);
      FUN_1004f8578(lVar13,lVar20);
      *(char *)(lVar20 + *(int *)(lVar2 + 0x14)) = (char)(0x503020400 >> ((IVar6 & 7) << 3));
      lVar2 = lVar5;
      _swift_getObjectType(lVar5);
      pcVar18 = *(code **)(lVar17 + 8);
      *(long *)(lVar16 + -0x10) = lVar17;
      IVar6 = local_218;
      uVar11 = local_228;
      pvVar14 = local_230;
      ppuVar10 = &PTR_PTR_10418d648;
      lVar13 = lVar20;
      IVar15 = local_218;
      (*pcVar18)(unaff_x20,&PTR_PTR_10418d648,lVar20,local_218,local_220,local_228,local_230,lVar2);
      FUN_1003568ec(IVar6,local_220,uVar11,pvVar14);
      FUN_100536028(unaff_x20,ppuVar10,lVar13,IVar15);
      _swift_unknownObjectRelease(lVar5);
      (**(code **)(lVar19 + 8))(lVar16,UVar3.unknown);
      FUN_100536bfc(lVar20);
    }
  }
  return unaff_x20;
}


