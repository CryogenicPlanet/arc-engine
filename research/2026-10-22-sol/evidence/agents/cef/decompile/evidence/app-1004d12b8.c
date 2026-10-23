// ==== 0x1004d12b8  FUN_1004d12b8  (5596 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Heritage AFTER dead removal. Example location: x0 : 0x0001004d1668 */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_1004d12b8(undefined8 param_1,undefined8 param_2,char *param_3,undefined *param_4)

{
  _CocoaArrayWrapper _Var1;
  ulong uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined1 uVar6;
  long *plVar7;
  undefined8 *puVar8;
  code *pcVar9;
  bool bVar10;
  uint uVar11;
  ulong uVar12;
  ulong uVar13;
  ID IVar14;
  ulong uVar15;
  char *pcVar16;
  undefined8 *puVar17;
  undefined8 *puVar18;
  ulong uVar20;
  Hasher HVar21;
  undefined *extraout_x1;
  undefined *extraout_x1_00;
  long extraout_x8;
  long extraout_x8_00;
  _CocoaArrayWrapper _Var22;
  undefined1 *puVar23;
  long lVar24;
  undefined *puVar25;
  undefined *puVar26;
  long extraout_x12;
  long extraout_x12_00;
  long extraout_x12_01;
  long extraout_x12_02;
  long extraout_x12_03;
  __int64 _Var27;
  undefined *puVar28;
  undefined *puVar29;
  undefined *puVar30;
  undefined8 uVar31;
  long lVar32;
  ulong *puVar33;
  undefined *puVar34;
  ulong uVar35;
  String SVar36;
  void *local_170;
  char *local_168;
  ulong local_160;
  undefined *local_158;
  long *local_150;
  ulong *local_148;
  undefined8 local_140;
  undefined8 local_138;
  char *local_130;
  ulong local_128;
  undefined8 *local_120;
  undefined *local_118;
  ID local_110;
  undefined *local_108;
  long local_100;
  undefined *local_f8;
  undefined *local_f0;
  undefined *local_e8;
  undefined *local_e0;
  long local_d8;
  long local_d0;
  undefined8 uStack_c8;
  undefined *local_c0;
  undefined *local_b8 [9];
  undefined *local_70;
  undefined *puVar19;
  
  puVar25 = &DAT_1044c8c18;
  local_158 = param_4;
  local_140 = param_1;
  local_138 = param_2;
  FUN_100005134(&DAT_1044c8c18,&BYTE_10363a690);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(puVar25 + -8) + 0x40));
  puVar25 = (undefined *)((long)&local_170 - (extraout_x8 + 0xfU & 0xfffffffffffffff0));
  local_e8 = puVar25;
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar24 = (long)puVar25 - extraout_x12;
  local_e0 = (undefined *)lVar24;
  (*(code *)PTR____chkstk_darwin_10415f308)();
  puVar29 = (undefined *)(lVar24 - extraout_x12_00);
  lVar24 = 0;
  FUN_1004cf480();
  local_d0 = *(long *)(lVar24 + -8);
  local_d8 = lVar24;
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(local_d0 + 0x40));
  puVar25 = puVar29 + -(extraout_x8_00 + 0xfU & 0xfffffffffffffff0);
  local_f0 = puVar25;
  (*(code *)PTR____chkstk_darwin_10415f308)();
  local_148 = (ulong *)(puVar25 + -extraout_x12_01);
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar24 = (long)(puVar25 + -extraout_x12_01) - extraout_x12_02;
  local_100 = lVar24;
  (*(code *)PTR____chkstk_darwin_10415f308)();
  local_150 = (long *)(lVar24 - extraout_x12_03);
  puVar25 = (undefined *)objc_stub::FUN_1035b5300((ID)param_3);
  uVar12 = 0;
  FUN_1000391c0(0,&DAT_1044c85d0,&PTR__OBJC_CLASS___ArcAutocompleteMatch_10446ffc8);
  local_128 = uVar12;
  (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
  puVar30 = (undefined *)((ulong)puVar25 & 0xffffffffffffff8);
  if ((ulong)puVar25 >> 0x3e == 0) {
    uVar12 = *(ulong *)(puVar30 + 0x10);
    _Var22.unknown = PTR___swiftEmptyArrayStorage_104167650;
  }
  else {
    _Var22.unknown = puVar30;
    if ((undefined *)0x7fffffffffffffff < puVar25) {
      _Var22.unknown = puVar25;
    }
    uVar12 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
    _Var22.unknown = PTR___swiftEmptyArrayStorage_104167650;
  }
  PTR___swiftEmptyArrayStorage_104167650 = _Var22.unknown;
  local_f8 = puVar29;
  if (uVar12 != 0) {
    uVar35 = 0;
    do {
      while( true ) {
        if (((ulong)puVar25 & 0xc000000000000001) == 0) {
          if (*(ulong *)(puVar30 + 0x10) <= uVar35) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1578);
            (*pcVar9)();
          }
          uVar13 = *(ulong *)(puVar25 + uVar35 * 8 + 0x20);
        }
        else {
          uVar13 = uVar35;
          FUN_1004b9ec4(uVar35,puVar25);
        }
        bVar10 = SCARRY8(uVar35,1);
        uVar35 = uVar35 + 1;
        if (bVar10) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1574);
          (*pcVar9)();
        }
        IVar14 = objc_stub::FUN_1035c3220(uVar13);
        if ((int)IVar14 == 0) break;
        if (uVar35 == uVar12) goto LAB_1004d1598;
      }
      puVar29 = _Var22.unknown;
      _swift_isUniquelyReferenced_nonNull_native();
      local_130 = param_3;
      local_b8[0] = _Var22.unknown;
      if (((ulong)puVar29 & 1) == 0) {
        FUN_1004badf4(0,*(long *)(_Var22.unknown + 0x10) + 1,1);
      }
      uVar15 = *(ulong *)(local_b8[0] + 0x10);
      if (*(ulong *)(local_b8[0] + 0x18) >> 1 <= uVar15) {
        FUN_1004badf4(1 < *(ulong *)(local_b8[0] + 0x18),uVar15 + 1,1);
      }
      *(ulong *)(local_b8[0] + 0x10) = uVar15 + 1;
      *(ulong *)(local_b8[0] + uVar15 * 8 + 0x20) = uVar13;
      _Var22.unknown = local_b8[0];
      param_3 = local_130;
    } while (uVar35 != uVar12);
  }
