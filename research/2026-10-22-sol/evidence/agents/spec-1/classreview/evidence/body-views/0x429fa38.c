// ==== 0x429fa38  FUN_0429fa38  (2620 bytes)

ulong FUN_0429fa38(long param_1,undefined8 param_2,undefined8 param_3,undefined4 *param_4,
                  undefined8 *param_5,undefined8 *param_6)

{
  uint uVar1;
  uint uVar2;
  ulong uVar3;
  ushort uVar4;
  undefined1 *puVar5;
  code *pcVar6;
  int iVar7;
  ulong uVar8;
  undefined4 *puVar9;
  undefined8 *puVar10;
  ulong uVar11;
  int *piVar12;
  ulong uVar13;
  long *plVar14;
  long *plVar15;
  long lVar16;
  ulong uVar17;
  long *plVar18;
  long *plVar19;
  bool bVar20;
  long lVar21;
  int *piVar22;
  ulong uVar23;
  uint uVar24;
  undefined8 uVar25;
  undefined8 *puVar26;
  undefined8 *puVar27;
  float fVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined1 *local_f0;
  undefined4 local_e8;
  undefined4 local_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  ulong local_c8;
  undefined4 local_c0 [2];
  undefined8 local_b8;
  undefined8 uStack_b0;
  ulong local_a8;
  int local_a0;
  undefined4 local_94;
  undefined2 local_90;
  int local_8c;
  undefined1 *local_88;
  int local_80;
  long local_78;
  undefined1 *local_70 [2];
  
  lVar21 = *(long *)(param_1 + 0x188);
  if (lVar21 == 0) {
    if (*(long *)(param_1 + 400) == 0) goto LAB_0429fa90;
  }
  else {
    uVar8 = FUN_001c5f40(lVar21 + 0x1f8,0);
    if ((uVar8 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(0,0x42a043c);
      (*pcVar6)();
    }
    if ((*(byte *)(lVar21 + 0x1a0) & 1) != 0) {
LAB_0429fa90:
      local_88 = &DAT_100000003;
      (*(code *)param_6[2])(param_6,&local_88);
      return 0;
    }
  }
  uVar4 = DAT_0e3baa98;
  uVar8 = (ulong)DAT_0e3baa98;
  DAT_0e3baa98 = DAT_0e3baa98 + 1;
  piVar22 = *(int **)(param_1 + 0x398);
  if ((piVar22 != (int *)0x0) && (iVar7 = *piVar22, *piVar22 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a0454);
    (*pcVar6)();
  }
  uVar25 = *(undefined8 *)(param_1 + 0x3a0);
  puVar9 = (undefined4 *)FUN_00008790(0x68);
  uVar29 = *param_6;
  uVar31 = param_6[3];
  uVar30 = param_6[2];
  *(undefined8 *)(puVar9 + 0x14) = param_6[1];
  *(undefined8 *)(puVar9 + 0x12) = uVar29;
  *puVar9 = 1;
  *(code **)(puVar9 + 2) = FUN_042a4648;
  *(code **)(puVar9 + 4) = FUN_042a472c;
  *(undefined1 **)(puVar9 + 6) = &LAB_04169bc0;
  *(undefined1 **)(puVar9 + 8) = &LAB_042a04b0;
  *(undefined8 *)(puVar9 + 10) = 0;
  *(int **)(puVar9 + 0xc) = piVar22;
  *(undefined8 *)(puVar9 + 0xe) = uVar25;
  *(ushort *)(puVar9 + 0x10) = uVar4;
  lVar21 = param_6[3];
  *(undefined8 *)(puVar9 + 0x18) = uVar31;
  *(undefined8 *)(puVar9 + 0x16) = uVar30;
  if (*(long *)(lVar21 + 8) != 0) {
    param_6[2] = 0;
    param_6[3] = &DAT_0c4b3500;
  }
  puVar10 = (undefined8 *)FUN_00008790(0x70);
  puVar26 = (undefined8 *)param_5[3];
  uVar29 = param_5[3];
  uVar25 = param_5[2];
  if (puVar26[1] != 0) {
    param_5[2] = 0;
    param_5[3] = &DAT_0c4b3500;
  }
  uVar30 = *param_5;
  uVar31 = param_5[1];
  *puVar10 = &PTR_FUN_0da7a9b0;
  puVar27 = puVar10 + 1;
  *puVar27 = 0;
  puVar10[2] = 0;
  piVar22 = (int *)FUN_00008790(8);
  *piVar22 = 0;
  *(undefined1 *)(piVar22 + 1) = 0;
  if ((piVar22 != (int *)0x0) && (iVar7 = *piVar22, *piVar22 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a0478);
    (*pcVar6)();
  }
  puVar10[3] = piVar22;
  puVar10[4] = puVar27;
  puVar10[5] = &PTR_viewWillStartLiveResize_0da7aa10;
  puVar10[6] = puVar10;
  puVar10[7] = uVar30;
  puVar10[8] = uVar31;
  puVar10[10] = uVar29;
  puVar10[9] = uVar25;
  if (puVar26[1] != 0) {
    puVar26 = &DAT_0c4b3500;
  }
  puVar10[0xb] = puVar9;
  piVar22 = (int *)FUN_00008790(8);
  *piVar22 = 0;
  *(undefined1 *)(piVar22 + 1) = 0;
  if ((piVar22 != (int *)0x0) && (iVar7 = *piVar22, *piVar22 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a0484);
    (*pcVar6)();
  }
  puVar10[0xc] = piVar22;
  puVar10[0xd] = puVar10;
  if ((code *)puVar26[1] != (code *)0x0) {
    (*(code *)puVar26[1])(uVar30);
  }
  local_88 = (undefined1 *)0x0;
  local_70[0] = (undefined1 *)0x0;
  FUN_003ad358(0,&local_88,local_70);
  puVar5 = local_88;
  local_88 = local_70[0];
  local_80 = 0;
  local_78 = 0;
  if (local_70[0] == (undefined1 *)0x0) {
    FUN_00365e78(puVar27);
    local_80 = 0;
    local_78 = 0;
  }
  else {
    puVar26 = (undefined8 *)FUN_00008790(8);
    *puVar26 = &PTR__cxx_destruct_0d9d47c8;
    FUN_0092d8c0(puVar27,&local_88,0,"ai.mojom.AIStreamedInferenceListener",puVar26,0,0,0,
                 puVar10 + 5,0,&LAB_041fed28,&LAB_04051dc0);
    lVar21 = local_78;
    if (local_80 == 1) {
      if (local_78 == 0) {
        local_80 = 0;
        goto LAB_0429fd00;
      }
      FUN_084a7784();
      lVar21 = local_78;
    }
    local_80 = 0;
    local_78 = 0;
    if (lVar21 != 0) {
      LOAcquire();
      iVar7 = *(int *)(lVar21 + 8);
      *(int *)(lVar21 + 8) = iVar7 + -1;
      LORelease();
      if (iVar7 == 1) {
        uVar25 = FUN_084a76fc();
        FUN_00005810(uVar25,0x38);
      }
      if (local_78 != 0) {
        LOAcquire();
        iVar7 = *(int *)(local_78 + 8);
        *(int *)(local_78 + 8) = iVar7 + -1;
        LORelease();
        if (iVar7 == 1) {
          uVar25 = FUN_084a76fc();
          FUN_00005810(uVar25,0x38);
        }
      }
    }
  }
LAB_0429fd00:
  if (local_88 != (undefined1 *)0x0) {
    FUN_005a359c();
  }
  piVar22 = (int *)puVar10[0xc];
  if ((piVar22 != (int *)0x0) && (iVar7 = *piVar22, *piVar22 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a0460);
    (*pcVar6)();
  }
  uVar25 = puVar10[0xd];
  puVar9 = (undefined4 *)FUN_00008790(0x48);
  *puVar9 = 1;
  *(undefined1 **)(puVar9 + 2) = &LAB_042a32e4;
  *(code **)(puVar9 + 4) = FUN_040b2900;
  *(undefined1 **)(puVar9 + 6) = &LAB_04169bc0;
  *(code **)(puVar9 + 8) = FUN_04297cf8;
  *(undefined8 *)(puVar9 + 10) = 0;
  *(int **)(puVar9 + 0xc) = piVar22;
  *(undefined8 *)(puVar9 + 0xe) = uVar25;
  puVar9[0x10] = 10;
  lVar21 = puVar10[2];
  uVar11 = FUN_001c5f40(lVar21 + 0x1f8,0);
  if ((uVar11 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a0448);
    (*pcVar6)();
  }
  piVar22 = *(int **)(lVar21 + 400);
  *(undefined4 **)(lVar21 + 400) = puVar9;
  if (piVar22 != (int *)0x0) {
    LOAcquire();
    iVar7 = *piVar22;
    *piVar22 = iVar7 + -1;
    LORelease();
    if (iVar7 == 1) {
      if (*(code **)(piVar22 + 4) == FUN_039a66c0) {
        piVar12 = *(int **)(piVar22 + 0xc);
        if (piVar12 != (int *)0x0) {
          LOAcquire();
          iVar7 = *piVar12;
          *piVar12 = iVar7 + -1;
          LORelease();
          if (iVar7 == 1) {
            FUN_00005810(piVar12,8);
          }
        }
        FUN_00005810(piVar22,0x48);
      }
      else {
        (**(code **)(piVar22 + 4))(piVar22);
      }
    }
  }
  piVar22 = *(int **)(lVar21 + 0x198);
  *(undefined8 *)(lVar21 + 0x198) = 0;
  if (piVar22 != (int *)0x0) {
    LOAcquire();
    iVar7 = *piVar22;
    *piVar22 = iVar7 + -1;
    LORelease();
    if (iVar7 == 1) {
      if (*(code **)(piVar22 + 4) == FUN_039a6ef8) {
        piVar12 = *(int **)(piVar22 + 10);
        if (piVar12 != (int *)0x0) {
          LOAcquire();
          iVar7 = *piVar12;
          *piVar12 = iVar7 + -1;
          LORelease();
          if (iVar7 == 1) {
            (**(code **)(piVar12 + 4))();
          }
        }
        FUN_00005810(piVar22,0x30);
      }
      else {
        (**(code **)(piVar22 + 4))(piVar22);
      }
    }
  }
  piVar22 = (int *)puVar10[0xc];
  if ((piVar22 != (int *)0x0) && (iVar7 = *piVar22, *piVar22 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a046c);
    (*pcVar6)();
  }
  uVar25 = puVar10[0xd];
  puVar9 = (undefined4 *)FUN_00008790(0x40);
  *puVar9 = 1;
  *(undefined1 **)(puVar9 + 2) = &LAB_04168060;
  *(code **)(puVar9 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar9 + 6) = &LAB_04169bc0;
  *(code **)(puVar9 + 8) = FUN_04297cf8;
  *(undefined8 *)(puVar9 + 10) = 0;
  *(int **)(puVar9 + 0xc) = piVar22;
  *(undefined8 *)(puVar9 + 0xe) = uVar25;
  if (puVar9 == (undefined4 *)0x0) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x42a03e4);
    (*pcVar6)();
  }
  uVar11 = *(ulong *)(param_1 + 0x1b8);
  uVar24 = (uint)uVar4;
  if (uVar11 == 0) {
    uVar23 = 0;
  }
  else {
    uVar13 = uVar11 - 1;
    if ((uVar11 & uVar13) == 0) {
      uVar23 = (uint)uVar11 - 1 & uVar8;
    }
    else {
      uVar23 = uVar8;
      if (uVar11 <= uVar8) {
        uVar1 = (uint)uVar11 & 0xffff;
        uVar2 = 0;
        if ((uVar11 & 0xffff) != 0) {
          uVar2 = uVar24 / uVar1;
        }
        uVar23 = (ulong)(uVar24 - uVar2 * uVar1);
      }
    }
    plVar14 = *(long **)(*(long *)(param_1 + 0x1b0) + uVar23 * 8);
    if (plVar14 != (long *)0x0) {
      do {
        while( true ) {
          plVar14 = (long *)*plVar14;
          if (plVar14 == (long *)0x0) goto LAB_0429ff38;
          uVar17 = plVar14[1];
          if (uVar17 == uVar8) break;
          if ((uVar11 & uVar13) == 0) {
            uVar17 = uVar17 & uVar13;
          }
          else if (uVar11 <= uVar17) {
            uVar3 = 0;
            if (uVar11 != 0) {
              uVar3 = uVar17 / uVar11;
            }
            uVar17 = uVar17 - uVar3 * uVar11;
          }
          if (uVar17 != uVar23) goto LAB_0429ff38;
        }
      } while (*(ushort *)(plVar14 + 2) != uVar24);
      goto LAB_042a0214;
    }
  }
