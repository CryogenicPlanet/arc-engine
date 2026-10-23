// ==== 0x42b4cc4  FUN_042b4cc4  (3924 bytes)

undefined1  [16] FUN_042b4cc4(long param_1,long *param_2)

{
  ulong *puVar1;
  long lVar2;
  undefined8 ****ppppuVar3;
  ulong uVar4;
  undefined7 uVar5;
  undefined1 uVar6;
  uint uVar7;
  long lVar8;
  long *plVar9;
  undefined8 *puVar11;
  undefined8 uVar12;
  byte bVar13;
  code *pcVar14;
  long lVar15;
  ulong uVar16;
  ulong uVar17;
  undefined8 *****pppppuVar18;
  long *plVar19;
  long *plVar20;
  long *plVar21;
  long lVar22;
  long lVar23;
  ulong uVar24;
  ulong uVar25;
  bool bVar26;
  ulong uVar27;
  float fVar28;
  byte bVar29;
  byte bVar30;
  byte bVar31;
  byte bVar32;
  byte bVar33;
  byte bVar34;
  byte bVar35;
  undefined1 auVar36 [16];
  undefined8 local_2d0;
  undefined8 uStack_2c8;
  undefined8 uStack_2c0;
  undefined8 uStack_2b8;
  undefined4 local_2b0;
  char *local_2a0;
  undefined8 local_298;
  undefined1 *puStack_290;
  undefined7 local_288;
  undefined1 uStack_281;
  undefined7 uStack_280;
  char local_279;
  undefined7 local_278;
  char cStack_271;
  undefined1 auStack_270 [128];
  undefined8 ****local_1f0;
  undefined8 ***pppuStack_1e8;
  undefined8 local_1e0;
  undefined4 local_1d8;
  undefined8 ****local_1c8;
  undefined8 ***pppuStack_1c0;
  undefined8 local_1b8;
  undefined1 *local_1b0;
  undefined7 local_1a8;
  undefined4 uStack_1a1;
  char local_19d;
  undefined4 uStack_19c;
  char local_198;
  undefined6 uStack_197;
  char cStack_191;
  undefined8 ****local_190;
  undefined8 ***pppuStack_188;
  undefined8 ***local_180;
  long local_78;
  undefined8 *****pppppuVar10;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  local_1c8 = (undefined8 *****)0x0;
  pppuStack_1c0 = (undefined8 ****)0x0;
  local_1b8 = (undefined8 ****)0x0;
  (**(code **)(*param_2 + 0x30))(&local_1a8,param_2);
  FUN_00df9708(&local_1c8,&local_1a8);
  if (cStack_191 < '\0') {
    FUN_00005810(CONCAT17((undefined1)uStack_1a1,local_1a8),
                 CONCAT17(cStack_191,CONCAT61(uStack_197,local_198)) & 0x7fffffffffffffff);
  }
  lVar8 = FUN_042c1ad4(param_1 + 0xd8,&local_1c8);
  if (lVar8 != 0) {
    lVar22 = *(long *)(lVar8 + 0x30);
    if (lVar22 != 0) {
      if (*(long *)(lVar22 + 8) != -1) {
        lVar15 = *(long *)(lVar22 + 8);
        do {
          if (lVar15 == -1) {
            auVar36 = ZEXT816(0);
            goto LAB_042b4d84;
          }
          lVar23 = lVar15 + 1;
          lVar2 = *(long *)(lVar22 + 8);
          bVar26 = lVar2 != lVar15;
          lVar15 = lVar2;
        } while (bVar26);
        *(long *)(lVar22 + 8) = lVar23;
        auVar36._8_8_ = lVar22;
        auVar36._0_8_ = *(undefined8 *)(lVar8 + 0x28);
        goto LAB_042b4d84;
      }
      if (*(long *)(lVar22 + 8) != -1) goto LAB_042b4f88;
    }
    FUN_04080740(&local_1a8,param_1 + 0xd8);
    lVar8 = CONCAT17((undefined1)uStack_1a1,local_1a8);
    if (lVar8 != 0) {
      if (local_198 != '\0') {
        if (lVar8 == -0x10) goto LAB_042b5738;
        plVar9 = *(long **)(lVar8 + 0x30);
        if (plVar9 != (long *)0x0) {
          if (plVar9[2] != 0) {
            LOAcquire();
            lVar22 = plVar9[2];
            plVar9[2] = lVar22 + -1;
            LORelease();
            if (lVar22 != 0) goto LAB_042b4f00;
          }
          pcVar14 = *(code **)(*plVar9 + 0x20);
          if (((pcVar14 == FUN_00f01bc0) || (pcVar14 == FUN_00f01bc0)) || (pcVar14 == FUN_013efb40))
          {
            FUN_00005810(plVar9,0x20);
          }
          else {
            (*pcVar14)();
          }
        }
LAB_042b4f00:
        if (*(char *)(lVar8 + 0x27) < '\0') {
          FUN_00005810(*(undefined8 *)(lVar8 + 0x10),*(ulong *)(lVar8 + 0x20) & 0x7fffffffffffffff);
        }
      }
      FUN_00005810(lVar8,0x38);
    }
    lVar8 = FUN_042c1ad4(param_1 + 0x100,&local_1c8);
    if (lVar8 != 0) {
      FUN_04080740(&local_1a8,param_1 + 0x100,lVar8);
      lVar8 = CONCAT17((undefined1)uStack_1a1,local_1a8);
      if (lVar8 != 0) {
        if (local_198 != '\0') {
          if (lVar8 == -0x10) goto LAB_042b5738;
          plVar9 = *(long **)(lVar8 + 0x30);
          if (plVar9 != (long *)0x0) {
            LOAcquire();
            lVar22 = plVar9[1];
            plVar9[1] = lVar22 + -1;
            LORelease();
            if (lVar22 == 0) {
              (**(code **)(*plVar9 + 0x10))(plVar9);
              if (plVar9[2] != 0) {
                LOAcquire();
                lVar22 = plVar9[2];
                plVar9[2] = lVar22 + -1;
                LORelease();
                if (lVar22 != 0) goto LAB_042b4f74;
              }
              pcVar14 = *(code **)(*plVar9 + 0x20);
              if (((pcVar14 == FUN_00f01bc0) || (pcVar14 == FUN_00f01bc0)) ||
                 (pcVar14 == FUN_013efb40)) {
                FUN_00005810(plVar9,0x20);
              }
              else {
                (*pcVar14)(plVar9);
              }
            }
          }
LAB_042b4f74:
          if (*(char *)(lVar8 + 0x27) < '\0') {
            FUN_00005810(*(undefined8 *)(lVar8 + 0x10),*(ulong *)(lVar8 + 0x20) & 0x7fffffffffffffff
                        );
          }
        }
        FUN_00005810(lVar8,0x38);
      }
    }
  }
LAB_042b4f88:
  auVar36 = FUN_042c5400(param_2,0);
  lVar8 = auVar36._8_8_;
  pppppuVar10 = auVar36._0_8_;
  puVar1 = (ulong *)(param_1 + 0x3f8);
  uVar17 = *(ulong *)(param_1 + 0x3f8);
  local_1f0 = pppppuVar10;
  if ((uVar17 & 0x3e) == 0) {
    puVar11 = (undefined8 *)(param_1 + 0x400);
    if (uVar17 < 0x800000) {
      *puVar1 = uVar17 | 0x800040;
    }
    else {
      if ((undefined8 *****)*puVar11 == pppppuVar10) {
LAB_042b5bfc:
                    /* WARNING: Does not return */
        pcVar14 = (code *)SoftwareBreakpoint(0,0x42b5c00);
        (*pcVar14)();
      }
      local_1a8 = SUB87(puVar1,0);
      uStack_1a1._0_1_ = (undefined1)((ulong)puVar1 >> 0x38);
      uStack_1a1._1_3_ = SUB83(&local_1f0,0);
      local_19d = (char)((ulong)&local_1f0 >> 0x18);
      uStack_19c = (undefined4)((ulong)&local_1f0 >> 0x20);
      puVar11 = (undefined8 *)FUN_000f79a0(param_1 + 0x3f8,&DAT_0df6a6d8,&local_1a8,FUN_03a728b4,0);
    }
  }
  else {
    lVar23 = *(long *)(param_1 + 0x400);
    uVar24 = -1L << (uVar17 & 0x3f);
    lVar22 = (lVar23 - uVar24) + 7;
    Hint_Prefetch(lVar23,0,2,0);
    uVar25 = uVar17 >> 6 & 0x1f;
    local_1a8 = auVar36._0_7_;
    uVar5 = local_1a8;
    uStack_1a1._0_1_ = auVar36[7];
    uVar6 = (undefined1)uStack_1a1;
    uVar27 = FUN_00018f3c(&local_1a8,8);
    lVar15 = 0;
    uVar17 = uVar27 ^ uVar25;
    bVar13 = (byte)(uVar27 >> 0x39);
    while( true ) {
      uVar17 = uVar17 & (uVar24 ^ 0xffffffffffffffff);
      Hint_Prefetch(lVar22 + uVar17 * 8,0,0,0);
      uVar12 = *(undefined8 *)(lVar23 + uVar17);
      bVar29 = (byte)((ulong)uVar12 >> 8);
      bVar30 = (byte)((ulong)uVar12 >> 0x10);
      bVar31 = (byte)((ulong)uVar12 >> 0x18);
      bVar32 = (byte)((ulong)uVar12 >> 0x20);
      bVar33 = (byte)((ulong)uVar12 >> 0x28);
      bVar34 = (byte)((ulong)uVar12 >> 0x30);
      bVar35 = (byte)((ulong)uVar12 >> 0x38);
      for (uVar27 = CONCAT17(-(bVar35 == bVar13),
                             CONCAT16(-(bVar34 == bVar13),
                                      CONCAT15(-(bVar33 == bVar13),
                                               CONCAT14(-(bVar32 == bVar13),
                                                        CONCAT13(-(bVar31 == bVar13),
                                                                 CONCAT12(-(bVar30 == bVar13),
                                                                          CONCAT11(-(bVar29 ==
                                                                                    bVar13),-((byte)
                                                  uVar12 == bVar13)))))))); uVar27 != 0;
          uVar27 = (uVar27 & 0x8080808080808080) - 0x80 & uVar27 & 0x8080808080808080) {
        uVar16 = (uVar27 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar27 & 0x5555555555555555) << 1;
        uVar16 = (uVar16 & 0xcccccccccccccccc) >> 2 | (uVar16 & 0x3333333333333333) << 2;
        uVar16 = (uVar16 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar16 & 0xf0f0f0f0f0f0f0f) << 4;
        uVar16 = (uVar16 & 0xff00ff00ff00ff00) >> 8 | (uVar16 & 0xff00ff00ff00ff) << 8;
        uVar16 = (uVar16 & 0xffff0000ffff0000) >> 0x10 | (uVar16 & 0xffff0000ffff) << 0x10;
        if (*(undefined8 ******)
             (lVar22 + (uVar17 + ((ulong)LZCOUNT(uVar16 >> 0x20 | uVar16 << 0x20) >> 3) &
                       (uVar24 ^ 0xffffffffffffffff)) * 8) == pppppuVar10) goto LAB_042b5bfc;
      }
      if (CONCAT17(-(bVar35 == 0x80),
                   CONCAT16(-(bVar34 == 0x80),
                            CONCAT15(-(bVar33 == 0x80),
                                     CONCAT14(-(bVar32 == 0x80),
                                              CONCAT13(-(bVar31 == 0x80),
                                                       CONCAT12(-(bVar30 == 0x80),
                                                                CONCAT11(-(bVar29 == 0x80),
                                                                         -((byte)uVar12 == 0x80)))))
                                    ))) != 0) break;
      lVar15 = lVar15 + 8;
      uVar17 = lVar15 + uVar17;
    }
    Hint_Prefetch(lVar23,0,2,0);
    uStack_1a1 = CONCAT31(uStack_1a1._1_3_,uVar6);
    local_1a8 = uVar5;
    uVar17 = FUN_00018f3c(&local_1a8,8);
    lVar15 = 0;
    uVar25 = uVar17 ^ uVar25;
    bVar13 = (byte)(uVar17 >> 0x39);
    while( true ) {
      uVar25 = uVar25 & (uVar24 ^ 0xffffffffffffffff);
      Hint_Prefetch(lVar22 + uVar25 * 8,0,0,0);
      uVar12 = *(undefined8 *)(lVar23 + uVar25);
      bVar29 = (byte)((ulong)uVar12 >> 8);
      bVar30 = (byte)((ulong)uVar12 >> 0x10);
      bVar31 = (byte)((ulong)uVar12 >> 0x18);
      bVar32 = (byte)((ulong)uVar12 >> 0x20);
      bVar33 = (byte)((ulong)uVar12 >> 0x28);
      bVar34 = (byte)((ulong)uVar12 >> 0x30);
      bVar35 = (byte)((ulong)uVar12 >> 0x38);
      for (uVar17 = CONCAT17(-(bVar35 == bVar13),
                             CONCAT16(-(bVar34 == bVar13),
                                      CONCAT15(-(bVar33 == bVar13),
                                               CONCAT14(-(bVar32 == bVar13),
                                                        CONCAT13(-(bVar31 == bVar13),
                                                                 CONCAT12(-(bVar30 == bVar13),
                                                                          CONCAT11(-(bVar29 ==
                                                                                    bVar13),-((byte)
                                                  uVar12 == bVar13)))))))); uVar17 != 0;
          uVar17 = (uVar17 & 0x8080808080808080) - 0x80 & uVar17 & 0x8080808080808080) {
        uVar27 = (uVar17 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar17 & 0x5555555555555555) << 1;
        uVar27 = (uVar27 & 0xcccccccccccccccc) >> 2 | (uVar27 & 0x3333333333333333) << 2;
        uVar27 = (uVar27 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar27 & 0xf0f0f0f0f0f0f0f) << 4;
        uVar27 = (uVar27 & 0xff00ff00ff00ff00) >> 8 | (uVar27 & 0xff00ff00ff00ff) << 8;
        uVar27 = (uVar27 & 0xffff0000ffff0000) >> 0x10 | (uVar27 & 0xffff0000ffff) << 0x10;
        pppppuVar18 = pppppuVar10;
        if (*(undefined8 ******)
             (lVar22 + (uVar25 + ((ulong)LZCOUNT(uVar27 >> 0x20 | uVar27 << 0x20) >> 3) &
                       (uVar24 ^ 0xffffffffffffffff)) * 8) == pppppuVar10) goto LAB_042b5148;
      }
      if (CONCAT17(-(bVar35 == 0x80),
                   CONCAT16(-(bVar34 == 0x80),
                            CONCAT15(-(bVar33 == 0x80),
                                     CONCAT14(-(bVar32 == 0x80),
                                              CONCAT13(-(bVar31 == 0x80),
                                                       CONCAT12(-(bVar30 == 0x80),
                                                                CONCAT11(-(bVar29 == 0x80),
                                                                         -((byte)uVar12 == 0x80)))))
                                    ))) != 0) break;
      lVar15 = lVar15 + 8;
      uVar25 = lVar15 + uVar25;
    }
    puVar11 = (undefined8 *)FUN_000f6c60(param_1 + 0x3f8,&DAT_0df6a6d8);
  }
  ppppuVar3 = local_1f0;
  if (puVar11 == (undefined8 *)0x0) goto LAB_042b5738;
  if (((ulong)local_1f0 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(local_1f0);
  }
  *puVar11 = ppppuVar3;
  pppppuVar18 = (undefined8 *****)local_1f0;
LAB_042b5148:
  FUN_042c8790(pppppuVar18 + 0x5b,*(undefined8 *)(param_1 + 0x3f0));
  if (DAT_0e4a3090 == 0) {
    uVar7 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
  }
  else {
    uVar7 = FUN_083a6f28(DAT_0e4a3090,"../../arc/browser/browser_application_impl.cc",0x2d);
  }
  if (2 < (int)uVar7) {
    FUN_00f195d8(&local_1a8,"../../arc/browser/browser_application_impl.cc",0x5d9,0xfffffffd);
    uVar12 = FUN_00040060(&local_198,"Start tracking browser context: ",0x20);
    if (*(char *)((long)pppppuVar10 + 0xa7) < '\0') {
      FUN_00060a60(&local_1f0,pppppuVar10[0x12],pppppuVar10[0x13]);
    }
    else {
      pppuStack_1e8 = pppppuVar10[0x13];
      local_1f0 = pppppuVar10[0x12];
      local_1e0 = pppppuVar10[0x14];
    }
    local_1d8 = *(undefined4 *)(pppppuVar10 + 0x15);
    pppppuVar10 = (undefined8 *****)local_1f0;
    if (-1 < (long)local_1e0._7_1_) {
      pppppuVar10 = &local_1f0;
    }
    ppppuVar3 = (undefined8 ****)pppuStack_1e8;
    if (-1 < (long)local_1e0) {
      ppppuVar3 = (undefined8 ****)(long)local_1e0._7_1_;
    }
    FUN_00040060(uVar12,pppppuVar10,ppppuVar3);
    if ((long)local_1e0 < 0) {
      FUN_00005810(local_1f0,(ulong)local_1e0 & 0x7fffffffffffffff);
    }
    FUN_00f1950c(&local_1a8);
  }
  uVar12 = FUN_02bed438();
  cStack_271 = '\x0f';
  if ("browser_context" < &local_279 && &local_288 < "rowser_context") goto LAB_042b5738;
  local_288 = 0x726573776f7262;
  uStack_281 = 0x5f;
  uStack_280 = 0x747865746e6f63;
  local_279 = '\0';
  local_2a0 = (char *)FUN_00008790(0x20);
  puStack_290 = &DAT_8000000000000020;
  local_298 = 0x1f;
  if ((local_2a0 + 0x1f < local_2a0) ||
     ("Browser context wrapper created" < local_2a0 + 0x1f &&
      local_2a0 < "rowser context wrapper created")) goto LAB_042b5738;
  builtin_strncpy(local_2a0,"Browser context wrapper created",0x20);
  if ((long)local_1b8 < 0) {
    FUN_00060a60(&local_1f0,local_1c8,pppuStack_1c0);
  }
  else {
    pppuStack_1e8 = pppuStack_1c0;
    local_1f0 = local_1c8;
    local_1e0 = local_1b8;
  }
  cStack_191 = '\v';
  if ("profile_dir" < &local_19d && &local_1a8 < "rofile_dir") goto LAB_042b5738;
  local_19d = '\0';
  pppuStack_188 = pppuStack_1e8;
  local_190 = local_1f0;
  local_180 = local_1e0;
  local_1f0 = (undefined8 *****)0x0;
  pppuStack_1e8 = (undefined8 ****)0x0;
  local_1e0 = (undefined8 ****)0x0;
  uStack_2c8 = 0;
  local_2d0 = 0;
  uStack_2b8 = 0;
  uStack_2c0 = 0;
  local_2b0 = 0x3f800000;
  local_1a8 = 0x656c69666f7270;
  uStack_1a1 = 0x7269645f;
  local_1b0 = (undefined1 *)&local_2d0;
  FUN_00ba2240(&local_1b0,&local_1a8,&local_1a8);
  FUN_02bed328(auStack_270,&local_288,&local_2a0,&local_2d0);
  FUN_02bed570(uVar12,auStack_270);
  FUN_08b90450(auStack_270);
  FUN_04b8e340(&local_2d0);
  if ((long)local_180 < 0) {
    FUN_00005810(local_190,(ulong)local_180 & 0x7fffffffffffffff);
  }
  if (cStack_191 < '\0') {
    FUN_00005810(CONCAT17((undefined1)uStack_1a1,local_1a8),
                 CONCAT17(cStack_191,CONCAT61(uStack_197,local_198)) & 0x7fffffffffffffff);
  }
  if ((long)local_1e0 < 0) {
    FUN_00005810(local_1f0,(ulong)local_1e0 & 0x7fffffffffffffff);
  }
  if ((long)puStack_290 < 0) {
    FUN_00005810(local_2a0,(ulong)puStack_290 & 0x7fffffffffffffff);
  }
  if (cStack_271 < '\0') {
    FUN_00005810(CONCAT17(uStack_281,local_288),CONCAT17(cStack_271,local_278) & 0x7fffffffffffffff)
    ;
  }
  pppppuVar10 = (undefined8 *****)local_1c8;
  if (-1 < (long)local_1b8._7_1_) {
    pppppuVar10 = &local_1c8;
  }
  ppppuVar3 = (undefined8 ****)pppuStack_1c0;
  if (-1 < (long)local_1b8) {
    ppppuVar3 = (undefined8 ****)(long)local_1b8._7_1_;
  }
  uVar17 = FUN_00018f3c(pppppuVar10,ppppuVar3);
  uVar25 = *(ulong *)(param_1 + 0xe0);
  if (uVar25 == 0) {
    uVar27 = 0;
  }
  else {
    uVar24 = uVar25 - 1;
    if ((uVar25 & uVar24) == 0) {
      uVar27 = uVar24 & uVar17;
    }
    else {
      uVar27 = uVar17;
      if (uVar25 <= uVar17) {
        uVar27 = 0;
        if (uVar25 != 0) {
          uVar27 = uVar17 / uVar25;
        }
        uVar27 = uVar17 - uVar27 * uVar25;
      }
    }
    plVar9 = *(long **)(*(long *)(param_1 + 0xd8) + uVar27 * 8);
    if (plVar9 != (long *)0x0) {
      for (plVar9 = (long *)*plVar9; plVar9 != (long *)0x0; plVar9 = (long *)*plVar9) {
        uVar16 = plVar9[1];
        if (uVar16 == uVar17) {
          uVar16 = FUN_00df8f58(plVar9 + 2,&local_1c8);
          if ((uVar16 & 1) != 0) goto LAB_042b4e28;
        }
        else {
          if ((uVar25 & uVar24) == 0) {
            uVar16 = uVar16 & uVar24;
          }
          else if (uVar25 <= uVar16) {
            uVar4 = 0;
            if (uVar25 != 0) {
              uVar4 = uVar16 / uVar25;
            }
            uVar16 = uVar16 - uVar4 * uVar25;
          }
          if (uVar16 != uVar27) break;
        }
      }
    }
  }
  plVar9 = (long *)FUN_00008790(0x38);
  if (plVar9 == (long *)0x0) goto LAB_042b5738;
  *plVar9 = 0;
  plVar9[1] = uVar17;
  plVar19 = plVar9 + 2;
  if (plVar19 == (long *)0x0) goto LAB_042b5738;
  if ((long)local_1b8 < 0) {
    FUN_00060a60(plVar19,local_1c8,pppuStack_1c0);
  }
  else {
    plVar9[3] = (long)pppuStack_1c0;
    *plVar19 = (long)local_1c8;
    plVar9[4] = (long)local_1b8;
  }
  *(undefined1 (*) [16])(plVar9 + 5) = auVar36;
  if (lVar8 != 0) {
    *(long *)(lVar8 + 0x10) = *(long *)(lVar8 + 0x10) + 1;
  }
  fVar28 = (float)(*(long *)(param_1 + 0xf0) + 1);
  if (*(float *)(param_1 + 0xf8) * (float)uVar25 < fVar28) {
    uVar27 = (ulong)((uVar25 & uVar25 - 1) != 0);
    if (uVar25 < 3) {
      uVar27 = 1;
    }
    uVar27 = uVar27 | uVar25 << 1;
    uVar25 = (ulong)(fVar28 / *(float *)(param_1 + 0xf8));
    if (uVar27 <= uVar25) {
      uVar27 = uVar25;
    }
    if (uVar27 - 1 == 0) {
      uVar27 = 2;
    }
    else if ((uVar27 & uVar27 - 1) != 0) {
      uVar27 = FUN_00f76630(uVar27);
    }
    uVar25 = *(ulong *)(param_1 + 0xe0);
    if (uVar25 < uVar27) {
LAB_042b55f0:
      if (uVar27 >> 0x3d != 0) goto LAB_042b5bf8;
      uVar12 = FUN_00008790(uVar27 << 3);
      bVar26 = false;
LAB_042b5604:
      lVar22 = *(long *)(param_1 + 0xd8);
      *(undefined8 *)(param_1 + 0xd8) = uVar12;
      if (lVar22 != 0) {
        FUN_00005810(lVar22,*(long *)(param_1 + 0xe0) << 3);
      }
      *(ulong *)(param_1 + 0xe0) = uVar27;
      if (!bVar26) {
        uVar25 = 0;
        do {
          *(undefined8 *)(*(long *)(param_1 + 0xd8) + uVar25 * 8) = 0;
          uVar25 = uVar25 + 1;
        } while (uVar27 != uVar25);
        plVar19 = *(long **)(param_1 + 0xe8);
        if (plVar19 != (long *)0x0) {
          uVar25 = plVar19[1];
          uVar24 = uVar27 - 1;
          if ((uVar27 & uVar24) == 0) {
            uVar25 = uVar25 & uVar24;
          }
          else if (uVar27 <= uVar25) {
            uVar16 = 0;
            if (uVar27 != 0) {
              uVar16 = uVar25 / uVar27;
            }
            uVar25 = uVar25 - uVar16 * uVar27;
          }
          *(undefined8 **)(*(long *)(param_1 + 0xd8) + uVar25 * 8) = (undefined8 *)(param_1 + 0xe8);
          plVar20 = (long *)*plVar19;
          while (plVar20 != (long *)0x0) {
            uVar16 = plVar20[1];
            if ((uVar27 & uVar24) == 0) {
              uVar16 = uVar16 & uVar24;
            }
            else if (uVar27 <= uVar16) {
              uVar4 = 0;
              if (uVar27 != 0) {
                uVar4 = uVar16 / uVar27;
              }
              uVar16 = uVar16 - uVar4 * uVar27;
            }
            plVar21 = plVar20;
            if (uVar16 != uVar25) {
              if (*(long *)(*(long *)(param_1 + 0xd8) + uVar16 * 8) == 0) {
                *(long **)(*(long *)(param_1 + 0xd8) + uVar16 * 8) = plVar19;
                uVar25 = uVar16;
              }
              else {
                *plVar19 = *plVar20;
                *plVar20 = **(undefined8 **)(*(long *)(param_1 + 0xd8) + uVar16 * 8);
                **(long **)(*(long *)(param_1 + 0xd8) + uVar16 * 8) = (long)plVar20;
                plVar21 = plVar19;
              }
            }
            plVar19 = plVar21;
            plVar20 = (long *)*plVar21;
          }
        }
      }
    }
    else if (uVar27 < uVar25) {
      uVar24 = (ulong)((float)*(ulong *)(param_1 + 0xf0) / *(float *)(param_1 + 0xf8));
      if ((uVar25 < 3) || ((uVar25 & uVar25 - 1) != 0)) {
        uVar24 = FUN_00f76630();
      }
      else if (1 < uVar24) {
        uVar24 = 1L << (-LZCOUNT(uVar24 - 1) & 0x3fU);
      }
      if (uVar27 <= uVar24) {
        uVar27 = uVar24;
      }
      if (uVar27 < uVar25) {
        if (uVar27 != 0) goto LAB_042b55f0;
        uVar12 = 0;
        bVar26 = true;
        goto LAB_042b5604;
      }
    }
    uVar25 = *(ulong *)(param_1 + 0xe0);
    if ((uVar25 & uVar25 - 1) == 0) {
      uVar27 = uVar25 - 1 & uVar17;
    }
    else {
      uVar27 = uVar17;
      if (uVar25 <= uVar17) {
        uVar27 = 0;
        if (uVar25 != 0) {
          uVar27 = uVar17 / uVar25;
        }
        uVar27 = uVar17 - uVar27 * uVar25;
      }
    }
  }
  lVar22 = *(long *)(param_1 + 0xd8);
  plVar19 = *(long **)(lVar22 + uVar27 * 8);
  if (plVar19 == (long *)0x0) {
    plVar19 = (long *)(param_1 + 0xe8);
    *plVar9 = *plVar19;
    *plVar19 = (long)plVar9;
    *(long **)(lVar22 + uVar27 * 8) = plVar19;
    if (*plVar9 != 0) {
      uVar17 = *(ulong *)(*plVar9 + 8);
      if ((uVar25 & uVar25 - 1) == 0) {
        uVar17 = uVar17 & uVar25 - 1;
      }
      else if (uVar25 <= uVar17) {
        uVar27 = 0;
        if (uVar25 != 0) {
          uVar27 = uVar17 / uVar25;
        }
        uVar17 = uVar17 - uVar27 * uVar25;
      }
      *(long **)(*(long *)(param_1 + 0xd8) + uVar17 * 8) = plVar9;
    }
  }
  else {
    *plVar9 = *plVar19;
    *plVar19 = (long)plVar9;
  }
  *(long *)(param_1 + 0xf0) = *(long *)(param_1 + 0xf0) + 1;
LAB_042b4e28:
  if ((*(byte *)(DAT_0e4ac750 + 0x24) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar14 = (code *)SoftwareBreakpoint(0,0x42b5c0c);
    (*pcVar14)();
  }
  if (*(int *)(DAT_0e4ac750 + 0x20) == 0) {
    pppppuVar10 = (undefined8 *****)local_1c8;
    if (-1 < (long)local_1b8._7_1_) {
      pppppuVar10 = &local_1c8;
    }
    ppppuVar3 = (undefined8 ****)pppuStack_1c0;
    if (-1 < (long)local_1b8) {
      ppppuVar3 = (undefined8 ****)(long)local_1b8._7_1_;
    }
    uVar17 = FUN_00018f3c(pppppuVar10,ppppuVar3);
    uVar25 = *(ulong *)(param_1 + 0x108);
    if (uVar25 == 0) {
      uVar27 = 0;
    }
    else {
      uVar24 = uVar25 - 1;
      if ((uVar25 & uVar24) == 0) {
        uVar27 = uVar24 & uVar17;
      }
      else {
        uVar27 = uVar17;
        if (uVar25 <= uVar17) {
          uVar27 = 0;
          if (uVar25 != 0) {
            uVar27 = uVar17 / uVar25;
          }
          uVar27 = uVar17 - uVar27 * uVar25;
        }
      }
      plVar9 = *(long **)(*(long *)(param_1 + 0x100) + uVar27 * 8);
      if (plVar9 != (long *)0x0) {
        for (plVar9 = (long *)*plVar9; plVar9 != (long *)0x0; plVar9 = (long *)*plVar9) {
          uVar16 = plVar9[1];
          if (uVar16 == uVar17) {
            uVar16 = FUN_00df8f58(plVar9 + 2,&local_1c8);
            if ((uVar16 & 1) != 0) goto LAB_042b4e40;
          }
          else {
            if ((uVar25 & uVar24) == 0) {
              uVar16 = uVar16 & uVar24;
            }
            else if (uVar25 <= uVar16) {
              uVar4 = 0;
              if (uVar25 != 0) {
                uVar4 = uVar16 / uVar25;
              }
              uVar16 = uVar16 - uVar4 * uVar25;
            }
            if (uVar16 != uVar27) break;
          }
        }
      }
    }
    plVar9 = (long *)FUN_00008790(0x38);
    if (plVar9 == (long *)0x0) {
LAB_042b5738:
                    /* WARNING: Does not return */
      pcVar14 = (code *)SoftwareBreakpoint(1,0x42b573c);
      (*pcVar14)();
    }
    *plVar9 = 0;
    plVar9[1] = uVar17;
    plVar19 = plVar9 + 2;
    if (plVar19 == (long *)0x0) goto LAB_042b5738;
    if ((long)local_1b8 < 0) {
      FUN_00060a60(plVar19,local_1c8,pppuStack_1c0);
    }
    else {
      plVar9[3] = (long)pppuStack_1c0;
      *plVar19 = (long)local_1c8;
      plVar9[4] = (long)local_1b8;
    }
    *(undefined1 (*) [16])(plVar9 + 5) = auVar36;
    if (lVar8 != 0) {
      *(long *)(lVar8 + 8) = *(long *)(lVar8 + 8) + 1;
    }
    fVar28 = (float)(*(long *)(param_1 + 0x118) + 1);
    if (*(float *)(param_1 + 0x120) * (float)uVar25 < fVar28) {
      uVar27 = (ulong)((uVar25 & uVar25 - 1) != 0);
      if (uVar25 < 3) {
        uVar27 = 1;
      }
      uVar27 = uVar27 | uVar25 << 1;
      uVar25 = (ulong)(fVar28 / *(float *)(param_1 + 0x120));
      if (uVar27 <= uVar25) {
        uVar27 = uVar25;
      }
      if (uVar27 - 1 == 0) {
        uVar27 = 2;
      }
      else if ((uVar27 & uVar27 - 1) != 0) {
        uVar27 = FUN_00f76630(uVar27);
      }
      uVar25 = *(ulong *)(param_1 + 0x108);
      if (uVar25 < uVar27) {
LAB_042b5a68:
        if (uVar27 >> 0x3d != 0) {
LAB_042b5bf8:
                    /* WARNING: Subroutine does not return */
          FUN_04b087f0();
        }
        uVar12 = FUN_00008790(uVar27 << 3);
        bVar26 = false;
LAB_042b5a7c:
        lVar8 = *(long *)(param_1 + 0x100);
        *(undefined8 *)(param_1 + 0x100) = uVar12;
        if (lVar8 != 0) {
          FUN_00005810(lVar8,*(long *)(param_1 + 0x108) << 3);
        }
        *(ulong *)(param_1 + 0x108) = uVar27;
        if (!bVar26) {
          uVar25 = 0;
          do {
            *(undefined8 *)(*(long *)(param_1 + 0x100) + uVar25 * 8) = 0;
            uVar25 = uVar25 + 1;
          } while (uVar27 != uVar25);
          plVar19 = *(long **)(param_1 + 0x110);
          if (plVar19 != (long *)0x0) {
            uVar25 = plVar19[1];
            uVar24 = uVar27 - 1;
            if ((uVar27 & uVar24) == 0) {
              uVar25 = uVar25 & uVar24;
            }
            else if (uVar27 <= uVar25) {
              uVar16 = 0;
              if (uVar27 != 0) {
                uVar16 = uVar25 / uVar27;
              }
              uVar25 = uVar25 - uVar16 * uVar27;
            }
            *(long *)(*(long *)(param_1 + 0x100) + uVar25 * 8) = param_1 + 0x110;
            plVar20 = (long *)*plVar19;
            while (plVar20 != (long *)0x0) {
              uVar16 = plVar20[1];
              if ((uVar27 & uVar24) == 0) {
                uVar16 = uVar16 & uVar24;
              }
              else if (uVar27 <= uVar16) {
                uVar4 = 0;
                if (uVar27 != 0) {
                  uVar4 = uVar16 / uVar27;
                }
                uVar16 = uVar16 - uVar4 * uVar27;
              }
              plVar21 = plVar20;
              if (uVar16 != uVar25) {
                if (*(long *)(*(long *)(param_1 + 0x100) + uVar16 * 8) == 0) {
                  *(long **)(*(long *)(param_1 + 0x100) + uVar16 * 8) = plVar19;
                  uVar25 = uVar16;
                }
                else {
                  *plVar19 = *plVar20;
                  *plVar20 = **(undefined8 **)(*(long *)(param_1 + 0x100) + uVar16 * 8);
                  **(long **)(*(long *)(param_1 + 0x100) + uVar16 * 8) = (long)plVar20;
                  plVar21 = plVar19;
                }
              }
              plVar19 = plVar21;
              plVar20 = (long *)*plVar21;
            }
          }
        }
      }
      else if (uVar27 < uVar25) {
        uVar24 = (ulong)((float)*(ulong *)(param_1 + 0x118) / *(float *)(param_1 + 0x120));
        if ((uVar25 < 3) || ((uVar25 & uVar25 - 1) != 0)) {
          uVar24 = FUN_00f76630();
        }
        else if (1 < uVar24) {
          uVar24 = 1L << (-LZCOUNT(uVar24 - 1) & 0x3fU);
        }
        if (uVar27 <= uVar24) {
          uVar27 = uVar24;
        }
        if (uVar27 < uVar25) {
          if (uVar27 != 0) goto LAB_042b5a68;
          uVar12 = 0;
          bVar26 = true;
          goto LAB_042b5a7c;
        }
      }
      uVar25 = *(ulong *)(param_1 + 0x108);
      if ((uVar25 & uVar25 - 1) == 0) {
        uVar27 = uVar25 - 1 & uVar17;
      }
      else {
        uVar27 = uVar17;
        if (uVar25 <= uVar17) {
          uVar27 = 0;
          if (uVar25 != 0) {
            uVar27 = uVar17 / uVar25;
          }
          uVar27 = uVar17 - uVar27 * uVar25;
        }
      }
    }
    lVar8 = *(long *)(param_1 + 0x100);
    plVar19 = *(long **)(lVar8 + uVar27 * 8);
    if (plVar19 == (long *)0x0) {
      *plVar9 = *(long *)(param_1 + 0x110);
      *(long **)(param_1 + 0x110) = plVar9;
      *(long *)(lVar8 + uVar27 * 8) = param_1 + 0x110;
      if (*plVar9 != 0) {
        uVar17 = *(ulong *)(*plVar9 + 8);
        if ((uVar25 & uVar25 - 1) == 0) {
          uVar17 = uVar17 & uVar25 - 1;
        }
        else if (uVar25 <= uVar17) {
          uVar27 = 0;
          if (uVar25 != 0) {
            uVar27 = uVar17 / uVar25;
          }
          uVar17 = uVar17 - uVar27 * uVar25;
        }
        *(long **)(*(long *)(param_1 + 0x100) + uVar17 * 8) = plVar9;
      }
    }
    else {
      *plVar9 = *plVar19;
      *plVar19 = (long)plVar9;
    }
    *(long *)(param_1 + 0x118) = *(long *)(param_1 + 0x118) + 1;
  }
LAB_042b4e40:
  uVar12 = FUN_04296d5c();
  FUN_04297504(uVar12,param_2);
LAB_042b4d84:
  if ((long)local_1b8 < 0) {
    FUN_00005810(local_1c8,(ulong)local_1b8 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_78) {
    return auVar36;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


