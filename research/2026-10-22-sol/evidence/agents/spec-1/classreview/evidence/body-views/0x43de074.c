// ==== 0x43de074  ArcContentConversionService::convertHtmlToMarkdown:url:linkStyle:strategy:urlTransform:skipNoscript:passThroughInputElements:metaTagsToCapture:deduplicateMetaTagContent:completion:  (1260 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x54 bytes */

void ArcContentConversionService::
     convertHtmlToMarkdown_url_linkStyle_strategy_urlTransform_skipNoscript_passThroughInputElements_metaTagsToCapture_deduplicateMetaTagContent_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5,long_long param_6,
               long_long param_7,bool param_8,bool param_9,ID param_10,bool param_11,ID param_12,
               undefined4 param_13)

{
  ulong uVar1;
  undefined4 *puVar2;
  code *pcVar3;
  ID IVar4;
  undefined4 *puVar5;
  ID IVar6;
  SEL extraout_x1;
  long lVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  ulong uVar10;
  long lVar11;
  ulong uVar12;
  undefined4 *puVar13;
  ID self;
  undefined8 uVar14;
  undefined4 *puVar15;
  ulong uVar16;
  undefined8 uVar17;
  undefined1 auStack_248 [104];
  undefined8 local_1e0;
  undefined8 uStack_1d8;
  undefined8 local_1d0;
  ulong local_1c8;
  undefined8 local_1c0;
  long lStack_1b8;
  long *local_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 uStack_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 local_148;
  undefined8 local_140;
  undefined4 *puStack_138;
  ulong local_130;
  ulong local_128;
  undefined8 local_120;
  ID *local_118 [2];
  undefined1 *local_108;
  undefined **local_100;
  undefined1 auStack_f8 [128];
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  puStack_138 = (undefined4 *)0x0;
  local_140 = 0;
  local_128 = 0;
  local_130 = 0;
  uStack_158 = 0;
  local_160 = 0;
  local_148 = 0;
  local_150 = 0;
  uStack_178 = 0;
  local_180 = 0;
  uStack_168 = 0;
  local_170 = 0;
  local_120 = 1;
  if (param_3 == 0) {
    local_1e0 = 0;
    uStack_1d8 = 0;
    local_1d0 = 0;
  }
  else {
    FUN_01b356b8(&local_1e0,param_3);
    param_2 = extraout_x1;
  }
  uStack_178 = uStack_1d8;
  local_180 = local_1e0;
  local_170 = local_1d0;
  if (param_4 != 0) {
    FUN_01b356b8(&local_1e0,param_4,param_2);
    local_160 = uStack_1d8;
    uStack_168 = local_1e0;
    uStack_158 = local_1d0;
    local_150 = CONCAT71(local_150._1_7_,1);
  }
  uVar14 = local_140;
  local_148 = CONCAT44((int)param_6,(int)param_5);
  local_140._6_2_ = SUB82(uVar14,6);
  local_140._0_6_ = CONCAT15((undefined1)param_9,CONCAT14((char)param_8,(int)param_7));
  IVar4 = _objc_msgSend(param_10,"count");
  if (IVar4 == 0) {
    puVar5 = (undefined4 *)0x0;
  }
  else {
    if (IVar4 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar5 = (undefined4 *)FUN_00008790(IVar4 << 5);
    local_130 = 0;
    puStack_138 = puVar5;
    local_128 = IVar4;
  }
  uStack_198 = 0;
  local_1a0 = 0;
  uStack_188 = 0;
  uStack_190 = 0;
  lStack_1b8 = 0;
  local_1c0 = 0;
  uStack_1a8 = 0;
  local_1b0 = (long *)0x0;
  IVar4 = _objc_msgSend(param_10,"countByEnumeratingWithState:objects:count:",&local_1c0,auStack_f8,
                        0x10);
  if (IVar4 == 0) {
LAB_043de348:
    local_120 = CONCAT71(local_120._1_7_,(undefined1)param_11);
    uVar14 = *(undefined8 *)(param_1 + 8);
    FUN_042a5b90(auStack_248,&local_180);
    local_108 = &LAB_043de6ec;
    local_100 = &PTR_FUN_0df6b528;
    local_118[0] = (ID *)FUN_00008790(8);
    *local_118[0] = param_12;
    FUN_042a75b4(uVar14,auStack_248,local_118);
    if ((code *)local_100[1] != (code *)0x0) {
      (*(code *)local_100[1])(local_118[0]);
    }
    FUN_042a5aac(auStack_248);
    FUN_042a5aac(&local_180);
    if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
      ___stack_chk_fail(0);
    }
    return;
  }
  uVar12 = 0;
  uVar16 = 0;
  lVar11 = *local_1b0;
  lVar7 = lVar11;
  do {
    if (lVar7 != lVar11) {
      _objc_enumerationMutation(param_10);
    }
    self = *(ID *)(lStack_1b8 + uVar16 * 8);
    IVar6 = _objc_msgSend(self,"kind");
    local_1e0 = CONCAT44(local_1e0._4_4_,(int)IVar6);
    IVar6 = _objc_msgSend(self,"value");
    if (IVar6 == 0) {
      uStack_1d8 = 0;
      local_1d0 = 0;
      local_1c8 = 0;
    }
    else {
      FUN_01b356b8(&uStack_1d8,IVar6);
    }
    if (uVar12 == local_128) {
      uVar1 = uVar12 + 1;
      if (uVar1 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380(0);
      }
      uVar10 = local_128 * 2;
      if (uVar10 < uVar1 || uVar10 - uVar1 == 0) {
        uVar10 = uVar1;
      }
      if (0x3fffffffffffffe < local_128) {
        uVar10 = 0x7ffffffffffffff;
      }
      if (uVar10 == 0) {
        lVar7 = 0;
      }
      else {
        if (uVar10 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
          FUN_04b087f0();
        }
        lVar7 = FUN_00008790(uVar10 << 5);
      }
      puVar13 = (undefined4 *)(lVar7 + uVar12 * 0x20);
      if (puVar13 == (undefined4 *)0x0) goto LAB_043de240;
      *puVar13 = (undefined4)local_1e0;
      *(ulong *)(puVar13 + 6) = local_1c8;
      *(undefined8 *)(puVar13 + 4) = local_1d0;
      *(undefined8 *)(puVar13 + 2) = uStack_1d8;
      local_1d0 = 0;
      local_1c8 = 0;
      uStack_1d8 = 0;
      puVar5 = puVar13 + local_130 * -8;
      if ((local_130 & 0x7ffffffffffffff) != 0) {
        puVar2 = puStack_138 + local_130 * 8;
        puVar8 = puStack_138;
        puVar9 = puVar5;
        do {
          if (puVar9 == (undefined4 *)0x0) goto LAB_043de240;
          *puVar9 = *puVar8;
          uVar17 = *(undefined8 *)(puVar8 + 4);
          uVar14 = *(undefined8 *)(puVar8 + 2);
          *(undefined8 *)(puVar9 + 6) = *(undefined8 *)(puVar8 + 6);
          *(undefined8 *)(puVar9 + 4) = uVar17;
          *(undefined8 *)(puVar9 + 2) = uVar14;
          *(undefined8 *)(puVar8 + 4) = 0;
          *(undefined8 *)(puVar8 + 6) = 0;
          *(undefined8 *)(puVar8 + 2) = 0;
          puVar8 = puVar8 + 8;
          puVar9 = puVar9 + 8;
          puVar15 = puStack_138;
        } while (puVar8 != puVar2);
        do {
          if (puVar15 == (undefined4 *)0x0) goto LAB_043de240;
          if (*(char *)((long)puVar15 + 0x1f) < '\0') {
            FUN_00005810(*(undefined8 *)(puVar15 + 2),*(ulong *)(puVar15 + 6) & 0x7fffffffffffffff);
          }
          puVar15 = puVar15 + 8;
        } while (puVar15 != puVar2);
      }
      puVar8 = puVar5;
      if (puStack_138 != (undefined4 *)0x0) {
        lVar7 = local_128 << 5;
        puVar8 = puStack_138;
        puStack_138 = puVar5;
        local_128 = uVar10;
        FUN_00005810(puVar8,lVar7);
        puVar8 = puStack_138;
        uVar10 = local_128;
      }
    }
    else {
      puVar13 = puVar5 + uVar12 * 8;
      if (puVar13 == (undefined4 *)0x0) goto LAB_043de240;
      *puVar13 = (undefined4)local_1e0;
      *(ulong *)(puVar13 + 6) = local_1c8;
      *(undefined8 *)(puVar13 + 4) = local_1d0;
      *(undefined8 *)(puVar13 + 2) = uStack_1d8;
      local_1d0 = 0;
      local_1c8 = 0;
      uStack_1d8 = 0;
      puVar5 = puStack_138;
      puVar8 = puStack_138;
      uVar10 = local_128;
    }
    local_128 = uVar10;
    puStack_138 = puVar8;
    lVar7 = (long)puVar13 + (0x20 - (long)puVar5);
    uVar12 = lVar7 >> 5;
    local_130 = uVar12;
    if (lVar7 == 0) {
LAB_043de240:
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x43de244);
      (*pcVar3)();
    }
    if ((long)local_1c8 < 0) {
      FUN_00005810(uStack_1d8,local_1c8 & 0x7fffffffffffffff);
    }
    uVar16 = uVar16 + 1;
    if (IVar4 <= uVar16) {
      IVar4 = _objc_msgSend(param_10,"countByEnumeratingWithState:objects:count:",&local_1c0,
                            auStack_f8,0x10);
      if (IVar4 == 0) goto LAB_043de348;
      uVar16 = 0;
    }
    lVar7 = *local_1b0;
  } while( true );
}


