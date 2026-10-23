// ==== 0x1004cefd0  FUN_1004cefd0  (1200 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void FUN_1004cefd0(ulong *param_1,code *param_2,long param_3)

{
  _CocoaArrayWrapper _Var1;
  void *pvVar2;
  char *pcVar3;
  code *pcVar4;
  long lVar5;
  char *pcVar6;
  ID IVar7;
  long lVar8;
  undefined *puVar9;
  undefined *puVar10;
  undefined *puVar11;
  ulong extraout_x1;
  ulong uVar12;
  long extraout_x8;
  long extraout_x8_00;
  ulong uVar13;
  long *plVar14;
  long extraout_x12;
  long extraout_x12_00;
  long lVar15;
  ID unaff_x20;
  __int64 _Var16;
  long lVar17;
  ulong uVar18;
  long lVar19;
  long lVar20;
  undefined *puVar21;
  String SVar22;
  long local_c0;
  code *pcStack_b8;
  void *local_b0;
  char *local_a8;
  char *local_a0;
  
  puVar21 = &DAT_1044c8c18;
  FUN_100005134(&DAT_1044c8c18,&BYTE_10363a690);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(puVar21 + -8) + 0x40));
  lVar20 = (long)&local_c0 - (extraout_x8 + 0xfU & 0xfffffffffffffff0);
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar17 = lVar20 - extraout_x12;
  lVar5 = 0;
  FUN_1004cf480();
  lVar15 = *(long *)(lVar5 + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar15 + 0x40));
  plVar14 = (long *)(lVar17 - (extraout_x8_00 + 0xfU & 0xfffffffffffffff0));
  (*(code *)PTR____chkstk_darwin_10415f308)();
  objc_stub::matchType(unaff_x20);
  SVar22 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  pcVar6 = SVar22.str;
  FUN_1004d0934(pcVar6,SVar22.bridgeObject);
  if ((((uint)pcVar6 & 0xff) == 0xb) ||
     (IVar7 = objc_stub::destinationUrlSpec(unaff_x20), IVar7 == 0)) {
    pcVar6 = (char *)0x0;
    uVar12 = 0;
    IVar7 = 0;
    uVar13 = 0;
    puVar21 = (undefined *)0x0;
    unaff_x20 = 0;
    uVar18 = 0;
  }
  else {
    local_a0 = pcVar6;
    SVar22 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
    local_b0 = SVar22.bridgeObject;
    local_a8 = SVar22.str;
    puVar21 = (undefined *)objc_stub::additionalData(unaff_x20);
    FUN_1000391c0(0,&DAT_1044c86f0,&PTR__OBJC_CLASS___ArcAutocompleteMatchAdditionalData_104470028);
    (extension_Foundation)::Swift::Array<undefined>::__unconditionallyBridgeFromObjectiveC();
    if ((ulong)puVar21 >> 0x3e == 0) {
      _Var16 = *(long *)(((ulong)puVar21 & 0xffffffffffffff8) + 0x10);
    }
    else {
      _Var1.unknown = (undefined *)((ulong)puVar21 & 0xffffffffffffff8);
      if ((undefined *)0x7fffffffffffffff < puVar21) {
        _Var1.unknown = puVar21;
      }
      _Var16 = Swift::_CocoaArrayWrapper::get_endIndex(_Var1);
    }
    puVar10 = PTR___swiftEmptyArrayStorage_104167650;
    local_c0 = param_3;
    pcStack_b8 = param_2;
    if (_Var16 != 0) {
      uVar18 = 0;
      do {
        if (((ulong)puVar21 & 0xc000000000000001) == 0) {
          if (*(ulong *)(((ulong)puVar21 & 0xffffffffffffff8) + 0x10) <= uVar18) {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(1,0x1004cf428);
            (*pcVar4)();
          }
        }
        else {
          FUN_1004b9ed8(uVar18,puVar21);
        }
        if (SCARRY8(uVar18,1)) {
                    /* WARNING: Does not return */
          pcVar4 = (code *)SoftwareBreakpoint(1,0x1004cf424);
          (*pcVar4)();
        }
        lVar19 = uVar18 + 1;
        FUN_1004cf7b4(lVar20);
        FUN_1004d0998(lVar20,lVar17);
        lVar8 = lVar17;
        (**(code **)(lVar15 + 0x30))(lVar17,1,lVar5);
        if ((int)lVar8 == 1) {
          FUN_1004d09e8(lVar17);
        }
        else {
          FUN_1004d0a30(lVar17,(long)plVar14 - extraout_x12_00);
          puVar11 = puVar10;
          _swift_isUniquelyReferenced_nonNull_native();
          puVar9 = puVar10;
          if (((ulong)puVar11 & 1) == 0) {
            puVar9 = (undefined *)0x0;
            FUN_1004ba158(0,*(long *)(puVar10 + 0x10) + 1,1,puVar10);
          }
          uVar12 = *(ulong *)(puVar9 + 0x10);
          puVar10 = puVar9;
          if (*(ulong *)(puVar9 + 0x18) >> 1 <= uVar12) {
            puVar10 = (undefined *)(ulong)(1 < *(ulong *)(puVar9 + 0x18));
            FUN_1004ba158(puVar10,uVar12 + 1,1,puVar9);
          }
          *(ulong *)(puVar10 + 0x10) = uVar12 + 1;
          FUN_1004d0a30((long)plVar14 - extraout_x12_00,
                        puVar10 + *(long *)(lVar15 + 0x48) * uVar12 +
                                  ((ulong)*(byte *)(lVar15 + 0x50) + 0x20 &
                                  ((ulong)*(byte *)(lVar15 + 0x50) ^ 0xffffffffffffffff)));
        }
        uVar18 = uVar18 + 1;
      } while (lVar19 != _Var16);
    }
    _swift_bridgeObjectRelease(puVar21);
    IVar7 = objc_stub::relevance(unaff_x20);
    *plVar14 = (long)(int)IVar7;
    _swift_storeEnumTagMultiPayload(plVar14,lVar5,0);
    puVar21 = puVar10;
    _swift_isUniquelyReferenced_nonNull_native();
    puVar11 = puVar10;
    if (((ulong)puVar21 & 1) == 0) {
      puVar11 = (undefined *)0x0;
      FUN_1004ba158(0,*(long *)(puVar10 + 0x10) + 1,1,puVar10);
    }
    pcVar3 = local_a0;
    pcVar6 = local_a8;
    pvVar2 = local_b0;
    uVar18 = *(ulong *)(puVar11 + 0x10);
    puVar21 = puVar11;
    if (*(ulong *)(puVar11 + 0x18) >> 1 <= uVar18) {
      puVar21 = (undefined *)(ulong)(1 < *(ulong *)(puVar11 + 0x18));
      FUN_1004ba158(puVar21,uVar18 + 1,1,puVar11);
    }
    *(ulong *)(puVar21 + 0x10) = uVar18 + 1;
    FUN_1004d0a30(plVar14,puVar21 + *(long *)(lVar15 + 0x48) * uVar18 +
                                    ((ulong)*(byte *)(lVar15 + 0x50) + 0x20 &
                                    ((ulong)*(byte *)(lVar15 + 0x50) ^ 0xffffffffffffffff)));
    IVar7 = objc_stub::deletable(unaff_x20);
    uVar12 = extraout_x1;
    if ((IVar7 & 1) == 0) {
      unaff_x20 = 0;
      uVar18 = 0;
    }
    else {
      (*pcStack_b8)();
      IVar7 = unaff_x20;
      uVar18 = uVar12;
    }
    FUN_1004d0094();
    FUN_1007e6288(pcVar6,pvVar2);
    uVar13 = (ulong)pcVar3 & 0xff;
  }
  *param_1 = uVar13;
  param_1[1] = IVar7;
  param_1[2] = uVar12;
  param_1[3] = (ulong)pcVar6;
  param_1[4] = (ulong)puVar21;
  param_1[5] = unaff_x20;
  param_1[6] = uVar18;
  return;
}


