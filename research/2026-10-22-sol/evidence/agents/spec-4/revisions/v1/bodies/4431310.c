// ==== 0x4431310  ArcCommandBarRankingTrainingResult::init:failureReason:delta:modelName:modelVersion:numSamples:lossesPerEpoch:gradientNormMean:gradientNormMax:deltaNorm:deltaToWeightRatio:trainingDurationMs:  (1100 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x54 bytes */

ID ArcCommandBarRankingTrainingResult::
   init_failureReason_delta_modelName_modelVersion_numSamples_lossesPerEpoch_gradientNormMean_gradientNormMax_deltaNorm_deltaToWeightRatio_trainingDurationMs_
             (ID param_1,SEL param_2,long_long param_3,long_long param_4,ID param_5,ID param_6,
             int param_7,int param_8,ID param_9,float param_10,float param_11,float param_12,
             float param_13,int param_14)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  long *plVar8;
  long lVar9;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  ulong local_198;
  long local_190;
  long local_188;
  long local_180;
  long local_178;
  long local_170;
  long local_168;
  undefined8 local_160;
  long lStack_158;
  long *local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  ID local_120;
  class_t *pcStack_118;
  undefined1 auStack_110 [128];
  long local_90;
  
  local_90 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_118 = &objc::class_t::ArcCommandBarRankingTrainingResult;
  local_120 = param_1;
  IVar2 = NSObject::init((ID)&local_120,"init");
  if (IVar2 == 0) {
LAB_04431514:
    if (*(long *)PTR____stack_chk_guard_0d824020 != local_90) {
                    /* WARNING: Subroutine does not return */
      ___stack_chk_fail(0);
    }
    return IVar2;
  }
  if (*(char *)(IVar2 + 0x78) != '\0') {
    FUN_042a43ac(IVar2 + 8);
    *(undefined1 *)(IVar2 + 0x78) = 0;
  }
  plVar8 = (long *)(IVar2 + 0x10);
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *plVar8 = 0;
  *(undefined8 *)(IVar2 + 0x70) = 0;
  *(undefined8 *)(IVar2 + 0x58) = 0;
  *(undefined8 *)(IVar2 + 0x50) = 0;
  *(undefined8 *)(IVar2 + 0x68) = 0;
  *(undefined8 *)(IVar2 + 0x60) = 0;
  *(undefined8 *)(IVar2 + 0x38) = 0;
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x48) = 0;
  *(undefined8 *)(IVar2 + 0x40) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined1 *)(IVar2 + 0x78) = 1;
  *(int *)(IVar2 + 8) = (int)param_3;
  *(int *)(IVar2 + 0xc) = (int)param_4;
  IVar3 = _objc_msgSend(param_5,"count");
  FUN_042a5644(plVar8,IVar3);
  uStack_138 = 0;
  local_140 = 0;
  uStack_128 = 0;
  uStack_130 = 0;
  lStack_158 = 0;
  local_160 = 0;
  uStack_148 = 0;
  local_150 = (long *)0x0;
  IVar3 = _objc_msgSend(param_5,"countByEnumeratingWithState:objects:count:",&local_160,auStack_110,
                        0x10);
  if (IVar3 != 0) {
    uVar6 = 0;
    lVar7 = *local_150;
    lVar9 = lVar7;
    do {
      if (lVar9 != lVar7) {
        _objc_enumerationMutation(param_5);
      }
      if ((*(byte *)(IVar2 + 0x78) & 1) == 0) goto LAB_04431598;
      FUN_044323f8(&local_1a8,*(undefined8 *)(lStack_158 + uVar6 * 8));
      if (*(long *)(IVar2 + 0x18) == *(long *)(IVar2 + 0x20)) {
        lVar9 = FUN_042a5854(plVar8,&local_1a8);
        lVar4 = *plVar8;
        lVar5 = (lVar9 - lVar4 >> 3) * -0x71c71c71c71c71c7;
      }
      else {
        lVar9 = *(long *)(IVar2 + 0x10) + *(long *)(IVar2 + 0x18) * 0x48;
        FUN_042a5760(lVar9,&local_1a8);
        lVar9 = lVar9 + 0x48;
        lVar4 = *(long *)(IVar2 + 0x10);
        lVar5 = (lVar9 - lVar4 >> 3) * -0x71c71c71c71c71c7;
        *(long *)(IVar2 + 0x18) = lVar5;
      }
      *(long *)(IVar2 + 0x18) = lVar5;
      if (lVar9 == lVar4) goto LAB_04431598;
      if (local_178 != 0) {
        if (local_170 * 4 != 0) {
          lVar9 = local_170 * -4;
          lVar4 = local_178 + local_170 * 4;
          do {
            lVar4 = lVar4 + -4;
            if (lVar4 == 0) goto LAB_04431598;
            lVar9 = lVar9 + 4;
          } while (lVar9 != 0);
        }
        local_170 = 0;
        FUN_00005810(local_178,local_168 << 2);
      }
      if (local_190 != 0) {
        if (local_188 * 4 != 0) {
          lVar9 = local_188 * -4;
          lVar4 = local_190 + local_188 * 4;
          do {
            lVar4 = lVar4 + -4;
            if (lVar4 == 0) goto LAB_04431598;
            lVar9 = lVar9 + 4;
          } while (lVar9 != 0);
        }
        local_188 = 0;
        FUN_00005810(local_190,local_180 << 2);
      }
      if ((long)local_198 < 0) {
        FUN_00005810(local_1a8,local_198 & 0x7fffffffffffffff);
      }
      uVar6 = uVar6 + 1;
      if (IVar3 <= uVar6) {
        IVar3 = _objc_msgSend(param_5,"countByEnumeratingWithState:objects:count:",&local_160,
                              auStack_110,0x10);
        if (IVar3 == 0) break;
        uVar6 = 0;
      }
      lVar9 = *local_150;
    } while( true );
  }
  if (param_6 == 0) {
    local_1a8 = 0;
    uStack_1a0 = 0;
    local_198 = 0;
  }
  else {
    FUN_01b356b8(&local_1a8,param_6);
  }
  if ((*(byte *)(IVar2 + 0x78) & 1) != 0) {
    if (*(char *)(IVar2 + 0x3f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x28),*(ulong *)(IVar2 + 0x38) & 0x7fffffffffffffff);
      *(undefined8 *)(IVar2 + 0x30) = uStack_1a0;
      *(undefined8 *)(IVar2 + 0x28) = local_1a8;
      *(ulong *)(IVar2 + 0x38) = local_198;
      if ((*(byte *)(IVar2 + 0x78) & 1) == 0) goto LAB_04431598;
    }
    else {
      *(undefined8 *)(IVar2 + 0x30) = uStack_1a0;
      *(undefined8 *)(IVar2 + 0x28) = local_1a8;
      *(ulong *)(IVar2 + 0x38) = local_198;
    }
    *(int *)(IVar2 + 0x40) = param_7;
    *(int *)(IVar2 + 0x44) = param_8;
    FUN_0443181c(&local_1a8,param_9);
    if ((*(byte *)(IVar2 + 0x78) & 1) != 0) {
      lVar9 = *(long *)(IVar2 + 0x48);
      if (lVar9 == 0) {
        *(undefined8 *)(IVar2 + 0x48) = local_1a8;
        *(undefined8 *)(IVar2 + 0x50) = uStack_1a0;
        *(ulong *)(IVar2 + 0x58) = local_198;
      }
      else {
        lVar7 = *(long *)(IVar2 + 0x50) * 4;
        if (lVar7 != 0) {
          lVar4 = *(long *)(IVar2 + 0x50) * -4;
          lVar7 = lVar7 + lVar9;
          do {
            lVar7 = lVar7 + -4;
            if (lVar7 == 0) goto LAB_04431598;
            lVar4 = lVar4 + 4;
          } while (lVar4 != 0);
        }
        *(undefined8 *)(IVar2 + 0x50) = 0;
        FUN_00005810(lVar9,*(long *)(IVar2 + 0x58) << 2);
        *(undefined8 *)(IVar2 + 0x48) = local_1a8;
        *(undefined8 *)(IVar2 + 0x50) = uStack_1a0;
        *(ulong *)(IVar2 + 0x58) = local_198;
        if ((*(byte *)(IVar2 + 0x78) & 1) == 0) goto LAB_04431598;
      }
      *(float *)(IVar2 + 0x60) = param_10;
      *(float *)(IVar2 + 100) = param_11;
      *(float *)(IVar2 + 0x68) = param_12;
      *(float *)(IVar2 + 0x6c) = param_13;
      *(int *)(IVar2 + 0x70) = param_14;
      goto LAB_04431514;
    }
  }
LAB_04431598:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443159c);
  (*pcVar1)();
}


