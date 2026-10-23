// ==== 0x435dd94  FUN_0435dd94  (2084 bytes)

int * FUN_0435dd94(ulong param_1,undefined8 *param_2,long param_3,int *param_4)

{
  uint uVar1;
  undefined1 **ppuVar2;
  ulong *puVar3;
  long lVar4;
  undefined4 uVar5;
  ulong uVar6;
  code *pcVar7;
  int iVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  undefined4 *puVar11;
  ulong uVar12;
  long lVar13;
  long lVar14;
  ulong *puVar15;
  ulong *puVar16;
  long *plVar17;
  long *plVar18;
  ulong uVar19;
  long *plVar20;
  long *plVar21;
  int *piVar22;
  ulong uVar23;
  ulong uVar24;
  bool bVar25;
  ulong uVar26;
  float fVar27;
  undefined1 *local_e0;
  long lStack_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 *local_80 [2];
  undefined1 local_70;
  byte local_68;
  
  if (*(long *)(param_1 + 8) == 0) {
    if (param_4 == (int *)0x0) goto LAB_0435de54;
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
    (**(code **)(param_4 + 2))(param_4,&local_98);
    LOAcquire();
    iVar8 = *param_4;
    *param_4 = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(param_4 + 4))(param_4);
    }
    puVar9 = &local_98;
