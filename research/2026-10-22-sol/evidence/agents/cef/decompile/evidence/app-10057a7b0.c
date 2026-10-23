// ==== 0x10057a7b0  FUN_10057a7b0  (836 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

undefined1  [16] FUN_10057a7b0(void)

{
  undefined8 *puVar1;
  _StringGuts _Var2;
  _StringGuts _Var3;
  long lVar4;
  code *pcVar5;
  bool bVar6;
  Dictionary<undefined,undefined> DVar7;
  undefined *puVar8;
  Dictionary<undefined,undefined> DVar9;
  long lVar10;
  ID unaff_x20;
  ulong uVar11;
  ulong *puVar12;
  long lVar13;
  undefined8 uVar14;
  ulong uVar15;
  ulong uVar16;
  undefined8 uVar17;
  ulong uVar18;
  undefined1 auVar19 [16];
  undefined8 local_80;
  undefined1 *local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  
  objc_stub::additionalData(unaff_x20);
  FUN_10057b67c();
  FUN_10057b6c0();
  DVar7 = (extension_Foundation)::Swift::Dictionary::__unconditionallyBridgeFromObjectiveC();
  puVar12 = (ulong *)(DVar7.unknown + 0x40);
  uVar18 = -1L << ((ulong)(byte)DVar7.unknown[0x20] & 0x3f);
  uVar15 = 0xffffffffffffffff;
  if (-uVar18 < 0x40) {
    uVar15 = ~(-1L << (-uVar18 & 0x3f));
  }
  uVar15 = uVar15 & *puVar12;
  _swift_bridgeObjectRetain(DVar7.unknown);
  lVar13 = 0;
  uVar11 = uVar15;
  lVar4 = lVar13;
  do {
    while (lVar10 = lVar4, uVar16 = uVar11, uVar15 == 0) {
      bVar6 = SCARRY8(lVar13,1);
      lVar13 = lVar13 + 1;
      if (bVar6) {
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(1,0x10057aaf0);
        (*pcVar5)();
      }
      if ((long)(0x3f - uVar18 >> 6) <= lVar13) {
        _swift_bridgeObjectRelease(DVar7.unknown);
        goto LAB_10057aa60;
      }
      uVar11 = uVar16;
      lVar4 = lVar10;
      uVar15 = puVar12[lVar13];
    }
    uVar11 = (uVar15 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar15 & 0x5555555555555555) << 1;
    uVar11 = (uVar11 & 0xcccccccccccccccc) >> 2 | (uVar11 & 0x3333333333333333) << 2;
    uVar11 = (uVar11 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar11 & 0xf0f0f0f0f0f0f0f) << 4;
    uVar11 = (uVar11 & 0xff00ff00ff00ff00) >> 8 | (uVar11 & 0xff00ff00ff00ff) << 8;
    uVar11 = (uVar11 & 0xffff0000ffff0000) >> 0x10 | (uVar11 & 0xffff0000ffff) << 0x10;
    puVar1 = (undefined8 *)
             (*(long *)(DVar7.unknown + 0x38) + LZCOUNT(uVar11 >> 0x20 | uVar11 << 0x20) * 0x10 +
             lVar13 * 0x400);
    _Var2.unknown = (undefined *)*puVar1;
    _Var3.unknown = (undefined *)puVar1[1];
    if (_Var2.unknown == (undefined *)0x6f746c69616d && _Var3.unknown == &DAT_e600000000000000)
    break;
    uVar15 = uVar15 - 1 & uVar15;
    bVar6 = Swift::_stringCompareWithSmolCheck(_Var2,_Var3,(_StringComparisonResult)0x6d);
    uVar11 = uVar15;
    lVar4 = lVar13;
  } while (!bVar6);
  _swift_bridgeObjectRelease(DVar7.unknown);
  FUN_100089af0(DVar7.unknown,puVar12,~uVar18,lVar10,uVar16);
  objc_stub::additionalData(unaff_x20);
  DVar7 = (extension_Foundation)::Swift::Dictionary::__unconditionallyBridgeFromObjectiveC();
  puVar12 = (ulong *)(DVar7.unknown + 0x40);
  uVar18 = -1L << ((ulong)(byte)DVar7.unknown[0x20] & 0x3f);
  uVar15 = 0xffffffffffffffff;
  if (-uVar18 < 0x40) {
    uVar15 = ~(-1L << (-uVar18 & 0x3f));
  }
  uVar15 = uVar15 & *puVar12;
  DVar9 = DVar7;
  _swift_bridgeObjectRetain(DVar7.unknown);
  lVar13 = 0;
  lVar10 = lVar13;
  uVar11 = uVar15;
  do {
    while (uVar15 != 0) {
      uVar16 = (uVar15 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar15 & 0x5555555555555555) << 1;
      uVar16 = (uVar16 & 0xcccccccccccccccc) >> 2 | (uVar16 & 0x3333333333333333) << 2;
      uVar16 = (uVar16 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar16 & 0xf0f0f0f0f0f0f0f) << 4;
      uVar16 = (uVar16 & 0xff00ff00ff00ff00) >> 8 | (uVar16 & 0xff00ff00ff00ff) << 8;
      uVar16 = (uVar16 & 0xffff0000ffff0000) >> 0x10 | (uVar16 & 0xffff0000ffff) << 0x10;
      puVar1 = (undefined8 *)
               (*(long *)(DVar7.unknown + 0x38) +
               (LZCOUNT(uVar16 >> 0x20 | uVar16 << 0x20) | lVar13 << 6) * 0x10);
      uVar17 = *puVar1;
      uVar14 = puVar1[1];
      local_80 = 0x70747468;
      local_78 = &DAT_e400000000000000;
      local_70 = uVar17;
      uStack_68 = uVar14;
      FUN_1004f1ed8();
      puVar8 = DVar9.unknown;
      FUN_1004f1f18();
      _swift_bridgeObjectRetain(uVar14);
      bVar6 = (extension__StringProcessing)::Swift::BidirectionalCollection::_starts
                        (&local_80,PTR___type_metadata_for_Swift_String_104164e78,
                         PTR___type_metadata_for_Swift_String_104164e78,DVar9.unknown,puVar8);
      if (bVar6) {
        _swift_bridgeObjectRelease(DVar7.unknown);
        FUN_100089af0(DVar7.unknown,puVar12,~uVar18,lVar10,uVar11);
LAB_10057aa70:
        auVar19._8_8_ = uVar14;
        auVar19._0_8_ = uVar17;
        return auVar19;
      }
      _swift_bridgeObjectRelease(uVar14);
      DVar9.unknown = (undefined *)0x0;
      uVar15 = uVar15 - 1 & uVar15;
      lVar10 = lVar13;
      uVar11 = uVar15;
    }
    bVar6 = SCARRY8(lVar13,1);
    lVar13 = lVar13 + 1;
    if (bVar6) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(1,0x10057aaf4);
      (*pcVar5)();
    }
    if ((long)(0x3f - uVar18 >> 6) <= lVar13) {
      _swift_bridgeObjectRelease(DVar7.unknown);
LAB_10057aa60:
      FUN_100089af0(DVar7.unknown,puVar12,~uVar18,lVar10,0);
      uVar17 = 0;
      uVar14 = 0;
      goto LAB_10057aa70;
    }
    uVar15 = puVar12[lVar13];
  } while( true );
}


