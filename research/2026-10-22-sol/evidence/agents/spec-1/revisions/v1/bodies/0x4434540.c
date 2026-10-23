// ==== 0x4434540  ArcCommandBarRankingCandidate::init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding:  (960 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x5c bytes */

ID ArcCommandBarRankingCandidate::
   init_archetype_destination_title_url_lastNavSeconds_freqEver_typedCount_siteEngagement_isExactURLMatch_isBaseDomainMatch_isTitlePrefix_titleEmbedding_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
             int param_8,int param_9,int param_10,float param_11,bool param_12,bool param_13,
             bool param_14,ID param_15)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined4 in_stack_00000004;
  undefined4 in_stack_00000014;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcCommandBarRankingCandidate;
  local_70 = param_1;
  IVar2 = NSObject::init((ID)&local_70,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0xb0) != '\0') {
    FUN_042a3ff4(IVar2 + 8);
  }
  *(undefined8 *)(IVar2 + 0xa8) = 0;
  *(undefined8 *)(IVar2 + 0xa0) = 0;
  *(undefined8 *)(IVar2 + 0x98) = 0;
  *(undefined8 *)(IVar2 + 0x90) = 0;
  *(undefined8 *)(IVar2 + 0x88) = 0;
  *(undefined8 *)(IVar2 + 0x80) = 0;
  *(undefined8 *)(IVar2 + 0x78) = 0;
  *(undefined8 *)(IVar2 + 0x70) = 0;
  *(undefined8 *)(IVar2 + 0x68) = 0;
  *(undefined8 *)(IVar2 + 0x60) = 0;
  *(undefined8 *)(IVar2 + 0x58) = 0;
  *(undefined8 *)(IVar2 + 0x50) = 0;
  *(undefined8 *)(IVar2 + 0x48) = 0;
  *(undefined8 *)(IVar2 + 0x40) = 0;
  *(undefined8 *)(IVar2 + 0x38) = 0;
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 8) = 0;
  *(undefined1 *)(IVar2 + 0xb0) = 1;
  if (param_3 == 0) {
    local_88 = 0;
    uStack_80 = 0;
    local_78 = 0;
  }
  else {
    FUN_01b356b8(&local_88,param_3);
  }
  if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x10) = uStack_80;
    *(undefined8 *)(IVar2 + 8) = local_88;
    *(undefined8 *)(IVar2 + 0x18) = local_78;
    if (param_4 == 0) {
      local_88 = 0;
      uStack_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_4);
    }
    if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
      if (*(char *)(IVar2 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
      }
      *(undefined8 *)(IVar2 + 0x28) = uStack_80;
      *(undefined8 *)(IVar2 + 0x20) = local_88;
      *(undefined8 *)(IVar2 + 0x30) = local_78;
      if (param_5 == 0) {
        local_88 = 0;
        uStack_80 = 0;
        local_78 = 0;
      }
      else {
        FUN_01b356b8(&local_88,param_5);
      }
      if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
        if (*(char *)(IVar2 + 0x4f) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x38),*(ulong *)(IVar2 + 0x48) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x40) = uStack_80;
        *(undefined8 *)(IVar2 + 0x38) = local_88;
        *(undefined8 *)(IVar2 + 0x48) = local_78;
        if (param_6 == 0) {
          local_88 = 0;
          uStack_80 = 0;
          local_78 = 0;
        }
        else {
          FUN_01b356b8(&local_88,param_6);
        }
        if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
          if (*(char *)(IVar2 + 0x67) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x50),*(ulong *)(IVar2 + 0x60) & 0x7fffffffffffffff
                        );
          }
          *(undefined8 *)(IVar2 + 0x58) = uStack_80;
          *(undefined8 *)(IVar2 + 0x50) = local_88;
          *(undefined8 *)(IVar2 + 0x60) = local_78;
          if (param_7 == 0) {
            local_88 = 0;
            uStack_80 = 0;
            local_78 = 0;
          }
          else {
            FUN_01b356b8(&local_88,param_7);
          }
          if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
            if (*(char *)(IVar2 + 0x7f) < '\0') {
              FUN_00005810(*(undefined8 *)(IVar2 + 0x68),
                           *(ulong *)(IVar2 + 0x78) & 0x7fffffffffffffff);
              *(undefined8 *)(IVar2 + 0x70) = uStack_80;
              *(undefined8 *)(IVar2 + 0x68) = local_88;
              *(undefined8 *)(IVar2 + 0x78) = local_78;
              if ((*(byte *)(IVar2 + 0xb0) & 1) == 0) goto LAB_044348dc;
            }
            else {
              *(undefined8 *)(IVar2 + 0x70) = uStack_80;
              *(undefined8 *)(IVar2 + 0x68) = local_88;
              *(undefined8 *)(IVar2 + 0x78) = local_78;
            }
            *(int *)(IVar2 + 0x80) = param_8;
            *(int *)(IVar2 + 0x84) = param_9;
            *(undefined4 *)(IVar2 + 0x88) = in_stack_00000004;
            *(float *)(IVar2 + 0x8c) = param_11;
            *(undefined1 *)(IVar2 + 0x90) = (undefined1)param_10;
            *(undefined1 *)(IVar2 + 0x91) = param_10._1_1_;
            *(undefined1 *)(IVar2 + 0x92) = param_10._2_1_;
            FUN_0443181c(&local_88,CONCAT44(in_stack_00000014,param_12));
            if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
              lVar3 = *(long *)(IVar2 + 0x98);
              if (lVar3 != 0) {
                lVar5 = *(long *)(IVar2 + 0xa0) * 4;
                if (lVar5 != 0) {
                  lVar4 = *(long *)(IVar2 + 0xa0) * -4;
                  lVar5 = lVar5 + lVar3;
                  do {
                    lVar5 = lVar5 + -4;
                    if (lVar5 == 0) goto LAB_044348dc;
                    lVar4 = lVar4 + 4;
                  } while (lVar4 != 0);
                }
                *(undefined8 *)(IVar2 + 0xa0) = 0;
                FUN_00005810(lVar3,*(long *)(IVar2 + 0xa8) << 2);
              }
              *(undefined8 *)(IVar2 + 0x98) = local_88;
              *(undefined8 *)(IVar2 + 0xa0) = uStack_80;
              *(undefined8 *)(IVar2 + 0xa8) = local_78;
              return IVar2;
            }
          }
        }
      }
    }
  }
LAB_044348dc:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44348e0);
  (*pcVar1)();
}


