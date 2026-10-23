// ==== 0x42b66bc  FUN_042b66bc  (4188 bytes)

/* WARNING: Removing unreachable block (ram,0x042b7410) */

void FUN_042b66bc(long *param_1)

{
  long *plVar1;
  size_t sVar2;
  long **pplVar3;
  long *plVar4;
  ulong uVar5;
  code *pcVar6;
  bool bVar7;
  int iVar8;
  ulong uVar9;
  undefined8 uVar10;
  undefined8 *puVar11;
  char *pcVar12;
  long lVar13;
  long lVar14;
  ulong *puVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long local_130;
  long local_128;
  long local_120;
  undefined8 local_118;
  undefined8 local_110;
  long local_108;
  long *local_100;
  undefined5 uStack_f8;
  undefined4 uStack_f3;
  uint uStack_ef;
  char cStack_eb;
  undefined1 uStack_ea;
  char cStack_e9;
  long *local_e8;
  ulong uStack_e0;
  undefined8 local_d8;
  long *local_c8;
  ulong uStack_c0;
  ulong local_b8;
  long local_b0;
  undefined8 uStack_a8;
  long local_a0;
  long *local_98;
  ulong uStack_90;
  ulong local_88;
  long local_80;
  undefined8 local_78;
  long local_70;
  
  lVar14 = DAT_0e481618;
  param_1[1] = 0;
  param_1[2] = 0;
  *param_1 = 0;
  if (lVar14 == 0) {
    return;
  }
  local_80 = 0;
  local_78 = 0;
  local_70 = 0;
  FUN_017e6e28(&local_98);
  lVar14 = 0;
  plVar1 = local_98;
  plVar4 = local_98;
  for (lVar18 = uStack_90 << 3; local_98 = plVar4, lVar18 != 0; lVar18 = lVar18 + -8) {
    lVar16 = *plVar1;
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
    uVar9 = (ulong)local_100 >> 8;
    local_100 = (long *)CONCAT71((int7)uVar9,*(char *)(*(long *)(lVar16 + 0x270) + 0x48));
    local_e8._1_7_ = (undefined7)((ulong)local_e8 >> 8);
    local_e8 = (long *)CONCAT71(local_e8._1_7_,1);
    FUN_0053a240(&local_b0,"off_the_record",0xe,&local_100);
    if (((ulong)local_e8 & 0xff) != 0xff) {
      (*(code *)(&PTR__cxx_destruct_0df6d258)[(ulong)local_e8 & 0xff])(&local_c8,&local_100);
    }
    lVar17 = *(long *)(lVar16 + 0x8d8);
    if (lVar17 != 0) {
      if ((*(long *)(lVar17 + 0x128) == 0) || (*(char *)(*(long *)(lVar17 + 0x128) + 4) != '\0')) {
                    /* WARNING: Does not return */
        pcVar6 = (code *)SoftwareBreakpoint(0,0x42b770c);
        (*pcVar6)();
      }
      lVar13 = *(long *)(*(long *)(lVar17 + 0x130) + 0x40);
      if (lVar13 == 0) {
                    /* WARNING: Does not return */
        pcVar6 = (code *)SoftwareBreakpoint(0,0x42b7718);
        (*pcVar6)();
      }
      local_100 = (long *)CONCAT44(local_100._4_4_,*(undefined4 *)(lVar13 + 0x198));
      local_e8 = (long *)CONCAT71(local_e8._1_7_,2);
      FUN_0053a240(&local_b0,"main_frame_process_id",0x15,&local_100);
      if (((char)local_e8 != -1) && ((char)local_e8 != '\x02')) {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(ulong)local_e8 & 0xff])(&local_c8,&local_100);
      }
      local_100 = (long *)CONCAT44(local_100._4_4_,*(undefined4 *)(lVar17 + 0x758));
      local_e8 = (long *)CONCAT71(local_e8._1_7_,2);
      FUN_0053a240(&local_b0,"main_frame_routing_id",0x15,&local_100);
      if (((char)local_e8 != -1) && ((char)local_e8 != '\x02')) {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(ulong)local_e8 & 0xff])(&local_c8,&local_100);
      }
    }
    lVar16 = FUN_000805e0(lVar16 + 8,&DAT_0c9d7104);
    if (lVar16 == 0) {
      lVar14 = lVar14 + 1;
    }
    else {
      FUN_009f4a24(&local_c8,*(undefined8 *)(lVar16 + 0x10));
      uStack_f8 = (undefined5)uStack_c0;
      uStack_f3._0_3_ = (undefined3)(uStack_c0 >> 0x28);
      local_100 = local_c8;
      uStack_f3._3_1_ = (undefined1)local_b8;
      uStack_ef = (uint)(local_b8 >> 8);
      cStack_eb = (char)(local_b8 >> 0x28);
      uStack_ea = (undefined1)(local_b8 >> 0x30);
      cStack_e9 = (char)(local_b8 >> 0x38);
      local_c8 = (long *)0x0;
      uStack_c0 = 0;
      local_b8 = 0;
      local_e8 = (long *)CONCAT71(local_e8._1_7_,4);
      FUN_0053a240(&local_b0,"diagnostic_id",0xd,&local_100);
      if (((ulong)local_e8 & 0xff) != 0xff) {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(ulong)local_e8 & 0xff])(&local_130,&local_100);
      }
      if ((long)local_b8 < 0) {
        FUN_00005810(local_c8,local_b8 & 0x7fffffffffffffff);
      }
    }
    FUN_00a28348(&local_80,&local_b0);
    FUN_005a58dc(&local_b0);
    plVar1 = plVar1 + 1;
    plVar4 = local_98;
  }
  if (plVar4 != (long *)0x0) {
    if ((uStack_90 & 0x1fffffffffffffff) != 0) {
      puVar15 = (ulong *)(plVar4 + uStack_90);
      lVar18 = uStack_90 * -8;
      do {
        puVar15 = puVar15 + -1;
        if (puVar15 == (ulong *)0x0) goto LAB_042b7460;
        if ((*puVar15 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0001fee4();
        }
        *puVar15 = 0;
        lVar18 = lVar18 + 8;
      } while (lVar18 != 0);
    }
    uStack_90 = (long)plVar4 - (long)local_98 >> 3;
    FUN_00005810(local_98,local_88 << 3);
  }
  local_100 = (long *)FUN_00008790(0x20);
  uStack_ef = 0;
  cStack_eb = '\0';
  uStack_ea = 0;
  cStack_e9 = -0x80;
  uStack_f8 = 0x19;
  uStack_f3 = 0x20000000;
  if ((local_100 <= (long *)((long)local_100 + 0x19)) &&
     ((long *)((long)local_100 + 0x19) <= "native_web_contents_count" ||
      "native_web_contents_count" < local_100)) {
    local_100[1] = 0x65746e6f635f6265;
    *local_100 = 0x775f65766974616e;
    builtin_strncpy((char *)((long)local_100 + 0x11),"ts_count",8);
    builtin_strncpy((char *)((long)local_100 + 9),"b_conten",8);
    *(char *)((long)local_100 + 0x19) = '\0';
    FUN_007a86f8(&local_e8,local_78);
    if (param_1[1] == param_1[2]) {
      pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
    }
    else {
      pcVar12 = (char *)(*param_1 + param_1[1] * 0x30);
      if (pcVar12 == (char *)0x0) goto LAB_042b7460;
      *(ulong *)(pcVar12 + 0x10) =
           CONCAT17(cStack_e9,
                    CONCAT16(uStack_ea,CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_))));
      *(ulong *)(pcVar12 + 8) = CONCAT35((undefined3)uStack_f3,uStack_f8);
      *(long **)pcVar12 = local_100;
      uStack_f8 = 0;
      uStack_f3 = 0;
      uStack_ef = 0;
      cStack_eb = '\0';
      uStack_ea = 0;
      cStack_e9 = '\0';
      local_100 = (long *)0x0;
      *(ulong *)(pcVar12 + 0x28) = local_d8;
      *(ulong *)(pcVar12 + 0x20) = uStack_e0;
      *(long **)(pcVar12 + 0x18) = local_e8;
      uStack_e0 = 0;
      local_d8 = 0;
      local_e8 = (long *)0x0;
      pcVar12 = pcVar12 + 0x30;
    }
    lVar16 = *param_1;
    lVar18 = (long)pcVar12 - lVar16;
    lVar17 = (lVar18 >> 4) * -0x5555555555555555;
    param_1[1] = lVar17;
    if (lVar18 != 0) {
      if ((long)local_d8 < 0) {
        FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
      }
      if (cStack_e9 < '\0') {
        FUN_00005810(local_100,
                     CONCAT17(cStack_e9,
                              CONCAT16(uStack_ea,
                                       CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_)))) &
                     0x7fffffffffffffff);
      }
      local_100 = (long *)FUN_00008790(0x30);
      uStack_ef = 0;
      cStack_eb = '\0';
      uStack_ea = 0;
      cStack_e9 = -0x80;
      uStack_f8 = 0x29;
      uStack_f3 = 0x30000000;
      if ((local_100 <= (long *)((long)local_100 + 0x29)) &&
         ((long *)((long)local_100 + 0x29) <= "native_web_contents_without_diagnostic_id" ||
          "native_web_contents_without_diagnostic_id" < local_100)) {
        local_100[1] = 0x65746e6f635f6265;
        *local_100 = 0x775f65766974616e;
        local_100[3] = 0x676169645f74756f;
        local_100[2] = 0x687469775f73746e;
        builtin_strncpy((char *)((long)local_100 + 0x21),"ostic_id",8);
        builtin_strncpy((char *)((long)local_100 + 0x19),"ut_diagn",8);
        *(char *)((long)local_100 + 0x29) = '\0';
        FUN_007a86f8(&local_e8,lVar14);
        if (lVar17 - param_1[2] == 0) {
          pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
        }
        else {
          if (pcVar12 == (char *)0x0) goto LAB_042b7460;
          pcVar12 = (char *)(lVar16 + lVar18);
          *(ulong *)(pcVar12 + 0x10) =
               CONCAT17(cStack_e9,
                        CONCAT16(uStack_ea,CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_))))
          ;
          *(ulong *)(pcVar12 + 8) = CONCAT35((undefined3)uStack_f3,uStack_f8);
          *(long **)pcVar12 = local_100;
          uStack_f8 = 0;
          uStack_f3 = 0;
          uStack_ef = 0;
          cStack_eb = '\0';
          uStack_ea = 0;
          cStack_e9 = '\0';
          local_100 = (long *)0x0;
          *(ulong *)(pcVar12 + 0x28) = local_d8;
          *(ulong *)(pcVar12 + 0x20) = uStack_e0;
          *(long **)(pcVar12 + 0x18) = local_e8;
          uStack_e0 = 0;
          local_d8 = 0;
          local_e8 = (long *)0x0;
          pcVar12 = pcVar12 + 0x30;
        }
        lVar18 = *param_1;
        lVar14 = (long)pcVar12 - lVar18;
        lVar16 = (lVar14 >> 4) * -0x5555555555555555;
        param_1[1] = lVar16;
        if (lVar14 != 0) {
          if ((long)local_d8 < 0) {
            FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
          }
          if (cStack_e9 < '\0') {
            FUN_00005810(local_100,
                         CONCAT17(cStack_e9,
                                  CONCAT16(uStack_ea,
                                           CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_))))
                         & 0x7fffffffffffffff);
          }
          local_98 = (long *)0x0;
          uStack_90 = 0;
          local_88 = 0;
          local_100 = &local_80;
          uStack_f3 = CONCAT13(7,(undefined3)uStack_f3);
          uVar9 = FUN_028b76a8(&local_100,0,&local_98,200);
          if ((uVar9 & 1) == 0) {
                    /* WARNING: Does not return */
            pcVar6 = (code *)SoftwareBreakpoint(0,0x42b7724);
            (*pcVar6)();
          }
          local_100 = (long *)FUN_00008790(0x20);
          uVar5 = local_88;
          uVar9 = uStack_90;
          plVar1 = local_98;
          uStack_ef = 0;
          cStack_eb = '\0';
          uStack_ea = 0;
          cStack_e9 = -0x80;
          uStack_f8 = 0x1d;
          uStack_f3 = 0x20000000;
          if ((local_100 <= (long *)((long)local_100 + 0x1d)) &&
             ((long *)((long)local_100 + 0x1d) <= "native_web_contents_inventory" ||
              "native_web_contents_inventory" < local_100)) {
            local_100[1] = 0x65746e6f635f6265;
            *local_100 = 0x775f65766974616e;
            builtin_strncpy((char *)((long)local_100 + 0x15),"nventory",8);
            builtin_strncpy((char *)((long)local_100 + 0xd),"ntents_i",8);
            *(char *)((long)local_100 + 0x1d) = '\0';
            uStack_e0 = uStack_90;
            local_e8 = local_98;
            local_d8 = local_88;
            local_98 = (long *)0x0;
            uStack_90 = 0;
            local_88 = 0;
            if (lVar16 - param_1[2] == 0) {
              puVar11 = (undefined8 *)FUN_042bf89c(param_1,&local_100);
            }
            else {
              if (pcVar12 == (char *)0x0) goto LAB_042b7460;
              puVar11 = (undefined8 *)(lVar18 + lVar14);
              puVar11[1] = 0x1d;
              *puVar11 = local_100;
              puVar11[2] = &DAT_8000000000000020;
              uStack_f8 = 0;
              uStack_f3 = 0;
              uStack_ef = 0;
              cStack_eb = '\0';
              uStack_ea = 0;
              cStack_e9 = '\0';
              local_100 = (long *)0x0;
              puVar11[4] = uVar9;
              puVar11[3] = plVar1;
              puVar11[5] = uVar5;
              local_e8 = (long *)0x0;
              uStack_e0 = 0;
              local_d8 = 0;
              puVar11 = puVar11 + 6;
            }
            lVar18 = *param_1;
            lVar14 = (long)puVar11 - lVar18;
            lVar16 = (lVar14 >> 4) * -0x5555555555555555;
            param_1[1] = lVar16;
            if (lVar14 != 0) {
              if ((long)local_d8 < 0) {
                FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
              }
              if (cStack_e9 < '\0') {
                FUN_00005810(local_100,
                             CONCAT17(cStack_e9,
                                      CONCAT16(uStack_ea,
                                               CONCAT15(cStack_eb,
                                                        CONCAT41(uStack_ef,uStack_f3._3_1_)))) &
                             0x7fffffffffffffff);
              }
              local_110 = 0;
              local_108 = 0;
              local_118 = 0;
              local_b0 = 0;
              uStack_a8 = 0;
              local_a0 = 0;
              local_100 = &local_108;
              uStack_f8 = SUB85(&local_110,0);
              uStack_f3._0_3_ = (undefined3)((ulong)&local_110 >> 0x28);
              local_e8 = &local_b0;
              uStack_f3._3_1_ = SUB81(&local_118,0);
              uStack_ef = (uint)((ulong)&local_118 >> 8);
              cStack_eb = (char)((ulong)&local_118 >> 0x28);
              uStack_ea = (undefined1)((ulong)&local_118 >> 0x30);
              cStack_e9 = (char)((ulong)&local_118 >> 0x38);
              FUN_018f0070(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),&local_100,
                           FUN_042c1bc8,0,0);
              cStack_e9 = '\x11';
              if (&uStack_ef <= "browser_tab_count" || "browser_tab_count" < &local_100) {
                uStack_ef = uStack_ef & 0xffffff00;
                uStack_f8 = 0x635f626174;
                uStack_f3 = 0x746e756f;
                local_100 = (long *)0x5f726573776f7262;
                FUN_007a86f8(&local_e8,local_108);
                if (lVar16 - param_1[2] == 0) {
                  pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
                }
                else {
                  if (puVar11 == (undefined8 *)0x0) goto LAB_042b7460;
                  pcVar12 = (char *)(lVar18 + lVar14);
                  *(ulong *)(pcVar12 + 0x10) =
                       CONCAT17(cStack_e9,
                                CONCAT16(uStack_ea,
                                         CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_))));
                  *(ulong *)(pcVar12 + 8) = CONCAT35((undefined3)uStack_f3,uStack_f8);
                  *(long **)pcVar12 = local_100;
                  uStack_f8 = 0;
                  uStack_f3 = 0;
                  uStack_ef = 0;
                  cStack_eb = '\0';
                  uStack_ea = 0;
                  cStack_e9 = '\0';
                  local_100 = (long *)0x0;
                  *(ulong *)(pcVar12 + 0x28) = local_d8;
                  *(ulong *)(pcVar12 + 0x20) = uStack_e0;
                  *(long **)(pcVar12 + 0x18) = local_e8;
                  uStack_e0 = 0;
                  local_d8 = 0;
                  local_e8 = (long *)0x0;
                  pcVar12 = pcVar12 + 0x30;
                }
                lVar18 = *param_1;
                lVar14 = (long)pcVar12 - lVar18;
                lVar16 = (lVar14 >> 4) * -0x5555555555555555;
                param_1[1] = lVar16;
                if (lVar14 != 0) {
                  if ((long)local_d8 < 0) {
                    FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
                  }
                  if (cStack_e9 < '\0') {
                    FUN_00005810(local_100,
                                 CONCAT17(cStack_e9,
                                          CONCAT16(uStack_ea,
                                                   CONCAT15(cStack_eb,
                                                            CONCAT41(uStack_ef,uStack_f3._3_1_)))) &
                                 0x7fffffffffffffff);
                  }
                  local_100 = (long *)FUN_00008790(0x30);
                  uStack_ef = 0;
                  cStack_eb = '\0';
                  uStack_ea = 0;
                  cStack_e9 = -0x80;
                  uStack_f8 = 0x22;
                  uStack_f3 = 0x30000000;
                  if ((local_100 <= (long *)((long)local_100 + 0x22)) &&
                     ((long *)((long)local_100 + 0x22) <= "browser_tabs_without_diagnostic_id" ||
                      "browser_tabs_without_diagnostic_id" < local_100)) {
                    *(undefined2 *)(local_100 + 4) = 0x6469;
                    local_100[1] = 0x7469775f73626174;
                    *local_100 = 0x5f726573776f7262;
                    local_100[3] = 0x5f636974736f6e67;
                    local_100[2] = 0x6169645f74756f68;
                    *(char *)((long)local_100 + 0x22) = '\0';
                    FUN_007a86f8(&local_e8,local_110);
                    if (lVar16 - param_1[2] == 0) {
                      pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
                    }
                    else {
                      if (pcVar12 == (char *)0x0) goto LAB_042b7460;
                      pcVar12 = (char *)(lVar18 + lVar14);
                      *(ulong *)(pcVar12 + 0x10) =
                           CONCAT17(cStack_e9,
                                    CONCAT16(uStack_ea,
                                             CONCAT15(cStack_eb,CONCAT41(uStack_ef,uStack_f3._3_1_))
                                            ));
                      *(ulong *)(pcVar12 + 8) = CONCAT35((undefined3)uStack_f3,uStack_f8);
                      *(long **)pcVar12 = local_100;
                      uStack_f8 = 0;
                      uStack_f3 = 0;
                      uStack_ef = 0;
                      cStack_eb = '\0';
                      uStack_ea = 0;
                      cStack_e9 = '\0';
                      local_100 = (long *)0x0;
                      *(ulong *)(pcVar12 + 0x28) = local_d8;
                      *(ulong *)(pcVar12 + 0x20) = uStack_e0;
                      *(long **)(pcVar12 + 0x18) = local_e8;
                      uStack_e0 = 0;
                      local_d8 = 0;
                      local_e8 = (long *)0x0;
                      pcVar12 = pcVar12 + 0x30;
                    }
                    lVar18 = *param_1;
                    lVar14 = (long)pcVar12 - lVar18;
                    lVar16 = (lVar14 >> 4) * -0x5555555555555555;
                    param_1[1] = lVar16;
                    if (lVar14 != 0) {
                      if ((long)local_d8 < 0) {
                        FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
                      }
                      if (cStack_e9 < '\0') {
                        FUN_00005810(local_100,
                                     CONCAT17(cStack_e9,
                                              CONCAT16(uStack_ea,
                                                       CONCAT15(cStack_eb,
                                                                CONCAT41(uStack_ef,uStack_f3._3_1_))
                                                      )) & 0x7fffffffffffffff);
                      }
                      local_100 = (long *)FUN_00008790(0x30);
                      uStack_ef = 0;
                      cStack_eb = '\0';
                      uStack_ea = 0;
                      cStack_e9 = -0x80;
                      uStack_f8 = 0x23;
                      uStack_f3 = 0x30000000;
                      if ((local_100 <= (long *)((long)local_100 + 0x23)) &&
                         ((long *)((long)local_100 + 0x23) <= "browser_tabs_with_null_web_contents"
                          || "browser_tabs_with_null_web_contents" < local_100)) {
                        builtin_strncpy((char *)((long)local_100 + 0x1f),"ents",4);
                        local_100[1] = 0x7469775f73626174;
                        *local_100 = 0x5f726573776f7262;
                        local_100[3] = 0x65746e6f635f6265;
                        local_100[2] = 0x775f6c6c756e5f68;
                        *(char *)((long)local_100 + 0x23) = '\0';
                        FUN_007a86f8(&local_e8,local_118);
                        if (lVar16 - param_1[2] == 0) {
                          pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
                        }
                        else {
                          if (pcVar12 == (char *)0x0) goto LAB_042b7460;
                          pcVar12 = (char *)(lVar18 + lVar14);
                          *(ulong *)(pcVar12 + 0x10) =
                               CONCAT17(cStack_e9,
                                        CONCAT16(uStack_ea,
                                                 CONCAT15(cStack_eb,
                                                          CONCAT41(uStack_ef,uStack_f3._3_1_))));
                          *(ulong *)(pcVar12 + 8) = CONCAT35((undefined3)uStack_f3,uStack_f8);
                          *(long **)pcVar12 = local_100;
                          uStack_f8 = 0;
                          uStack_f3 = 0;
                          uStack_ef = 0;
                          cStack_eb = '\0';
                          uStack_ea = 0;
                          cStack_e9 = '\0';
                          local_100 = (long *)0x0;
                          *(ulong *)(pcVar12 + 0x28) = local_d8;
                          *(ulong *)(pcVar12 + 0x20) = uStack_e0;
                          *(long **)(pcVar12 + 0x18) = local_e8;
                          uStack_e0 = 0;
                          local_d8 = 0;
                          local_e8 = (long *)0x0;
                          pcVar12 = pcVar12 + 0x30;
                        }
                        lVar18 = *param_1;
                        lVar14 = (long)pcVar12 - lVar18;
                        lVar16 = (lVar14 >> 4) * -0x5555555555555555;
                        param_1[1] = lVar16;
                        if (lVar14 != 0) {
                          if ((long)local_d8 < 0) {
                            FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
                          }
                          if (cStack_e9 < '\0') {
                            FUN_00005810(local_100,
                                         CONCAT17(cStack_e9,
                                                  CONCAT16(uStack_ea,
                                                           CONCAT15(cStack_eb,
                                                                    CONCAT41(uStack_ef,
                                                                             uStack_f3._3_1_)))) &
                                         0x7fffffffffffffff);
                          }
                          local_c8 = (long *)0x0;
                          uStack_c0 = 0;
                          local_b8 = 0;
                          local_100 = &local_b0;
                          uStack_f3 = CONCAT13(7,(undefined3)uStack_f3);
                          uVar9 = FUN_028b76a8(&local_100,0,&local_c8,200);
                          if ((uVar9 & 1) == 0) {
                    /* WARNING: Does not return */
                            pcVar6 = (code *)SoftwareBreakpoint(0,0x42b7730);
                            (*pcVar6)();
                          }
                          local_100 = (long *)FUN_00008790(0x20);
                          uVar5 = local_b8;
                          uVar9 = uStack_c0;
                          plVar1 = local_c8;
                          uStack_ef = 0;
                          cStack_eb = '\0';
                          uStack_ea = 0;
                          cStack_e9 = -0x80;
                          uStack_f8 = 0x1e;
                          uStack_f3 = 0x20000000;
                          if ((local_100 <= (long *)((long)local_100 + 0x1e)) &&
                             ((long *)((long)local_100 + 0x1e) <= "browser_web_contents_inventory"
                              || "browser_web_contents_inventory" < local_100)) {
                            local_100[1] = 0x746e6f635f626577;
                            *local_100 = 0x5f726573776f7262;
                            builtin_strncpy((char *)((long)local_100 + 0x16),"nventory",8);
                            builtin_strncpy((char *)((long)local_100 + 0xe),"ntents_i",8);
                            *(char *)((long)local_100 + 0x1e) = '\0';
                            uStack_e0 = uStack_c0;
                            local_e8 = local_c8;
                            local_d8 = local_b8;
                            local_c8 = (long *)0x0;
                            uStack_c0 = 0;
                            local_b8 = 0;
                            if (lVar16 - param_1[2] == 0) {
                              puVar11 = (undefined8 *)FUN_042bf89c(param_1,&local_100);
                            }
                            else {
                              if (pcVar12 == (char *)0x0) goto LAB_042b7460;
                              puVar11 = (undefined8 *)(lVar18 + lVar14);
                              puVar11[1] = 0x1e;
                              *puVar11 = local_100;
                              puVar11[2] = &DAT_8000000000000020;
                              uStack_f8 = 0;
                              uStack_f3 = 0;
                              uStack_ef = 0;
                              cStack_eb = '\0';
                              uStack_ea = 0;
                              cStack_e9 = '\0';
                              local_100 = (long *)0x0;
                              puVar11[4] = uVar9;
                              puVar11[3] = plVar1;
                              puVar11[5] = uVar5;
                              local_e8 = (long *)0x0;
                              uStack_e0 = 0;
                              local_d8 = 0;
                              puVar11 = puVar11 + 6;
                            }
                            lVar18 = *param_1;
                            lVar14 = (long)puVar11 - lVar18;
                            lVar16 = (lVar14 >> 4) * -0x5555555555555555;
                            param_1[1] = lVar16;
                            if (lVar14 != 0) {
                              if ((long)local_d8 < 0) {
                                FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
                              }
                              if (cStack_e9 < '\0') {
                                FUN_00005810(local_100,
                                             CONCAT17(cStack_e9,
                                                      CONCAT16(uStack_ea,
                                                               CONCAT15(cStack_eb,
                                                                        CONCAT41(uStack_ef,
                                                                                 uStack_f3._3_1_))))
                                             & 0x7fffffffffffffff);
                              }
                              cStack_e9 = '\x15';
                              if (&cStack_eb <= "active_profiles_count" ||
                                  "active_profiles_count" < &local_100) {
                                uStack_f8 = 0x6c69666f72;
                                local_100 = (long *)0x705f657669746361;
                                uStack_f3 = 0x635f7365;
                                uStack_ef = 0x746e756f;
                                cStack_eb = '\0';
                                FUN_01a29860(DAT_0e481618);
                                FUN_01a298f8();
                                FUN_01a2af78(&local_130);
                                FUN_007a86f8(&local_e8,local_128);
                                if (lVar16 - param_1[2] == 0) {
                                  pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
                                }
                                else {
                                  if (puVar11 == (undefined8 *)0x0) goto LAB_042b7460;
                                  pcVar12 = (char *)(lVar18 + lVar14);
                                  *(ulong *)(pcVar12 + 0x10) =
                                       CONCAT17(cStack_e9,
                                                CONCAT16(uStack_ea,
                                                         CONCAT15(cStack_eb,
                                                                  CONCAT41(uStack_ef,uStack_f3._3_1_
                                                                          ))));
                                  *(ulong *)(pcVar12 + 8) =
                                       CONCAT35((undefined3)uStack_f3,uStack_f8);
                                  *(long **)pcVar12 = local_100;
                                  uStack_f8 = 0;
                                  uStack_f3 = 0;
                                  uStack_ef = 0;
                                  cStack_eb = '\0';
                                  uStack_ea = 0;
                                  cStack_e9 = '\0';
                                  local_100 = (long *)0x0;
                                  *(ulong *)(pcVar12 + 0x28) = local_d8;
                                  *(ulong *)(pcVar12 + 0x20) = uStack_e0;
                                  *(long **)(pcVar12 + 0x18) = local_e8;
                                  uStack_e0 = 0;
                                  local_d8 = 0;
                                  local_e8 = (long *)0x0;
                                  pcVar12 = pcVar12 + 0x30;
                                }
                                lVar18 = *param_1;
                                lVar14 = (long)pcVar12 - lVar18;
                                lVar16 = (lVar14 >> 4) * -0x5555555555555555;
                                param_1[1] = lVar16;
                                if (lVar14 != 0) {
                                  if ((long)local_d8 < 0) {
                                    FUN_00005810(local_e8,local_d8 & 0x7fffffffffffffff);
                                  }
                                  if (cStack_e9 < '\0') {
                                    FUN_00005810(local_100,
                                                 CONCAT17(cStack_e9,
                                                          CONCAT16(uStack_ea,
                                                                   CONCAT15(cStack_eb,
                                                                            CONCAT41(uStack_ef,
                                                                                     uStack_f3._3_1_
                                                                                    )))) &
                                                 0x7fffffffffffffff);
                                  }
                                  if (local_130 != 0) {
                                    if (local_128 * 8 != 0) {
                                      lVar17 = local_128 * -8;
                                      lVar13 = local_128 * 8 + local_130;
                                      do {
                                        lVar13 = lVar13 + -8;
                                        if (lVar13 == 0) goto LAB_042b7460;
                                        lVar17 = lVar17 + 8;
                                      } while (lVar17 != 0);
                                    }
                                    local_128 = 0;
                                    FUN_00005810(local_130,local_120 << 3);
                                  }
                                  local_100 = (long *)FUN_00008790(0x20);
                                  uStack_ef = 0;
                                  cStack_eb = '\0';
                                  uStack_ea = 0;
                                  cStack_e9 = -0x80;
                                  uStack_f8 = 0x17;
                                  uStack_f3 = 0x20000000;
                                  if ((local_100 <= (long *)((long)local_100 + 0x17)) &&
                                     ((long *)((long)local_100 + 0x17) <= "gpu_process_crash_count"
                                      || "gpu_process_crash_count" < local_100)) {
                                    local_100[1] = 0x736172635f737365;
                                    *local_100 = 0x636f72705f757067;
                                    builtin_strncpy((char *)((long)local_100 + 0xf),"sh_count",8);
                                    *(char *)((long)local_100 + 0x17) = '\0';
                                    FUN_01261c00(&local_e8,DAT_0e495550);
                                    if (lVar16 - param_1[2] == 0) {
                                      pcVar12 = (char *)FUN_042bf89c(param_1,&local_100);
                                    }
                                    else {
                                      if (pcVar12 == (char *)0x0) goto LAB_042b7460;
                                      pcVar12 = (char *)(lVar18 + lVar14);
                                      *(ulong *)(pcVar12 + 0x10) =
                                           CONCAT17(cStack_e9,
                                            