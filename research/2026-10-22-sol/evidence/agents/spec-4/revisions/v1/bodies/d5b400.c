// ==== 0xd5b400  FUN_00d5b400  (4980 bytes)

/* WARNING: Type propagation algorithm not settling */

undefined8 FUN_00d5b400(long param_1,undefined8 param_2,undefined4 *param_3)

{
  uint uVar1;
  byte bVar2;
  uint7 uVar3;
  code *pcVar4;
  bool bVar5;
  int iVar6;
  long *plVar8;
  undefined8 *puVar9;
  int iVar7;
  undefined4 *puVar10;
  undefined8 uVar11;
  long lVar12;
  undefined8 in_x7;
  undefined4 uVar13;
  undefined8 *puVar14;
  long lVar15;
  undefined8 *puVar16;
  ulong uVar17;
  uint *puVar18;
  long *plVar19;
  ulong uVar20;
  undefined8 *******pppppppuVar21;
  long *plVar22;
  undefined1 auVar23 [16];
  undefined1 auVar24 [16];
  undefined1 auVar25 [16];
  undefined8 *******local_1b0;
  long lStack_1a8;
  undefined8 local_1a0;
  undefined8 local_198;
  undefined8 local_190;
  undefined8 uStack_188;
  ulong local_180;
  undefined8 local_178;
  undefined8 local_170;
  undefined8 uStack_168;
  ulong local_160;
  undefined8 local_158;
  undefined8 local_150;
  undefined8 uStack_148;
  ulong local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined4 local_100;
  undefined4 uStack_fc;
  undefined6 uStack_f8;
  undefined2 uStack_f2;
  undefined6 uStack_f0;
  undefined2 local_ea;
  undefined6 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined6 uStack_b8;
  undefined2 uStack_b2;
  undefined6 uStack_b0;
  undefined8 uStack_aa;
  long local_a0;
  long lStack_98;
  long local_90;
  byte local_88;
  undefined1 auStack_71 [17];
  
  puVar9 = DAT_0e53e4d8;
  if (DAT_0e53e4d8 < (undefined8 *)0x2) {
    if (DAT_0e53e4d8 == (undefined8 *)0x0) {
      DAT_0e53e4d8 = (undefined8 *)0x1;
      uVar20 = FUN_011951c0();
      puVar9 = DAT_0e53e550;
      if (DAT_0e53e550 < (undefined8 *)0x2) {
        if (DAT_0e53e550 == (undefined8 *)0x0) {
          DAT_0e53e550 = (undefined8 *)0x1;
          DAT_0e53e568 = 0;
          DAT_0e53e560 = 0;
          DAT_0e53e558 = &DAT_0e53e560;
          FUN_0376c4e8(&DAT_0e53e558);
          DAT_0e53e550 = &DAT_0e53e558;
          puVar10 = (undefined4 *)FUN_00008790(0x30);
          *puVar10 = 1;
          *(code **)(puVar10 + 2) = FUN_01673440;
          *(code **)(puVar10 + 4) = FUN_039a57d0;
          *(code **)(puVar10 + 6) = FUN_00b77080;
          *(code **)(puVar10 + 8) = FUN_03c20914;
          if (DAT_0e2f2208 == 0) {
            FUN_0000ffa8(&DAT_0e53e550);
          }
          *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e550;
          FUN_00378000(puVar10);
          puVar9 = &DAT_0e53e558;
        }
        else if (DAT_0e53e550 == (undefined8 *)0x1) {
          lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
          do {
            lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
            uVar17 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
            if (!SBORROW8(lVar12,lVar15)) {
              uVar17 = lVar12 - lVar15;
            }
            if ((long)uVar17 < 1000) {
              FUN_083b146c();
            }
            else {
              FUN_00005dd8(1000);
            }
            puVar9 = DAT_0e53e550;
          } while (DAT_0e53e550 == (undefined8 *)0x1);
        }
      }
      DAT_0e53e4e8 = 0;
      DAT_0e53e4f0 = 0;
      DAT_0e53e4e0 = &DAT_0e53e4e8;
      if ((uVar20 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(uVar20);
      }
      DAT_0e53e4f8 = uVar20;
      if (((ulong)puVar9 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(puVar9);
      }
      DAT_0e53e500 = puVar9;
      FUN_01fb2e80(&DAT_0e53e4e0);
      DAT_0e53e4d8 = &DAT_0e53e4e0;
      puVar10 = (undefined4 *)FUN_00008790(0x30);
      *puVar10 = 1;
      *(code **)(puVar10 + 2) = FUN_01673440;
      *(code **)(puVar10 + 4) = FUN_039a57d0;
      *(code **)(puVar10 + 6) = FUN_00b77080;
      *(code **)(puVar10 + 8) = FUN_03c1ed48;
      if (DAT_0e2f2208 == 0) {
        FUN_0000ffa8(&DAT_0e53e4d8);
      }
      *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e4d8;
      FUN_00378000(puVar10);
      puVar9 = &DAT_0e53e4e0;
    }
    else if (DAT_0e53e4d8 == (undefined8 *)0x1) {
      lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
      do {
        lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
        uVar20 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
        if (!SBORROW8(lVar12,lVar15)) {
          uVar20 = lVar12 - lVar15;
        }
        if ((long)uVar20 < 1000) {
          FUN_083b146c();
        }
        else {
          FUN_00005dd8(1000);
        }
        puVar9 = DAT_0e53e4d8;
      } while (DAT_0e53e4d8 == (undefined8 *)0x1);
    }
  }
  puVar14 = (undefined8 *)puVar9[1];
joined_r0x00d5b444:
  if (puVar14 == (undefined8 *)0x0) {
LAB_00d5bfbc:
    if (((DAT_0e457bc8 & 1) == 0) && (iVar7 = thunk_FUN_0c44b610(&DAT_0e457bc8), iVar7 != 0)) {
      DAT_0e457bc0 = FUN_01dc08b8("content_settings-content_type",0x20);
      thunk_FUN_0c44b760(&DAT_0e457bc8);
    }
    uVar11 = DAT_0e457bc0;
    FUN_01261c00(&local_1b0,param_2);
    pppppppuVar21 = local_1b0;
    if (-1 < (long)local_1a0._7_1_) {
      pppppppuVar21 = &local_1b0;
    }
    lVar15 = lStack_1a8;
    if (-1 < (long)local_1a0) {
      lVar15 = (long)local_1a0._7_1_;
    }
    FUN_01f694e4(uVar11,pppppppuVar21,lVar15);
    if ((long)local_1a0 < 0) {
      FUN_00005810(local_1b0,local_1a0 & 0x7fffffffffffffff);
    }
    if (((DAT_0e457bd8 & 1) == 0) && (iVar7 = thunk_FUN_0c44b610(&DAT_0e457bd8), iVar7 != 0)) {
      DAT_0e457bd0 = FUN_01dc08b8("content_settings-histogram_value",0x20);
      thunk_FUN_0c44b760(&DAT_0e457bd8);
    }
    uVar11 = DAT_0e457bd0;
    thunk_FUN_09e5d718(param_2);
    FUN_01261c00(&local_1b0);
    if (-1 < (long)local_1a0._7_1_) {
      local_1b0 = &local_1b0;
    }
    if (-1 < local_1a0._7_1_) {
      lStack_1a8 = (long)local_1a0._7_1_;
    }
    FUN_01f694e4(uVar11,local_1b0,lStack_1a8);
    thunk_FUN_09c8a2e0(&local_1b0);
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0,0xd5c0d4);
    (*pcVar4)();
  }
  iVar7 = (int)param_2;
  if (iVar7 < *(int *)(puVar14 + 4)) {
    puVar14 = (undefined8 *)*puVar14;
    goto joined_r0x00d5b444;
  }
  if (*(int *)(puVar14 + 4) < iVar7) {
    puVar14 = (undefined8 *)puVar14[1];
    goto joined_r0x00d5b444;
  }
  if ((puVar14 == puVar9 + 1) || (puVar14[5] == 0)) goto LAB_00d5bfbc;
  puVar9 = DAT_0e53e528;
  if (DAT_0e53e528 < (undefined8 *)0x2) {
    if (DAT_0e53e528 == (undefined8 *)0x0) {
      DAT_0e53e528 = (undefined8 *)0x1;
      puVar9 = DAT_0e53e550;
      if (DAT_0e53e550 < (undefined8 *)0x2) {
        if (DAT_0e53e550 == (undefined8 *)0x0) {
          DAT_0e53e550 = (undefined8 *)0x1;
          DAT_0e53e568 = 0;
          DAT_0e53e560 = 0;
          DAT_0e53e558 = &DAT_0e53e560;
          FUN_0376c4e8(&DAT_0e53e558);
          DAT_0e53e550 = &DAT_0e53e558;
          puVar10 = (undefined4 *)FUN_00008790(0x30);
          *puVar10 = 1;
          *(code **)(puVar10 + 2) = FUN_01673440;
          *(code **)(puVar10 + 4) = FUN_039a57d0;
          *(code **)(puVar10 + 6) = FUN_00b77080;
          *(code **)(puVar10 + 8) = FUN_03c20914;
          if (DAT_0e2f2208 == 0) {
            FUN_0000ffa8(&DAT_0e53e550);
          }
          *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e550;
          FUN_00378000(puVar10);
          puVar9 = &DAT_0e53e558;
        }
        else if (DAT_0e53e550 == (undefined8 *)0x1) {
          lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
          do {
            lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
            uVar20 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
            if (!SBORROW8(lVar12,lVar15)) {
              uVar20 = lVar12 - lVar15;
            }
            if ((long)uVar20 < 1000) {
              FUN_083b146c();
            }
            else {
              FUN_00005dd8(1000);
            }
            puVar9 = DAT_0e53e550;
          } while (DAT_0e53e550 == (undefined8 *)0x1);
        }
      }
      DAT_0e53e538 = 0;
      DAT_0e53e540 = 0;
      DAT_0e53e530 = &DAT_0e53e538;
      if (((ulong)puVar9 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(puVar9);
      }
      DAT_0e53e548 = puVar9;
      puVar9 = (undefined8 *)FUN_00008790(8);
      *puVar9 = &PTR__cxx_destruct_0db2e908;
      FUN_01fb431c(&DAT_0e53e530,5,"geolocation-with-options",0x18,0x300000003,1,1,in_x7,0,0,
                   0x5f00000004,0,puVar9);
      DAT_0e53e528 = &DAT_0e53e530;
      puVar10 = (undefined4 *)FUN_00008790(0x30);
      *puVar10 = 1;
      *(code **)(puVar10 + 2) = FUN_01673440;
      *(code **)(puVar10 + 4) = FUN_039a57d0;
      *(code **)(puVar10 + 6) = FUN_00b77080;
      *(code **)(puVar10 + 8) = FUN_03c208b4;
      if (DAT_0e2f2208 == 0) {
        FUN_0000ffa8(&DAT_0e53e528);
      }
      *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e528;
      FUN_00378000(puVar10);
      puVar9 = &DAT_0e53e530;
    }
    else if (DAT_0e53e528 == (undefined8 *)0x1) {
      lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
      do {
        lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
        uVar20 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
        if (!SBORROW8(lVar12,lVar15)) {
          uVar20 = lVar12 - lVar15;
        }
        if ((long)uVar20 < 1000) {
          FUN_083b146c();
        }
        else {
          FUN_00005dd8(1000);
        }
        puVar9 = DAT_0e53e528;
      } while (DAT_0e53e528 == (undefined8 *)0x1);
    }
  }
  puVar14 = (undefined8 *)puVar9[1];
joined_r0x00d5b49c:
  do {
    if (puVar14 == (undefined8 *)0x0) {
LAB_00d5c0dc:
      if (((DAT_0e457be8 & 1) == 0) && (iVar7 = thunk_FUN_0c44b610(&DAT_0e457be8), iVar7 != 0)) {
        DAT_0e457be0 = FUN_01dc08b8("content_settings-content_type",0x20);
        thunk_FUN_0c44b760(&DAT_0e457be8);
      }
      uVar11 = DAT_0e457be0;
      FUN_01261c00(&local_1b0,param_2);
      pppppppuVar21 = local_1b0;
      if (-1 < (long)local_1a0._7_1_) {
        pppppppuVar21 = &local_1b0;
      }
      lVar15 = lStack_1a8;
      if (-1 < (long)local_1a0) {
        lVar15 = (long)local_1a0._7_1_;
      }
      FUN_01f694e4(uVar11,pppppppuVar21,lVar15);
      if ((long)local_1a0 < 0) {
        FUN_00005810(local_1b0,local_1a0 & 0x7fffffffffffffff);
      }
      if (((DAT_0e457bf8 & 1) == 0) && (iVar7 = thunk_FUN_0c44b610(&DAT_0e457bf8), iVar7 != 0)) {
        DAT_0e457bf0 = FUN_01dc08b8("content_settings-histogram_value",0x20);
        thunk_FUN_0c44b760(&DAT_0e457bf8);
      }
      uVar11 = DAT_0e457bf0;
      thunk_FUN_09e5d718(param_2);
      FUN_01261c00(&local_1b0);
      if (-1 < (long)local_1a0._7_1_) {
        local_1b0 = &local_1b0;
      }
      if (-1 < local_1a0._7_1_) {
        lStack_1a8 = (long)local_1a0._7_1_;
      }
      FUN_01f694e4(uVar11,local_1b0,lStack_1a8);
      thunk_FUN_09c8a2e0(&local_1b0);
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0xd5c1f4);
      (*pcVar4)();
    }
    if (*(int *)(puVar14 + 4) <= iVar7) {
      if (iVar7 <= *(int *)(puVar14 + 4)) {
        if ((puVar14 == puVar9 + 1) || (puVar14[5] == 0)) goto LAB_00d5c0dc;
        plVar22 = *(long **)(param_1 + 0x30);
        while( true ) {
          if (plVar22 == (long *)(param_1 + 0x38)) {
            if (iVar7 != 0xb) {
                    /* WARNING: Does not return */
              pcVar4 = (code *)SoftwareBreakpoint(0,0xd5c668);
              (*pcVar4)();
            }
            auVar25 = ZEXT816(0);
            uVar13 = 0xd;
            goto LAB_00d5ba30;
          }
          if (((int)plVar22[4] != 8) &&
             (plVar8 = (long *)(**(code **)(*(long *)plVar22[5] + 0x10))
                                         ((long *)plVar22[5],param_2,0), plVar8 != (long *)0x0))
          break;
LAB_00d5b53c:
          plVar8 = (long *)plVar22[1];
          plVar19 = plVar22;
          if ((long *)plVar22[1] == (long *)0x0) {
            do {
              plVar22 = (long *)plVar19[2];
              bVar5 = plVar19 != (long *)*plVar22;
              plVar19 = plVar22;
            } while (bVar5);
          }
          else {
            do {
              plVar22 = plVar8;
              plVar8 = (long *)*plVar22;
            } while ((long *)*plVar22 != (long *)0x0);
          }
        }
        lStack_1a8 = 0;
        local_1b0 = (undefined8 *******)0x0;
        local_1a0 = 0;
        local_198 = 1;
        local_190 = 0;
        uStack_188 = 0;
        local_180 = 0;
        local_178 = 1;
        local_170 = 0;
        uStack_168 = 0;
        local_160 = 0;
        local_158 = 1;
        local_150 = 0;
        uStack_148 = 0;
        local_140 = 0;
        local_138 = 1;
        local_130 = 1;
LAB_00d5b628:
        lVar15 = *plVar8;
        if (*(code **)(lVar15 + 0x10) == FUN_0189181c) {
          if ((*(byte *)(plVar8 + 1) & 1) != 0) goto LAB_00d5bef0;
        }
        else {
          iVar6 = (**(code **)(lVar15 + 0x10))(plVar8);
          if (iVar6 == 0) {
LAB_00d5bef0:
            FUN_00587ec0(&local_1b0);
            uVar20 = 0;
            pppppppuVar21 = (undefined8 *******)0x0;
            goto LAB_00d5b9d8;
          }
          lVar15 = *plVar8;
        }
        if (*(code **)(lVar15 + 0x18) == (code *)&LAB_01891f78) {
          if ((char)plVar8[1] != '\0') {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(0,0xd5bf20);
            (*pcVar4)();
          }
          *(undefined1 *)(plVar8 + 1) = 1;
          local_100 = 0;
          uStack_118 = 0;
          local_120 = 0;
          uStack_108 = 0;
          uStack_110 = 0;
          uStack_f8 = 0;
          uStack_f2 = 0;
          uStack_f0 = 0;
          local_ea = 0;
          uStack_e8 = 0;
          puVar9 = (undefined8 *)FUN_00008790(0x170);
          lVar15 = local_a0;
          uVar3 = local_a0._1_7_;
          local_a0 = (ulong)local_a0._1_7_ << 8;
          local_88 = 0xff;
          bVar2 = *(byte *)(plVar8 + 5);
          if (bVar2 == 2) {
            local_a0 = CONCAT44(SUB84(lVar15,4),(int)plVar8[2]);
            local_88 = bVar2;
          }
          else if (bVar2 < 4) {
            local_88 = bVar2;
            if (bVar2 != 0) {
              if (bVar2 == 1) {
                local_a0 = CONCAT71(uVar3,(char)plVar8[2]);
              }
              else {
                local_a0 = plVar8[2];
              }
            }
          }
          else if (bVar2 < 5) {
            lStack_98 = plVar8[3];
            local_a0 = plVar8[2];
            local_90 = plVar8[4];
            plVar8[3] = 0;
            plVar8[4] = 0;
            plVar8[2] = 0;
            local_88 = bVar2;
          }
          else if ((bVar2 < 7) || (bVar2 == 7)) {
            local_a0 = plVar8[2];
            plVar8[2] = 0;
            lStack_98 = plVar8[3];
            plVar8[3] = 0;
            local_90 = plVar8[4];
            plVar8[4] = 0;
            local_88 = bVar2;
          }
          lVar12 = local_90;
          lVar15 = lStack_98;
          uStack_d8 = uStack_118;
          local_e0 = local_120;
          uStack_c8 = uStack_108;
          uStack_d0 = uStack_110;
          local_c0 = CONCAT44(uStack_fc,local_100);
          uStack_b8 = uStack_f8;
          uStack_aa = CONCAT62(uStack_e8,local_ea);
          uStack_b2 = uStack_f2;
          uStack_b0 = uStack_f0;
          puVar9[1] = 0;
          puVar9[2] = 0;
          *puVar9 = 0;
          *(undefined1 *)(puVar9 + 3) = 1;
          puVar9[5] = 0;
          puVar9[6] = 0;
          puVar9[4] = 0;
          *(undefined1 *)(puVar9 + 7) = 1;
          puVar9[9] = 0;
          puVar9[10] = 0;
          puVar9[8] = 0;
          *(undefined1 *)(puVar9 + 0xb) = 1;
          puVar9[0xd] = 0;
          puVar9[0xe] = 0;
          puVar9[0xc] = 0;
          *(undefined1 *)(puVar9 + 0xf) = 1;
          *(undefined1 *)(puVar9 + 0x10) = 1;
          puVar9[0x12] = 0;
          puVar9[0x13] = 0;
          puVar9[0x11] = 0;
          *(undefined1 *)(puVar9 + 0x14) = 1;
          puVar9[0x16] = 0;
          puVar9[0x17] = 0;
          puVar9[0x15] = 0;
          *(undefined1 *)(puVar9 + 0x18) = 1;
          puVar9[0x1a] = 0;
          puVar9[0x1b] = 0;
          puVar9[0x19] = 0;
          *(undefined1 *)(puVar9 + 0x1c) = 1;
          puVar9[0x1d] = 0;
          puVar9[0x1e] = 0;
          puVar9[0x1f] = 0;
          *(undefined1 *)(puVar9 + 0x20) = 1;
          *(undefined1 *)(puVar9 + 0x21) = 1;
          plVar19 = puVar9 + 0x22;
          *(undefined1 *)(puVar9 + 0x22) = 0;
          *(undefined1 *)(puVar9 + 0x25) = 0xff;
          if (bVar2 == 2) {
            *(undefined4 *)plVar19 = (undefined4)local_a0;
LAB_00d5b750:
            *(byte *)(puVar9 + 0x25) = bVar2;
LAB_00d5b754:
            puVar9[0x27] = uStack_118;
            puVar9[0x26] = local_120;
            puVar9[0x29] = uStack_108;
            puVar9[0x28] = uStack_110;
            puVar9[0x2b] = CONCAT26(uStack_f2,uStack_f8);
            puVar9[0x2a] = CONCAT44(uStack_fc,local_100);
          }
          else {
            if (bVar2 < 4) {
              if (bVar2 != 0) {
                if (bVar2 == 1) {
                  *(undefined1 *)plVar19 = (undefined1)local_a0;
                }
                else {
                  *plVar19 = local_a0;
                }
LAB_00d5b830:
                *(byte *)(puVar9 + 0x25) = bVar2;
                puVar9[0x27] = uStack_118;
                puVar9[0x26] = local_120;
                puVar9[0x29] = uStack_108;
                puVar9[0x28] = uStack_110;
                puVar9[0x2b] = CONCAT26(uStack_f2,uStack_f8);
                puVar9[0x2a] = local_c0;
                *(undefined8 *)((long)puVar9 + 0x166) = uStack_aa;
                *(ulong *)((long)puVar9 + 0x15e) = CONCAT62(uStack_f0,uStack_f2);
                (*(code *)(&PTR__cxx_destruct_0df6d258)[bVar2])(auStack_71,&local_a0);
                goto LAB_00d5b770;
              }
              goto LAB_00d5b750;
            }
            if (bVar2 < 5) {
              puVar9[0x23] = lStack_98;
              *plVar19 = local_a0;
              puVar9[0x24] = local_90;
              *(undefined1 *)(puVar9 + 0x25) = 4;
              goto LAB_00d5b754;
            }
            if ((bVar2 < 7) || (bVar2 == 7)) {
              puVar9[0x22] = local_a0;
              local_a0 = 0;
              lStack_98 = 0;
              local_90 = 0;
              puVar9[0x24] = lVar12;
              puVar9[0x23] = lVar15;
              goto LAB_00d5b830;
            }
            puVar9[0x27] = uStack_118;
            puVar9[0x26] = local_120;
            puVar9[0x29] = uStack_108;
            puVar9[0x28] = uStack_110;
            puVar9[0x2b] = CONCAT26(uStack_f2,uStack_f8);
            puVar9[0x2a] = CONCAT44(uStack_fc,local_100);
          }
          *(ulong *)((long)puVar9 + 0x166) = CONCAT62(uStack_e8,local_ea);
          *(ulong *)((long)puVar9 + 0x15e) = CONCAT62(uStack_f0,uStack_f2);
        }
        else {
          puVar9 = (undefined8 *)(**(code **)(lVar15 + 0x18))(plVar8);
        }
LAB_00d5b770:
        iVar6 = FUN_00d5c7c0(puVar9,&local_1b0);
        if ((iVar6 != 0) || (iVar6 = FUN_00d5c7c0(puVar9 + 0x11,&local_1b0), iVar6 != 0)) {
          if (puVar9 != (undefined8 *)0x0) {
            uVar11 = FUN_011953a8(puVar9);
            FUN_00005810(uVar11,0x170);
          }
          goto LAB_00d5b628;
        }
        puVar14 = DAT_0e53e528;
        if (DAT_0e53e528 < (undefined8 *)0x2) {
          if (DAT_0e53e528 == (undefined8 *)0x0) {
            DAT_0e53e528 = (undefined8 *)0x1;
            puVar14 = DAT_0e53e550;
            if (DAT_0e53e550 < (undefined8 *)0x2) {
              if (DAT_0e53e550 == (undefined8 *)0x0) {
                DAT_0e53e550 = (undefined8 *)0x1;
                DAT_0e53e568 = 0;
                DAT_0e53e560 = 0;
                DAT_0e53e558 = &DAT_0e53e560;
                FUN_0376c4e8(&DAT_0e53e558);
                DAT_0e53e550 = &DAT_0e53e558;
                puVar10 = (undefined4 *)FUN_00008790(0x30);
                *puVar10 = 1;
                *(code **)(puVar10 + 2) = FUN_01673440;
                *(code **)(puVar10 + 4) = FUN_039a57d0;
                *(code **)(puVar10 + 6) = FUN_00b77080;
                *(code **)(puVar10 + 8) = FUN_03c20914;
                if (DAT_0e2f2208 == 0) {
                  FUN_0000ffa8(&DAT_0e53e550);
                }
                *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e550;
                FUN_00378000(puVar10);
                puVar14 = &DAT_0e53e558;
              }
              else if (DAT_0e53e550 == (undefined8 *)0x1) {
                lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
                do {
                  lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
                  uVar20 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
                  if (!SBORROW8(lVar12,lVar15)) {
                    uVar20 = lVar12 - lVar15;
                  }
                  if ((long)uVar20 < 1000) {
                    FUN_083b146c();
                  }
                  else {
                    FUN_00005dd8(1000);
                  }
                  puVar14 = DAT_0e53e550;
                } while (DAT_0e53e550 == (undefined8 *)0x1);
              }
            }
            DAT_0e53e540 = 0;
            DAT_0e53e538 = 0;
            DAT_0e53e530 = &DAT_0e53e538;
            if (((ulong)puVar14 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(puVar14);
            }
            DAT_0e53e548 = puVar14;
            puVar14 = (undefined8 *)FUN_00008790(8);
            *puVar14 = &PTR__cxx_destruct_0db2e908;
            FUN_01fb431c(&DAT_0e53e530,5,"geolocation-with-options",0x18,0x300000003,1,1,in_x7,0,0,
                         0x5f00000004,0,puVar14);
            DAT_0e53e528 = &DAT_0e53e530;
            puVar10 = (undefined4 *)FUN_00008790(0x30);
            *puVar10 = 1;
            *(code **)(puVar10 + 2) = FUN_01673440;
            *(code **)(puVar10 + 4) = FUN_039a57d0;
            *(code **)(puVar10 + 6) = FUN_00b77080;
            *(code **)(puVar10 + 8) = FUN_03c208b4;
            if (DAT_0e2f2208 == 0) {
              FUN_0000ffa8(&DAT_0e53e528);
            }
            *(undefined8 ***)(puVar10 + 10) = &DAT_0e53e528;
            FUN_00378000(puVar10);
            puVar14 = &DAT_0e53e530;
          }
          else if (DAT_0e53e528 == (undefined8 *)0x1) {
            lVar15 = (*(code *)PTR_FUN_0e2f1ea8)();
            do {
              lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
              uVar20 = lVar12 - lVar15 >> 0x3f ^ 0x8000000000000000;
              if (!SBORROW8(lVar12,lVar15)) {
                uVar20 = lVar12 - lVar15;
              }
              if ((long)uVar20 < 1000) {
                FUN_083b146c();
              }
              else {
                FUN_00005dd8(1000);
              }
              puVar14 = DAT_0e53e528;
            } while (DAT_0e53e528 == (undefined8 *)0x1);
          }
        }
        puVar16 = (undefined8 *)puVar14[1];
        while (puVar16 != (undefined8 *)0x0) {
          if (iVar7 < *(int *)((long)puVar16 + 0x20)) {
            puVar16 = (undefined8 *)*puVar16;
          }
          else {
            if (iVar7 <= *(int *)((long)puVar16 + 0x20)) {
              if (puVar16 != puVar14 + 1) {
                plVar19 = *(long **)((long)puVar16 + 0x28);
                goto LAB_00d5b8a4;
              }
              break;
            }
            puVar16 = *(undefined8 **)((long)puVar16 + 8);
          }
        }
        plVar19 = (long *)0x0;
LAB_00d5b8a4:
        pcVar4 = *(code **)(*(long *)plVar19[5] + 0x78);
        if (pcVar4 == (code *)&LAB_0458a580) {
          bVar2 = *(byte *)(puVar9 + 0x25);
          uVar20 = (ulong)bVar2;
          if (bVar2 == 2) {
            uVar1 = 0;
            if (*(uint *)(puVar9 + 0x22) < 5) {
              uVar1 = *(uint *)(puVar9 + 0x22);
            }
            uVar17 = 0;
            if (uVar1 != 0) {
              uVar17 = 0x100000000;
            }
            uVar17 = uVar17 | uVar1;
LAB_00d5b8ec:
            uVar20 = uVar17 & 0x100000000;
            uVar17 = (ulong)((uint)uVar17 & -(int)(uVar17 >> 0x20));
          }
          else {
            uVar17 = uVar20;
            if (bVar2 != 0) {
              uVar17 = 0;
              goto LAB_00d5b8ec;
            }
          }
          auVar23._8_8_ = uVar20;
          auVar23._0_8_ = uVar17;
          lVar15 = *plVar19;
          puVar18 = (uint *)(lVar15 + 0x68);
LAB_00d5b900:
          bVar2 = *(byte *)(lVar15 + 0x80);
          auVar24._1_7_ = 0;
          auVar24[0] = bVar2;
          auVar24[8] = bVar2;
          auVar24._9_7_ = 0;
          if (bVar2 == 2) {
            uVar1 = 0;
            if (*puVar18 < 5) {
              uVar1 = *puVar18;
            }
            uVar20 = 0;
            if (uVar1 != 0) {
              uVar20 = 0x100000000;
            }
            uVar20 = uVar20 | uVar1;
          }
          else {
            if (bVar2 == 0) goto LAB_00d5b934;
            uVar20 = 0;
          }
          auVar24._4_4_ = 0;
          auVar24._0_4_ = (uint)uVar20 & -(int)(uVar20 >> 0x20);
          auVar24._8_8_ = uVar20 & 0x100000000;
        }
        else {
          auVar23 = (*pcVar4)((long *)plVar19[5],puVar9 + 0x22);
          pcVar4 = *(code **)(*(long *)plVar19[5] + 0x78);
          lVar15 = *plVar19;
          puVar18 = (uint *)(lVar15 + 0x68);
          if (pcVar4 == (code *)&LAB_0458a580) goto LAB_00d5b900;
          auVar24 = (*pcVar4)((long *)plVar19[5],puVar18);
        }
LAB_00d5b934:
        if ((auVar24._8_8_ >> 0x20 & 1) == 0) {
                    /* WARNING: Does not return */
          pcVar4 = (code *)SoftwareBreakpoint(1,0xd5bf28);
          (*pcVar4)();
        }
        if ((auVar23._8_8_ & 0x100000000) != 0) {
          auVar24 = auVar23;
        }
        pppppppuVar21 = auVar24._0_8_;
        if (puVar9 != (undefined8 *)0x0) {
          bVar2 = *(byte *)(puVar9 + 0x25);
          if ((bVar2 != 0xff) && ((bVar2 & 0xfd) != 0)) {
            if (bVar2 == 4) {
              if (*(char *)((long)puVar9 + 0x127) < '\0') {
                FUN_00005810(puVar9[0x22],puVar9[0x24] & 0x7fffffffffffffff);
              }
            }
            else {
              (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)bVar2])(&local_e0,puVar9 + 0x22);
            }
          }
          *(undefined1 *)(puVar9 + 0x25) = 0xff;
          if (*(char *)((long)puVar9 + 0xff) < '\0') {
            FUN_00005810(puVar9[0x1d],puVar9[0x1f] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0xdf) < '\0') {
            FUN_00005810(puVar9[0x19],puVar9[0x1b] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0xbf) < '\0') {
            FUN_00005810(puVar9[0x15],puVar9[0x17] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0x9f) < '\0') {
            FUN_00005810(puVar9[0x11],puVar9[0x13] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0x77) < '\0') {
            FUN_00005810(puVar9[0xc],puVar9[0xe] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0x57) < '\0') {
            FUN_00005810(puVar9[8],puVar9[10] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0x37) < '\0') {
            FUN_00005810(puVar9[4],puVar9[6] & 0x7fffffffffffffff);
          }
          if (*(char *)((long)puVar9 + 0x17) < '\0') {
            FUN_00005810(*puVar9,puVar9[2] & 0x7fffffffffffffff);
          }
          FUN_00005810(puVar9,0x170);
        }
        if ((long)local_140 < 0) {
          FUN_00005810(local_150,local_140 & 0x7fffffffffffffff);
        }
        if ((long)local_160 < 0) {
          FUN_00005810(local_170,local_160 & 0x7fffffffffffffff);
        }
        if ((long)local_180 < 0) {
          FUN_00005810(local_190,local_180 & 0x7fffffffffffffff);
        }
        if ((long)local_1a0 < 0) {
          FUN_00005810(local_1b0,local_1a0 & 0x7fffffffffffffff);
        }
        uVar20 = auVar24._8_8_ | 0x100000000;
LAB_00d5b9d8:
        auVar25._8_8_ = uVar20;
        auVar25._0_8_ = pppppppuVar21;
        if (*(code **)(*plVar8 + 8) == FUN_0189182c) {
          *plVar8 = (long)&PTR_FUN_0db2e998;
          bVar2 = *(byte *)(plVar8 + 5);
          if ((bVar2 != 0xff) && ((bVar2 & 0xfd) != 0)) {
            if (bVar2 == 4) {
              if (*(char *)((long)plVar8 + 0x27) < '\0') {
                FUN_00005810(plVar8[2],plVar8[4] & 0x7fffffffffffffff);
              }
            }
            else {
              (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)bVar2])(&local_1b0,plVar8 + 2);
            }
          }
          FUN_00005810(plVar8,0x30);
        }
        else {
          (**(code **)(*plVar8 + 8))(plVar8);
        }
        if ((*(char *)(param_1 + 0x28) == '\0') || ((uVar20 >> 0x20 & 1) == 0)) {
          if ((uVar20 >> 0x20 & 1) == 0) goto LAB_00d5b53c;
        }
        else {
          lStack_1a8 = CONCAT44(lStack_1a8._4_4_,(int)uVar20);
          local_1b0 = pppppppuVar21;
          uVar11 = FUN_011951c0();
          lVar15 = FUN_025c8dc8(uVar11,param_2);
          auVar25 = (**(code **)(**(long **)(lVar15 + 0x28) + 0x20))
                              (*(long **)(lVar15 + 0x28),&local_1b0);
        }
        uVar13 = (undefined4)plVar22[4];
LAB_00d5ba30:
        if (param_3 != (undefined4 *)0x0) {
          *param_3 = uVar13;
        }
        if ((auVar25._8_8_ & 0xff) == 0) {
          return auVar25._0_8_;
        }
        if ((auVar25._8_4_ & 0xff) == 0xff) {
                    /* WARNING: Subroutine does not return */
          FUN_04b335b8("std::get: variant is valueless");
        }
                    /* WARNING: Subroutine does not return */
        FUN_04b335b8("std::get: wrong alternative for variant");
      }
      puVar14 = (undefined8 *)puVar14[1];
      goto joined_r0x00d5b49c;
    }
    puVar14 = (undefined8 *)*puVar14;
  } while( true );
}