LAB_1004d1598:
  _swift_bridgeObjectRelease(puVar25);
  if (((long)_Var22.unknown < 0) || (((ulong)_Var22.unknown >> 0x3e & 1) != 0)) {
    puVar25 = (undefined *)Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
  }
  else {
    puVar25 = *(undefined **)(_Var22.unknown + 0x10);
  }
  local_120 = (undefined8 *)PTR___swiftEmptyArrayStorage_104167650;
  if (puVar25 != (undefined *)0x0) {
    uVar12 = 0;
    local_160 = (ulong)_Var22.unknown & 0xc000000000000001;
    local_120 = (undefined8 *)PTR___swiftEmptyArrayStorage_104167650;
    local_108 = puVar25;
    uVar35 = local_160;
    do {
      while( true ) {
        if (uVar35 == 0) {
          if (*(ulong *)(_Var22.unknown + 0x10) <= uVar12) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1ab8);
            (*pcVar9)();
          }
          uVar13 = *(ulong *)(_Var22.unknown + uVar12 * 8 + 0x20);
        }
        else {
          uVar13 = uVar12;
          FUN_1004b9ec4(uVar12,_Var22.unknown);
        }
        bVar10 = SCARRY8(uVar12,1);
        uVar12 = uVar12 + 1;
        if (bVar10) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1ab4);
          (*pcVar9)();
        }
        objc_stub::matchType(uVar13);
        SVar36 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
        FUN_100005134(&DAT_1044c8e00,&DAT_10363aae0);
        _swift_initStaticObject();
        uVar15 = Swift::_findStringSwitchCase(SVar36.str,SVar36.str,SVar36.bridgeObject);
        _swift_bridgeObjectRelease(SVar36.bridgeObject);
        if ((uVar15 < 0xb) && (IVar14 = objc_stub::destinationUrlSpec(uVar13), IVar14 != 0)) break;
        if ((undefined *)uVar12 == puVar25) goto LAB_1004d1ae0;
      }
      SVar36 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
      local_170 = SVar36.bridgeObject;
      puVar29 = (undefined *)objc_stub::additionalData(uVar13);
      FUN_1000391c0(0,&DAT_1044c86f0,&PTR__OBJC_CLASS___ArcAutocompleteMatchAdditionalData_104470028
                   );
      (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
      if ((ulong)puVar29 >> 0x3e == 0) {
        _Var27 = *(long *)(((ulong)puVar29 & 0xffffffffffffff8) + 0x10);
      }
      else {
        _Var1.unknown = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
        if ((undefined *)0x7fffffffffffffff < puVar29) {
          _Var1.unknown = puVar29;
        }
        _Var27 = Swift::_CocoaArrayWrapper::get_endIndex(_Var1);
        puVar25 = local_108;
      }
      local_168 = SVar36.str;
      local_130 = param_3;
      if (_Var27 == 0) {
        local_c0 = PTR___swiftEmptyArrayStorage_104167650;
      }
      else {
        uVar35 = 0;
        local_110 = (ulong)puVar29 & 0xc000000000000001;
        local_118 = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
        local_c0 = PTR___swiftEmptyArrayStorage_104167650;
        do {
          if (local_110 == 0) {
            puVar25 = local_e0;
            if (*(ulong *)(local_118 + 0x10) <= uVar35) {
                    /* WARNING: Does not return */
              pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1ac0);
              (*pcVar9)();
            }
          }
          else {
            FUN_1004b9ed8(uVar35);
            puVar25 = local_e0;
          }
          local_e0 = puVar25;
          if (SCARRY8(uVar35,1)) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d1abc);
            (*pcVar9)();
          }
          lVar24 = uVar35 + 1;
          FUN_1004cf7b4(puVar25);
          puVar29 = local_e8;
          FUN_1004d0998(puVar25,local_e8);
          puVar25 = puVar29;
          (**(code **)(local_d0 + 0x30))(puVar29,1,local_d8);
          if ((int)puVar25 == 1) {
            FUN_1004d09e8(puVar29);
          }
          else {
            FUN_1004d0a30(puVar29,local_f0);
            puVar25 = local_c0;
            _swift_isUniquelyReferenced_nonNull_native();
            if (((ulong)puVar25 & 1) == 0) {
              puVar25 = (undefined *)0x0;
              FUN_1004ba158(0,*(long *)(local_c0 + 0x10) + 1,1);
              local_c0 = puVar25;
            }
            uVar20 = *(ulong *)(local_c0 + 0x10);
            if (*(ulong *)(local_c0 + 0x18) >> 1 <= uVar20) {
              puVar25 = (undefined *)(ulong)(1 < *(ulong *)(local_c0 + 0x18));
              FUN_1004ba158(puVar25,uVar20 + 1,1,local_c0);
              local_c0 = puVar25;
            }
            *(ulong *)(local_c0 + 0x10) = uVar20 + 1;
            FUN_1004d0a30(local_f0,local_c0 +
                                   *(long *)(local_d0 + 0x48) * uVar20 +
                                   ((ulong)*(byte *)(local_d0 + 0x50) + 0x20 &
                                   ((ulong)*(byte *)(local_d0 + 0x50) ^ 0xffffffffffffffff)));
          }
          uVar35 = uVar35 + 1;
          puVar25 = local_108;
        } while (lVar24 != _Var27);
      }
      _swift_bridgeObjectRelease();
      IVar14 = objc_stub::relevance(uVar13);
      puVar33 = local_148;
      *local_148 = (long)(int)IVar14;
      _swift_storeEnumTagMultiPayload(puVar33,local_d8,0);
      puVar29 = local_c0;
      _swift_isUniquelyReferenced_nonNull_native();
      if (((ulong)puVar29 & 1) == 0) {
        puVar29 = (undefined *)0x0;
        FUN_1004ba158(0,*(long *)(local_c0 + 0x10) + 1,1);
        local_c0 = puVar29;
      }
      pcVar16 = local_168;
      uVar35 = *(ulong *)(local_c0 + 0x10);
      if (*(ulong *)(local_c0 + 0x18) >> 1 <= uVar35) {
        puVar29 = (undefined *)(ulong)(1 < *(ulong *)(local_c0 + 0x18));
        FUN_1004ba158(puVar29,uVar35 + 1,1,local_c0);
        local_c0 = puVar29;
      }
      *(ulong *)(local_c0 + 0x10) = uVar35 + 1;
      FUN_1004d0a30(local_148,
                    local_c0 +
                    *(long *)(local_d0 + 0x48) * uVar35 +
                    ((ulong)*(byte *)(local_d0 + 0x50) + 0x20 &
                    ((ulong)*(byte *)(local_d0 + 0x50) ^ 0xffffffffffffffff)));
      IVar14 = objc_stub::deletable(uVar13);
      if ((int)IVar14 == 0) {
        local_110 = 0;
        puVar30 = (undefined *)0x0;
        puVar29 = extraout_x1;
      }
      else {
        puVar29 = local_158;
        FUN_100527a8c();
        IVar14 = uVar13;
        puVar30 = puVar29;
        local_110 = uVar13;
      }
      FUN_1004d0094();
      FUN_1007e6288(pcVar16,local_170);
      puVar17 = local_120;
      _swift_isUniquelyReferenced_nonNull_native();
      puVar18 = local_120;
      if (((ulong)puVar17 & 1) == 0) {
        puVar18 = (undefined8 *)0x0;
        FUN_1004ba14c(0,local_120[2] + 1,1);
      }
      uVar35 = puVar18[2];
      if ((ulong)puVar18[3] >> 1 <= uVar35) {
        puVar18 = (undefined8 *)(ulong)(1 < (ulong)puVar18[3]);
        FUN_1004ba14c(puVar18,uVar35 + 1,1);
      }
      puVar18[2] = uVar35 + 1;
      *(char *)(puVar18 + uVar35 * 7 + 4) = (char)uVar15;
      puVar18[uVar35 * 7 + 5] = IVar14;
      puVar18[uVar35 * 7 + 6] = puVar29;
      puVar18[uVar35 * 7 + 7] = pcVar16;
      puVar18[uVar35 * 7 + 8] = local_c0;
      puVar18[uVar35 * 7 + 9] = local_110;
      puVar18[uVar35 * 7 + 10] = puVar30;
      param_3 = local_130;
      local_120 = puVar18;
      uVar35 = local_160;
    } while ((undefined *)uVar12 != puVar25);
  }
