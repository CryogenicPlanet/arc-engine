// ==== 0x42e72fc  FUN_042e72fc  (2044 bytes)

void FUN_042e72fc(long param_1,long *param_2,undefined8 *param_3)

{
  size_t sVar1;
  size_t sVar2;
  undefined8 *******pppppppuVar3;
  byte bVar4;
  long lVar5;
  ulong uVar6;
  int iVar7;
  long lVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  ulong uVar11;
  int *piVar12;
  undefined4 *puVar13;
  int *piVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  code *pcVar17;
  undefined8 *puVar18;
  long *plVar19;
  ulong uVar20;
  long *plVar21;
  long *plVar22;
  long *plVar23;
  int *piVar24;
  bool bVar25;
  ulong uVar26;
  ulong uVar27;
  ulong uVar28;
  ulong uVar29;
  float fVar30;
  undefined8 ******local_d0;
  long lStack_c8;
  undefined8 local_c0;
  undefined8 ******local_b0;
  size_t sStack_a8;
  undefined8 local_a0;
  undefined8 *local_98;
  ulong local_90;
  ulong local_88;
  undefined8 *local_78;
  ulong local_70;
  long local_68;
  
  if (*(long *)(param_1 + 0x68) == 0) {
    return;
  }
  lVar8 = FUN_042dda40(param_1 + 0x98);
  if (lVar8 != 0) {
    if (*(long *)(lVar8 + 0x30) == *(long *)(lVar8 + 0x38)) {
      puVar15 = (undefined8 *)FUN_042bfa68((long *)(lVar8 + 0x28),param_3);
    }
    else {
      puVar15 = (undefined8 *)(*(long *)(lVar8 + 0x28) + *(long *)(lVar8 + 0x30) * 0x20);
      if (puVar15 == (undefined8 *)0x0) goto LAB_042e7490;
      uVar9 = *param_3;
      puVar15[1] = param_3[1];
      *puVar15 = uVar9;
      puVar15[2] = param_3[2];
      puVar16 = (undefined8 *)param_3[3];
      puVar15[3] = puVar16;
      pcVar17 = (code *)*puVar16;
      if (pcVar17 != (code *)0x0) {
        uVar9 = (*pcVar17)(*param_3);
        *puVar15 = uVar9;
      }
      puVar15 = puVar15 + 4;
    }
    lVar5 = (long)puVar15 - *(long *)(lVar8 + 0x28);
    *(long *)(lVar8 + 0x30) = lVar5 >> 5;
    if (lVar5 != 0) {
      return;
    }
LAB_042e7490:
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(1,0x42e7494);
    (*pcVar17)();
  }
  local_78 = (undefined8 *)0x0;
  local_70 = 0;
  local_68 = 0;
  puVar16 = (undefined8 *)FUN_042bfa68(&local_78,param_3);
  puVar15 = local_78;
  lVar8 = (long)puVar16 - (long)local_78;
  uVar26 = lVar8 >> 5;
  local_70 = uVar26;
  if (lVar8 == 0) goto LAB_042e7490;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_b0,*param_2,param_2[1]);
  }
  else {
    sStack_a8 = param_2[1];
    local_b0 = (undefined8 ******)*param_2;
    local_a0 = param_2[2];
  }
  local_98 = (undefined8 *)0x0;
  local_90 = 0;
  local_88 = 0;
  if (uVar26 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  puVar10 = (undefined8 *)FUN_00008790(lVar8);
  local_90 = 0;
  local_98 = puVar10;
  local_88 = uVar26;
  do {
    if (puVar10 == (undefined8 *)0x0) goto LAB_042e7490;
    uVar9 = *puVar15;
    puVar10[1] = puVar15[1];
    *puVar10 = uVar9;
    puVar10[2] = puVar15[2];
    puVar18 = (undefined8 *)puVar15[3];
    puVar10[3] = puVar18;
    pcVar17 = (code *)*puVar18;
    if (pcVar17 != (code *)0x0) {
      uVar9 = (*pcVar17)(*puVar15);
      *puVar10 = uVar9;
    }
    puVar18 = local_98;
    puVar15 = puVar15 + 4;
    puVar10 = puVar10 + 4;
  } while (puVar15 != puVar16);
  uVar26 = (long)puVar10 - (long)local_98 >> 5;
  pppppppuVar3 = (undefined8 *******)local_b0;
  if (-1 < (long)local_a0._7_1_) {
    pppppppuVar3 = &local_b0;
  }
  sVar1 = sStack_a8;
  if (-1 < (long)local_a0) {
    sVar1 = (long)local_a0._7_1_;
  }
  local_90 = uVar26;
  uVar11 = FUN_00018f3c(pppppppuVar3,sVar1);
  uVar27 = *(ulong *)(param_1 + 0xa0);
  if (uVar27 == 0) {
    uVar28 = 0;
  }
  else {
    uVar29 = uVar27 - 1;
    if ((uVar27 & uVar29) == 0) {
      uVar28 = uVar29 & uVar11;
    }
    else {
      uVar28 = uVar11;
      if (uVar27 <= uVar11) {
        uVar28 = 0;
        if (uVar27 != 0) {
          uVar28 = uVar11 / uVar27;
        }
        uVar28 = uVar11 - uVar28 * uVar27;
      }
    }
    plVar19 = *(long **)(*(long *)(param_1 + 0x98) + uVar28 * 8);
    if (plVar19 != (long *)0x0) {
      for (plVar19 = (long *)*plVar19; plVar19 != (long *)0x0; plVar19 = (long *)*plVar19) {
        uVar20 = plVar19[1];
        if (uVar20 == uVar11) {
          bVar4 = *(byte *)((long)plVar19 + 0x27);
          sVar2 = plVar19[3];
          if (-1 < (char)bVar4) {
            sVar2 = (ulong)bVar4;
          }
          if (sVar2 == sVar1) {
            puVar15 = (void *)plVar19[2];
            if (-1 < (char)bVar4) {
              puVar15 = plVar19 + 2;
            }
            iVar7 = _memcmp(puVar15,pppppppuVar3,sVar1);
            if (iVar7 == 0) goto LAB_042e7744;
          }
        }
        else {
          if ((uVar27 & uVar29) == 0) {
            uVar20 = uVar20 & uVar29;
          }
          else if (uVar27 <= uVar20) {
            uVar6 = 0;
            if (uVar27 != 0) {
              uVar6 = uVar20 / uVar27;
            }
            uVar20 = uVar20 - uVar6 * uVar27;
          }
          if (uVar20 != uVar28) break;
        }
      }
    }
  }
  plVar19 = (long *)FUN_00008790(0x40);
  uVar29 = local_88;
  if (plVar19 == (long *)0x0) goto LAB_042e7490;
  *plVar19 = 0;
  plVar19[1] = uVar11;
  if (plVar19 + 2 == (long *)0x0) goto LAB_042e7490;
  plVar19[4] = local_a0;
  plVar19[3] = sStack_a8;
  plVar19[2] = (long)local_b0;
  sStack_a8 = 0;
  local_b0 = (undefined8 *******)0x0;
  local_98 = (undefined8 *)0x0;
  local_a0 = 0;
  plVar19[5] = (long)puVar18;
  plVar19[6] = uVar26;
  local_90 = 0;
  local_88 = 0;
  fVar30 = (float)(*(long *)(param_1 + 0xb0) + 1);
  plVar19[7] = uVar29;
  if (fVar30 <= *(float *)(param_1 + 0xb8) * (float)uVar27) goto LAB_042e76e0;
  uVar26 = (ulong)((uVar27 & uVar27 - 1) != 0);
  if (uVar27 < 3) {
    uVar26 = 1;
  }
  uVar26 = uVar26 | uVar27 << 1;
  uVar28 = (ulong)(fVar30 / *(float *)(param_1 + 0xb8));
  if (uVar26 <= uVar28) {
    uVar26 = uVar28;
  }
  if (uVar26 - 1 == 0) {
    uVar26 = 2;
  }
  else if ((uVar26 & uVar26 - 1) != 0) {
    uVar26 = FUN_00f76630(uVar26);
    uVar27 = *(ulong *)(param_1 + 0xa0);
  }
  if (uVar27 < uVar26) {
LAB_042e7660:
    if (uVar26 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b087f0();
    }
    uVar9 = FUN_00008790(uVar26 << 3);
    bVar25 = false;
LAB_042e7674:
    lVar8 = *(long *)(param_1 + 0x98);
    *(undefined8 *)(param_1 + 0x98) = uVar9;
    if (lVar8 != 0) {
      FUN_00005810(lVar8,*(long *)(param_1 + 0xa0) << 3);
    }
    *(ulong *)(param_1 + 0xa0) = uVar26;
    if (!bVar25) {
      uVar27 = 0;
      do {
        *(undefined8 *)(*(long *)(param_1 + 0x98) + uVar27 * 8) = 0;
        uVar27 = uVar27 + 1;
      } while (uVar26 != uVar27);
      plVar21 = *(long **)(param_1 + 0xa8);
      if (plVar21 != (long *)0x0) {
        uVar27 = plVar21[1];
        uVar28 = uVar26 - 1;
        if ((uVar26 & uVar28) == 0) {
          uVar27 = uVar27 & uVar28;
        }
        else if (uVar26 <= uVar27) {
          uVar29 = 0;
          if (uVar26 != 0) {
            uVar29 = uVar27 / uVar26;
          }
          uVar27 = uVar27 - uVar29 * uVar26;
        }
        *(undefined8 **)(*(long *)(param_1 + 0x98) + uVar27 * 8) = (undefined8 *)(param_1 + 0xa8);
        plVar23 = (long *)*plVar21;
        while (plVar23 != (long *)0x0) {
          uVar29 = plVar23[1];
          if ((uVar26 & uVar28) == 0) {
            uVar29 = uVar29 & uVar28;
          }
          else if (uVar26 <= uVar29) {
            uVar20 = 0;
            if (uVar26 != 0) {
              uVar20 = uVar29 / uVar26;
            }
            uVar29 = uVar29 - uVar20 * uVar26;
          }
          plVar22 = plVar23;
          if (uVar29 != uVar27) {
            if (*(long *)(*(long *)(param_1 + 0x98) + uVar29 * 8) == 0) {
              *(long **)(*(long *)(param_1 + 0x98) + uVar29 * 8) = plVar21;
              uVar27 = uVar29;
            }
            else {
              *plVar21 = *plVar23;
              *plVar23 = **(undefined8 **)(*(long *)(param_1 + 0x98) + uVar29 * 8);
              **(long **)(*(long *)(param_1 + 0x98) + uVar29 * 8) = (long)plVar23;
              plVar22 = plVar21;
            }
          }
          plVar21 = plVar22;
          plVar23 = (long *)*plVar22;
        }
      }
    }
  }
  else if (uVar26 < uVar27) {
    uVar28 = (ulong)((float)*(ulong *)(param_1 + 0xb0) / *(float *)(param_1 + 0xb8));
    if ((uVar27 < 3) || ((uVar27 & uVar27 - 1) != 0)) {
      uVar28 = FUN_00f76630();
    }
    else if (1 < uVar28) {
      uVar28 = 1L << (-LZCOUNT(uVar28 - 1) & 0x3fU);
    }
    if (uVar26 <= uVar28) {
      uVar26 = uVar28;
    }
    if (uVar26 < uVar27) {
      if (uVar26 != 0) goto LAB_042e7660;
      uVar9 = 0;
      bVar25 = true;
      goto LAB_042e7674;
    }
  }
  uVar27 = *(ulong *)(param_1 + 0xa0);
  if ((uVar27 & uVar27 - 1) == 0) {
    uVar28 = uVar27 - 1 & uVar11;
  }
  else {
    uVar28 = uVar11;
    if (uVar27 <= uVar11) {
      uVar26 = 0;
      if (uVar27 != 0) {
        uVar26 = uVar11 / uVar27;
      }
      uVar28 = uVar11 - uVar26 * uVar27;
    }
  }
LAB_042e76e0:
  lVar8 = *(long *)(param_1 + 0x98);
  plVar21 = *(long **)(lVar8 + uVar28 * 8);
  if (plVar21 == (long *)0x0) {
    plVar21 = (long *)(param_1 + 0xa8);
    *plVar19 = *plVar21;
    *plVar21 = (long)plVar19;
    *(long **)(lVar8 + uVar28 * 8) = plVar21;
    if (*plVar19 != 0) {
      uVar26 = *(ulong *)(*plVar19 + 8);
      if ((uVar27 & uVar27 - 1) == 0) {
        uVar26 = uVar26 & uVar27 - 1;
      }
      else if (uVar27 <= uVar26) {
        uVar11 = 0;
        if (uVar27 != 0) {
          uVar11 = uVar26 / uVar27;
        }
        uVar26 = uVar26 - uVar11 * uVar27;
      }
      *(long **)(*(long *)(param_1 + 0x98) + uVar26 * 8) = plVar19;
    }
  }
  else {
    *plVar19 = *plVar21;
    *plVar21 = (long)plVar19;
  }
  *(long *)(param_1 + 0xb0) = *(long *)(param_1 + 0xb0) + 1;
  puVar18 = local_98;
LAB_042e7744:
  if (puVar18 != (undefined8 *)0x0) {
    puVar15 = puVar18;
    if ((local_90 & 0x7ffffffffffffff) != 0) {
      lVar8 = local_90 * -0x20;
      puVar16 = puVar18 + local_90 * 4;
      do {
        puVar10 = puVar16 + -4;
        if (puVar10 == (undefined8 *)0x0) goto LAB_042e7490;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar10);
        }
        lVar8 = lVar8 + 0x20;
        puVar15 = local_98;
        puVar16 = puVar10;
      } while (lVar8 != 0);
    }
    local_90 = (long)puVar18 - (long)puVar15 >> 5;
    FUN_00005810(puVar15,local_88 << 5);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  piVar12 = (int *)FUN_06b38910(*(undefined8 *)(param_1 + 0x68));
  *(undefined1 *)((long)piVar12 + 0x39) = 1;
  piVar24 = *(int **)(param_1 + 0x130);
  if ((piVar24 != (int *)0x0) && (iVar7 = *piVar24, *piVar24 = iVar7 + 1, iVar7 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7af8);
    (*pcVar17)();
  }
  uVar9 = *(undefined8 *)(param_1 + 0x138);
  puVar13 = (undefined4 *)FUN_00008790(0x40);
  *puVar13 = 1;
  *(undefined1 **)(puVar13 + 2) = &LAB_041c9668;
  *(code **)(puVar13 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar13 + 6) = &LAB_04169bc0;
  *(code **)(puVar13 + 8) = FUN_042e7b00;
  *(undefined8 *)(puVar13 + 10) = 0;
  *(int **)(puVar13 + 0xc) = piVar24;
  *(undefined8 *)(puVar13 + 0xe) = uVar9;
  piVar24 = *(int **)(piVar12 + 0x1c);
  *(undefined4 **)(piVar12 + 0x1c) = puVar13;
  if (piVar24 != (int *)0x0) {
    LOAcquire();
    iVar7 = *piVar24;
    *piVar24 = iVar7 + -1;
    LORelease();
    if (iVar7 == 1) {
      if (*(code **)(piVar24 + 4) == FUN_039a66c0) {
        piVar14 = *(int **)(piVar24 + 0xc);
        if (piVar14 != (int *)0x0) {
          LOAcquire();
          iVar7 = *piVar14;
          *piVar14 = iVar7 + -1;
          LORelease();
          if (iVar7 == 1) {
            FUN_00005810(piVar14,8);
          }
        }
        FUN_00005810(piVar24,0x48);
      }
      else {
        (**(code **)(piVar24 + 4))(piVar24);
      }
    }
  }
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_d0,*param_2,param_2[1]);
  }
  else {
    lStack_c8 = param_2[1];
    local_d0 = (undefined8 ******)*param_2;
    local_c0 = param_2[2];
  }
  pppppppuVar3 = (undefined8 *******)local_d0;
  if (-1 < (long)local_c0._7_1_) {
    pppppppuVar3 = &local_d0;
  }
  lVar8 = lStack_c8;
  if (-1 < (long)local_c0) {
    lVar8 = (long)local_c0._7_1_;
  }
  FUN_008a7fc4(&local_b0,pppppppuVar3,lVar8);
  FUN_06b38c20(piVar12,&local_b0);
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  if ((long)local_c0 < 0) {
    FUN_00005810(local_d0,local_c0 & 0x7fffffffffffffff);
  }
  if (piVar12 != (int *)0x0) {
    LOAcquire();
    iVar7 = *piVar12;
    *piVar12 = iVar7 + -1;
    LORelease();
    if (iVar7 == 1) {
      FUN_06b39bc0(piVar12);
    }
  }
  puVar15 = local_78;
  if (local_78 != (undefined8 *)0x0) {
    if ((local_70 & 0x7ffffffffffffff) != 0) {
      lVar8 = local_70 * -0x20;
      puVar16 = local_78 + local_70 * 4;
      do {
        puVar10 = puVar16 + -4;
        if (puVar10 == (undefined8 *)0x0) goto LAB_042e7490;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar10);
        }
        lVar8 = lVar8 + 0x20;
        puVar16 = puVar10;
      } while (lVar8 != 0);
    }
    FUN_00005810(puVar15,local_68 << 5);
  }
  return;
}


