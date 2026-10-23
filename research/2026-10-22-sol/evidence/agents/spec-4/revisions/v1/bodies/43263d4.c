// ==== 0x43263d4  FUN_043263d4  (2884 bytes)

void FUN_043263d4(undefined8 *param_1,undefined8 *param_2)

{
  ulong *puVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  ulong uVar5;
  code *pcVar6;
  int iVar7;
  long lVar8;
  undefined4 *puVar9;
  ulong uVar10;
  undefined8 uVar11;
  undefined8 *puVar12;
  long lVar13;
  ulong uVar14;
  long *plVar15;
  ulong uVar16;
  ulong uVar17;
  long *plVar18;
  long *plVar19;
  bool bVar20;
  ulong uVar21;
  ulong uVar22;
  ulong *puVar23;
  long *plVar24;
  size_t sVar25;
  long lVar26;
  uint uVar27;
  void *pvVar28;
  ulong uVar29;
  undefined8 *puVar30;
  ulong *puVar31;
  ulong uVar32;
  ulong uVar33;
  ulong uVar34;
  float fVar35;
  ulong local_78;
  ulong *local_70;
  ulong uStack_68;
  
  puVar31 = (ulong *)*param_2;
  uVar22 = *puVar31;
  *(int *)(uVar22 + 0x90) = *(int *)(uVar22 + 0x90) + 1;
  puVar23 = puVar31 + 1;
  cVar3 = *(char *)((long)puVar31 + 0x1f);
  puVar1 = (ulong *)*puVar23;
  if (-1 < (long)cVar3) {
    puVar1 = puVar23;
  }
  uVar10 = puVar31[2];
  if (-1 < cVar3) {
    uVar10 = (long)cVar3;
  }
  local_70 = puVar1;
  uStack_68 = uVar10;
  lVar8 = FUN_04184480(uVar22 + 0x40,&local_70);
  if (lVar8 == 0) {
    if ((uVar22 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar22);
    }
    puVar9 = (undefined4 *)FUN_00008790(0x38);
    *puVar9 = 1;
    *(undefined1 **)(puVar9 + 2) = &LAB_04326f18;
    *(code **)(puVar9 + 4) = FUN_039a7300;
    *(code **)(puVar9 + 6) = FUN_00b77080;
    *(undefined1 **)(puVar9 + 8) = &LAB_0432613c;
    *(undefined8 *)(puVar9 + 10) = 0;
    *(ulong *)(puVar9 + 0xc) = uVar22;
    FUN_017a70c8(uVar22 + 0x98,puVar1,uVar10,puVar9);
    local_70 = (ulong *)(uVar22 + 0x18);
    FUN_010d8c54(&local_70,puVar23,puVar23);
  }
  lVar8 = FUN_050aafc0(uVar22 + 0x18,puVar23);
  if (lVar8 == 0) goto LAB_04326da0;
  sVar25 = (size_t)*(char *)(lVar8 + 0x27);
  if ((long)sVar25 < 0) {
    pvVar28 = *(void **)(lVar8 + 0x10);
    sVar25 = *(size_t *)(lVar8 + 0x18);
  }
  else {
    pvVar28 = (void *)(lVar8 + 0x10);
  }
  uVar10 = FUN_00018f3c(pvVar28,sVar25);
  uVar33 = *(ulong *)(uVar22 + 0x48);
  if (uVar33 == 0) {
    uVar21 = 0;
  }
  else {
    uVar29 = uVar33 - 1;
    if ((uVar33 & uVar29) == 0) {
      uVar21 = uVar29 & uVar10;
    }
    else {
      uVar21 = uVar10;
      if (uVar33 <= uVar10) {
        uVar21 = 0;
        if (uVar33 != 0) {
          uVar21 = uVar10 / uVar33;
        }
        uVar21 = uVar10 - uVar21 * uVar33;
      }
    }
    puVar12 = *(undefined8 **)(*(long *)(uVar22 + 0x40) + uVar21 * 8);
    if (puVar12 != (undefined8 *)0x0) {
      for (plVar24 = (long *)*puVar12; plVar24 != (long *)0x0; plVar24 = (long *)*plVar24) {
        uVar16 = plVar24[1];
        if (uVar16 == uVar10) {
          if ((plVar24[3] == sVar25) &&
             (iVar7 = _memcmp((void *)plVar24[2],pvVar28,sVar25), iVar7 == 0)) goto LAB_0432679c;
        }
        else {
          if ((uVar33 & uVar29) == 0) {
            uVar16 = uVar16 & uVar29;
          }
          else if (uVar33 <= uVar16) {
            uVar34 = 0;
            if (uVar33 != 0) {
              uVar34 = uVar16 / uVar33;
            }
            uVar16 = uVar16 - uVar34 * uVar33;
          }
          if (uVar16 != uVar21) break;
        }
      }
    }
  }
  plVar24 = (long *)FUN_00008790(0x48);
  if (plVar24 == (long *)0x0) goto LAB_04326da0;
  *plVar24 = 0;
  plVar24[1] = uVar10;
  if (plVar24 == (long *)0xfffffffffffffff0) goto LAB_04326da0;
  plVar24[2] = (long)pvVar28;
  plVar24[3] = sVar25;
  plVar24[5] = 0;
  plVar24[4] = 0;
  plVar24[7] = 0;
  plVar24[6] = 0;
  *(undefined4 *)(plVar24 + 8) = 0x3f800000;
  fVar35 = (float)(*(long *)(uVar22 + 0x58) + 1);
  if (*(float *)(uVar22 + 0x60) * (float)uVar33 < fVar35) {
    uVar21 = (ulong)((uVar33 & uVar33 - 1) != 0);
    if (uVar33 < 3) {
      uVar21 = 1;
    }
    uVar21 = uVar21 | uVar33 << 1;
    uVar29 = (ulong)(fVar35 / *(float *)(uVar22 + 0x60));
    if (uVar21 <= uVar29) {
      uVar21 = uVar29;
    }
    if (uVar21 - 1 == 0) {
      uVar21 = 2;
    }
    else if ((uVar21 & uVar21 - 1) != 0) {
      uVar21 = FUN_00f76630(uVar21);
      uVar33 = *(ulong *)(uVar22 + 0x48);
    }
    if (uVar33 < uVar21) {
LAB_04326664:
      if (uVar21 >> 0x3d != 0) goto LAB_04326eac;
      uVar11 = FUN_00008790(uVar21 << 3);
      bVar20 = false;
LAB_04326678:
      lVar13 = *(long *)(uVar22 + 0x40);
      *(undefined8 *)(uVar22 + 0x40) = uVar11;
      if (lVar13 != 0) {
        FUN_00005810(lVar13,*(long *)(uVar22 + 0x48) << 3);
      }
      *(ulong *)(uVar22 + 0x48) = uVar21;
      if (!bVar20) {
        uVar33 = 0;
        do {
          *(undefined8 *)(*(long *)(uVar22 + 0x40) + uVar33 * 8) = 0;
          uVar33 = uVar33 + 1;
        } while (uVar21 != uVar33);
        plVar15 = *(long **)(uVar22 + 0x50);
        if (plVar15 != (long *)0x0) {
          uVar33 = plVar15[1];
          uVar29 = uVar21 - 1;
          if ((uVar21 & uVar29) == 0) {
            uVar33 = uVar33 & uVar29;
          }
          else if (uVar21 <= uVar33) {
            uVar16 = 0;
            if (uVar21 != 0) {
              uVar16 = uVar33 / uVar21;
            }
            uVar33 = uVar33 - uVar16 * uVar21;
          }
          *(undefined8 **)(*(long *)(uVar22 + 0x40) + uVar33 * 8) = (undefined8 *)(uVar22 + 0x50);
          plVar18 = (long *)*plVar15;
          while (plVar18 != (long *)0x0) {
            uVar16 = plVar18[1];
            if ((uVar21 & uVar29) == 0) {
              uVar16 = uVar16 & uVar29;
            }
            else if (uVar21 <= uVar16) {
              uVar34 = 0;
              if (uVar21 != 0) {
                uVar34 = uVar16 / uVar21;
              }
              uVar16 = uVar16 - uVar34 * uVar21;
            }
            plVar19 = plVar18;
            if (uVar16 != uVar33) {
              if (*(long *)(*(long *)(uVar22 + 0x40) + uVar16 * 8) == 0) {
                *(long **)(*(long *)(uVar22 + 0x40) + uVar16 * 8) = plVar15;
                uVar33 = uVar16;
              }
              else {
                *plVar15 = *plVar18;
                *plVar18 = **(undefined8 **)(*(long *)(uVar22 + 0x40) + uVar16 * 8);
                **(long **)(*(long *)(uVar22 + 0x40) + uVar16 * 8) = (long)plVar18;
                plVar19 = plVar15;
              }
            }
            plVar15 = plVar19;
            plVar18 = (long *)*plVar19;
          }
        }
      }
    }
    else if (uVar21 < uVar33) {
      uVar29 = (ulong)((float)*(ulong *)(uVar22 + 0x58) / *(float *)(uVar22 + 0x60));
      if ((uVar33 < 3) || ((uVar33 & uVar33 - 1) != 0)) {
        uVar29 = FUN_00f76630();
      }
      else if (1 < uVar29) {
        uVar29 = 1L << (-LZCOUNT(uVar29 - 1) & 0x3fU);
      }
      if (uVar21 <= uVar29) {
        uVar21 = uVar29;
      }
      if (uVar21 < uVar33) {
        if (uVar21 != 0) goto LAB_04326664;
        uVar11 = 0;
        bVar20 = true;
        goto LAB_04326678;
      }
    }
    uVar33 = *(ulong *)(uVar22 + 0x48);
    if ((uVar33 & uVar33 - 1) == 0) {
      uVar21 = uVar33 - 1 & uVar10;
    }
    else {
      uVar21 = uVar10;
      if (uVar33 <= uVar10) {
        uVar21 = 0;
        if (uVar33 != 0) {
          uVar21 = uVar10 / uVar33;
        }
        uVar21 = uVar10 - uVar21 * uVar33;
      }
    }
  }
  lVar13 = *(long *)(uVar22 + 0x40);
  plVar15 = *(long **)(lVar13 + uVar21 * 8);
  if (plVar15 == (long *)0x0) {
    plVar15 = (long *)(uVar22 + 0x50);
    *plVar24 = *plVar15;
    *plVar15 = (long)plVar24;
    *(long **)(lVar13 + uVar21 * 8) = plVar15;
    if (*plVar24 != 0) {
      uVar10 = *(ulong *)(*plVar24 + 8);
      if ((uVar33 & uVar33 - 1) == 0) {
        uVar10 = uVar10 & uVar33 - 1;
      }
      else if (uVar33 <= uVar10) {
        uVar21 = 0;
        if (uVar33 != 0) {
          uVar21 = uVar10 / uVar33;
        }
        uVar10 = uVar10 - uVar21 * uVar33;
      }
      *(long **)(*(long *)(uVar22 + 0x40) + uVar10 * 8) = plVar24;
    }
  }
  else {
    *plVar24 = *plVar15;
    *plVar15 = (long)plVar24;
  }
  *(long *)(uVar22 + 0x58) = *(long *)(uVar22 + 0x58) + 1;
LAB_0432679c:
  uVar2 = *(uint *)(uVar22 + 0x90);
  uVar33 = (ulong)uVar2;
  uVar10 = plVar24[5];
  if (uVar10 == 0) {
    uVar21 = 0;
LAB_04326834:
    plVar15 = (long *)FUN_00008790(0x18);
    if (plVar15 == (long *)0x0) goto LAB_04326da0;
    *plVar15 = 0;
    plVar15[1] = uVar33;
    if (plVar15 + 2 == (long *)0x0) goto LAB_04326da0;
    *(undefined4 *)(plVar15 + 2) = *(undefined4 *)(uVar22 + 0x90);
    if (*(float *)(plVar24 + 8) * (float)uVar10 < (float)(plVar24[7] + 1)) {
      uVar21 = (ulong)((uVar10 & uVar10 - 1) != 0);
      if (uVar10 < 3) {
        uVar21 = 1;
      }
      uVar21 = uVar21 | uVar10 << 1;
      uVar29 = (ulong)((float)(plVar24[7] + 1) / *(float *)(plVar24 + 8));
      if (uVar21 <= uVar29) {
        uVar21 = uVar29;
      }
      if (uVar21 - 1 == 0) {
        uVar21 = 2;
      }
      else if ((uVar21 & uVar21 - 1) != 0) {
        uVar21 = FUN_00f76630(uVar21);
        uVar10 = plVar24[5];
      }
      if (uVar10 < uVar21) {
LAB_043268b8:
        FUN_01edcde0(plVar24 + 4,uVar21);
      }
      else if (uVar21 < uVar10) {
        uVar29 = (ulong)((float)(ulong)plVar24[7] / *(float *)(plVar24 + 8));
        if ((uVar10 < 3) || ((uVar10 & uVar10 - 1) != 0)) {
          uVar29 = FUN_00f76630();
        }
        else if (1 < uVar29) {
          uVar29 = 1L << (-LZCOUNT(uVar29 - 1) & 0x3fU);
        }
        if (uVar21 <= uVar29) {
          uVar21 = uVar29;
        }
        if (uVar21 < uVar10) goto LAB_043268b8;
      }
      uVar10 = plVar24[5];
      if ((uVar10 & uVar10 - 1) == 0) {
        uVar21 = (ulong)((int)uVar10 - 1U & uVar2);
      }
      else {
        uVar21 = uVar33;
        if (uVar10 <= uVar33) {
          uVar21 = 0;
          if (uVar10 != 0) {
            uVar21 = uVar33 / uVar10;
          }
          uVar21 = uVar33 - uVar21 * uVar10;
        }
      }
    }
    lVar13 = plVar24[4];
    plVar18 = *(long **)(lVar13 + uVar21 * 8);
    if (plVar18 == (long *)0x0) {
      plVar18 = plVar24 + 6;
      *plVar15 = *plVar18;
      *plVar18 = (long)plVar15;
      *(long **)(lVar13 + uVar21 * 8) = plVar18;
      if (*plVar15 != 0) {
        uVar33 = *(ulong *)(*plVar15 + 8);
        if ((uVar10 & uVar10 - 1) == 0) {
          uVar33 = uVar33 & uVar10 - 1;
        }
        else if (uVar10 <= uVar33) {
          uVar21 = 0;
          if (uVar10 != 0) {
            uVar21 = uVar33 / uVar10;
          }
          uVar33 = uVar33 - uVar21 * uVar10;
        }
        *(long **)(plVar24[4] + uVar33 * 8) = plVar15;
      }
    }
    else {
      *plVar15 = *plVar18;
      *plVar18 = (long)plVar15;
    }
    plVar24[7] = plVar24[7] + 1;
  }
  else {
    uVar29 = uVar10 - 1;
    uVar27 = (uint)uVar10;
    if ((uVar10 & uVar29) == 0) {
      uVar21 = (ulong)(uVar27 - 1 & uVar2);
    }
    else {
      uVar21 = uVar33;
      if (uVar10 <= uVar33) {
        uVar4 = 0;
        if (uVar27 != 0) {
          uVar4 = uVar2 / uVar27;
        }
        uVar21 = (ulong)(uVar2 - uVar4 * uVar27);
      }
    }
    plVar15 = *(long **)(plVar24[4] + uVar21 * 8);
    if (plVar15 == (long *)0x0) goto LAB_04326834;
    do {
      while( true ) {
        plVar15 = (long *)*plVar15;
        if (plVar15 == (long *)0x0) goto LAB_04326834;
        uVar16 = plVar15[1];
        if (uVar16 == uVar33) break;
        if ((uVar10 & uVar29) == 0) {
          uVar16 = uVar16 & uVar29;
        }
        else if (uVar10 <= uVar16) {
          uVar34 = 0;
          if (uVar10 != 0) {
            uVar34 = uVar16 / uVar10;
          }
          uVar16 = uVar16 - uVar34 * uVar10;
        }
        if (uVar16 != uVar21) goto LAB_04326834;
      }
    } while (*(uint *)(plVar15 + 2) != uVar2);
  }
  lVar13 = lVar8 + 0x10;
  lVar26 = (long)*(char *)(lVar8 + 0x27);
  if (lVar26 < 0) {
    lVar13 = *(long *)(lVar8 + 0x10);
    lVar26 = *(long *)(lVar8 + 0x18);
  }
  uVar10 = puVar31[4];
  uVar33 = puVar31[5];
  puVar12 = (undefined8 *)puVar31[7];
  uVar29 = puVar31[7];
  uVar21 = puVar31[6];
  local_78 = uVar10;
  if ((code *)*puVar12 != (code *)0x0) {
    uVar10 = (*(code *)*puVar12)(uVar10);
    local_78 = uVar10;
    if ((code *)*puVar12 != (code *)0x0) {
      local_78 = (*(code *)*puVar12)(uVar10);
    }
  }
  uVar2 = *(uint *)(uVar22 + 0x90);
  uVar16 = (ulong)uVar2;
  uVar34 = *(ulong *)(uVar22 + 0x70);
  puVar30 = puVar12;
  if (uVar34 == 0) {
    uVar32 = 0;
  }
  else {
    uVar14 = uVar34 - 1;
    uVar27 = (uint)uVar34;
    if ((uVar34 & uVar14) == 0) {
      uVar32 = (ulong)(uVar27 - 1 & uVar2);
    }
    else {
      uVar32 = uVar16;
      if (uVar34 <= uVar16) {
        uVar4 = 0;
        if (uVar27 != 0) {
          uVar4 = uVar2 / uVar27;
        }
        uVar32 = (ulong)(uVar2 - uVar4 * uVar27);
      }
    }
    plVar24 = *(long **)(*(long *)(uVar22 + 0x68) + uVar32 * 8);
    if (plVar24 != (long *)0x0) {
      do {
        while( true ) {
          plVar24 = (long *)*plVar24;
          if (plVar24 == (long *)0x0) goto LAB_04326a50;
          uVar17 = plVar24[1];
          if (uVar17 == uVar16) break;
          if ((uVar34 & uVar14) == 0) {
            uVar17 = uVar17 & uVar14;
          }
          else if (uVar34 <= uVar17) {
            uVar5 = 0;
            if (uVar34 != 0) {
              uVar5 = uVar17 / uVar34;
            }
            uVar17 = uVar17 - uVar5 * uVar34;
          }
          if (uVar17 != uVar32) goto LAB_04326a50;
        }
      } while (*(uint *)(plVar24 + 2) != uVar2);
      goto LAB_04326c14;
    }
  }
LAB_04326a50:
  plVar24 = (long *)FUN_00008790(0x48);
  if (plVar24 == (long *)0x0) {
LAB_04326da0:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x4326da4);
    (*pcVar6)();
  }
  *plVar24 = 0;
  plVar24[1] = uVar16;
  if (plVar24 == (long *)0xfffffffffffffff0) goto LAB_04326da0;
  *(undefined4 *)(plVar24 + 2) = *(undefined4 *)(uVar22 + 0x90);
  plVar24[3] = lVar13;
  plVar24[4] = lVar26;
  plVar24[5] = local_78;
  plVar24[6] = uVar33;
  plVar24[8] = uVar29;
  plVar24[7] = uVar21;
  fVar35 = (float)(*(long *)(uVar22 + 0x80) + 1);
  if (puVar12[1] != 0) {
    puVar30 = &DAT_0c4b3500;
  }
  if (*(float *)(uVar22 + 0x88) * (float)uVar34 < fVar35) {
    uVar33 = (ulong)((uVar34 & uVar34 - 1) != 0);
    if (uVar34 < 3) {
      uVar33 = 1;
    }
    uVar33 = uVar33 | uVar34 << 1;
    uVar21 = (ulong)(fVar35 / *(float *)(uVar22 + 0x88));
    if (uVar33 <= uVar21) {
      uVar33 = uVar21;
    }
    if (uVar33 - 1 == 0) {
      uVar33 = 2;
    }
    else if ((uVar33 & uVar33 - 1) != 0) {
      uVar33 = FUN_00f76630(uVar33);
      uVar34 = *(ulong *)(uVar22 + 0x70);
    }
    if (uVar34 < uVar33) {
LAB_04326b18:
      if (uVar33 >> 0x3d != 0) {
LAB_04326eac:
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      uVar11 = FUN_00008790(uVar33 << 3);
      bVar20 = false;
LAB_04326b2c:
      lVar8 = *(long *)(uVar22 + 0x68);
      *(undefined8 *)(uVar22 + 0x68) = uVar11;
      if (lVar8 != 0) {
        FUN_00005810(lVar8,*(long *)(uVar22 + 0x70) << 3);
      }
      *(ulong *)(uVar22 + 0x70) = uVar33;
      if (!bVar20) {
        uVar21 = 0;
        do {
          *(undefined8 *)(*(long *)(uVar22 + 0x68) + uVar21 * 8) = 0;
          uVar21 = uVar21 + 1;
        } while (uVar33 != uVar21);
        plVar15 = *(long **)(uVar22 + 0x78);
        if (plVar15 != (long *)0x0) {
          uVar21 = plVar15[1];
          uVar29 = uVar33 - 1;
          if ((uVar33 & uVar29) == 0) {
            uVar21 = uVar21 & uVar29;
          }
          else if (uVar33 <= uVar21) {
            uVar34 = 0;
            if (uVar33 != 0) {
              uVar34 = uVar21 / uVar33;
            }
            uVar21 = uVar21 - uVar34 * uVar33;
          }
          *(undefined8 **)(*(long *)(uVar22 + 0x68) + uVar21 * 8) = (undefined8 *)(uVar22 + 0x78);
          plVar18 = (long *)*plVar15;
          while (plVar18 != (long *)0x0) {
            uVar34 = plVar18[1];
            if ((uVar33 & uVar29) == 0) {
              uVar34 = uVar34 & uVar29;
            }
            else if (uVar33 <= uVar34) {
              uVar32 = 0;
              if (uVar33 != 0) {
                uVar32 = uVar34 / uVar33;
              }
              uVar34 = uVar34 - uVar32 * uVar33;
            }
            plVar19 = plVar18;
            if (uVar34 != uVar21) {
              if (*(long *)(*(long *)(uVar22 + 0x68) + uVar34 * 8) == 0) {
                *(long **)(*(long *)(uVar22 + 0x68) + uVar34 * 8) = plVar15;
                uVar21 = uVar34;
              }
              else {
                *plVar15 = *plVar18;
                *plVar18 = **(undefined8 **)(*(long *)(uVar22 + 0x68) + uVar34 * 8);
                **(long **)(*(long *)(uVar22 + 0x68) + uVar34 * 8) = (long)plVar18;
                plVar19 = plVar15;
              }
            }
            plVar15 = plVar19;
            plVar18 = (long *)*plVar19;
          }
        }
      }
    }
    else if (uVar33 < uVar34) {
      uVar21 = (ulong)((float)*(ulong *)(uVar22 + 0x80) / *(float *)(uVar22 + 0x88));
      if ((uVar34 < 3) || ((uVar34 & uVar34 - 1) != 0)) {
        uVar21 = FUN_00f76630();
      }
      else if (1 < uVar21) {
        uVar21 = 1L << (-LZCOUNT(uVar21 - 1) & 0x3fU);
      }
      if (uVar33 <= uVar21) {
        uVar33 = uVar21;
      }
      if (uVar33 < uVar34) {
        if (uVar33 != 0) goto LAB_04326b18;
        uVar11 = 0;
        bVar20 = true;
        goto LAB_04326b2c;
      }
    }
    uVar34 = *(ulong *)(uVar22 + 0x70);
    if ((uVar34 & uVar34 - 1) == 0) {
      uVar32 = (ulong)((int)uVar34 - 1U & uVar2);
    }
    else {
      uVar32 = uVar16;
      if (uVar34 <= uVar16) {
        uVar33 = 0;
        if (uVar34 != 0) {
          uVar33 = uVar16 / uVar34;
        }
        uVar32 = uVar16 - uVar33 * uVar34;
      }
    }
  }
  lVar8 = *(long *)(uVar22 + 0x68);
  plVar15 = *(long **)(lVar8 + uVar32 * 8);
  if (plVar15 == (long *)0x0) {
    plVar15 = (long *)(uVar22 + 0x78);
    lVar13 = *plVar15;
    *plVar24 = lVar13;
    *plVar15 = (long)plVar24;
    *(long **)(lVar8 + uVar32 * 8) = plVar15;
    if (lVar13 != 0) {
      uVar33 = *(ulong *)(lVar13 + 8);
      if ((uVar34 & uVar34 - 1) == 0) {
        uVar33 = uVar33 & uVar34 - 1;
      }
      else if (uVar34 <= uVar33) {
        uVar21 = 0;
        if (uVar34 != 0) {
          uVar21 = uVar33 / uVar34;
        }
        uVar33 = uVar33 - uVar21 * uVar34;
      }
      *(long **)(*(long *)(uVar22 + 0x68) + uVar33 * 8) = plVar24;
    }
  }
  else {
    *plVar24 = *plVar15;
    *plVar15 = (long)plVar24;
  }
  *(long *)(uVar22 + 0x80) = *(long *)(uVar22 + 0x80) + 1;
LAB_04326c14:
  if ((code *)puVar30[1] != (code *)0x0) {
    (*(code *)puVar30[1])(local_78);
  }
  if ((code *)puVar12[1] != (code *)0x0) {
    (*(code *)puVar12[1])(uVar10);
  }
  *(undefined4 *)(param_1 + 1) = *(undefined4 *)(uVar22 + 0x90);
  *param_1 = 1;
  return;
}