LAB_1004d1ae0:
  _swift_release(_Var22.unknown);
  puVar17 = local_120;
  lVar24 = local_120[2];
  puVar25 = PTR___swiftEmptyArrayStorage_104167650;
  if (lVar24 != 0) {
    local_b8[0] = PTR___swiftEmptyArrayStorage_104167650;
    FUN_1004badd8(0,lVar24,0);
    puVar25 = local_b8[0];
    uVar12 = *(ulong *)(local_b8[0] + 0x10);
    do {
      puVar17 = puVar17 + 7;
      uVar31 = *puVar17;
      uVar35 = uVar12 + 1;
      uVar13 = *(ulong *)(puVar25 + 0x18);
      local_b8[0] = puVar25;
      _swift_retain(uVar31);
      if (uVar13 >> 1 <= uVar12) {
        FUN_1004badd8(1 < uVar13,uVar35,1);
        puVar25 = local_b8[0];
      }
      *(ulong *)(puVar25 + 0x10) = uVar35;
      *(undefined8 *)(puVar25 + uVar12 * 8 + 0x20) = uVar31;
      lVar24 = lVar24 + -1;
      uVar12 = uVar35;
    } while (lVar24 != 0);
  }
  puVar29 = puVar25;
  FUN_1004d08b0();
  local_c0 = puVar29;
  _swift_bridgeObjectRelease(puVar25);
  objc_stub::FUN_1035c8080((ID)param_3);
  FUN_100005134(&DAT_1044c8e08,&DAT_10363a7d0);
  local_108 = (undefined *)
              (extension_Foundation)::Swift::Dictionary::__unconditionallyBridgeFromObjectiveC();
  puVar33 = (ulong *)(local_108 + 0x40);
  local_130 = (char *)(-1L << ((ulong)(byte)local_108[0x20] & 0x3f));
  uVar12 = 0xffffffffffffffff;
  if ((ulong)-(long)local_130 < 0x40) {
    uVar12 = ~(-1L << (-(long)local_130 & 0x3fU));
  }
  uVar12 = uVar12 & *puVar33;
  uVar35 = 0x3f - (long)local_130;
  _swift_bridgeObjectRetain(local_108);
  puVar25 = (undefined *)0x0;
  local_110 = uVar35 >> 6;
  puVar30 = puVar25;
  puVar29 = PTR___swiftEmptyArrayStorage_104167650;
