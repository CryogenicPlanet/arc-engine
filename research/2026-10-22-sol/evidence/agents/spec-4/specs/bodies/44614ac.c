// ==== 0x44614ac  FUN_044614ac  (776 bytes)

void FUN_044614ac(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,ulong param_4,
                 undefined8 param_5,undefined8 param_6)

{
  undefined *puVar1;
  undefined1 auVar2 [16];
  undefined8 uVar3;
  long lVar4;
  byte bVar5;
  byte bVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  undefined8 uVar6;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  undefined8 uVar14;
  byte bVar20;
  byte bVar21;
  byte bVar22;
  byte bVar23;
  byte bVar24;
  byte bVar25;
  byte bVar26;
  byte bVar27;
  byte bVar28;
  byte bVar29;
  byte bVar30;
  byte bVar31;
  byte bVar32;
  byte bVar33;
  byte bVar34;
  byte bVar35;
  byte bVar36;
  undefined1 auVar37 [16];
  undefined8 local_400;
  undefined8 uStack_3f8;
  undefined8 local_3f0;
  undefined8 uStack_3e8;
  undefined8 uStack_3e0;
  undefined8 uStack_3d8;
  undefined8 local_3d0;
  undefined8 uStack_3c8;
  undefined8 uStack_3c0;
  undefined8 uStack_3b8;
  undefined8 local_3b0;
  undefined8 uStack_3a8;
  undefined8 uStack_3a0;
  undefined8 uStack_398;
  undefined8 local_390;
  undefined8 local_380;
  undefined8 uStack_378;
  undefined8 uStack_370;
  undefined8 uStack_368;
  undefined8 local_360;
  undefined8 uStack_358;
  undefined8 uStack_350;
  undefined8 uStack_348;
  undefined8 local_340;
  undefined8 uStack_338;
  undefined8 uStack_330;
  undefined8 uStack_328;
  undefined8 local_320;
  undefined8 uStack_318;
  undefined8 uStack_310;
  undefined8 uStack_308;
  undefined8 local_300;
  undefined8 uStack_2f8;
  undefined8 uStack_2f0;
  undefined8 uStack_2e8;
  ulong local_2e0;
  ulong uStack_2d8;
  undefined8 local_2d0;
  ulong uStack_2c8;
  undefined8 local_2c0;
  undefined8 uStack_2b8;
  undefined8 uStack_2b0;
  undefined8 uStack_2a8;
  undefined8 local_2a0;
  undefined8 uStack_298;
  undefined8 uStack_290;
  undefined8 uStack_288;
  undefined8 local_280;
  undefined8 uStack_278;
  undefined8 uStack_270;
  undefined8 uStack_268;
  undefined8 local_260;
  undefined8 uStack_258;
  undefined8 uStack_250;
  undefined8 uStack_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined8 uStack_230;
  undefined8 uStack_228;
  undefined8 local_220;
  undefined8 uStack_218;
  undefined8 uStack_210;
  undefined8 uStack_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined8 uStack_1f0;
  undefined8 uStack_1e8;
  undefined8 local_1e0;
  undefined8 uStack_1d8;
  undefined8 uStack_1d0;
  undefined8 uStack_1c8;
  undefined8 local_1c0;
  undefined8 uStack_1b8;
  undefined8 uStack_1b0;
  undefined8 uStack_1a8;
  ulong local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  ulong local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 local_a0 [9];
  long local_58;
  
  uVar3 = 0;
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((0x1f < param_4) && (*(int *)((long)param_1 + 0xc4) == 0)) {
    local_a0[5] = 0;
    local_a0[4] = 0;
    local_a0[7] = 0;
    local_a0[6] = 0;
    local_a0[1] = 0;
    local_a0[0] = 0;
    local_a0[3] = 0;
    local_a0[2] = 0;
    FUN_000e7a3c(local_a0,0x40);
    FUN_083fc464(local_a0);
    auVar37 = ZEXT816(0);
    lVar4 = 0;
    do {
      uVar14 = *(undefined8 *)((long)local_a0 + lVar4 + 8);
      uVar3 = *(undefined8 *)((long)local_a0 + lVar4);
      bVar21 = (byte)uVar3 >> 5;
      bVar5 = (byte)((ulong)uVar3 >> 8);
      bVar22 = bVar5 >> 5;
      bVar7 = (byte)((ulong)uVar3 >> 0x10);
      bVar23 = bVar7 >> 5;
      bVar8 = (byte)((ulong)uVar3 >> 0x18);
      bVar24 = bVar8 >> 5;
      bVar9 = (byte)((ulong)uVar3 >> 0x20);
      bVar25 = bVar9 >> 5;
      bVar10 = (byte)((ulong)uVar3 >> 0x28);
      bVar26 = bVar10 >> 5;
      bVar11 = (byte)((ulong)uVar3 >> 0x30);
      bVar27 = bVar11 >> 5;
      bVar12 = (byte)((ulong)uVar3 >> 0x38);
      bVar28 = bVar12 >> 5;
      bVar29 = (byte)uVar14 >> 5;
      bVar13 = (byte)((ulong)uVar14 >> 8);
      bVar30 = bVar13 >> 5;
      bVar15 = (byte)((ulong)uVar14 >> 0x10);
      bVar31 = bVar15 >> 5;
      bVar16 = (byte)((ulong)uVar14 >> 0x18);
      bVar32 = bVar16 >> 5;
      bVar17 = (byte)((ulong)uVar14 >> 0x20);
      bVar33 = bVar17 >> 5;
      bVar18 = (byte)((ulong)uVar14 >> 0x28);
      bVar34 = bVar18 >> 5;
      bVar19 = (byte)((ulong)uVar14 >> 0x30);
      bVar35 = bVar19 >> 5;
      bVar20 = (byte)((ulong)uVar14 >> 0x38);
      bVar36 = bVar20 >> 5;
      auVar2[1] = bVar22;
      auVar2[0] = bVar21;
      auVar2[2] = bVar23;
      auVar2[3] = bVar24;
      auVar2[4] = bVar25;
      auVar2[5] = bVar26;
      auVar2[6] = bVar27;
      auVar2[7] = bVar28;
      auVar2[8] = bVar29;
      auVar2[9] = bVar30;
      auVar2[10] = bVar31;
      auVar2[0xb] = bVar32;
      auVar2[0xc] = bVar33;
      auVar2[0xd] = bVar34;
      auVar2[0xe] = bVar35;
      auVar2[0xf] = bVar36;
      auVar37 = NEON_ext(auVar37,auVar2,0xf,1);
      *(ulong *)((long)local_a0 + lVar4 + 8) =
           CONCAT17(bVar20 << 3 | auVar37[0xf],
                    CONCAT16(bVar19 << 3 | auVar37[0xe],
                             CONCAT15(bVar18 << 3 | auVar37[0xd],
                                      CONCAT14(bVar17 << 3 | auVar37[0xc],
                                               CONCAT13(bVar16 << 3 | auVar37[0xb],
                                                        CONCAT12(bVar15 << 3 | auVar37[10],
                                                                 CONCAT11(bVar13 << 3 | auVar37[9],
                                                                          (byte)uVar14 << 3 |
                                                                          auVar37[8])))))));
      *(ulong *)((long)local_a0 + lVar4) =
           CONCAT17(bVar12 << 3 | auVar37[7],
                    CONCAT16(bVar11 << 3 | auVar37[6],
                             CONCAT15(bVar10 << 3 | auVar37[5],
                                      CONCAT14(bVar9 << 3 | auVar37[4],
                                               CONCAT13(bVar8 << 3 | auVar37[3],
                                                        CONCAT12(bVar7 << 3 | auVar37[2],
                                                                 CONCAT11(bVar5 << 3 | auVar37[1],
                                                                          (byte)uVar3 << 3 |
                                                                          auVar37[0])))))));
      lVar4 = lVar4 + 0x10;
      auVar37[9] = bVar30;
      auVar37[8] = bVar29;
      auVar37[10] = bVar31;
      auVar37[0xb] = bVar32;
      auVar37[0xc] = bVar33;
      auVar37[0xd] = bVar34;
      auVar37[0xe] = bVar35;
      auVar37[0xf] = bVar36;
      auVar37[1] = bVar22;
      auVar37[0] = bVar21;
      auVar37[2] = bVar23;
      auVar37[3] = bVar24;
      auVar37[4] = bVar25;
      auVar37[5] = bVar26;
      auVar37[6] = bVar27;
      auVar37[7] = bVar28;
    } while (lVar4 != 0x20);
    param_1[1] = local_a0[1];
    *param_1 = local_a0[0];
    param_1[3] = local_a0[3];
    param_1[2] = local_a0[2];
    uStack_f8 = 0;
    local_100 = 0;
    uStack_e8 = 0;
    uStack_f0 = 0;
    uStack_118 = 0;
    local_120 = 0;
    uStack_108 = 0;
    uStack_110 = 0;
    uStack_138 = 0;
    local_140 = 0;
    uStack_128 = 0;
    uStack_130 = 0;
    uStack_158 = 0;
    local_160 = 0;
    uStack_148 = 0;
    uStack_150 = 0;
    uStack_178 = 0;
    local_180 = 0;
    uStack_168 = 0;
    uStack_170 = 0;
    FUN_013a6780(&local_180,param_1,&DAT_0c9dc758);
    uStack_b8 = 0;
    local_c0 = 0;
    uStack_a8 = 0;
    uStack_b0 = 0;
    uStack_d8 = 0;
    local_e0 = 0;
    uStack_c8 = 0;
    uStack_d0 = 0;
    FUN_08415e8c(param_5,param_6,&local_e0);
    param_1[0xd] = uStack_d8;
    param_1[0xc] = local_e0;
    param_1[0xf] = uStack_c8;
    param_1[0xe] = uStack_d0;
    param_1[0x11] = uStack_b8;
    param_1[0x10] = local_c0;
    param_1[0x13] = uStack_a8;
    param_1[0x12] = uStack_b0;
    FUN_083fc464(&local_e0);
    uStack_198 = uStack_d8;
    local_1a0 = local_e0;
    uStack_188 = uStack_c8;
    uStack_190 = uStack_d0;
    if (*(char *)(param_1 + 0x19) == '\0') {
      uStack_238 = 0x14def9dea2f79cd6;
      local_240 = 0x5812631a5cf5d3ed;
      uStack_228 = 0x1000000000000000;
      uStack_230 = 0;
      uStack_2c8 = -(local_e0 & 1);
      local_2e0 = uStack_2c8 & 0x5812631a5cf5d3ed;
      uStack_2d8 = uStack_2c8 & 0x14def9dea2f79cd6;
      uStack_2c8 = uStack_2c8 & 0x1000000000000000;
      local_2d0 = 0;
      FUN_0193cff0(&local_1a0,&local_1a0,&local_2e0,4);
      FUN_0193cff0(&local_240,&local_240,&local_240,4);
      lVar4 = -(local_1a0 >> 1 & 1);
      bVar5 = (byte)lVar4;
      bVar7 = (byte)((ulong)lVar4 >> 8);
      bVar8 = (byte)((ulong)lVar4 >> 0x10);
      bVar9 = (byte)((ulong)lVar4 >> 0x18);
      bVar10 = (byte)((ulong)lVar4 >> 0x20);
      bVar11 = (byte)((ulong)lVar4 >> 0x28);
      bVar12 = (byte)((ulong)lVar4 >> 0x30);
      bVar13 = (byte)((ulong)lVar4 >> 0x38);
      local_2d0 = CONCAT17((byte)((ulong)uStack_230 >> 0x38) & bVar13,
                           CONCAT16((byte)((ulong)uStack_230 >> 0x30) & bVar12,
                                    CONCAT15((byte)((ulong)uStack_230 >> 0x28) & bVar11,
                                             CONCAT14((byte)((ulong)uStack_230 >> 0x20) & bVar10,
                                                      CONCAT13((byte)((ulong)uStack_230 >> 0x18) &
                                                               bVar9,CONCAT12((byte)((ulong)
                                                  uStack_230 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_230 >> 8) & bVar7,
                                                           (byte)uStack_230 & bVar5)))))));
      uStack_2c8 = CONCAT17((byte)((ulong)uStack_228 >> 0x38) & bVar13,
                            CONCAT16((byte)((ulong)uStack_228 >> 0x30) & bVar12,
                                     CONCAT15((byte)((ulong)uStack_228 >> 0x28) & bVar11,
                                              CONCAT14((byte)((ulong)uStack_228 >> 0x20) & bVar10,
                                                       CONCAT13((byte)((ulong)uStack_228 >> 0x18) &
                                                                bVar9,CONCAT12((byte)((ulong)
                                                  uStack_228 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_228 >> 8) & bVar7,
                                                           (byte)uStack_228 & bVar5)))))));
      uStack_2d8 = CONCAT17((byte)((ulong)uStack_238 >> 0x38) & bVar13,
                            CONCAT16((byte)((ulong)uStack_238 >> 0x30) & bVar12,
                                     CONCAT15((byte)((ulong)uStack_238 >> 0x28) & bVar11,
                                              CONCAT14((byte)((ulong)uStack_238 >> 0x20) & bVar10,
                                                       CONCAT13((byte)((ulong)uStack_238 >> 0x18) &
                                                                bVar9,CONCAT12((byte)((ulong)
                                                  uStack_238 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_238 >> 8) & bVar7,
                                                           (byte)uStack_238 & bVar5)))))));
      local_2e0 = CONCAT17((byte)((ulong)local_240 >> 0x38) & bVar13,
                           CONCAT16((byte)((ulong)local_240 >> 0x30) & bVar12,
                                    CONCAT15((byte)((ulong)local_240 >> 0x28) & bVar11,
                                             CONCAT14((byte)((ulong)local_240 >> 0x20) & bVar10,
                                                      CONCAT13((byte)((ulong)local_240 >> 0x18) &
                                                               bVar9,CONCAT12((byte)((ulong)
                                                  local_240 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)local_240 >> 8) & bVar7,
                                                           (byte)local_240 & bVar5)))))));
      FUN_0193cff0(&local_1a0,&local_1a0,&local_2e0,4);
      FUN_0193cff0(&local_240,&local_240,&local_240,4);
      lVar4 = -(local_1a0 >> 2 & 1);
      bVar5 = (byte)lVar4;
      bVar7 = (byte)((ulong)lVar4 >> 8);
      bVar8 = (byte)((ulong)lVar4 >> 0x10);
      bVar9 = (byte)((ulong)lVar4 >> 0x18);
      bVar10 = (byte)((ulong)lVar4 >> 0x20);
      bVar11 = (byte)((ulong)lVar4 >> 0x28);
      bVar12 = (byte)((ulong)lVar4 >> 0x30);
      bVar13 = (byte)((ulong)lVar4 >> 0x38);
      local_2d0 = CONCAT17((byte)((ulong)uStack_230 >> 0x38) & bVar13,
                           CONCAT16((byte)((ulong)uStack_230 >> 0x30) & bVar12,
                                    CONCAT15((byte)((ulong)uStack_230 >> 0x28) & bVar11,
                                             CONCAT14((byte)((ulong)uStack_230 >> 0x20) & bVar10,
                                                      CONCAT13((byte)((ulong)uStack_230 >> 0x18) &
                                                               bVar9,CONCAT12((byte)((ulong)
                                                  uStack_230 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_230 >> 8) & bVar7,
                                                           (byte)uStack_230 & bVar5)))))));
      uStack_2c8 = CONCAT17((byte)((ulong)uStack_228 >> 0x38) & bVar13,
                            CONCAT16((byte)((ulong)uStack_228 >> 0x30) & bVar12,
                                     CONCAT15((byte)((ulong)uStack_228 >> 0x28) & bVar11,
                                              CONCAT14((byte)((ulong)uStack_228 >> 0x20) & bVar10,
                                                       CONCAT13((byte)((ulong)uStack_228 >> 0x18) &
                                                                bVar9,CONCAT12((byte)((ulong)
                                                  uStack_228 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_228 >> 8) & bVar7,
                                                           (byte)uStack_228 & bVar5)))))));
      uStack_2d8 = CONCAT17((byte)((ulong)uStack_238 >> 0x38) & bVar13,
                            CONCAT16((byte)((ulong)uStack_238 >> 0x30) & bVar12,
                                     CONCAT15((byte)((ulong)uStack_238 >> 0x28) & bVar11,
                                              CONCAT14((byte)((ulong)uStack_238 >> 0x20) & bVar10,
                                                       CONCAT13((byte)((ulong)uStack_238 >> 0x18) &
                                                                bVar9,CONCAT12((byte)((ulong)
                                                  uStack_238 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)uStack_238 >> 8) & bVar7,
                                                           (byte)uStack_238 & bVar5)))))));
      local_2e0 = CONCAT17((byte)((ulong)local_240 >> 0x38) & bVar13,
                           CONCAT16((byte)((ulong)local_240 >> 0x30) & bVar12,
                                    CONCAT15((byte)((ulong)local_240 >> 0x28) & bVar11,
                                             CONCAT14((byte)((ulong)local_240 >> 0x20) & bVar10,
                                                      CONCAT13((byte)((ulong)local_240 >> 0x18) &
                                                               bVar9,CONCAT12((byte)((ulong)
                                                  local_240 >> 0x10) & bVar8,
                                                  CONCAT11((byte)((ulong)local_240 >> 8) & bVar7,
                                                           (byte)local_240 & bVar5)))))));
      FUN_0193cff0(&local_1a0,&local_1a0,&local_2e0,4);
    }
    param_1[9] = uStack_198;
    param_1[8] = local_1a0;
    param_1[0xb] = uStack_188;
    param_1[10] = uStack_190;
    uStack_1b8 = 0;
    local_1c0 = 0;
    uStack_1a8 = 0;
    uStack_1b0 = 0;
    uStack_1d8 = 0;
    local_1e0 = 0;
    uStack_1c8 = 0;
    uStack_1d0 = 0;
    uStack_1f8 = 0;
    local_200 = 0;
    uStack_1e8 = 0;
    uStack_1f0 = 0;
    uStack_218 = 0;
    local_220 = 0;
    uStack_208 = 0;
    uStack_210 = 0;
    puVar1 = &DAT_0c9dcef8;
    if (*(int *)(param_1 + 0x18) != 0) {
      puVar1 = &DAT_0c9dd2b8;
    }
    uStack_238 = 0;
    local_240 = 0;
    uStack_228 = 0;
    uStack_230 = 0;
    FUN_013a6780(&local_240,param_1 + 8,puVar1);
    uStack_258 = 0;
    local_260 = 0;
    uStack_248 = 0;
    uStack_250 = 0;
    uStack_278 = 0;
    local_280 = 0;
    uStack_268 = 0;
    uStack_270 = 0;
    uStack_298 = 0;
    local_2a0 = 0;
    uStack_288 = 0;
    uStack_290 = 0;
    uStack_2b8 = 0;
    local_2c0 = 0;
    uStack_2a8 = 0;
    uStack_2b0 = 0;
    uStack_2d8 = 0;
    local_2e0 = 0;
    uStack_2c8 = 0;
    local_2d0 = 0;
    FUN_0445d850(&local_2e0,&local_240);
    uStack_2f8 = 0;
    local_300 = 0;
    uStack_2e8 = 0;
    uStack_2f0 = 0;
    uStack_318 = 0;
    local_320 = 0;
    uStack_308 = 0;
    uStack_310 = 0;
    uStack_338 = 0;
    local_340 = 0;
    uStack_328 = 0;
    uStack_330 = 0;
    uStack_358 = 0;
    local_360 = 0;
    uStack_348 = 0;
    uStack_350 = 0;
    uStack_378 = 0;
    local_380 = 0;
    uStack_368 = 0;
    uStack_370 = 0;
    FUN_013a8570(&local_380,&local_180,&local_2e0);
    local_390 = 0;
    uStack_3a8 = 0;
    local_3b0 = 0;
    uStack_398 = 0;
    uStack_3a0 = 0;
    uStack_3c8 = 0;
    local_3d0 = 0;
    uStack_3b8 = 0;
    uStack_3c0 = 0;
    uStack_3e8 = 0;
    local_3f0 = 0;
    uStack_3d8 = 0;
    uStack_3e0 = 0;
    uStack_3f8 = 0;
    local_400 = 0;
    FUN_0445dc40(&local_400,&local_380);
    FUN_083fc080(param_1 + 4,&local_400);
    uVar6 = param_1[4];
    uVar14 = param_1[7];
    uVar3 = param_1[6];
    param_2[1] = param_1[5];
    *param_2 = uVar6;
    param_2[3] = uVar14;
    param_2[2] = uVar3;
    *param_3 = 0x20;
    uVar3 = 1;
    *(undefined4 *)((long)param_1 + 0xc4) = 1;
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(uVar3);
}


