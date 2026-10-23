// ==== 0x44424b8  ArcWebContents::findTextWithDirectives:completion:  (1040 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcWebContents::findTextWithDirectives_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  ulong uVar1;
  bool bVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  ID IVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  undefined8 *local_160;
  ulong local_158;
  ulong local_150;
  undefined8 local_148;
  undefined8 uStack_140;
  ulong local_138;
  undefined8 local_130;
  long lStack_128;
  long *local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  ID *local_f0 [2];
  undefined1 *local_e0;
  undefined **local_d8;
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
LAB_044428a4:
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44428a8);
    (*pcVar3)();
  }
  local_160 = (undefined8 *)0x0;
  local_158 = 0;
  local_150 = 0;
  IVar4 = _objc_msgSend(param_3,"count",param_3,param_4,(ulong)param_5);
  puVar7 = (undefined8 *)0x0;
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar7 = (undefined8 *)FUN_00008790(IVar4 * 0x18);
    local_158 = 0;
    local_160 = puVar7;
    local_150 = IVar4;
  }
  uStack_108 = 0;
  local_110 = 0;
  uStack_f8 = 0;
  uStack_100 = 0;
  lStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  local_120 = (long *)0x0;
  IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_130,local_f0,
                        0x10);
  if (IVar4 != 0) {
    uVar11 = 0;
    lVar12 = *local_120;
    do {
      IVar9 = 0;
      puVar5 = puVar7;
      do {
        if (*local_120 != lVar12) {
          _objc_enumerationMutation(param_3);
        }
        lVar10 = *(long *)(lStack_128 + IVar9 * 8);
        if (lVar10 == 0) {
          local_148 = 0;
          uStack_140 = 0;
          local_138 = 0;
        }
        else {
          FUN_01b356b8(&local_148,lVar10);
        }
        if (uVar11 == local_150) {
          uVar1 = uVar11 + 1;
          if (0xaaaaaaaaaaaaaaa < uVar1) {
                    /* WARNING: Subroutine does not return */
            FUN_04047380(0);
          }
          uVar6 = uVar11 * 2;
          if (uVar6 < uVar1 || uVar6 - uVar1 == 0) {
            uVar6 = uVar1;
          }
          if (0x555555555555554 < uVar11) {
            uVar6 = 0xaaaaaaaaaaaaaaa;
          }
          if (uVar6 == 0) {
            puVar5 = (undefined8 *)0x0;
          }
          else {
            if (0xaaaaaaaaaaaaaaa < uVar6) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            puVar5 = (undefined8 *)FUN_00008790(uVar6 * 0x18);
          }
          puVar7 = puVar5 + uVar11 * 3;
          if (puVar7 == (undefined8 *)0x0) goto LAB_044428a4;
          puVar7[2] = local_138;
          puVar7[1] = uStack_140;
          *puVar7 = local_148;
          uStack_140 = 0;
          local_138 = 0;
          local_148 = 0;
          puVar7 = puVar7 + local_158 * -3;
          if (local_158 * 0x18 != 0) {
            _memcpy(puVar7,local_160,local_158 * 0x18);
          }
          puVar8 = local_160;
          bVar2 = local_160 != (undefined8 *)0x0;
          local_160 = puVar7;
          if (bVar2) {
            lVar10 = local_150 * 0x18;
            local_150 = uVar6;
            FUN_00005810(puVar8,lVar10);
            uVar6 = local_150;
          }
        }
        else {
          puVar7 = puVar5 + uVar11 * 3;
          if (puVar7 == (undefined8 *)0x0) goto LAB_044428a4;
          puVar7[2] = local_138;
          puVar7[1] = uStack_140;
          *puVar7 = local_148;
          uStack_140 = 0;
          local_138 = 0;
          local_148 = 0;
          uVar6 = local_150;
        }
        local_150 = uVar6;
        puVar7 = local_160;
        lVar10 = (long)puVar5 + ((uVar11 * 0x18 + 0x18) - (long)local_160);
        uVar11 = (lVar10 >> 3) * -0x5555555555555555;
        local_158 = uVar11;
        if (lVar10 == 0) goto LAB_044428a4;
        if ((long)local_138 < 0) {
          FUN_00005810(local_148,local_138 & 0x7fffffffffffffff);
        }
        IVar9 = IVar9 + 1;
        puVar5 = puVar7;
      } while (IVar4 != IVar9);
      IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_130,local_f0
                            ,0x10);
    } while (IVar4 != 0);
  }
  local_f0[0] = (ID *)FUN_00008790(8);
  *local_f0[0] = param_4;
  local_e0 = &LAB_04445164;
  local_d8 = &PTR_FUN_0df6c3f8;
  FUN_04377260(*(undefined8 *)(param_1 + 0x118),&local_160,local_f0);
  if ((code *)local_d8[1] != (code *)0x0) {
    (*(code *)local_d8[1])(local_f0[0]);
  }
  puVar7 = local_160;
  if (local_160 != (undefined8 *)0x0) {
    if (local_158 * 0x18 != 0) {
      lVar12 = local_158 * -0x18;
      puVar5 = local_160 + local_158 * 3;
      do {
        puVar8 = puVar5 + -3;
        if (puVar8 == (undefined8 *)0x0) goto LAB_044428a4;
        if (*(char *)((long)puVar5 + -1) < '\0') {
          FUN_00005810(*puVar8,puVar5[-1] & 0x7fffffffffffffff);
        }
        lVar12 = lVar12 + 0x18;
        puVar5 = puVar8;
      } while (lVar12 != 0);
    }
    local_158 = ((long)puVar7 - (long)local_160 >> 3) * -0x5555555555555555;
    FUN_00005810(local_160,local_150 * 0x18);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