joined_r0x0001004d1c5c:
  puVar34 = local_108;
  if (uVar12 != 0) {
    uVar35 = (uVar12 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar12 & 0x5555555555555555) << 1;
    uVar35 = (uVar35 & 0xcccccccccccccccc) >> 2 | (uVar35 & 0x3333333333333333) << 2;
    uVar35 = (uVar35 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar35 & 0xf0f0f0f0f0f0f0f) << 4;
    uVar35 = (uVar35 & 0xff00ff00ff00ff00) >> 8 | (uVar35 & 0xff00ff00ff00ff) << 8;
    uVar35 = (uVar35 & 0xffff0000ffff0000) >> 0x10 | (uVar35 & 0xffff0000ffff) << 0x10;
    puVar30 = *(undefined **)
               (*(long *)(local_108 + 0x38) + LZCOUNT(uVar35 >> 0x20 | uVar35 << 0x20) * 8 +
               (long)puVar25 * 0x200);
    if ((ulong)puVar30 >> 0x3e == 0) {
      puVar34 = *(undefined **)((undefined *)((ulong)puVar30 & 0xffffffffffffff8) + 0x10);
    }
    else {
      _Var22.unknown = (undefined *)((ulong)puVar30 & 0xffffffffffffff8);
      if (((ulong)puVar30 & 0x8000000000000000) != 0) {
        _Var22.unknown = puVar30;
      }
      puVar34 = (undefined *)Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
    }
    uVar35 = (ulong)puVar29 >> 0x3e;
    if (uVar35 == 0) {
      lVar24 = *(long *)((undefined *)((ulong)puVar29 & 0xffffffffffffff8) + 0x10);
    }
    else {
      _Var22.unknown = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
      if (((ulong)puVar29 & 0x8000000000000000) != 0) {
        _Var22.unknown = puVar29;
      }
      lVar24 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
    }
    if (SCARRY8(lVar24,(long)puVar34)) {
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20d4);
      (*pcVar9)();
    }
    puVar28 = puVar34 + lVar24;
    _swift_bridgeObjectRetain(puVar30);
    puVar19 = puVar29;
    _swift_isUniquelyReferenced_nonNull_bridgeObject();
    uVar11 = 0;
    if (uVar35 == 0) {
      uVar11 = (uint)puVar19;
    }
    puVar19 = (undefined *)(ulong)uVar11;
    if ((uVar11 != 1) ||
       (puVar26 = (undefined *)((ulong)puVar29 & 0xffffffffffffff8),
       (long)(*(ulong *)(puVar26 + 0x18) >> 1) < (long)puVar28)) {
      if (uVar35 == 0) {
        puVar26 = *(undefined **)((undefined *)((ulong)puVar29 & 0xffffffffffffff8) + 0x10);
      }
      else {
        _Var22.unknown = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
        if (((ulong)puVar29 & 0x8000000000000000) != 0) {
          _Var22.unknown = puVar29;
        }
        puVar26 = (undefined *)Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
      }
      if ((long)puVar26 <= (long)puVar28) {
        puVar26 = puVar28;
      }
      FUN_1004ba014(puVar19,puVar26,1,puVar29);
      puVar26 = (undefined *)((ulong)puVar19 & 0xffffffffffffff8);
      puVar29 = puVar19;
    }
    uVar12 = uVar12 - 1 & uVar12;
    lVar24 = *(long *)(puVar26 + 0x10);
    uVar35 = (*(ulong *)(puVar26 + 0x18) >> 1) - lVar24;
    _Var22.unknown = puVar30 & 0xffffffffffffff8;
    local_f0 = puVar34;
    local_e8 = puVar26;
    if ((ulong)puVar30 >> 0x3e == 0) {
      uVar13 = *(ulong *)(_Var22.unknown + 0x10);
      if (uVar13 != 0) {
        if (uVar35 < uVar13) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20e0);
          (*pcVar9)();
        }
        _swift_arrayInitWithCopy(puVar26 + lVar24 * 8 + 0x20,_Var22.unknown + 0x20,uVar13,local_128)
        ;