LAB_0435de4c:
    FUN_005a58dc(puVar9);
  }
  else {
    if (*(char *)(param_1 + 0x44) != '\0') {
      if (param_4 == (int *)0x0) goto LAB_0435de54;
      local_b0 = 0;
      uStack_a8 = 0;
      local_a0 = 0;
      (**(code **)(param_4 + 2))(param_4,&local_b0);
      LOAcquire();
      iVar8 = *param_4;
      *param_4 = iVar8 + -1;
      LORelease();
      if (iVar8 == 1) {
        (**(code **)(param_4 + 4))(param_4);
      }
      puVar9 = &local_b0;
      goto LAB_0435de4c;
    }
    if (*(long *)(param_1 + 0x48) != 0) {
LAB_0435de98:
      local_c8 = 0;
      uStack_c0 = 0;
      local_b8 = 0;
      uVar1 = DAT_0e3bae54 + 1;
      DAT_0e3bae54 = uVar1;
      if (param_4 != (int *)0x0) {
        uVar24 = (ulong)(int)uVar1;
        uVar26 = *(ulong *)(param_1 + 0x18);
        if (uVar26 == 0) {
          uVar23 = 0;
        }
        else {
          uVar12 = uVar26 - 1;
          if ((uVar26 & uVar12) == 0) {
            uVar23 = uVar12 & uVar24;
          }
          else {
            uVar23 = uVar24;
            if (uVar26 <= uVar24) {
              uVar23 = 0;
              if (uVar26 != 0) {
                uVar23 = uVar24 / uVar26;
              }
              uVar23 = uVar24 - uVar23 * uVar26;
            }
          }
          plVar17 = *(long **)(*(long *)(param_1 + 0x10) + uVar23 * 8);
          if (plVar17 != (long *)0x0) {
            do {
              while( true ) {
                plVar17 = (long *)*plVar17;
                if (plVar17 == (long *)0x0) goto LAB_0435dffc;
                uVar19 = plVar17[1];
                if (uVar19 == uVar24) break;
                if ((uVar26 & uVar12) == 0) {
                  uVar19 = uVar19 & uVar12;
                }
                else if (uVar26 <= uVar19) {
                  uVar6 = 0;
                  if (uVar26 != 0) {
                    uVar6 = uVar19 / uVar26;
                  }
                  uVar19 = uVar19 - uVar6 * uVar26;
                }
                if (uVar19 != uVar23) goto LAB_0435dffc;
              }
            } while (*(uint *)(plVar17 + 2) != uVar1);
            LOAcquire();
            iVar8 = *param_4;
            *param_4 = iVar8 + -1;
            LORelease();
            if (iVar8 == 1) {
              (**(code **)(param_4 + 4))(param_4);
            }
            goto LAB_0435e288;
          }
        }
LAB_0435dffc:
        plVar17 = (long *)FUN_00008790(0x20);
        if (plVar17 == (long *)0x0) {
LAB_0435e560:
                    /* WARNING: Does not return */
          pcVar7 = (code *)SoftwareBreakpoint(1,0x435e564);
          (*pcVar7)();
        }
        *plVar17 = 0;
        plVar17[1] = uVar24;
        if (plVar17 == (long *)0xfffffffffffffff0) goto LAB_0435e560;
        *(uint *)(plVar17 + 2) = uVar1;
        fVar27 = (float)(*(long *)(param_1 + 0x28) + 1);
        plVar17[3] = (long)param_4;
        if (*(float *)(param_1 + 0x30) * (float)uVar26 < fVar27) {
          uVar23 = (ulong)((uVar26 & uVar26 - 1) != 0);
          if (uVar26 < 3) {
            uVar23 = 1;
          }
          uVar23 = uVar23 | uVar26 << 1;
          uVar12 = (ulong)(fVar27 / *(float *)(param_1 + 0x30));
          if (uVar23 <= uVar12) {
            uVar23 = uVar12;
          }
          if (uVar23 - 1 == 0) {
            uVar23 = 2;
          }
          else if ((uVar23 & uVar23 - 1) != 0) {
            uVar23 = FUN_00f76630(uVar23);
            uVar26 = *(ulong *)(param_1 + 0x18);
          }
          if (uVar26 < uVar23) {
LAB_0435e10c:
            if (uVar23 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            uVar10 = FUN_00008790(uVar23 << 3);
            bVar25 = false;
LAB_0435e120:
            lVar13 = *(long *)(param_1 + 0x10);
            *(undefined8 *)(param_1 + 0x10) = uVar10;
            if (lVar13 != 0) {
              FUN_00005810(lVar13,*(long *)(param_1 + 0x18) << 3);
            }
            *(ulong *)(param_1 + 0x18) = uVar23;
            if (!bVar25) {
              uVar26 = 0;
              do {
                *(undefined8 *)(*(long *)(param_1 + 0x10) + uVar26 * 8) = 0;
                uVar26 = uVar26 + 1;
              } while (uVar23 != uVar26);
              plVar18 = *(long **)(param_1 + 0x20);
              if (plVar18 != (long *)0x0) {
                uVar26 = plVar18[1];
                uVar12 = uVar23 - 1;
                if ((uVar23 & uVar12) == 0) {
                  uVar26 = uVar26 & uVar12;
                }
                else if (uVar23 <= uVar26) {
                  uVar19 = 0;
                  if (uVar23 != 0) {
                    uVar19 = uVar26 / uVar23;
                  }
                  uVar26 = uVar26 - uVar19 * uVar23;
                }
                *(undefined8 **)(*(long *)(param_1 + 0x10) + uVar26 * 8) =
                     (undefined8 *)(param_1 + 0x20);
                plVar20 = (long *)*plVar18;
                while (plVar20 != (long *)0x0) {
                  uVar19 = plVar20[1];
                  if ((uVar23 & uVar12) == 0) {
                    uVar19 = uVar19 & uVar12;
                  }
                  else if (uVar23 <= uVar19) {
                    uVar6 = 0;
                    if (uVar23 != 0) {
                      uVar6 = uVar19 / uVar23;
                    }
                    uVar19 = uVar19 - uVar6 * uVar23;
                  }
                  plVar21 = plVar20;
                  if (uVar19 != uVar26) {
                    if (*(long *)(*(long *)(param_1 + 0x10) + uVar19 * 8) == 0) {
                      *(long **)(*(long *)(param_1 + 0x10) + uVar19 * 8) = plVar18;
                      uVar26 = uVar19;
                    }
                    else {
                      *plVar18 = *plVar20;
                      *plVar20 = **(undefined8 **)(*(long *)(param_1 + 0x10) + uVar19 * 8);
                      **(long **)(*(long *)(param_1 + 0x10) + uVar19 * 8) = (long)plVar20;
                      plVar21 = plVar18;
                    }
                  }
                  plVar18 = plVar21;
                  plVar20 = (long *)*plVar21;
                }
              }
            }
          }
          else if (uVar23 < uVar26) {
            uVar12 = (ulong)((float)*(ulong *)(param_1 + 0x28) / *(float *)(param_1 + 0x30));
            if ((uVar26 < 3) || ((uVar26 & uVar26 - 1) != 0)) {
              uVar12 = FUN_00f76630();
            }
            else if (1 < uVar12) {
              uVar12 = 1L << (-LZCOUNT(uVar12 - 1) & 0x3fU);
            }
            if (uVar23 <= uVar12) {
              uVar23 = uVar12;
            }
            if (uVar23 < uVar26) {
              if (uVar23 != 0) goto LAB_0435e10c;
              uVar10 = 0;
              bVar25 = true;
              goto LAB_0435e120;
            }
          }
          uVar26 = *(ulong *)(param_1 + 0x18);
          if ((uVar26 & uVar26 - 1) == 0) {
            uVar23 = uVar26 - 1 & uVar24;
          }
          else {
            uVar23 = uVar24;
            if (uVar26 <= uVar24) {
              uVar23 = 0;
              if (uVar26 != 0) {
                uVar23 = uVar24 / uVar26;
              }
              uVar23 = uVar24 - uVar23 * uVar26;
            }
          }
        }
        lVar13 = *(long *)(param_1 + 0x10);
        plVar18 = *(long **)(lVar13 + uVar23 * 8);
        if (plVar18 == (long *)0x0) {
          plVar18 = (long *)(param_1 + 0x20);
          *plVar17 = *plVar18;
          *plVar18 = (long)plVar17;
          *(long **)(lVar13 + uVar23 * 8) = plVar18;
          if (*plVar17 != 0) {
            uVar24 = *(ulong *)(*plVar17 + 8);
            if ((uVar26 & uVar26 - 1) == 0) {
              uVar24 = uVar24 & uVar26 - 1;
            }
            else if (uVar26 <= uVar24) {
              uVar23 = 0;
              if (uVar26 != 0) {
                uVar23 = uVar24 / uVar26;
              }
              uVar24 = uVar24 - uVar23 * uVar26;
            }
            *(long **)(*(long *)(param_1 + 0x10) + uVar24 * 8) = plVar17;
          }
        }
        else {
          *plVar17 = *plVar18;
          *plVar18 = (long)plVar17;
        }
        *(long *)(param_1 + 0x28) = *(long *)(param_1 + 0x28) + 1;
      }
LAB_0435e288:
      local_80[0] = (undefined8 *)CONCAT44(local_80[0]._4_4_,uVar1);
      local_68 = 2;
      FUN_0053a240(&local_c8,&DAT_0c9d717a,2,local_80);
      if ((local_68 != 0xff) && (local_68 != 2)) {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[local_68])(&local_e0,local_80);
      }
      lVar13 = (long)*(char *)((long)param_2 + 0x17);
      puVar9 = param_2;
      if (lVar13 < 0) {
        lVar13 = param_2[1];
        puVar9 = (undefined8 *)*param_2;
      }
      FUN_014ba128(&local_c8,"method",6,puVar9,lVar13);
      if (param_3 != 0) {
        FUN_005a5a40(&local_c8,"params",6,param_3);
      }
      local_e0 = (undefined1 *)0x0;
      lStack_d8 = 0;
      local_d0 = 0;
      local_70 = 6;
      local_80[0] = &local_c8;
      FUN_028b76a8(local_80,0,&local_e0,200);
      ppuVar2 = (undefined1 **)local_e0;
      if (-1 < (long)local_d0._7_1_) {
        ppuVar2 = &local_e0;
      }
      lVar13 = lStack_d8;
      if (-1 < (long)local_d0) {
        lVar13 = (long)local_d0._7_1_;
      }
      puVar3 = *(ulong **)(*(long *)(param_1 + 0x48) + 0x40);
      lVar4 = *(long *)(*(long *)(param_1 + 0x48) + 0x48);
      lVar14 = lVar4 << 4;
      puVar16 = puVar3;
      if (lVar14 != 0) {
        uVar24 = lVar14 >> 4;
        puVar15 = puVar3;
        do {
          uVar26 = uVar24 >> 1;
          puVar16 = puVar15 + uVar26 * 2 + 2;
          uVar24 = uVar24 + ~(uVar24 >> 1);
          if (param_1 <= puVar15[uVar26 * 2]) {
            puVar16 = puVar15;
            uVar24 = uVar26;
          }
          puVar15 = puVar16;
        } while (uVar24 != 0);
      }
      if (((puVar16 != puVar3 + lVar4 * 2) && (*puVar16 <= param_1)) && (puVar16[1] != 0)) {
        FUN_01fe9cf0(puVar16[1],ppuVar2,lVar13);
      }
      if (*(char *)((long)param_2 + 0x17) < '\0') {
        if (param_2[1] == 0x28) {
          iVar8 = _memcmp((void *)*param_2,"Page.removeScriptToEvaluateOnNewDocument",0x28);
          if ((iVar8 == 0) && (iVar8 = *(int *)(param_1 + 0x40) + -1, 0 < *(int *)(param_1 + 0x40)))
          goto LAB_0435e40c;
        }
        else if ((param_2[1] == 0x25) &&
                (iVar8 = _memcmp((void *)*param_2,"Page.addScriptToEvaluateOnNewDocument",0x25),
                iVar8 == 0)) {
          iVar8 = *(int *)(param_1 + 0x40) + 1;
LAB_0435e40c:
          *(int *)(param_1 + 0x40) = iVar8;
        }
      }
      if ((param_4 == (int *)0x0) && (*(long *)(param_1 + 0x48) != 0)) {
        FUN_00029e40(local_80,"ScheduleIdleDetachCheck",
                     "../../arc/browser/web_contents/devtools_protocol_wrapper.cc",0xe9);
        piVar22 = *(int **)(param_1 + 0xb0);
        if ((piVar22 != (int *)0x0) && (iVar8 = *piVar22, *piVar22 = iVar8 + 1, iVar8 == 0x7fffffff)
           ) {
                    /* WARNING: Does not return */
          pcVar7 = (code *)SoftwareBreakpoint(0,0x435e574);
          (*pcVar7)();
        }
        uVar10 = *(undefined8 *)(param_1 + 0xb8);
        puVar11 = (undefined4 *)FUN_00008790(0x40);
        *puVar11 = 1;
        *(undefined1 **)(puVar11 + 2) = &LAB_040774c0;
        *(code **)(puVar11 + 4) = FUN_040742c0;
        *(undefined1 **)(puVar11 + 6) = &LAB_040c0f80;
        *(code **)(puVar11 + 8) = FUN_0435e83c;
        *(undefined8 *)(puVar11 + 10) = 0;
        *(int **)(puVar11 + 0xc) = piVar22;
        *(undefined8 *)(puVar11 + 0xe) = uVar10;
        FUN_003b866c(param_1 + 0x50,local_80,5000000,puVar11);
      }
      if ((long)local_d0 < 0) {
        FUN_00005810(local_e0,local_d0 & 0x7fffffffffffffff);
      }
      FUN_005a58dc(&local_c8);
      param_4 = (int *)((ulong)uVar1 | 0x100000000);
      goto LAB_0435de54;
    }
    lVar13 = *(long *)(param_1 + 8) + 0x398;
    if (lVar13 != 0) {
      plVar17 = (long *)FUN_017d3e6c(lVar13);
      plVar18 = *(long **)(param_1 + 0x48);
      *(long **)(param_1 + 0x48) = plVar17;
      if (plVar18 != (long *)0x0) {
        lVar13 = plVar18[1];
        iVar8 = (int)lVar13 + -1;
        *(int *)(plVar18 + 1) = iVar8;
        if ((int)lVar13 == 0) {
                    /* WARNING: Does not return */
          pcVar7 = (code *)SoftwareBreakpoint(0,0x435e55c);
          (*pcVar7)();
        }
        if (iVar8 == 0) {
          (**(code **)(*plVar18 + 0xe8))();
        }
        plVar17 = *(long **)(param_1 + 0x48);
      }
      if (plVar17 != (long *)0x0) {
        puVar3 = (ulong *)plVar17[8];
        lVar13 = plVar17[9] << 4;
        puVar16 = puVar3;
        if (lVar13 != 0) {
          uVar24 = lVar13 >> 4;
          puVar15 = puVar3;
          do {
            uVar26 = uVar24 >> 1;
            puVar16 = puVar15 + uVar26 * 2 + 2;
            uVar24 = uVar24 + ~(uVar24 >> 1);
            if (param_1 <= puVar15[uVar26 * 2]) {
              puVar16 = puVar15;
              uVar24 = uVar26;
            }
            puVar15 = puVar16;
          } while (uVar24 != 0);
        }
        if (((puVar16 == puVar3 + plVar17[9] * 2) || (param_1 < *puVar16)) || (puVar16[1] == 0)) {
          uVar5 = *(undefined4 *)(*plVar17 + -0x14);
          uVar10 = FUN_00008790(0x1e0);
          FUN_02be6474(uVar10,param_1,uVar5);
          FUN_0247ab3c(plVar17,uVar10);
        }
        goto LAB_0435de98;
      }
    }
    if (param_4 == (int *)0x0) goto LAB_0435de54;
    LOAcquire();
    iVar8 = *param_4;
    *param_4 = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(param_4 + 4))(param_4);
    }
  }
  param_4 = (int *)0x0;
LAB_0435de54:
  if (param_3 != 0) {
    uVar10 = FUN_005a58dc(param_3);
    FUN_00005810(uVar10,0x18);
  }
  return param_4;
}