LAB_0429ff38:
  plVar14 = (long *)FUN_00008790(0x20);
  if (plVar14 == (long *)0x0) {
LAB_042a03e8:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x42a03ec);
    (*pcVar6)();
  }
  *plVar14 = 0;
  plVar14[1] = uVar8;
  if (plVar14 == (long *)0xfffffffffffffff0) goto LAB_042a03e8;
  *(ushort *)(plVar14 + 2) = uVar4;
  fVar28 = (float)(*(long *)(param_1 + 0x1c8) + 1);
  plVar14[3] = (long)puVar10;
  if (*(float *)(param_1 + 0x1d0) * (float)uVar11 < fVar28) {
    uVar23 = (ulong)((uVar11 & uVar11 - 1) != 0);
    if (uVar11 < 3) {
      uVar23 = 1;
    }
    uVar23 = uVar23 | uVar11 << 1;
    uVar13 = (ulong)(fVar28 / *(float *)(param_1 + 0x1d0));
    if (uVar23 <= uVar13) {
      uVar23 = uVar13;
    }
    if (uVar23 - 1 == 0) {
      uVar23 = 2;
    }
    else if ((uVar23 & uVar23 - 1) != 0) {
      uVar23 = FUN_00f76630(uVar23);
      uVar11 = *(ulong *)(param_1 + 0x1b8);
    }
    if (uVar11 < uVar23) {
LAB_042a0048:
      if (uVar23 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      uVar25 = FUN_00008790(uVar23 << 3);
      bVar20 = false;
LAB_042a005c:
      lVar21 = *(long *)(param_1 + 0x1b0);
      *(undefined8 *)(param_1 + 0x1b0) = uVar25;
      if (lVar21 != 0) {
        FUN_00005810(lVar21,*(long *)(param_1 + 0x1b8) << 3);
      }
      *(ulong *)(param_1 + 0x1b8) = uVar23;
      if (!bVar20) {
        uVar11 = 0;
        do {
          *(undefined8 *)(*(long *)(param_1 + 0x1b0) + uVar11 * 8) = 0;
          uVar11 = uVar11 + 1;
        } while (uVar23 != uVar11);
        plVar15 = *(long **)(param_1 + 0x1c0);
        if (plVar15 != (long *)0x0) {
          uVar11 = plVar15[1];
          uVar13 = uVar23 - 1;
          if ((uVar23 & uVar13) == 0) {
            uVar11 = uVar11 & uVar13;
          }
          else if (uVar23 <= uVar11) {
            uVar17 = 0;
            if (uVar23 != 0) {
              uVar17 = uVar11 / uVar23;
            }
            uVar11 = uVar11 - uVar17 * uVar23;
          }
          *(long *)(*(long *)(param_1 + 0x1b0) + uVar11 * 8) = param_1 + 0x1c0;
          plVar18 = (long *)*plVar15;
          while (plVar18 != (long *)0x0) {
            uVar17 = plVar18[1];
            if ((uVar23 & uVar13) == 0) {
              uVar17 = uVar17 & uVar13;
            }
            else if (uVar23 <= uVar17) {
              uVar3 = 0;
              if (uVar23 != 0) {
                uVar3 = uVar17 / uVar23;
              }
              uVar17 = uVar17 - uVar3 * uVar23;
            }
            plVar19 = plVar18;
            if (uVar17 != uVar11) {
              if (*(long *)(*(long *)(param_1 + 0x1b0) + uVar17 * 8) == 0) {
                *(long **)(*(long *)(param_1 + 0x1b0) + uVar17 * 8) = plVar15;
                uVar11 = uVar17;
              }
              else {
                *plVar15 = *plVar18;
                *plVar18 = **(undefined8 **)(*(long *)(param_1 + 0x1b0) + uVar17 * 8);
                **(long **)(*(long *)(param_1 + 0x1b0) + uVar17 * 8) = (long)plVar18;
                plVar19 = plVar15;
              }
            }
            plVar15 = plVar19;
            plVar18 = (long *)*plVar19;
          }
        }
      }
    }
    else if (uVar23 < uVar11) {
      uVar13 = (ulong)((float)*(ulong *)(param_1 + 0x1c8) / *(float *)(param_1 + 0x1d0));
      if ((uVar11 < 3) || ((uVar11 & uVar11 - 1) != 0)) {
        uVar13 = FUN_00f76630();
      }
      else if (1 < uVar13) {
        uVar13 = 1L << (-LZCOUNT(uVar13 - 1) & 0x3fU);
      }
      if (uVar23 <= uVar13) {
        uVar23 = uVar13;
      }
      if (uVar23 < uVar11) {
        if (uVar23 != 0) goto LAB_042a0048;
        uVar25 = 0;
        bVar20 = true;
        goto LAB_042a005c;
      }
    }
    uVar11 = *(ulong *)(param_1 + 0x1b8);
    if ((uVar11 & uVar11 - 1) == 0) {
      uVar23 = (int)uVar11 - 1 & uVar8;
    }
    else {
      uVar23 = uVar8;
      if (uVar11 <= uVar8) {
        uVar23 = 0;
        if (uVar11 != 0) {
          uVar23 = uVar8 / uVar11;
        }
        uVar23 = uVar8 - uVar23 * uVar11;
      }
    }
  }
  lVar21 = *(long *)(param_1 + 0x1b0);
  plVar15 = *(long **)(lVar21 + uVar23 * 8);
  if (plVar15 == (long *)0x0) {
    lVar16 = *(long *)(param_1 + 0x1c0);
    *plVar14 = lVar16;
    *(long **)(param_1 + 0x1c0) = plVar14;
    *(long *)(lVar21 + uVar23 * 8) = param_1 + 0x1c0;
    if (lVar16 != 0) {
      uVar23 = *(ulong *)(lVar16 + 8);
      if ((uVar11 & uVar11 - 1) == 0) {
        uVar23 = uVar23 & uVar11 - 1;
      }
      else if (uVar11 <= uVar23) {
        uVar13 = 0;
        if (uVar11 != 0) {
          uVar13 = uVar23 / uVar11;
        }
        uVar23 = uVar23 - uVar13 * uVar11;
      }
      *(long **)(*(long *)(param_1 + 0x1b0) + uVar23 * 8) = plVar14;
    }
  }
  else {
    *plVar14 = *plVar15;
    *plVar15 = (long)plVar14;
  }
  puVar10 = (undefined8 *)0x0;
  *(long *)(param_1 + 0x1c8) = *(long *)(param_1 + 0x1c8) + 1;
LAB_042a0214:
  if (*(long *)(param_1 + 0x1a8) == 0) {
    puVar26 = (undefined8 *)FUN_00008790(8);
    *puVar26 = &PTR__cxx_destruct_0d9d47f8;
    iVar7 = FUN_009180c0(param_1 + 0x180,0,0,0,puVar26,"ai.mojom.AIService",&LAB_041f5160,
                         &LAB_04051dc0);
    if (iVar7 != 0) {
      uVar25 = *(undefined8 *)(param_1 + 0x188);
      puVar26 = (undefined8 *)FUN_00008790(0x10);
      *puVar26 = &PTR__cxx_destruct_0d9d46b8;
      puVar26[1] = uVar25;
      lVar21 = *(long *)(param_1 + 0x1a8);
      *(undefined8 **)(param_1 + 0x1a8) = puVar26;
      if (lVar21 != 0) {
        FUN_00005810(lVar21,0x10);
      }
    }
  }
  uVar25 = *(undefined8 *)(param_1 + 0x1a8);
  local_94 = (undefined4)param_3;
  local_90 = (undefined2)((ulong)param_3 >> 0x20);
  local_8c = 1;
  local_e0 = *param_4;
  if (*(char *)((long)param_4 + 0x1f) < '\0') {
    FUN_00060a60(&local_d8,*(undefined8 *)(param_4 + 2),*(undefined8 *)(param_4 + 4));
  }
  else {
    uStack_d0 = *(undefined8 *)(param_4 + 4);
    local_d8 = *(undefined8 *)(param_4 + 2);
    local_c8 = *(ulong *)(param_4 + 6);
  }
  local_c0[0] = local_e0;
  if ((long)local_c8 < 0) {
    FUN_00060a60(&local_b8,local_d8,uStack_d0);
  }
  else {
    uStack_b0 = uStack_d0;
    local_b8 = local_d8;
    local_a8 = local_c8;
  }
  local_a0 = 1;
  local_f0 = puVar5;
  local_e8 = 0;
  FUN_041f683c(uVar25,param_2,&local_94,local_c0,&local_f0,puVar9);
  if (local_f0 != (undefined1 *)0x0) {
    FUN_005a359c();
  }
  if ((local_a0 != 0) && (local_a0 = 0, (long)local_a8 < 0)) {
    FUN_00005810(local_b8,local_a8 & 0x7fffffffffffffff);
  }
  if ((long)local_c8 < 0) {
    FUN_00005810(local_d8,local_c8 & 0x7fffffffffffffff);
  }
  if (local_8c != 0) {
    local_8c = 0;
  }
  if (puVar10 == (undefined8 *)0x0) {
    return uVar8;
  }
  FUN_04297b44(puVar10);
  return uVar8;
}


