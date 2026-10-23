// ==== 0x435b114  FUN_0435b114  (1640 bytes)

void FUN_0435b114(undefined8 *param_1,long param_2,undefined8 *param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  code *pcVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined4 *puVar7;
  ulong uVar8;
  long lVar9;
  char *pcVar10;
  undefined8 uVar11;
  ulong uVar12;
  long *plVar13;
  long *plVar14;
  long lVar15;
  ulong uVar16;
  long *plVar17;
  long *plVar18;
  bool bVar19;
  int *piVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  ulong uVar23;
  ulong uVar24;
  float fVar25;
  undefined8 local_c8 [2];
  undefined7 local_b8;
  char cStack_b1;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  ulong local_98;
  ulong local_90;
  undefined8 *local_88;
  long lStack_80;
  undefined8 *local_78;
  long lStack_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((*(long *)(param_2 + 0x50) == 0) || (*(char *)(*(long *)(param_2 + 0x50) + 4) != '\0')) ||
     (*(long *)(param_2 + 0x58) == 0)) {
    uVar21 = FUN_04b37104(0x39,"Associated WebContents is not available.",0x28,0x22a,
                          "../../arc/browser/web_contents/devtools_impl.cc");
    *param_1 = uVar21;
    goto LAB_0435b60c;
  }
  local_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  local_98 = 0;
  local_a0 = 0;
  lVar9 = (long)*(char *)((long)param_3 + 0x47);
  if (lVar9 < 0) {
    puVar6 = (undefined8 *)param_3[6];
    lVar9 = param_3[7];
  }
  else {
    puVar6 = param_3 + 6;
  }
  FUN_02347848(&local_b0,puVar6,lVar9,0x34,200);
  if (((local_90 & 1) == 0) || ((byte)local_98 != '\x06')) {
    pcVar10 = "Malformed params.";
    uVar21 = 0xd;
    uVar22 = 0x11;
    uVar11 = 0x231;
LAB_0435b688:
    uVar21 = FUN_04b37104(uVar21,pcVar10,uVar22,uVar11,
                          "../../arc/browser/web_contents/devtools_impl.cc");
  }
  else {
    puVar6 = (undefined8 *)FUN_00008790(0x18);
    uVar8 = local_a0;
    uVar21 = uStack_a8;
    *puVar6 = local_b0;
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
    puVar6[2] = uVar8;
    puVar6[1] = uVar21;
    lStack_80 = (long)*(char *)((long)param_3 + 0x17);
    local_88 = param_3;
    if (lStack_80 < 0) {
      lStack_80 = param_3[1];
      local_88 = (undefined8 *)*param_3;
    }
    uVar21 = *(undefined8 *)(param_2 + 0x48);
    lStack_70 = (long)*(char *)((long)param_3 + 0x2f);
    if (lStack_70 < 0) {
      local_78 = (undefined8 *)param_3[3];
      lStack_70 = param_3[4];
    }
    else {
      local_78 = param_3 + 3;
    }
    FUN_0219069c(local_c8,&local_88,2,".",1);
    piVar20 = *(int **)(param_2 + 0x128);
    if ((piVar20 != (int *)0x0) && (iVar5 = *piVar20, *piVar20 = iVar5 + 1, iVar5 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0x435b77c);
      (*pcVar4)();
    }
    uVar22 = *(undefined8 *)(param_2 + 0x130);
    puVar7 = (undefined4 *)FUN_00008790(0x40);
    *puVar7 = 1;
    *(undefined1 **)(puVar7 + 2) = &LAB_0435cca8;
    *(code **)(puVar7 + 4) = FUN_040a6300;
    *(undefined1 **)(puVar7 + 6) = &LAB_04169bc0;
    *(code **)(puVar7 + 8) = FUN_0435a838;
    *(undefined8 *)(puVar7 + 10) = 0;
    *(int **)(puVar7 + 0xc) = piVar20;
    *(undefined8 *)(puVar7 + 0xe) = uVar22;
    uVar8 = FUN_0435dd94(uVar21,local_c8,puVar6,puVar7);
    if (cStack_b1 < '\0') {
      FUN_00005810(local_c8[0],CONCAT17(cStack_b1,local_b8) & 0x7fffffffffffffff);
    }
    if ((uVar8 >> 0x20 & 1) == 0) {
      pcVar10 = "DevTools agent host closed";
      uVar21 = 0x39;
      uVar22 = 0x1a;
      uVar11 = 0x23c;
      goto LAB_0435b688;
    }
    puVar6 = (undefined8 *)param_4[3];
    lVar15 = param_4[3];
    lVar9 = param_4[2];
    if (puVar6[1] != 0) {
      param_4[2] = 0;
      param_4[3] = (long)&DAT_0c4b3500;
    }
    lVar1 = *param_4;
    lVar2 = param_4[1];
    iVar5 = (int)uVar8;
    uVar8 = (ulong)iVar5;
    uVar23 = *(ulong *)(param_2 + 0xb0);
    if (uVar23 == 0) {
      uVar24 = 0;
LAB_0435b370:
      plVar13 = (long *)FUN_00008790(0x38);
      if (plVar13 == (long *)0x0) {
LAB_0435b76c:
                    /* WARNING: Does not return */
        pcVar4 = (code *)SoftwareBreakpoint(1,0x435b770);
        (*pcVar4)();
      }
      *plVar13 = 0;
      plVar13[1] = uVar8;
      if (plVar13 == (long *)0xfffffffffffffff0) goto LAB_0435b76c;
      *(int *)(plVar13 + 2) = iVar5;
      plVar13[3] = lVar1;
      plVar13[4] = lVar2;
      plVar13[6] = lVar15;
      plVar13[5] = lVar9;
      fVar25 = (float)(*(long *)(param_2 + 0xc0) + 1);
      if (puVar6[1] != 0) {
        puVar6 = &DAT_0c4b3500;
      }
      if (*(float *)(param_2 + 200) * (float)uVar23 < fVar25) {
        uVar24 = (ulong)((uVar23 & uVar23 - 1) != 0);
        if (uVar23 < 3) {
          uVar24 = 1;
        }
        uVar24 = uVar24 | uVar23 << 1;
        uVar12 = (ulong)(fVar25 / *(float *)(param_2 + 200));
        if (uVar24 <= uVar12) {
          uVar24 = uVar12;
        }
        if (uVar24 - 1 == 0) {
          uVar24 = 2;
        }
        else if ((uVar24 & uVar24 - 1) != 0) {
          uVar24 = FUN_00f76630(uVar24);
          uVar23 = *(ulong *)(param_2 + 0xb0);
        }
        if (uVar23 < uVar24) {
LAB_0435b494:
          if (uVar24 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          uVar21 = FUN_00008790(uVar24 << 3);
          bVar19 = false;
LAB_0435b4a8:
          lVar9 = *(long *)(param_2 + 0xa8);
          *(undefined8 *)(param_2 + 0xa8) = uVar21;
          if (lVar9 != 0) {
            FUN_00005810(lVar9,*(long *)(param_2 + 0xb0) << 3);
          }
          *(ulong *)(param_2 + 0xb0) = uVar24;
          if (!bVar19) {
            uVar23 = 0;
            do {
              *(undefined8 *)(*(long *)(param_2 + 0xa8) + uVar23 * 8) = 0;
              uVar23 = uVar23 + 1;
            } while (uVar24 != uVar23);
            plVar14 = *(long **)(param_2 + 0xb8);
            if (plVar14 != (long *)0x0) {
              uVar23 = plVar14[1];
              uVar12 = uVar24 - 1;
              if ((uVar24 & uVar12) == 0) {
                uVar23 = uVar23 & uVar12;
              }
              else if (uVar24 <= uVar23) {
                uVar16 = 0;
                if (uVar24 != 0) {
                  uVar16 = uVar23 / uVar24;
                }
                uVar23 = uVar23 - uVar16 * uVar24;
              }
              *(undefined8 **)(*(long *)(param_2 + 0xa8) + uVar23 * 8) =
                   (undefined8 *)(param_2 + 0xb8);
              plVar17 = (long *)*plVar14;
              while (plVar17 != (long *)0x0) {
                uVar16 = plVar17[1];
                if ((uVar24 & uVar12) == 0) {
                  uVar16 = uVar16 & uVar12;
                }
                else if (uVar24 <= uVar16) {
                  uVar3 = 0;
                  if (uVar24 != 0) {
                    uVar3 = uVar16 / uVar24;
                  }
                  uVar16 = uVar16 - uVar3 * uVar24;
                }
                plVar18 = plVar17;
                if (uVar16 != uVar23) {
                  if (*(long *)(*(long *)(param_2 + 0xa8) + uVar16 * 8) == 0) {
                    *(long **)(*(long *)(param_2 + 0xa8) + uVar16 * 8) = plVar14;
                    uVar23 = uVar16;
                  }
                  else {
                    *plVar14 = *plVar17;
                    *plVar17 = **(undefined8 **)(*(long *)(param_2 + 0xa8) + uVar16 * 8);
                    **(long **)(*(long *)(param_2 + 0xa8) + uVar16 * 8) = (long)plVar17;
                    plVar18 = plVar14;
                  }
                }
                plVar14 = plVar18;
                plVar17 = (long *)*plVar18;
              }
            }
          }
        }
        else if (uVar24 < uVar23) {
          uVar12 = (ulong)((float)*(ulong *)(param_2 + 0xc0) / *(float *)(param_2 + 200));
          if ((uVar23 < 3) || ((uVar23 & uVar23 - 1) != 0)) {
            uVar12 = FUN_00f76630();
          }
          else if (1 < uVar12) {
            uVar12 = 1L << (-LZCOUNT(uVar12 - 1) & 0x3fU);
          }
          if (uVar24 <= uVar12) {
            uVar24 = uVar12;
          }
          if (uVar24 < uVar23) {
            if (uVar24 != 0) goto LAB_0435b494;
            uVar21 = 0;
            bVar19 = true;
            goto LAB_0435b4a8;
          }
        }
        uVar23 = *(ulong *)(param_2 + 0xb0);
        if ((uVar23 & uVar23 - 1) == 0) {
          uVar24 = uVar23 - 1 & uVar8;
        }
        else {
          uVar24 = uVar8;
          if (uVar23 <= uVar8) {
            uVar24 = 0;
            if (uVar23 != 0) {
              uVar24 = uVar8 / uVar23;
            }
            uVar24 = uVar8 - uVar24 * uVar23;
          }
        }
      }
      lVar9 = *(long *)(param_2 + 0xa8);
      plVar14 = *(long **)(lVar9 + uVar24 * 8);
      if (plVar14 == (long *)0x0) {
        plVar14 = (long *)(param_2 + 0xb8);
        lVar15 = *plVar14;
        *plVar13 = lVar15;
        *plVar14 = (long)plVar13;
        *(long **)(lVar9 + uVar24 * 8) = plVar14;
        if (lVar15 != 0) {
          uVar8 = *(ulong *)(lVar15 + 8);
          if ((uVar23 & uVar23 - 1) == 0) {
            uVar8 = uVar8 & uVar23 - 1;
          }
          else if (uVar23 <= uVar8) {
            uVar24 = 0;
            if (uVar23 != 0) {
              uVar24 = uVar8 / uVar23;
            }
            uVar8 = uVar8 - uVar24 * uVar23;
          }
          *(long **)(*(long *)(param_2 + 0xa8) + uVar8 * 8) = plVar13;
        }
      }
      else {
        *plVar13 = *plVar14;
        *plVar14 = (long)plVar13;
      }
      *(long *)(param_2 + 0xc0) = *(long *)(param_2 + 0xc0) + 1;
    }
    else {
      uVar12 = uVar23 - 1;
      if ((uVar23 & uVar12) == 0) {
        uVar24 = uVar12 & uVar8;
      }
      else {
        uVar24 = uVar8;
        if (uVar23 <= uVar8) {
          uVar24 = 0;
          if (uVar23 != 0) {
            uVar24 = uVar8 / uVar23;
          }
          uVar24 = uVar8 - uVar24 * uVar23;
        }
      }
      plVar13 = *(long **)(*(long *)(param_2 + 0xa8) + uVar24 * 8);
      if (plVar13 == (long *)0x0) goto LAB_0435b370;
      do {
        while( true ) {
          plVar13 = (long *)*plVar13;
          if (plVar13 == (long *)0x0) goto LAB_0435b370;
          uVar16 = plVar13[1];
          if (uVar16 == uVar8) break;
          if ((uVar23 & uVar12) == 0) {
            uVar16 = uVar16 & uVar12;
          }
          else if (uVar23 <= uVar16) {
            uVar3 = 0;
            if (uVar23 != 0) {
              uVar3 = uVar16 / uVar23;
            }
            uVar16 = uVar16 - uVar3 * uVar23;
          }
          if (uVar16 != uVar24) goto LAB_0435b370;
        }
      } while (*(int *)(plVar13 + 2) != iVar5);
    }
    if ((code *)puVar6[1] != (code *)0x0) {
      (*(code *)puVar6[1])(lVar1);
    }
    uVar21 = 1;
  }
  *param_1 = uVar21;
  if ((((local_90 & 1) != 0) && ((byte)local_98 != 0xff)) && ((local_98 & 0xfd) != 0)) {
    if ((byte)local_98 == 4) {
      if ((long)local_a0 < 0) {
        FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
      }
    }
    else {
      (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)(byte)local_98])(&local_88,&local_b0);
    }
  }
LAB_0435b60c:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_68) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