LAB_1004d1f1c:
        _swift_bridgeObjectRelease(puVar30);
        if ((long)uVar13 < (long)local_f0) goto LAB_1004d20d4;
        puVar30 = puVar25;
        if (0 < (long)uVar13) {
          if (SCARRY8(*(long *)(local_e8 + 0x10),uVar13)) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20dc);
            (*pcVar9)();
          }
          *(ulong *)(local_e8 + 0x10) = *(long *)(local_e8 + 0x10) + uVar13;
        }
        goto joined_r0x0001004d1c5c;
      }
    }
    else {
      if (((ulong)puVar30 & 0x8000000000000000) != 0) {
        _Var22.unknown = puVar30;
      }
      _Var27 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
      if (_Var27 != 0) {
        uVar13 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
        if ((long)uVar35 < (long)uVar13) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20e4);
          (*pcVar9)();
        }
        if (_Var27 < 1) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20e8);
          (*pcVar9)();
        }
        puVar17 = (undefined8 *)(local_e8 + lVar24 * 8 + 0x20);
        local_118 = puVar25;
        if (((ulong)puVar30 & 0xc000000000000001) == 0) {
          *puVar17 = *(undefined8 *)(puVar30 + 0x20);
          uVar35 = _Var27 - 1;
          if (uVar35 != 0) {
            if (_Var27 - 2U < 7) {
              lVar32 = 1;
            }
            else {
              uVar15 = uVar35 & 0xfffffffffffffff8;
              puVar18 = (undefined8 *)(puVar30 + 0x30);
              lVar32 = 1;
              puVar8 = (undefined8 *)(local_e8 + lVar24 * 8 + 0x60);
              do {
                puVar17 = puVar8;
                puVar17[-7] = puVar18[-1];
                puVar17[-6] = *puVar18;
                puVar17[-5] = puVar18[1];
                puVar17[-4] = puVar18[2];
                puVar17[-3] = puVar18[3];
                puVar17[-2] = puVar18[4];
                puVar17[-1] = puVar18[5];
                lVar32 = lVar32 + 8;
                *puVar17 = puVar18[6];
                puVar18 = puVar18 + 8;
                uVar15 = uVar15 - 8;
                puVar8 = puVar17 + 8;
                local_160 = uVar35;
                local_148 = puVar33;
              } while (uVar15 != 0);
            }
            uVar35 = uVar35 & 7;
            if (uVar35 != 0) {
              puVar18 = (undefined8 *)(puVar30 + lVar32 * 8 + 0x20);
              do {
                puVar17 = puVar17 + 1;
                *puVar17 = *puVar18;
                uVar35 = uVar35 - 1;
                puVar18 = puVar18 + 1;
              } while (uVar35 != 0);
            }
          }
        }
        else {
          lVar24 = 0;
          do {
            lVar32 = lVar24;
            FUN_1004b9ec4(lVar24,puVar30);
            puVar17[lVar24] = lVar32;
            lVar24 = lVar24 + 1;
            puVar25 = local_118;
          } while (_Var27 != lVar24);
        }
        goto LAB_1004d1f1c;
      }
    }
    _swift_bridgeObjectRelease(puVar30);
    puVar30 = puVar25;
    if (0 < (long)local_f0) {
LAB_1004d20d4:
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20d8);
      (*pcVar9)();
    }
    goto joined_r0x0001004d1c5c;
  }
  bVar10 = SCARRY8((long)puVar25,1);
  puVar25 = puVar25 + 1;
  if (bVar10) {
                    /* WARNING: Does not return */
    pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20c8);
    (*pcVar9)();
  }
  if ((long)puVar25 < (long)local_110) {
    uVar12 = puVar33[(long)puVar25];
    goto joined_r0x0001004d1c5c;
  }
  _swift_bridgeObjectRelease(local_108);
  FUN_100089af0(puVar34,puVar33,~(ulong)local_130,puVar30,0);
  puVar25 = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
  if ((ulong)puVar29 >> 0x3e == 0) {
    uVar12 = *(ulong *)(puVar25 + 0x10);
    _Var22.unknown = PTR___swiftEmptyArrayStorage_104167650;
  }
  else {
    _Var22.unknown = puVar25;
    if ((undefined *)0x7fffffffffffffff < puVar29) {
      _Var22.unknown = puVar29;
    }
    uVar12 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
    _Var22.unknown = PTR___swiftEmptyArrayStorage_104167650;
  }
  PTR___swiftEmptyArrayStorage_104167650 = _Var22.unknown;
  if (uVar12 != 0) {
    uVar35 = 0;
    do {
      if (((ulong)puVar29 & 0xc000000000000001) == 0) {
        if (*(ulong *)(puVar25 + 0x10) <= uVar35) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20d0);
          (*pcVar9)();
        }
        uVar13 = *(ulong *)(puVar29 + uVar35 * 8 + 0x20);
      }
      else {
        uVar13 = uVar35;
        FUN_1004b9ec4(uVar35,puVar29);
      }
      bVar10 = SCARRY8(uVar35,1);
      uVar35 = uVar35 + 1;
      if (bVar10) {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d20cc);
        (*pcVar9)();
      }
      IVar14 = objc_stub::FUN_1035c3220(uVar13);
      if ((int)IVar14 == 0) {
        puVar30 = _Var22.unknown;
        _swift_isUniquelyReferenced_nonNull_native();
        local_b8[0] = _Var22.unknown;
        if (((ulong)puVar30 & 1) == 0) {
          FUN_1004badf4(0,*(long *)(_Var22.unknown + 0x10) + 1,1);
        }
        uVar15 = *(ulong *)(local_b8[0] + 0x10);
        if (*(ulong *)(local_b8[0] + 0x18) >> 1 <= uVar15) {
          FUN_1004badf4(1 < *(ulong *)(local_b8[0] + 0x18),uVar15 + 1,1);
        }
        *(ulong *)(local_b8[0] + 0x10) = uVar15 + 1;
        *(ulong *)(local_b8[0] + uVar15 * 8 + 0x20) = uVar13;
        _Var22.unknown = local_b8[0];
      }
    } while (uVar35 != uVar12);
  }
  _swift_bridgeObjectRelease(puVar29);
  if (((long)_Var22.unknown < 0) || (((ulong)_Var22.unknown >> 0x3e & 1) != 0)) {
    uVar12 = Swift::_CocoaArrayWrapper::get_endIndex(_Var22);
    puVar25 = PTR___swiftEmptyArrayStorage_104167650;
  }
  else {
    uVar12 = *(ulong *)(_Var22.unknown + 0x10);
    puVar25 = PTR___swiftEmptyArrayStorage_104167650;
  }
  PTR___swiftEmptyArrayStorage_104167650 = puVar25;
  if (uVar12 != 0) {
    uVar35 = 0;
    uVar13 = (ulong)_Var22.unknown & 0xc000000000000001;
    do {
      while( true ) {
        if (uVar13 == 0) {
          if (*(ulong *)(_Var22.unknown + 0x10) <= uVar35) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d25f4);
            (*pcVar9)();
          }
          uVar15 = *(ulong *)(_Var22.unknown + uVar35 * 8 + 0x20);
        }
        else {
          uVar15 = uVar35;
          FUN_1004b9ec4(uVar35,_Var22.unknown);
        }
        bVar10 = SCARRY8(uVar35,1);
        uVar35 = uVar35 + 1;
        if (bVar10) {
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d25f0);
          (*pcVar9)();
        }
        objc_stub::matchType(uVar15);
        SVar36 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
        FUN_100005134(&DAT_1044c8e00,&DAT_10363aae0);
        _swift_initStaticObject();
        uVar20 = Swift::_findStringSwitchCase(SVar36.str,SVar36.str,SVar36.bridgeObject);
        _swift_bridgeObjectRelease(SVar36.bridgeObject);
        if ((uVar20 < 0xb) && (IVar14 = objc_stub::destinationUrlSpec(uVar15), IVar14 != 0)) break;
        if (uVar35 == uVar12) goto LAB_1004d2614;
      }
      SVar36 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
      local_148 = SVar36.bridgeObject;
      local_130 = SVar36.str;
      puVar29 = (undefined *)objc_stub::additionalData(uVar15);
      FUN_1000391c0(0,&DAT_1044c86f0,&PTR__OBJC_CLASS___ArcAutocompleteMatchAdditionalData_104470028
                   );
      (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
      if ((ulong)puVar29 >> 0x3e == 0) {
        _Var27 = *(long *)(((ulong)puVar29 & 0xffffffffffffff8) + 0x10);
      }
      else {
        _Var1.unknown = (undefined *)((ulong)puVar29 & 0xffffffffffffff8);
        if ((undefined *)0x7fffffffffffffff < puVar29) {
          _Var1.unknown = puVar29;
        }
        _Var27 = Swift::_CocoaArrayWrapper::get_endIndex(_Var1);
      }
      local_128 = uVar13;
      local_e8 = puVar25;
      if (_Var27 == 0) {
        local_f0 = PTR___swiftEmptyArrayStorage_104167650;
      }
      else {
        uVar13 = 0;
        local_108 = (undefined *)((ulong)puVar29 & 0xc000000000000001);
        local_110 = (ulong)puVar29 & 0xffffffffffffff8;
        local_f0 = PTR___swiftEmptyArrayStorage_104167650;
        local_118 = _Var22.unknown;
        do {
          if (local_108 == (undefined *)0x0) {
            puVar25 = local_e0;
            if (*(ulong *)(local_110 + 0x10) <= uVar13) {
                    /* WARNING: Does not return */
              pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d25fc);
              (*pcVar9)();
            }
          }
          else {
            FUN_1004b9ed8(uVar13,puVar29);
            puVar25 = local_e0;
          }
          local_e0 = puVar25;
          if (SCARRY8(uVar13,1)) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d25f8);
            (*pcVar9)();
          }
          lVar24 = uVar13 + 1;
          FUN_1004cf7b4(puVar25);
          puVar30 = local_f8;
          FUN_1004d0998(puVar25,local_f8);
          puVar25 = puVar30;
          (**(code **)(local_d0 + 0x30))(puVar30,1,local_d8);
          if ((int)puVar25 == 1) {
            FUN_1004d09e8(puVar30);
          }
          else {
            FUN_1004d0a30(puVar30,local_100);
            puVar25 = local_f0;
            _swift_isUniquelyReferenced_nonNull_native();
            if (((ulong)puVar25 & 1) == 0) {
              puVar25 = (undefined *)0x0;
              FUN_1004ba158(0,*(long *)(local_f0 + 0x10) + 1,1);
              local_f0 = puVar25;
            }
            uVar2 = *(ulong *)(local_f0 + 0x10);
            if (*(ulong *)(local_f0 + 0x18) >> 1 <= uVar2) {
              puVar25 = (undefined *)(ulong)(1 < *(ulong *)(local_f0 + 0x18));
              FUN_1004ba158(puVar25,uVar2 + 1,1,local_f0);
              local_f0 = puVar25;
            }
            *(ulong *)(local_f0 + 0x10) = uVar2 + 1;
            FUN_1004d0a30(local_100,
                          local_f0 +
                          *(long *)(local_d0 + 0x48) * uVar2 +
                          ((ulong)*(byte *)(local_d0 + 0x50) + 0x20 &
                          ((ulong)*(byte *)(local_d0 + 0x50) ^ 0xffffffffffffffff)));
          }
          uVar13 = uVar13 + 1;
          _Var22.unknown = local_118;
        } while (lVar24 != _Var27);
      }
      _swift_bridgeObjectRelease(puVar29);
      IVar14 = objc_stub::relevance(uVar15);
      plVar7 = local_150;
      *local_150 = (long)(int)IVar14;
      _swift_storeEnumTagMultiPayload(plVar7,local_d8,0);
      puVar25 = local_f0;
      _swift_isUniquelyReferenced_nonNull_native();
      if (((ulong)puVar25 & 1) == 0) {
        puVar25 = (undefined *)0x0;
        FUN_1004ba158(0,*(long *)(local_f0 + 0x10) + 1,1);
        local_f0 = puVar25;
      }
      uVar13 = *(ulong *)(local_f0 + 0x10);
      if (*(ulong *)(local_f0 + 0x18) >> 1 <= uVar13) {
        puVar25 = (undefined *)(ulong)(1 < *(ulong *)(local_f0 + 0x18));
        FUN_1004ba158(puVar25,uVar13 + 1,1,local_f0);
        local_f0 = puVar25;
      }
      *(ulong *)(local_f0 + 0x10) = uVar13 + 1;
      FUN_1004d0a30(local_150,
                    local_f0 +
                    *(long *)(local_d0 + 0x48) * uVar13 +
                    ((ulong)*(byte *)(local_d0 + 0x50) + 0x20 &
                    ((ulong)*(byte *)(local_d0 + 0x50) ^ 0xffffffffffffffff)));
      IVar14 = objc_stub::deletable(uVar15);
      if ((int)IVar14 == 0) {
        uVar15 = 0;
        local_108 = (undefined *)0x0;
        puVar25 = extraout_x1_00;
      }
      else {
        puVar25 = local_158;
        FUN_100527a8c();
        IVar14 = uVar15;
        local_108 = puVar25;
      }
      FUN_1004d0094();
      pcVar16 = local_130;
      FUN_1007e6288(local_130,local_148);
      puVar29 = local_e8;
      _swift_isUniquelyReferenced_nonNull_native();
      if (((ulong)puVar29 & 1) == 0) {
        puVar29 = (undefined *)0x0;
        FUN_1004ba14c(0,*(long *)(local_e8 + 0x10) + 1,1);
        local_e8 = puVar29;
      }
      uVar13 = *(ulong *)(local_e8 + 0x10);
      if (*(ulong *)(local_e8 + 0x18) >> 1 <= uVar13) {
        puVar29 = (undefined *)(ulong)(1 < *(ulong *)(local_e8 + 0x18));
        FUN_1004ba14c(puVar29,uVar13 + 1,1,local_e8);
        local_e8 = puVar29;
      }
      *(ulong *)(local_e8 + 0x10) = uVar13 + 1;
      local_e8[uVar13 * 0x38 + 0x20] = (char)uVar20;
      *(ID *)(local_e8 + uVar13 * 0x38 + 0x28) = IVar14;
      *(undefined **)(local_e8 + uVar13 * 0x38 + 0x30) = puVar25;
      *(char **)(local_e8 + uVar13 * 0x38 + 0x38) = pcVar16;
      *(undefined **)(local_e8 + uVar13 * 0x38 + 0x40) = local_f0;
      *(ulong *)(local_e8 + uVar13 * 0x38 + 0x48) = uVar15;
      *(undefined **)(local_e8 + uVar13 * 0x38 + 0x50) = local_108;
      puVar25 = local_e8;
      uVar13 = local_128;
    } while (uVar35 != uVar12);
  }
