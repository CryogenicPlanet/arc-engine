// ==== 0x4355df8  FUN_04355df8  (1732 bytes)

void FUN_04355df8(undefined8 *param_1,long param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  ulong uVar3;
  code *pcVar4;
  int iVar5;
  ulong uVar6;
  undefined8 *puVar7;
  int *piVar8;
  long *plVar9;
  undefined8 uVar10;
  long *plVar11;
  char *pcVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  long lVar15;
  ulong uVar16;
  undefined8 *puVar17;
  long lVar18;
  ulong uVar19;
  long *plVar20;
  long *plVar21;
  undefined8 *puVar22;
  bool bVar23;
  ulong uVar24;
  ulong uVar25;
  float fVar26;
  undefined8 uVar27;
  
  uVar6 = FUN_020cf42c(0x2c);
  if ((uVar6 & 1) == 0) {
    pcVar12 = "DevTools protocol connections are disabled";
    uVar10 = 0x25;
    uVar13 = 0x2a;
    uVar14 = 0x80;
LAB_043563a0:
    uVar10 = FUN_04b37104(uVar10,pcVar12,uVar13,uVar14,
                          "../../arc/browser/web_contents/devtools_impl.cc");
    *param_1 = uVar10;
    return;
  }
  if (((*(long *)(param_2 + 0x50) == 0) || (*(char *)(*(long *)(param_2 + 0x50) + 4) != '\0')) ||
     (*(long *)(param_2 + 0x58) == 0)) {
    pcVar12 = "Associated WebContents is not available";
    uVar10 = 0x39;
    uVar13 = 0x27;
    uVar14 = 0x84;
    goto LAB_043563a0;
  }
  puVar7 = (undefined8 *)FUN_00008790(0x48);
  uVar10 = *param_3;
  uVar13 = param_3[1];
  puVar22 = (undefined8 *)param_3[3];
  uVar27 = param_3[3];
  uVar14 = param_3[2];
  if (puVar22[1] != 0) {
    param_3[2] = 0;
    param_3[3] = &DAT_0c4b3500;
    if (puVar22[1] != 0) {
      puVar22 = &DAT_0c4b3500;
    }
  }
  *puVar7 = &PTR_FUN_0da804e0;
  puVar7[1] = uVar10;
  puVar7[2] = uVar13;
  puVar7[4] = uVar27;
  puVar7[3] = uVar14;
  puVar7[5] = 0;
  *(undefined1 *)(puVar7 + 6) = 0;
  piVar8 = (int *)FUN_00008790(8);
  *piVar8 = 0;
  *(undefined1 *)(piVar8 + 1) = 0;
  if ((piVar8 != (int *)0x0) && (iVar5 = *piVar8, *piVar8 = iVar5 + 1, iVar5 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0,0x4356470);
    (*pcVar4)();
  }
  puVar7[7] = piVar8;
  puVar7[8] = puVar7;
  if ((code *)puVar22[1] != (code *)0x0) {
    (*(code *)puVar22[1])(uVar10);
  }
  lVar15 = 0;
  if (*(long *)(param_2 + 0x50) != 0) {
    if (*(char *)(*(long *)(param_2 + 0x50) + 4) == '\0') {
      lVar15 = *(long *)(param_2 + 0x58);
    }
    else {
      lVar15 = 0;
    }
  }
  if (lVar15 + 0x398 == 0) {
    plVar9 = (long *)0x0;
LAB_043561f4:
    bVar23 = true;
  }
  else {
    plVar9 = (long *)FUN_017d3e6c(lVar15 + 0x398);
    if (plVar9 == (long *)0x0) goto LAB_043561f4;
    puVar1 = (undefined8 *)plVar9[8];
    lVar15 = plVar9[9] << 4;
    puVar22 = puVar1;
    if (lVar15 != 0) {
      uVar6 = lVar15 >> 4;
      puVar17 = puVar1;
      do {
        uVar24 = uVar6 >> 1;
        puVar22 = puVar17 + uVar24 * 2 + 2;
        uVar6 = uVar6 + ~(uVar6 >> 1);
        if (puVar7 <= (undefined8 *)puVar17[uVar24 * 2]) {
          puVar22 = puVar17;
          uVar6 = uVar24;
        }
        puVar17 = puVar22;
      } while (uVar6 != 0);
    }
    if (((puVar22 == puVar1 + plVar9[9] * 2) || (puVar7 < (undefined8 *)*puVar22)) ||
       (puVar22[1] == 0)) {
      uVar2 = *(undefined4 *)(*plVar9 + -0x14);
      uVar10 = FUN_00008790(0x1e0);
      FUN_02be6474(uVar10,puVar7,uVar2);
      uVar6 = FUN_0247ab3c(plVar9,uVar10);
      if ((uVar6 & 1) != 0) {
        plVar11 = (long *)puVar7[5];
        puVar7[5] = plVar9;
        if (plVar11 != (long *)0x0) {
          lVar15 = plVar11[1];
          iVar5 = (int)lVar15 + -1;
          *(int *)(plVar11 + 1) = iVar5;
          if ((int)lVar15 == 0) {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(0,0x4356460);
            (*pcVar4)();
          }
          if (iVar5 == 0) {
            (**(code **)(*plVar11 + 0xe8))();
          }
        }
        iVar5 = *(int *)(param_2 + 0xf8) + 1;
        *(int *)(param_2 + 0xf8) = iVar5;
        uVar6 = (ulong)iVar5;
        uVar24 = *(ulong *)(param_2 + 0x108);
        if (uVar24 == 0) {
          uVar25 = 0;
LAB_04356018:
          plVar9 = (long *)FUN_00008790(0x20);
          if (plVar9 == (long *)0x0) {
LAB_04356464:
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(1,0x4356468);
            (*pcVar4)();
          }
          *plVar9 = 0;
          plVar9[1] = uVar6;
          if (plVar9 == (long *)0xfffffffffffffff0) goto LAB_04356464;
          *(int *)(plVar9 + 2) = iVar5;
          fVar26 = (float)(*(long *)(param_2 + 0x118) + 1);
          plVar9[3] = (long)puVar7;
          if (*(float *)(param_2 + 0x120) * (float)uVar24 < fVar26) {
            uVar25 = (ulong)((uVar24 & uVar24 - 1) != 0);
            if (uVar24 < 3) {
              uVar25 = 1;
            }
            uVar25 = uVar25 | uVar24 << 1;
            uVar16 = (ulong)(fVar26 / *(float *)(param_2 + 0x120));
            if (uVar25 <= uVar16) {
              uVar25 = uVar16;
            }
            if (uVar25 - 1 == 0) {
              uVar25 = 2;
            }
            else if ((uVar25 & uVar25 - 1) != 0) {
              uVar25 = FUN_00f76630(uVar25);
              uVar24 = *(ulong *)(param_2 + 0x108);
            }
            if (uVar24 < uVar25) {
LAB_04356128:
              if (uVar25 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
                FUN_04b087f0();
              }
              uVar10 = FUN_00008790(uVar25 << 3);
              bVar23 = false;
LAB_0435613c:
              lVar15 = *(long *)(param_2 + 0x100);
              *(undefined8 *)(param_2 + 0x100) = uVar10;
              if (lVar15 != 0) {
                FUN_00005810(lVar15,*(long *)(param_2 + 0x108) << 3);
              }
              *(ulong *)(param_2 + 0x108) = uVar25;
              if (!bVar23) {
                uVar24 = 0;
                do {
                  *(undefined8 *)(*(long *)(param_2 + 0x100) + uVar24 * 8) = 0;
                  uVar24 = uVar24 + 1;
                } while (uVar25 != uVar24);
                plVar11 = *(long **)(param_2 + 0x110);
                if (plVar11 != (long *)0x0) {
                  uVar24 = plVar11[1];
                  uVar16 = uVar25 - 1;
                  if ((uVar25 & uVar16) == 0) {
                    uVar24 = uVar24 & uVar16;
                  }
                  else if (uVar25 <= uVar24) {
                    uVar19 = 0;
                    if (uVar25 != 0) {
                      uVar19 = uVar24 / uVar25;
                    }
                    uVar24 = uVar24 - uVar19 * uVar25;
                  }
                  *(long *)(*(long *)(param_2 + 0x100) + uVar24 * 8) = param_2 + 0x110;
                  plVar20 = (long *)*plVar11;
                  while (plVar20 != (long *)0x0) {
                    uVar19 = plVar20[1];
                    if ((uVar25 & uVar16) == 0) {
                      uVar19 = uVar19 & uVar16;
                    }
                    else if (uVar25 <= uVar19) {
                      uVar3 = 0;
                      if (uVar25 != 0) {
                        uVar3 = uVar19 / uVar25;
                      }
                      uVar19 = uVar19 - uVar3 * uVar25;
                    }
                    plVar21 = plVar20;
                    if (uVar19 != uVar24) {
                      if (*(long *)(*(long *)(param_2 + 0x100) + uVar19 * 8) == 0) {
                        *(long **)(*(long *)(param_2 + 0x100) + uVar19 * 8) = plVar11;
                        uVar24 = uVar19;
                      }
                      else {
                        *plVar11 = *plVar20;
                        *plVar20 = **(undefined8 **)(*(long *)(param_2 + 0x100) + uVar19 * 8);
                        **(long **)(*(long *)(param_2 + 0x100) + uVar19 * 8) = (long)plVar20;
                        plVar21 = plVar11;
                      }
                    }
                    plVar11 = plVar21;
                    plVar20 = (long *)*plVar21;
                  }
                }
              }
            }
            else if (uVar25 < uVar24) {
              uVar16 = (ulong)((float)*(ulong *)(param_2 + 0x118) / *(float *)(param_2 + 0x120));
              if ((uVar24 < 3) || ((uVar24 & uVar24 - 1) != 0)) {
                uVar16 = FUN_00f76630();
              }
              else if (1 < uVar16) {
                uVar16 = 1L << (-LZCOUNT(uVar16 - 1) & 0x3fU);
              }
              if (uVar25 <= uVar16) {
                uVar25 = uVar16;
              }
              if (uVar25 < uVar24) {
                if (uVar25 != 0) goto LAB_04356128;
                uVar10 = 0;
                bVar23 = true;
                goto LAB_0435613c;
              }
            }
            uVar24 = *(ulong *)(param_2 + 0x108);
            if ((uVar24 & uVar24 - 1) == 0) {
              uVar25 = uVar24 - 1 & uVar6;
            }
            else {
              uVar25 = uVar6;
              if (uVar24 <= uVar6) {
                uVar25 = 0;
                if (uVar24 != 0) {
                  uVar25 = uVar6 / uVar24;
                }
                uVar25 = uVar6 - uVar25 * uVar24;
              }
            }
          }
          lVar15 = *(long *)(param_2 + 0x100);
          plVar11 = *(long **)(lVar15 + uVar25 * 8);
          if (plVar11 == (long *)0x0) {
            lVar18 = *(long *)(param_2 + 0x110);
            *plVar9 = lVar18;
            *(long **)(param_2 + 0x110) = plVar9;
            *(long *)(lVar15 + uVar25 * 8) = param_2 + 0x110;
            if (lVar18 != 0) {
              uVar6 = *(ulong *)(lVar18 + 8);
              if ((uVar24 & uVar24 - 1) == 0) {
                uVar6 = uVar6 & uVar24 - 1;
              }
              else if (uVar24 <= uVar6) {
                uVar25 = 0;
                if (uVar24 != 0) {
                  uVar25 = uVar6 / uVar24;
                }
                uVar6 = uVar6 - uVar25 * uVar24;
              }
              *(long **)(*(long *)(param_2 + 0x100) + uVar6 * 8) = plVar9;
            }
          }
          else {
            *plVar9 = *plVar11;
            *plVar11 = (long)plVar9;
          }
          puVar7 = (undefined8 *)0x0;
          *(long *)(param_2 + 0x118) = *(long *)(param_2 + 0x118) + 1;
        }
        else {
          uVar16 = uVar24 - 1;
          if ((uVar24 & uVar16) == 0) {
            uVar25 = uVar16 & uVar6;
          }
          else {
            uVar25 = uVar6;
            if (uVar24 <= uVar6) {
              uVar25 = 0;
              if (uVar24 != 0) {
                uVar25 = uVar6 / uVar24;
              }
              uVar25 = uVar6 - uVar25 * uVar24;
            }
          }
          plVar9 = *(long **)(*(long *)(param_2 + 0x100) + uVar25 * 8);
          if (plVar9 == (long *)0x0) goto LAB_04356018;
          do {
            while( true ) {
              plVar9 = (long *)*plVar9;
              if (plVar9 == (long *)0x0) goto LAB_04356018;
              uVar19 = plVar9[1];
              if (uVar19 == uVar6) break;
              if ((uVar24 & uVar16) == 0) {
                uVar19 = uVar19 & uVar16;
              }
              else if (uVar24 <= uVar19) {
                uVar3 = 0;
                if (uVar24 != 0) {
                  uVar3 = uVar19 / uVar24;
                }
                uVar19 = uVar19 - uVar3 * uVar24;
              }
              if (uVar19 != uVar25) goto LAB_04356018;
            }
          } while (*(int *)(plVar9 + 2) != iVar5);
        }
        *(int *)(param_1 + 1) = iVar5;
        uVar10 = 1;
        goto LAB_043562e4;
      }
    }
    bVar23 = false;
  }
  lVar15 = puVar7[4];
  puVar7[3] = 0;
  puVar7[4] = &DAT_0c4b3500;
  if (*(code **)(lVar15 + 8) != (code *)0x0) {
    (**(code **)(lVar15 + 8))(puVar7[1]);
  }
  if (!bVar23) {
    lVar15 = plVar9[1];
    iVar5 = (int)lVar15 + -1;
    *(int *)(plVar9 + 1) = iVar5;
    if ((int)lVar15 == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0x43564d0);
      (*pcVar4)();
    }
    if (iVar5 == 0) {
      (**(code **)(*plVar9 + 0xe8))(plVar9);
    }
  }
  uVar10 = FUN_04b37104(0x39,"Unable to attach DevTools client",0x20,0x88,
                        "../../arc/browser/web_contents/devtools_impl.cc");
