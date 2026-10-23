// ==== 0x4433ba8  ArcCommandBarRankingEvaluation::init:numExamplesEvaluated:evaluationDurationMs:  (1052 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x20 bytes */

ID ArcCommandBarRankingEvaluation::init_numExamplesEvaluated_evaluationDurationMs_
             (ID param_1,SEL param_2,ID param_3,int param_4,int param_5)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  ID self;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined8 *puVar7;
  long lVar8;
  ulong uVar9;
  undefined8 extraout_d0;
  long local_190;
  long local_188;
  long local_180;
  ID local_178;
  class_t *pcStack_170;
  undefined8 local_168;
  undefined8 uStack_160;
  ulong local_158;
  undefined8 local_150;
  undefined8 uStack_148;
  ulong local_140;
  undefined8 local_138;
  undefined8 local_130;
  long lStack_128;
  long *local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_170 = &objc::class_t::ArcCommandBarRankingEvaluation;
  local_178 = param_1;
  IVar2 = NSObject::init((ID)&local_178,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (lVar6 = *(long *)(IVar2 + 8), lVar6 != 0)) {
      lVar5 = *(long *)(IVar2 + 0x10) * 0x20;
      lVar8 = lVar6;
      if (lVar5 != 0) {
        lVar8 = *(long *)(IVar2 + 0x10) * -0x20;
        puVar4 = (undefined8 *)(lVar5 + lVar6);
        do {
          puVar7 = puVar4 + -4;
          if (puVar7 == (undefined8 *)0x0) goto LAB_04433f14;
          if (*(char *)((long)puVar4 + -9) < '\0') {
            FUN_00005810(*puVar7,puVar4[-2] & 0x7fffffffffffffff);
          }
          lVar8 = lVar8 + 0x20;
          puVar4 = puVar7;
        } while (lVar8 != 0);
        lVar8 = *(long *)(IVar2 + 8);
      }
      *(long *)(IVar2 + 0x10) = lVar6 - lVar8 >> 5;
      FUN_00005810(lVar8,*(long *)(IVar2 + 0x18) << 5);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    local_190 = 0;
    local_188 = 0;
    local_180 = 0;
    IVar3 = _objc_msgSend(param_3,"count");
    FUN_042a5290(&local_190,IVar3);
    uStack_108 = 0;
    local_110 = 0;
    uStack_f8 = 0;
    uStack_100 = 0;
    lStack_128 = 0;
    local_130 = 0;
    uStack_118 = 0;
    local_120 = (long *)0x0;
    IVar3 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_130,auStack_f0
                          ,0x10);
    if (IVar3 != 0) {
      uVar9 = 0;
      lVar8 = *local_120;
      lVar6 = lVar8;
      do {
        if (lVar6 != lVar8) {
          _objc_enumerationMutation(param_3);
        }
        lVar6 = *(long *)(lStack_128 + uVar9 * 8);
        uStack_148 = 0;
        local_150 = 0;
        local_138 = 0;
        local_140 = 0;
        if (lVar6 == 0) {
          local_168 = 0;
          uStack_160 = 0;
          local_158 = 0;
        }
        else {
          FUN_01b356b8(&local_168,lVar6);
        }
        if ((long)local_140 < 0) {
          FUN_00005810(local_150,local_140 & 0x7fffffffffffffff);
        }
        uStack_148 = uStack_160;
        local_150 = local_168;
        local_140 = local_158;
        self = _objc_msgSend(param_3,"objectForKeyedSubscript:",lVar6);
        _objc_msgSend(self,"doubleValue");
        if (local_188 == local_180) {
          puVar4 = (undefined8 *)FUN_042a54c0(&local_190,&local_150);
        }
        else {
          puVar4 = (undefined8 *)(local_190 + local_188 * 0x20);
          if (puVar4 == (undefined8 *)0x0) goto LAB_04433f14;
          local_138 = extraout_d0;
          if ((long)local_140 < 0) {
            FUN_00060a60(puVar4,local_150,uStack_148);
          }
          else {
            puVar4[2] = local_140;
            puVar4[1] = uStack_148;
            *puVar4 = local_150;
          }
          puVar4[3] = local_138;
          puVar4 = puVar4 + 4;
        }
        local_188 = (long)puVar4 - local_190 >> 5;
        if ((long)puVar4 - local_190 == 0) goto LAB_04433f14;
        if ((long)local_140 < 0) {
          FUN_00005810(local_150,local_140 & 0x7fffffffffffffff);
        }
        uVar9 = uVar9 + 1;
        if (IVar3 <= uVar9) {
          IVar3 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_130,
                                auStack_f0,0x10);
          if (IVar3 == 0) break;
          uVar9 = 0;
        }
        lVar6 = *local_120;
      } while( true );
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
LAB_04433f14:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4433f18);
      (*pcVar1)();
    }
    lVar6 = *(long *)(IVar2 + 8);
    if (lVar6 == 0) {
      *(long *)(IVar2 + 8) = local_190;
      *(long *)(IVar2 + 0x10) = local_188;
      *(long *)(IVar2 + 0x18) = local_180;
    }
    else {
      lVar5 = *(long *)(IVar2 + 0x10) * 0x20;
      lVar8 = lVar6;
      if (lVar5 != 0) {
        lVar8 = *(long *)(IVar2 + 0x10) * -0x20;
        puVar4 = (undefined8 *)(lVar5 + lVar6);
        do {
          puVar7 = puVar4 + -4;
          if (puVar7 == (undefined8 *)0x0) goto LAB_04433f14;
          if (*(char *)((long)puVar4 + -9) < '\0') {
            FUN_00005810(*puVar7,puVar4[-2] & 0x7fffffffffffffff);
          }
          lVar8 = lVar8 + 0x20;
          puVar4 = puVar7;
        } while (lVar8 != 0);
        lVar8 = *(long *)(IVar2 + 8);
      }
      *(long *)(IVar2 + 0x10) = lVar6 - lVar8 >> 5;
      FUN_00005810(lVar8,*(long *)(IVar2 + 0x18) << 5);
      *(long *)(IVar2 + 8) = local_190;
      *(long *)(IVar2 + 0x10) = local_188;
      *(long *)(IVar2 + 0x18) = local_180;
      if ((*(byte *)(IVar2 + 0x28) & 1) == 0) goto LAB_04433f14;
    }
    *(int *)(IVar2 + 0x20) = param_4;
    *(int *)(IVar2 + 0x24) = param_5;
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return IVar2;
}


