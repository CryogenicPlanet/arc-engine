// ==== 0x42bdf88  FUN_042bdf88  (2076 bytes)

void FUN_042bdf88(long param_1,long *param_2,int *param_3,undefined8 *param_4)

{
  ulong uVar1;
  undefined8 uVar2;
  ulong *puVar3;
  ulong uVar4;
  byte bVar5;
  long lVar6;
  code *pcVar7;
  ulong uVar8;
  long lVar9;
  undefined8 *puVar10;
  ulong uVar11;
  long *plVar12;
  long *plVar13;
  long *plVar14;
  long *plVar15;
  ulong *puVar16;
  long lVar17;
  ulong uVar18;
  bool bVar19;
  ulong uVar20;
  float fVar21;
  byte bVar22;
  byte bVar23;
  byte bVar24;
  byte bVar25;
  byte bVar26;
  byte bVar27;
  byte bVar28;
  ulong local_b0 [2];
  long *local_a0;
  ulong local_90 [2];
  long *local_80;
  int local_6c;
  ulong *local_68;
  ulong *local_60;
  ulong **ppuStack_58;
  
  local_6c = *param_3;
  if (local_6c < 1) {
    local_6c = -1;
  }
  puVar10 = (undefined8 *)(param_1 + 8);
  if (*(char *)(param_1 + 0x28) == '\0') {
    if (puVar10 == (undefined8 *)0x0) goto LAB_042be754;
    uVar2 = *param_4;
    *(undefined8 *)(param_1 + 0x10) = param_4[1];
    *puVar10 = uVar2;
    *(undefined8 *)(param_1 + 0x18) = param_4[2];
    lVar6 = param_4[3];
    *(long *)(param_1 + 0x20) = lVar6;
    if (*(long *)(lVar6 + 8) != 0) {
      param_4[2] = 0;
      param_4[3] = &DAT_0c4b3500;
    }
    *(undefined1 *)(param_1 + 0x28) = 1;
  }
  else {
    lVar6 = *(long *)(param_1 + 0x20);
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 **)(param_1 + 0x20) = &DAT_0c4b3500;
    pcVar7 = *(code **)(lVar6 + 8);
    if (pcVar7 != (code *)0x0) {
      (*pcVar7)(*puVar10);
    }
    uVar2 = *param_4;
    *(undefined8 *)(param_1 + 0x10) = param_4[1];
    *puVar10 = uVar2;
    *(undefined8 *)(param_1 + 0x18) = param_4[2];
    *(undefined8 *)(param_1 + 0x20) = param_4[3];
    param_4[2] = 0;
    param_4[3] = &DAT_0c4b3500;
  }
  lVar6 = *(long *)(*param_2 + 0x88);
  if (lVar6 == 0) {
    if ((*(byte *)(param_1 + 0x28) & 1) == 0) goto LAB_042be754;
    local_90[0] = FUN_04b37104(0x39,"Cannot restore browser with given browser context",0x31,0x775,
                               "../../arc/browser/browser_application_impl.cc");
    if (local_90[0] == 1) {
      FUN_04b37cec(local_90);
    }
    (**(code **)(param_1 + 0x18))(param_1 + 8,local_90);
    uVar4 = local_90[0];
    plVar15 = local_80;
  }
  else {
    uVar2 = FUN_02302728();
    puVar3 = (ulong *)FUN_0009278c(uVar2,lVar6,1);
    if (puVar3 != (ulong *)0x0) {
      local_60 = puVar3;
      uVar4 = FUN_00018f3c(&local_60,8);
      uVar20 = *(ulong *)(param_1 + 0x60);
      if (uVar20 == 0) {
        uVar18 = 0;
      }
      else {
        uVar8 = uVar20 - 1;
        if ((uVar20 & uVar8) == 0) {
          uVar18 = uVar8 & uVar4;
        }
        else {
          uVar18 = uVar4;
          if (uVar20 <= uVar4) {
            uVar18 = 0;
            if (uVar20 != 0) {
              uVar18 = uVar4 / uVar20;
            }
            uVar18 = uVar4 - uVar18 * uVar20;
          }
        }
        puVar10 = *(undefined8 **)(*(long *)(param_1 + 0x58) + uVar18 * 8);
        if (puVar10 != (undefined8 *)0x0) {
          for (plVar15 = (long *)*puVar10; plVar15 != (long *)0x0; plVar15 = (long *)*plVar15) {
            uVar11 = plVar15[1];
            if (uVar11 == uVar4) {
              if ((ulong *)plVar15[2] == puVar3) goto LAB_042be554;
            }
            else {
              if ((uVar20 & uVar8) == 0) {
                uVar11 = uVar11 & uVar8;
              }
              else if (uVar20 <= uVar11) {
                uVar1 = 0;
                if (uVar20 != 0) {
                  uVar1 = uVar11 / uVar20;
                }
                uVar11 = uVar11 - uVar1 * uVar20;
              }
              if (uVar11 != uVar18) break;
            }
          }
        }
      }
      plVar15 = (long *)FUN_00008790(0x30);
      if (plVar15 == (long *)0x0) goto LAB_042be754;
      *plVar15 = 0;
      plVar15[1] = uVar4;
      if (plVar15 == (long *)0xfffffffffffffff0) goto LAB_042be754;
      plVar15[2] = (long)puVar3;
      plVar15[3] = 0;
      fVar21 = (float)(*(long *)(param_1 + 0x70) + 1);
      plVar15[4] = 0;
      plVar15[5] = 0;
      if (*(float *)(param_1 + 0x78) * (float)uVar20 < fVar21) {
        uVar18 = (ulong)((uVar20 & uVar20 - 1) != 0);
        if (uVar20 < 3) {
          uVar18 = 1;
        }
        uVar18 = uVar18 | uVar20 << 1;
        uVar8 = (ulong)(fVar21 / *(float *)(param_1 + 0x78));
        if (uVar18 <= uVar8) {
          uVar18 = uVar8;
        }
        if (uVar18 - 1 == 0) {
          uVar18 = 2;
        }
        else if ((uVar18 & uVar18 - 1) != 0) {
          uVar18 = FUN_00f76630(uVar18);
          uVar20 = *(ulong *)(param_1 + 0x60);
        }
        if (uVar20 < uVar18) {
LAB_042be41c:
          if (uVar18 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          uVar2 = FUN_00008790(uVar18 << 3);
          bVar19 = false;
LAB_042be430:
          lVar6 = *(long *)(param_1 + 0x58);
          *(undefined8 *)(param_1 + 0x58) = uVar2;
          if (lVar6 != 0) {
            FUN_00005810(lVar6,*(long *)(param_1 + 0x60) << 3);
          }
          *(ulong *)(param_1 + 0x60) = uVar18;
          if (!bVar19) {
            uVar20 = 0;
            do {
              *(undefined8 *)(*(long *)(param_1 + 0x58) + uVar20 * 8) = 0;
              uVar20 = uVar20 + 1;
            } while (uVar18 != uVar20);
            plVar12 = *(long **)(param_1 + 0x68);
            if (plVar12 != (long *)0x0) {
              uVar20 = plVar12[1];
              uVar8 = uVar18 - 1;
              if ((uVar18 & uVar8) == 0) {
                uVar20 = uVar20 & uVar8;
              }
              else if (uVar18 <= uVar20) {
                uVar11 = 0;
                if (uVar18 != 0) {
                  uVar11 = uVar20 / uVar18;
                }
                uVar20 = uVar20 - uVar11 * uVar18;
              }
              *(undefined8 **)(*(long *)(param_1 + 0x58) + uVar20 * 8) =
                   (undefined8 *)(param_1 + 0x68);
              plVar13 = (long *)*plVar12;
              while (plVar13 != (long *)0x0) {
                uVar11 = plVar13[1];
                if ((uVar18 & uVar8) == 0) {
                  uVar11 = uVar11 & uVar8;
                }
                else if (uVar18 <= uVar11) {
                  uVar1 = 0;
                  if (uVar18 != 0) {
                    uVar1 = uVar11 / uVar18;
                  }
                  uVar11 = uVar11 - uVar1 * uVar18;
                }
                plVar14 = plVar13;
                if (uVar11 != uVar20) {
                  if (*(long *)(*(long *)(param_1 + 0x58) + uVar11 * 8) == 0) {
                    *(long **)(*(long *)(param_1 + 0x58) + uVar11 * 8) = plVar12;
                    uVar20 = uVar11;
                  }
                  else {
                    *plVar12 = *plVar13;
                    *plVar13 = **(undefined8 **)(*(long *)(param_1 + 0x58) + uVar11 * 8);
                    **(long **)(*(long *)(param_1 + 0x58) + uVar11 * 8) = (long)plVar13;
                    plVar14 = plVar12;
                  }
                }
                plVar12 = plVar14;
                plVar13 = (long *)*plVar14;
              }
            }
          }
        }
        else if (uVar18 < uVar20) {
          uVar8 = (ulong)((float)*(ulong *)(param_1 + 0x70) / *(float *)(param_1 + 0x78));
          if ((uVar20 < 3) || ((uVar20 & uVar20 - 1) != 0)) {
            uVar8 = FUN_00f76630();
          }
          else if (1 < uVar8) {
            uVar8 = 1L << (-LZCOUNT(uVar8 - 1) & 0x3fU);
          }
          if (uVar18 <= uVar8) {
            uVar18 = uVar8;
          }
          if (uVar18 < uVar20) {
            if (uVar18 != 0) goto LAB_042be41c;
            uVar2 = 0;
            bVar19 = true;
            goto LAB_042be430;
          }
        }
        uVar20 = *(ulong *)(param_1 + 0x60);
        if ((uVar20 & uVar20 - 1) == 0) {
          uVar18 = uVar20 - 1 & uVar4;
        }
        else {
          uVar18 = uVar4;
          if (uVar20 <= uVar4) {
            uVar18 = 0;
            if (uVar20 != 0) {
              uVar18 = uVar4 / uVar20;
            }
            uVar18 = uVar4 - uVar18 * uVar20;
          }
        }
      }
      lVar6 = *(long *)(param_1 + 0x58);
      plVar12 = *(long **)(lVar6 + uVar18 * 8);
      if (plVar12 == (long *)0x0) {
        plVar12 = (long *)(param_1 + 0x68);
        *plVar15 = *plVar12;
        *plVar12 = (long)plVar15;
        *(long **)(lVar6 + uVar18 * 8) = plVar12;
        if (*plVar15 != 0) {
          uVar4 = *(ulong *)(*plVar15 + 8);
          if ((uVar20 & uVar20 - 1) == 0) {
            uVar4 = uVar4 & uVar20 - 1;
          }
          else if (uVar20 <= uVar4) {
            uVar18 = 0;
            if (uVar20 != 0) {
              uVar18 = uVar4 / uVar20;
            }
            uVar4 = uVar4 - uVar18 * uVar20;
          }
          *(long **)(*(long *)(param_1 + 0x58) + uVar4 * 8) = plVar15;
        }
      }
      else {
        *plVar15 = *plVar12;
        *plVar12 = (long)plVar15;
      }
      *(long *)(param_1 + 0x70) = *(long *)(param_1 + 0x70) + 1;
LAB_042be554:
      FUN_04bafb00(plVar15 + 3,&local_6c);
      if ((puVar3[2] == 0) || ((*(byte *)(puVar3[2] + 0x30) & 5) == 0)) {
        FUN_042bee6c(param_1,puVar3);
        return;
      }
      FUN_0328b690();
      puVar16 = (ulong *)(param_1 + 0x88);
      uVar4 = *puVar16;
      local_68 = puVar3;
      if ((uVar4 & 0x3e) == 0) {
        puVar10 = (undefined8 *)(param_1 + 0x90);
        if (uVar4 < 0x800000) {
          *puVar16 = uVar4 | 0x800040;
        }
        else {
          if ((ulong *)*puVar10 == puVar3) {
LAB_042be7a0:
                    /* WARNING: Does not return */
            pcVar7 = (code *)SoftwareBreakpoint(0,0x42be7a4);
            (*pcVar7)();
          }
          ppuStack_58 = &local_68;
          local_60 = puVar16;
          puVar10 = (undefined8 *)FUN_000f79a0(puVar16,&DAT_0df6a7a8,&local_60,FUN_03a728b4,0);
        }
      }
      else {
        lVar17 = *(long *)(param_1 + 0x90);
        uVar8 = -1L << (uVar4 & 0x3f);
        lVar6 = (lVar17 - uVar8) + 7;
        Hint_Prefetch(lVar17,0,2,0);
        uVar20 = uVar4 >> 6 & 0x1f;
        local_60 = puVar3;
        uVar18 = FUN_00018f3c(&local_60,8);
        lVar9 = 0;
        uVar4 = uVar18 ^ uVar20;
        bVar5 = (byte)(uVar18 >> 0x39);
        while( true ) {
          uVar4 = uVar4 & (uVar8 ^ 0xffffffffffffffff);
          Hint_Prefetch(lVar6 + uVar4 * 8,0,0,0);
          uVar2 = *(undefined8 *)(lVar17 + uVar4);
          bVar22 = (byte)((ulong)uVar2 >> 8);
          bVar23 = (byte)((ulong)uVar2 >> 0x10);
          bVar24 = (byte)((ulong)uVar2 >> 0x18);
          bVar25 = (byte)((ulong)uVar2 >> 0x20);
          bVar26 = (byte)((ulong)uVar2 >> 0x28);
          bVar27 = (byte)((ulong)uVar2 >> 0x30);
          bVar28 = (byte)((ulong)uVar2 >> 0x38);
          for (uVar18 = CONCAT17(-(bVar28 == bVar5),
                                 CONCAT16(-(bVar27 == bVar5),
                                          CONCAT15(-(bVar26 == bVar5),
                                                   CONCAT14(-(bVar25 == bVar5),
                                                            CONCAT13(-(bVar24 == bVar5),
                                                                     CONCAT12(-(bVar23 == bVar5),
                                                                              CONCAT11(-(bVar22 ==
                                                                                        bVar5),-((
                                                  byte)uVar2 == bVar5)))))))); uVar18 != 0;
              uVar18 = (uVar18 & 0x8080808080808080) - 0x80 & uVar18 & 0x8080808080808080) {
            uVar11 = (uVar18 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar18 & 0x5555555555555555) << 1;
            uVar11 = (uVar11 & 0xcccccccccccccccc) >> 2 | (uVar11 & 0x3333333333333333) << 2;
            uVar11 = (uVar11 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar11 & 0xf0f0f0f0f0f0f0f) << 4;
            uVar11 = (uVar11 & 0xff00ff00ff00ff00) >> 8 | (uVar11 & 0xff00ff00ff00ff) << 8;
            uVar11 = (uVar11 & 0xffff0000ffff0000) >> 0x10 | (uVar11 & 0xffff0000ffff) << 0x10;
            if (*(ulong **)
                 (lVar6 + (uVar4 + ((ulong)LZCOUNT(uVar11 >> 0x20 | uVar11 << 0x20) >> 3) &
                          (uVar8 ^ 0xffffffffffffffff)) * 8) == puVar3) goto LAB_042be7a0;
          }
          if (CONCAT17(-(bVar28 == 0x80),
                       CONCAT16(-(bVar27 == 0x80),
                                CONCAT15(-(bVar26 == 0x80),
                                         CONCAT14(-(bVar25 == 0x80),
                                                  CONCAT13(-(bVar24 == 0x80),
                                                           CONCAT12(-(bVar23 == 0x80),
                                                                    CONCAT11(-(bVar22 == 0x80),
                                                                             -((byte)uVar2 == 0x80))
                                                                   )))))) != 0) break;
          lVar9 = lVar9 + 8;
          uVar4 = lVar9 + uVar4;
        }
        Hint_Prefetch(lVar17,0,2,0);
        local_60 = puVar3;
        uVar4 = FUN_00018f3c(&local_60,8);
        lVar9 = 0;
        uVar20 = uVar4 ^ uVar20;
        bVar5 = (byte)(uVar4 >> 0x39);
        while( true ) {
          uVar20 = uVar20 & (uVar8 ^ 0xffffffffffffffff);
          Hint_Prefetch(lVar6 + uVar20 * 8,0,0,0);
          uVar2 = *(undefined8 *)(lVar17 + uVar20);
          bVar22 = (byte)((ulong)uVar2 >> 8);
          bVar23 = (byte)((ulong)uVar2 >> 0x10);
          bVar24 = (byte)((ulong)uVar2 >> 0x18);
          bVar25 = (byte)((ulong)uVar2 >> 0x20);
          bVar26 = (byte)((ulong)uVar2 >> 0x28);
          bVar27 = (byte)((ulong)uVar2 >> 0x30);
          bVar28 = (byte)((ulong)uVar2 >> 0x38);
          for (uVar4 = CONCAT17(-(bVar28 == bVar5),
                                CONCAT16(-(bVar27 == bVar5),
                                         CONCAT15(-(bVar26 == bVar5),
                                                  CONCAT14(-(bVar25 == bVar5),
                                                           CONCAT13(-(bVar24 == bVar5),
                                                                    CONCAT12(-(bVar23 == bVar5),
                                                                             CONCAT11(-(bVar22 ==
                                                                                       bVar5),-((
                                                  byte)uVar2 == bVar5)))))))); uVar4 != 0;
              uVar4 = (uVar4 & 0x8080808080808080) - 0x80 & uVar4 & 0x8080808080808080) {
            uVar18 = (uVar4 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar4 & 0x5555555555555555) << 1;
            uVar18 = (uVar18 & 0xcccccccccccccccc) >> 2 | (uVar18 & 0x3333333333333333) << 2;
            uVar18 = (uVar18 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar18 & 0xf0f0f0f0f0f0f0f) << 4;
            uVar18 = (uVar18 & 0xff00ff00ff00ff00) >> 8 | (uVar18 & 0xff00ff00ff00ff) << 8;
            uVar18 = (uVar18 & 0xffff0000ffff0000) >> 0x10 | (uVar18 & 0xffff0000ffff) << 0x10;
            if (*(ulong **)
                 (lVar6 + (uVar20 + ((ulong)LZCOUNT(uVar18 >> 0x20 | uVar18 << 0x20) >> 3) &
                          (uVar8 ^ 0xffffffffffffffff)) * 8) == puVar3) goto LAB_042be220;
          }
          if (CONCAT17(-(bVar28 == 0x80),
                       CONCAT16(-(bVar27 == 0x80),
                                CONCAT15(-(bVar26 == 0x80),
                                         CONCAT14(-(bVar25 == 0x80),
                                                  CONCAT13(-(bVar24 == 0x80),
                                                           CONCAT12(-(bVar23 == 0x80),
                                                                    CONCAT11(-(bVar22 == 0x80),
                                                                             -((byte)uVar2 == 0x80))
                                                                   )))))) != 0) break;
          lVar9 = lVar9 + 8;
          uVar20 = lVar9 + uVar20;
        }
        puVar10 = (undefined8 *)FUN_000f6c60(puVar16,&DAT_0df6a7a8);
      }
      puVar3 = local_68;
      if (puVar10 != (undefined8 *)0x0) {
        if (((ulong)local_68 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0000ffa8(local_68);
        }
        *puVar10 = puVar3;
        puVar3 = local_68;
LAB_042be220:
        FUN_008ef5e0(puVar3 + 0xb,*(undefined8 *)(param_1 + 0x80));
        return;
      }
LAB_042be754:
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(1,0x42be758);
      (*pcVar7)();
    }
    if ((*(byte *)(param_1 + 0x28) & 1) == 0) goto LAB_042be754;
    local_b0[0] = FUN_04b37104(0x39,"Cannot restore browser with given browser context",0x31,0x77f,
                               "../../arc/browser/browser_application_impl.cc");
    if (local_b0[0] == 1) {
      FUN_04b37cec(local_b0);
    }
    (**(code **)(param_1 + 0x18))(param_1 + 8,local_b0);
    uVar4 = local_b0[0];
    plVar15 = local_a0;
  }
  if (uVar4 == 1) {
    if (plVar15 != (long *)0x0) {
      LOAcquire();
      lVar6 = plVar15[1];
      plVar15[1] = lVar6 + -1;
      LORelease();
      if (lVar6 == 0) {
        (**(code **)(*plVar15 + 0x10))(plVar15);
        if (plVar15[2] != 0) {
          LOAcquire();
          lVar6 = plVar15[2];
          plVar15[2] = lVar6 + -1;
          LORelease();
          if (lVar6 != 0) goto LAB_042be188;
        }
        pcVar7 = *(code **)(*plVar15 + 0x20);
        if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
          FUN_00005810(plVar15,0x20);
        }
        else {
          (*pcVar7)(plVar15);
        }
      }
    }
  }
  else if ((uVar4 & 1) == 0) {
    FUN_04b35898();
  }
LAB_042be188:
  if (*(char *)(param_1 + 0x28) != '\0') {
    pcVar7 = *(code **)(*(long *)(param_1 + 0x20) + 8);
    if (pcVar7 != (code *)0x0) {
      (*pcVar7)(*puVar10);
    }
    *(undefined1 *)(param_1 + 0x28) = 0;
  }
  return;
}