LAB_043562e4:
  *param_1 = uVar10;
  if (puVar7 == (undefined8 *)0x0) {
    return;
  }
  FUN_0435c74c(puVar7);
  puVar7[8] = 0;
  if (puVar7[7] != 0) {
    *(undefined1 *)(puVar7[7] + 4) = 1;
    piVar8 = (int *)puVar7[7];
    if (piVar8 != (int *)0x0) {
      LOAcquire();
      iVar5 = *piVar8;
      *piVar8 = iVar5 + -1;
      LORelease();
      if (iVar5 == 1) {
        FUN_00005810(piVar8,8);
      }
    }
  }
  plVar9 = (long *)puVar7[5];
  if (plVar9 != (long *)0x0) {
    lVar15 = plVar9[1];
    iVar5 = (int)lVar15 + -1;
    *(int *)(plVar9 + 1) = iVar5;
    if ((int)lVar15 == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0x43564c4);
      (*pcVar4)();
    }
    if (iVar5 == 0) {
      (**(code **)(*plVar9 + 0xe8))();
    }
  }
  if (*(code **)(puVar7[4] + 8) != (code *)0x0) {
    (**(code **)(puVar7[4] + 8))(puVar7[1]);
  }
  FUN_00005810(puVar7,0x48);
  return;
}


