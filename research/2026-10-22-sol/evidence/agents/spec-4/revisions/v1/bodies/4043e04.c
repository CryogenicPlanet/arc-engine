// ==== 0x4043e04  objc::Categories::ArcBrowserContextHistory_Extensions_::addVisits:completion:  (932 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::addVisits_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  ulong uVar1;
  ulong uVar2;
  code *pcVar3;
  ID IVar4;
  ID self;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long lVar11;
  ulong uVar12;
  long lVar13;
  long lVar14;
  undefined8 local_188 [2];
  byte local_178;
  undefined8 local_170;
  long lStack_168;
  long *local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  long local_128;
  ulong local_120;
  ulong local_118;
  ID *local_110 [2];
  undefined1 *local_100;
  undefined **local_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  local_128 = 0;
  local_120 = 0;
  local_118 = 0;
  IVar4 = _objc_msgSend(param_3,"count",param_3,param_4,(ulong)param_5);
  FUN_0431a574(&local_128,IVar4);
  uStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  uStack_140 = 0;
  lStack_168 = 0;
  local_170 = 0;
  uStack_158 = 0;
  local_160 = (long *)0x0;
  IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_170,auStack_f0,
                        0x10);
  if (IVar4 != 0) {
    uVar12 = 0;
    lVar13 = *local_160;
    lVar9 = lVar13;
    do {
      if (lVar9 != lVar13) {
        _objc_enumerationMutation(param_3);
      }
      self = *(ID *)(lStack_168 + uVar12 * 8);
      if (self == 0) {
        local_188[0] = 0;
      }
      else {
        _objc_msgSend(self,"toAnnotatedVisit",local_188);
      }
      uVar2 = local_120;
      if (local_120 == local_118) {
        uVar1 = local_120 + 1;
        if (uVar1 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
          FUN_040416c0();
        }
        uVar10 = local_118 * 2;
        if (uVar10 < uVar1 || uVar10 - uVar1 == 0) {
          uVar10 = uVar1;
        }
        if (0xffffffffffffffe < local_118) {
          uVar10 = 0x1fffffffffffffff;
        }
        if (uVar10 == 0) {
          lVar9 = 0;
        }
        else {
          if (uVar10 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          lVar9 = FUN_00008790(uVar10 << 3);
        }
        lVar9 = lVar9 + uVar2 * 8;
        if (lVar9 == 0) goto LAB_04043ec8;
        FUN_03738fe0(lVar9,local_188);
        uVar2 = local_120;
        lVar5 = local_128;
        lVar6 = lVar9 + local_120 * -8;
        lVar11 = local_120 << 3;
        lVar7 = lVar6;
        lVar8 = lVar11;
        lVar14 = local_128;
        if (lVar11 != 0) {
          do {
            if (lVar7 == 0) goto LAB_04043ec8;
            lVar7 = FUN_03738fe0(lVar7,lVar14);
            lVar8 = lVar8 + -8;
            lVar7 = lVar7 + 8;
            lVar14 = lVar14 + 8;
          } while (lVar8 != 0);
          do {
            if (lVar5 == 0) goto LAB_04043ec8;
            FUN_043364d8(lVar5);
            lVar5 = lVar5 + 8;
            lVar11 = lVar11 + -8;
          } while (lVar11 != 0);
        }
        local_120 = (long)(uVar2 * 8 + 8) >> 3;
        if (local_128 != 0) {
          lVar8 = local_118 << 3;
          lVar5 = local_128;
          local_128 = lVar6;
          local_118 = uVar10;
          FUN_00005810(lVar5,lVar8);
          lVar6 = local_128;
          uVar10 = local_118;
        }
      }
      else {
        lVar9 = local_128 + local_120 * 8;
        if (lVar9 == 0) goto LAB_04043ec8;
        FUN_03738fe0(lVar9,local_188);
        lVar6 = local_128;
        uVar10 = local_118;
      }
      local_118 = uVar10;
      local_128 = lVar6;
      lVar9 = (lVar9 + 8) - local_128;
      local_120 = lVar9 >> 3;
      if (lVar9 == 0) goto LAB_04043ec8;
      FUN_043364d8(local_188);
      uVar12 = uVar12 + 1;
      if (IVar4 <= uVar12) {
        IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_170,
                              auStack_f0,0x10);
        if (IVar4 == 0) break;
        uVar12 = 0;
      }
      lVar9 = *local_160;
    } while( true );
  }
  _objc_msgSend(param_1,"browser_context_history",local_188);
  if ((local_178 & 1) == 0) {
LAB_04043ec8:
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4043ecc);
    (*pcVar3)();
  }
  local_100 = &LAB_04045100;
  local_f8 = &PTR_FUN_0da87c28;
  local_110[0] = (ID *)FUN_00008790(8);
  *local_110[0] = param_4;
  FUN_04333058(local_188,&local_128,local_110);
  if ((code *)local_f8[1] != (code *)0x0) {
    (*(code *)local_f8[1])(local_110[0]);
  }
  if (local_178 != 0) {
    FUN_04332750(local_188);
  }
  lVar9 = local_128;
  if (local_128 != 0) {
    if (local_120 * 8 != 0) {
      lVar5 = local_120 * 8 + local_128;
      lVar13 = local_120 * -8;
      do {
        if (lVar5 == 8) goto LAB_04043ec8;
        lVar5 = FUN_043364d8();
        lVar13 = lVar13 + 8;
      } while (lVar13 != 0);
    }
    local_120 = lVar9 - local_128 >> 3;
    FUN_00005810(local_128,local_118 << 3);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


