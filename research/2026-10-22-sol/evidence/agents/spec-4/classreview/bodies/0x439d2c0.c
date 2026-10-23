// ==== 0x439d2c0  FUN_0439d2c0  (1828 bytes)

/* WARNING: Type propagation algorithm not settling */

void FUN_0439d2c0(long *param_1,long param_2,undefined8 *param_3)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined4 *puVar4;
  code *pcVar5;
  bool bVar6;
  long *plVar7;
  long lVar8;
  undefined4 *puVar9;
  char *******pppppppcVar10;
  char *******pppppppcVar11;
  size_t sVar12;
  undefined8 *puVar13;
  undefined8 uVar14;
  ulong uVar15;
  long lVar16;
  long *plVar17;
  ulong uVar18;
  undefined4 *puVar19;
  ulong uVar20;
  undefined8 *puVar21;
  long lVar22;
  ulong *puVar23;
  ulong uVar24;
  undefined4 *local_1b8;
  ulong local_1b0;
  ulong local_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  ulong local_180;
  undefined8 local_170;
  undefined8 uStack_168;
  ulong local_160;
  undefined1 local_153;
  byte local_152;
  undefined1 local_151;
  undefined8 local_150;
  undefined8 uStack_148;
  ulong local_140;
  undefined8 local_138;
  undefined8 uStack_130;
  ulong local_128;
  undefined8 local_120;
  undefined8 uStack_118;
  ulong local_110;
  char *******local_100;
  size_t sStack_f8;
  undefined8 local_f0;
  undefined4 local_e4;
  long local_e0;
  undefined8 *puStack_d8;
  undefined8 *local_d0;
  long lStack_c8;
  ulong local_c0;
  ulong local_b8;
  long local_a8;
  long local_a0;
  long local_98;
  undefined8 local_90;
  undefined1 uStack_88;
  undefined7 uStack_87;
  undefined1 uStack_80;
  undefined8 uStack_7f;
  
  if (*(long *)(param_2 + 0x130) == 0) {
LAB_0439d318:
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    return;
  }
  local_90 = *param_3;
  uStack_88 = (undefined1)param_3[1];
  uStack_7f = *(undefined8 *)((long)param_3 + 0x11);
  uStack_87 = (undefined7)*(undefined8 *)((long)param_3 + 9);
  uStack_80 = (undefined1)((ulong)*(undefined8 *)((long)param_3 + 9) >> 0x38);
  plVar7 = (long *)FUN_09564ac8(&local_90);
  if ((plVar7 == (long *)0x0) || ((*(byte *)(plVar7 + 3) & 1) == 0)) goto LAB_0439d318;
  local_a8 = 0;
  local_a0 = 0;
  local_98 = 0;
  lStack_c8 = 0;
  local_d0 = (undefined8 *)0x0;
  local_b8 = 0;
  local_c0 = 0;
  puStack_d8 = (undefined8 *)0x0;
  local_e0 = 0;
  lVar8 = (**(code **)(*plVar7 + 0x118))(plVar7);
  if ((lVar8 == 0) || (lVar8 = FUN_095a0160(plVar7,*(undefined4 *)(lVar8 + 0x40)), lVar8 == 0)) {
    puVar21 = (undefined8 *)0x0;
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    puVar13 = (undefined8 *)0x0;
  }
  else {
    FUN_043a3670(&local_e0);
    if ((puStack_d8 == local_d0) ||
       (plVar17 = (long *)(puStack_d8[local_c0 + local_b8 >> 6] + (local_c0 + local_b8 & 0x3f) * 8),
       plVar17 == (long *)0x0)) goto LAB_0439d420;
    *plVar17 = lVar8;
    bVar6 = local_b8 < 0xffffffffffffffff;
    local_b8 = local_b8 + 1;
    puVar21 = puStack_d8;
    if (bVar6) {
      lVar22 = 0;
      lVar8 = 0;
      uVar24 = local_c0;
      do {
        puVar13 = (undefined8 *)(puVar21[uVar24 >> 6] + (uVar24 & 0x3f) * 8);
        if (puVar13 == (undefined8 *)0x0) goto LAB_0439d420;
        plVar17 = (long *)*puVar13;
        local_b8 = local_b8 - 1;
        local_c0 = uVar24 + 1;
        if (0x7f < local_c0) {
          puVar13 = puVar21 + 1;
          FUN_00005810(*puVar21,0x200);
          puVar21 = puVar13;
          local_c0 = uVar24 - 0x3f;
          puStack_d8 = puVar13;
        }
        uVar24 = local_c0;
        lVar16 = plVar17[1];
        local_c0 = uVar24;
        if (lVar16 == 0) {
          local_100 = (char *******)0x0;
          sStack_f8 = 0;
          local_f0 = 0;
          lVar16 = (**(code **)(*plVar17 + 0x10))(plVar17);
          local_e4 = 0;
          puVar9 = (undefined4 *)(lVar16 + 4);
        }
        else {
          local_e4 = *(undefined4 *)(lVar16 + 0x40);
          local_100 = (char *******)0x0;
          sStack_f8 = 0;
          puVar9 = (undefined4 *)(lVar16 + 0x44);
          local_f0 = 0;
        }
        pppppppcVar11 = (char *******)FUN_0926bf94(*puVar9);
        if (pppppppcVar11 == (char *******)0x0) goto LAB_0439d420;
        sVar12 = _strlen((char *)pppppppcVar11);
        if (0x7fffffffffffffee < sVar12) {
                    /* WARNING: Subroutine does not return */
          FUN_04b089c0();
        }
        if (sVar12 < 0x17) {
          local_f0 = CONCAT17((char)sVar12,(undefined7)local_f0);
          pppppppcVar10 = (char *******)&local_100;
        }
        else {
          pppppppcVar10 = (char *******)FUN_00008790((sVar12 & 0x7ffffffffffffff0) + 0x10);
          local_f0 = (sVar12 & 0x7ffffffffffffff0) + 0x8000000000000010;
          local_100 = pppppppcVar10;
          sStack_f8 = sVar12;
        }
        if (((char *******)((long)pppppppcVar10 + sVar12) < pppppppcVar10) ||
           (pppppppcVar10 <= pppppppcVar11 &&
            pppppppcVar11 < (char *******)((long)pppppppcVar10 + sVar12))) goto LAB_0439d420;
        _memmove(pppppppcVar10,pppppppcVar11,sVar12);
        *(undefined1 *)((long)pppppppcVar10 + sVar12) = 0;
        local_120 = 0;
        uStack_118 = 0;
        local_110 = 0;
        puVar13 = (undefined8 *)FUN_02ce44cc(plVar17);
        if (*(char *)((long)puVar13 + 0x17) < '\0') {
          FUN_00060a60(&local_120,*puVar13,puVar13[1]);
        }
        else {
          uStack_118 = puVar13[1];
          local_120 = *puVar13;
          local_110 = puVar13[2];
        }
        local_138 = 0;
        uStack_130 = 0;
        local_128 = 0;
        FUN_0955703c(&local_138,plVar17[1]);
        local_150 = 0;
        uStack_148 = 0;
        local_140 = 0;
        puVar13 = (undefined8 *)FUN_04558a88(plVar17);
        if (*(char *)((long)puVar13 + 0x17) < '\0') {
          FUN_00060a60(&local_150,*puVar13,puVar13[1]);
        }
        else {
          uStack_148 = puVar13[1];
          local_150 = *puVar13;
          local_140 = puVar13[2];
        }
        (**(code **)(*plVar17 + 0x10))(plVar17);
        local_151 = FUN_0926e054();
        local_152 = (byte)(*(uint *)(plVar17[1] + 0x48) >> 4) & 1;
        if ((*(uint *)(plVar17[1] + 0x48) >> 6 & 1) == 0) {
          local_153 = FUN_09558d6c();
        }
        else {
          local_153 = 1;
        }
        local_170 = 0;
        uStack_168 = 0;
        local_160 = 0;
        local_180 = 0;
        uStack_198 = 0;
        local_1a0 = 0;
        uStack_188 = 0;
        uStack_190 = 0;
        FUN_01c4be90(&local_1a0,plVar17[1] + 0x40);
        if ((local_180 & 1) != 0) {
          FUN_0266f700(&local_170,&local_1a0);
        }
        local_1b8 = (undefined4 *)0x0;
        local_1b0 = 0;
        local_1a8 = 0;
        lVar16 = *(long *)(plVar17[1] + 0x30) << 3;
        if (lVar16 != 0) {
          uVar20 = 0;
          puVar23 = *(ulong **)(plVar17[1] + 0x28);
          puVar9 = (undefined4 *)0x0;
          do {
            uVar18 = *puVar23;
            if ((uVar18 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(uVar18);
            }
            uVar1 = *(undefined4 *)(uVar18 + 0x40);
            if (uVar20 == local_1a8) {
              uVar24 = uVar20 + 1;
              if (uVar24 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
                FUN_04047380();
              }
              uVar15 = uVar20 * 2;
              if (uVar15 < uVar24 || uVar15 - uVar24 == 0) {
                uVar15 = uVar24;
              }
              if (0x1ffffffffffffffe < uVar20) {
                uVar15 = 0x3fffffffffffffff;
              }
              if (uVar15 == 0) {
                puVar9 = (undefined4 *)0x0;
              }
              else {
                if (uVar15 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
                  FUN_04b087f0();
                }
                puVar9 = (undefined4 *)FUN_00008790(uVar15 << 2);
              }
              puVar19 = puVar9 + uVar20;
              if (puVar19 == (undefined4 *)0x0) goto LAB_0439d420;
              *puVar19 = uVar1;
              puVar19 = puVar19 + -local_1b0;
              _memcpy(puVar19,local_1b8,local_1b0 << 2);
              puVar4 = local_1b8;
              bVar6 = local_1b8 != (undefined4 *)0x0;
              local_1b8 = puVar19;
              if (bVar6) {
                lVar8 = local_1a8 << 2;
                local_1a8 = uVar15;
                FUN_00005810(puVar4,lVar8);
                uVar15 = local_1a8;
              }
            }
            else {
              if (puVar9 + uVar20 == (undefined4 *)0x0) goto LAB_0439d420;
              puVar9[uVar20] = uVar1;
              uVar15 = local_1a8;
            }
            local_1a8 = uVar15;
            puVar19 = local_1b8;
            lVar8 = (long)puVar9 + ((uVar20 * 4 + 4) - (long)local_1b8);
            uVar20 = lVar8 >> 2;
            local_1b0 = uVar20;
            if (lVar8 == 0) goto LAB_0439d420;
            uVar14 = FUN_095a0160(plVar7,*(undefined4 *)(uVar18 + 0x40));
            lVar8 = 0;
            if (local_d0 != puStack_d8) {
              lVar8 = ((long)local_d0 - (long)puStack_d8) * 8 + -1;
            }
            if (lVar8 == local_b8 + local_c0) {
              FUN_043a3670(&local_e0);
            }
            uVar24 = local_c0;
            puVar21 = puStack_d8;
            if ((puStack_d8 == local_d0) ||
               (puVar13 = (undefined8 *)
                          (puStack_d8[local_c0 + local_b8 >> 6] + (local_c0 + local_b8 & 0x3f) * 8),
               puVar13 == (undefined8 *)0x0)) goto LAB_0439d420;
            *puVar13 = uVar14;
            local_b8 = local_b8 + 1;
            if ((uVar18 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_000270f0(uVar18);
            }
            puVar23 = puVar23 + 1;
            lVar16 = lVar16 + -8;
            puVar9 = puVar19;
            lVar8 = local_a8;
            lVar22 = local_a0;
          } while (lVar16 != 0);
        }
        if (lVar22 == local_98) {
          lVar16 = FUN_043a0340(&local_a8,&local_e4,&local_100,&local_120,&local_138,&local_150,
                                &local_151,&local_152,&local_153,&local_170,&local_1b8);
        }
        else {
          lVar16 = FUN_043a00f0(lVar8 + lVar22 * 0xa0,&local_e4,&local_100,&local_120,&local_138,
                                &local_150,&local_151,&local_152,&local_153,&local_170,&local_1b8);
          lVar16 = lVar16 + 0xa0;
        }
        lVar8 = local_a8;
        lVar22 = (lVar16 - local_a8 >> 5) * -0x3333333333333333;
        local_a0 = lVar22;
        if (lVar16 - local_a8 == 0) goto LAB_0439d420;
        if (local_1b8 != (undefined4 *)0x0) {
          if ((local_1b0 & 0x3fffffffffffffff) != 0) {
            lVar16 = local_1b0 * -4;
            puVar9 = local_1b8 + local_1b0;
            do {
              puVar9 = puVar9 + -1;
              if (puVar9 == (undefined4 *)0x0) goto LAB_0439d420;
              lVar16 = lVar16 + 4;
            } while (lVar16 != 0);
          }
          local_1b0 = 0;
          FUN_00005810(local_1b8,local_1a8 << 2);
        }
        if ((long)local_160 < 0) {
          FUN_00005810(local_170,local_160 & 0x7fffffffffffffff);
        }
        if ((long)local_140 < 0) {
          FUN_00005810(local_150,local_140 & 0x7fffffffffffffff);
        }
        if ((long)local_128 < 0) {
          FUN_00005810(local_138,local_128 & 0x7fffffffffffffff);
        }
        if ((long)local_110 < 0) {
          FUN_00005810(local_120,local_110 & 0x7fffffffffffffff);
        }
        if ((long)local_f0 < 0) {
          FUN_00005810(local_100,local_f0 & 0x7fffffffffffffff);
        }
        lVar16 = local_98;
      } while (local_b8 != 0);
    }
    else {
      lVar22 = 0;
      lVar8 = 0;
      lVar16 = 0;
    }
    *param_1 = lVar8;
    param_1[1] = lVar22;
    local_a0 = 0;
    local_98 = 0;
    local_a8 = 0;
    param_1[2] = lVar16;
    puVar13 = local_d0;
  }
  local_b8 = 0;
  puVar2 = puStack_d8;
  while (uVar24 = (long)puVar13 - (long)puVar21 >> 3, 2 < uVar24) {
    FUN_00005810(*puVar21,0x200);
    puVar21 = puVar21 + 1;
    puVar2 = puVar21;
  }
  puStack_d8 = puVar2;
  puVar2 = puVar13;
  puVar3 = puVar21;
  if (uVar24 == 1) {
    local_c0 = 0x20;
  }
  else if (uVar24 == 2) {
    local_c0 = 0x40;
  }
  for (; puVar3 != puVar13; puVar3 = puVar3 + 1) {
    FUN_00005810(*puVar3,0x200);
    puVar2 = puVar21;
  }
  do {
    if (puVar13 == puVar2) {
      if (local_e0 == 0) {
        return;
      }
      FUN_00005810(local_e0,lStack_c8 - local_e0);
      return;
    }
    puVar13 = puVar13 + -1;
  } while (puVar13 != (undefined8 *)0x0);
LAB_0439d420:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x439d424);
  (*pcVar5)();
}