LAB_1004d2614:
  _swift_release(_Var22.unknown);
  puVar29 = local_c0;
  local_f0 = *(undefined **)(puVar25 + 0x10);
  if (local_f0 == (undefined *)0x0) {
    local_e0 = PTR___swiftEmptyArrayStorage_104167650;
  }
  else {
    uVar12 = 0;
    local_f8 = puVar25 + 0x20;
    local_e0 = PTR___swiftEmptyArrayStorage_104167650;
    local_e8 = puVar25;
    do {
      puVar25 = local_c0;
      if (*(ulong *)(local_e8 + 0x10) <= uVar12) {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(1,0x1004d2894);
        (*pcVar9)();
      }
      puVar23 = local_f8 + uVar12 * 0x38;
      uVar6 = *puVar23;
      local_d8 = *(long *)(puVar23 + 8);
      uStack_c8 = *(undefined8 *)(puVar23 + 0x18);
      local_d0 = *(long *)(puVar23 + 0x10);
      uVar31 = *(undefined8 *)(puVar23 + 0x18);
      uVar4 = *(undefined8 *)(puVar23 + 0x20);
      uVar3 = *(undefined8 *)(puVar23 + 0x28);
      uVar5 = *(undefined8 *)(puVar23 + 0x30);
      uVar12 = uVar12 + 1;
      if (*(long *)(local_c0 + 0x10) == 0) {
        _swift_bridgeObjectRetain(uVar5);
        _swift_bridgeObjectRetain(local_d0);
        _swift_retain(uVar31);
        _swift_bridgeObjectRetain(uVar4);
      }
      else {
        Swift::Hasher::init(*(__int64 *)(local_c0 + 0x28));
        _swift_bridgeObjectRetain(uVar5);
        lVar24 = local_d0;
        _swift_bridgeObjectRetain(local_d0);
        _swift_retain(uVar31);
        _swift_bridgeObjectRetain(uVar4);
        HVar21.unknown = (undefined *)local_b8;
        FUN_1007e64b8(HVar21.unknown,uVar31);
        uVar35 = Swift::Hasher::_finalize(HVar21);
        uVar13 = -1L << ((ulong)(byte)puVar25[0x20] & 0x3f);
        uVar35 = uVar35 & (uVar13 ^ 0xffffffffffffffff);
        if ((*(ulong *)(puVar29 + (uVar35 >> 6) * 8 + 0x38) >> (uVar35 & 0x3f) & 1) != 0) {
          do {
            uVar15 = *(ulong *)(*(long *)(local_c0 + 0x30) + uVar35 * 8);
            FUN_1007e6488(uVar15,uVar31);
            if ((uVar15 & 1) != 0) {
              _swift_bridgeObjectRelease(uVar5);
              _swift_bridgeObjectRelease(uVar4);
              _swift_release(uVar31);
              _swift_bridgeObjectRelease(lVar24);
              goto LAB_1004d2678;
            }
            uVar35 = uVar35 + 1 & ~uVar13;
          } while ((*(ulong *)(puVar29 + (uVar35 >> 6) * 8 + 0x38) >> (uVar35 & 0x3f) & 1) != 0);
        }
      }
      puVar25 = local_e0;
      puVar30 = local_e0;
      _swift_isUniquelyReferenced_nonNull_native();
      local_70 = puVar25;
      if (((ulong)puVar30 & 1) == 0) {
        FUN_1004badb4(0,*(long *)(puVar25 + 0x10) + 1,1);
      }
      uVar35 = *(ulong *)(local_70 + 0x10);
      if (*(ulong *)(local_70 + 0x18) >> 1 <= uVar35) {
        FUN_1004badb4(1 < *(ulong *)(local_70 + 0x18),uVar35 + 1,1);
      }
      *(ulong *)(local_70 + 0x10) = uVar35 + 1;
      local_70[uVar35 * 0x38 + 0x20] = uVar6;
      *(long *)(local_70 + uVar35 * 0x38 + 0x28) = local_d8;
      *(undefined8 *)(local_70 + uVar35 * 0x38 + 0x38) = uStack_c8;
      *(long *)(local_70 + uVar35 * 0x38 + 0x30) = local_d0;
      *(undefined8 *)(local_70 + uVar35 * 0x38 + 0x40) = uVar4;
      *(undefined8 *)(local_70 + uVar35 * 0x38 + 0x48) = uVar3;
      *(undefined8 *)(local_70 + uVar35 * 0x38 + 0x50) = uVar5;
      local_e0 = local_70;
LAB_1004d2678:
      puVar25 = local_e8;
    } while ((undefined *)uVar12 != local_f0);
  }
  _swift_bridgeObjectRelease(local_c0);
  _swift_bridgeObjectRelease(puVar25);
  _swift_bridgeObjectRetain(local_138);
  return local_140;
}


