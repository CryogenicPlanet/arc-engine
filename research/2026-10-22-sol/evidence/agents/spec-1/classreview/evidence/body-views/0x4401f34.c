// ==== 0x4401f34  objc::Categories::ArcCommandBarRankingExample_Extensions_::candidates  (932 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcCommandBarRankingExample_Extensions_::candidates(ID param_1,SEL param_2)

{
  int iVar1;
  int iVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  char cVar6;
  code *pcVar7;
  undefined8 *puVar8;
  ID self;
  ID IVar9;
  ID IVar10;
  ID IVar11;
  ID IVar12;
  ID IVar13;
  ID IVar14;
  ID IVar15;
  ID self_00;
  long lVar16;
  long lVar17;
  undefined8 *puVar18;
  uint *puVar19;
  float fVar20;
  int in_stack_fffffffffffffec8;
  undefined1 uVar21;
  bool in_stack_fffffffffffffed8;
  char *pcVar22;
  char *op;
  long local_b8;
  long local_b0;
  long local_a8;
  undefined8 *local_a0;
  long local_98;
  long local_90;
  byte local_80;
  
  _objc_msgSend(param_1,"command_bar_ranking_example",&local_b8);
  puVar18 = local_a0;
  if ((local_80 & 1) == 0) {
LAB_044022d4:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x44022d8);
    (*pcVar7)();
  }
  if (local_a0 != (undefined8 *)0x0) {
    if (local_98 * 0xa8 != 0) {
      puVar8 = local_a0 + local_98 * 0x15;
      lVar17 = local_98 * -0xa8;
      do {
        if ((dword *)puVar8 == &section_00000068.flags) goto LAB_044022d4;
        puVar8 = (undefined8 *)FUN_042a3ff4();
        lVar17 = lVar17 + 0xa8;
      } while (lVar17 != 0);
    }
    local_98 = ((long)puVar18 - (long)local_a0 >> 3) * -0x30c30c30c30c30c3;
    FUN_00005810(local_a0,local_90 * 0xa8);
  }
  if (local_b8 != 0) {
    if (local_b0 * 4 != 0) {
      lVar17 = local_b0 * -4;
      lVar16 = local_b0 * 4 + local_b8;
      do {
        lVar16 = lVar16 + -4;
        if (lVar16 == 0) goto LAB_044022d4;
        lVar17 = lVar17 + 4;
      } while (lVar17 != 0);
    }
    local_b0 = 0;
    FUN_00005810(local_b8,local_a8 << 2);
  }
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_98);
  if (local_98 * 0xa8 != 0) {
    lVar17 = local_98 * 0x15;
    pcVar22 = 
    "init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding:"
    ;
    op = "addObject:";
    puVar18 = local_a0;
    do {
      IVar10 = _objc_alloc(&class_t::ArcCommandBarRankingCandidate);
      cVar6 = *(char *)((long)puVar18 + 0x17);
      puVar8 = (undefined8 *)*puVar18;
      if (-1 < (long)cVar6) {
        puVar8 = puVar18;
      }
      lVar16 = puVar18[1];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar11 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x2f);
      puVar8 = (undefined8 *)puVar18[3];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 3;
      }
      lVar16 = puVar18[4];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar12 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x47);
      puVar8 = (undefined8 *)puVar18[6];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 6;
      }
      lVar16 = puVar18[7];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar13 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x5f);
      puVar8 = (undefined8 *)puVar18[9];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 9;
      }
      lVar16 = puVar18[10];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar14 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x77);
      puVar8 = (undefined8 *)puVar18[0xc];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 0xc;
      }
      lVar16 = puVar18[0xd];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar15 = FUN_01c391bc(puVar8,lVar16);
      iVar1 = *(int *)(puVar18 + 0xf);
      iVar2 = *(int *)((long)puVar18 + 0x7c);
      fVar20 = *(float *)((long)puVar18 + 0x84);
      uVar3 = *(undefined1 *)(puVar18 + 0x11);
      uVar4 = *(undefined1 *)((long)puVar18 + 0x89);
      uVar5 = *(undefined1 *)((long)puVar18 + 0x8a);
      self_00 = NSMutableArray::arrayWithCapacity_
                          ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",puVar18[0x13]);
      uVar21 = (undefined1)(in_stack_fffffffffffffec8 >> 0x18);
      lVar16 = puVar18[0x13] << 2;
      if (lVar16 != 0) {
        puVar19 = (uint *)puVar18[0x12];
        do {
          IVar9 = NSNumber::numberWithFloat_
                            ((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",(ulong)*puVar19);
          NSMutableArray::addObject_(self_00,"addObject:",IVar9);
          uVar21 = (undefined1)(in_stack_fffffffffffffec8 >> 0x18);
          lVar16 = lVar16 + -4;
          puVar19 = puVar19 + 1;
        } while (lVar16 != 0);
      }
      in_stack_fffffffffffffec8 = CONCAT31(CONCAT21(CONCAT11(uVar21,uVar5),uVar4),uVar3);
      IVar10 = ArcCommandBarRankingCandidate::
               init_archetype_destination_title_url_lastNavSeconds_freqEver_typedCount_siteEngagement_isExactURLMatch_isBaseDomainMatch_isTitlePrefix_titleEmbedding_
                         (IVar10,pcVar22,IVar11,IVar12,IVar13,IVar14,IVar15,iVar1,iVar2,
                          in_stack_fffffffffffffec8,fVar20,(bool)self_00,in_stack_fffffffffffffed8,
                          (bool)pcVar22,(ID)op);
      NSMutableArray::addObject_(self,op,IVar10);
      puVar18 = puVar18 + 0x15;
    } while (puVar18 != local_a0 + lVar17);
  }
  return self;
}


