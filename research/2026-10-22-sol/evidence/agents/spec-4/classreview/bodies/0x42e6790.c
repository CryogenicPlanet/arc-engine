// ==== 0x42e6790  FUN_042e6790  (2420 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_042e6790(long param_1,undefined8 *param_2,int param_3,undefined8 *param_4)

{
  undefined8 *******pppppppuVar1;
  size_t sVar2;
  size_t sVar3;
  byte bVar4;
  long lVar5;
  ulong uVar6;
  undefined8 ******ppppppuVar7;
  int iVar8;
  long lVar9;
  undefined8 uVar10;
  undefined4 *puVar11;
  long *plVar12;
  undefined8 *puVar13;
  ulong uVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  code *pcVar17;
  undefined8 *puVar18;
  ulong uVar19;
  long *plVar20;
  long *plVar21;
  long *plVar22;
  long *plVar23;
  bool bVar24;
  int *piVar25;
  ulong uVar26;
  ulong uVar27;
  undefined8 *puVar28;
  ulong uVar29;
  ulong uVar30;
  ulong uVar31;
  float fVar32;
  undefined8 ******local_b0;
  size_t sStack_a8;
  undefined8 local_a0;
  long *local_98;
  undefined8 *local_90;
  ulong local_88;
  ulong local_80;
  undefined8 *local_78;
  ulong local_70;
  long local_68;
  
  if (*(long *)(param_1 + 0x68) == 0) {
    return;
  }
  lVar9 = FUN_042dda40(param_1 + 0x70);
  if (lVar9 != 0) {
    if (*(long *)(lVar9 + 0x38) == *(long *)(lVar9 + 0x40)) {
      puVar15 = (undefined8 *)FUN_042bfa68((long *)(lVar9 + 0x30),param_4);
    }
    else {
      puVar15 = (undefined8 *)(*(long *)(lVar9 + 0x30) + *(long *)(lVar9 + 0x38) * 0x20);
      if (puVar15 == (undefined8 *)0x0) goto LAB_042e70cc;
      uVar10 = *param_4;
      puVar15[1] = param_4[1];
      *puVar15 = uVar10;
      puVar15[2] = param_4[2];
      puVar16 = (undefined8 *)param_4[3];
      puVar15[3] = puVar16;
      pcVar17 = (code *)*puVar16;
      if (pcVar17 != (code *)0x0) {
        uVar10 = (*pcVar17)(*param_4);
        *puVar15 = uVar10;
      }
      puVar15 = puVar15 + 4;
    }
    lVar5 = (long)puVar15 - *(long *)(lVar9 + 0x30);
    *(long *)(lVar9 + 0x38) = lVar5 >> 5;
    if (lVar5 != 0) {
      return;
    }
LAB_042e70cc:
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(1,0x42e70d0);
    (*pcVar17)();
  }
  if (param_3 == 0) {
    puVar15 = (undefined8 *)FUN_00008790(8);
    *puVar15 = 0;
    piVar25 = *(int **)(param_1 + 0x130);
    if ((piVar25 != (int *)0x0) && (iVar8 = *piVar25, *piVar25 = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e710c);
      (*pcVar17)();
    }
    uVar10 = *(undefined8 *)(param_1 + 0x138);
    puVar11 = (undefined4 *)FUN_00008790(0x58);
    *puVar11 = 1;
    *(undefined1 **)(puVar11 + 2) = &LAB_042ec4c8;
    *(code **)(puVar11 + 4) = FUN_040b2960;
    *(undefined1 **)(puVar11 + 6) = &LAB_04169bc0;
    *(code **)(puVar11 + 8) = FUN_042e712c;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(int **)(puVar11 + 0xc) = piVar25;
    *(undefined8 *)(puVar11 + 0xe) = uVar10;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(puVar11 + 0x10,*param_2,param_2[1]);
    }
    else {
      uVar10 = *param_2;
      *(undefined8 *)(puVar11 + 0x12) = param_2[1];
      *(undefined8 *)(puVar11 + 0x10) = uVar10;
      *(undefined8 *)(puVar11 + 0x14) = param_2[2];
    }
    plVar12 = (long *)FUN_00008790(400);
    uVar10 = *(undefined8 *)(param_1 + 0x68);
    local_b0 = (undefined8 ******)FUN_00008790(8);
    *local_b0 = (undefined8 *****)0x0;
    *local_b0 = (undefined8 *****)*puVar15;
    sStack_a8 = 0;
    FUN_0786cd50(plVar12,param_2,uVar10,&local_b0,puVar11);
    ppppppuVar7 = local_b0;
    local_b0 = (undefined8 *******)0x0;
    if (ppppppuVar7 != (undefined8 ******)0x0) {
      _objc_destroyWeak(ppppppuVar7);
      FUN_00005810(ppppppuVar7,8);
    }
    if ((plVar12 != (long *)0x0) &&
       (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7124);
      (*pcVar17)();
    }
    if (puVar15 != (undefined8 *)0x0) {
      _objc_destroyWeak(puVar15);
      FUN_00005810(puVar15,8);
    }
  }
  else {
    piVar25 = *(int **)(param_1 + 0x130);
    if ((piVar25 != (int *)0x0) && (iVar8 = *piVar25, *piVar25 = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7100);
      (*pcVar17)();
    }
    uVar10 = *(undefined8 *)(param_1 + 0x138);
    puVar11 = (undefined4 *)FUN_00008790(0x58);
    *puVar11 = 1;
    *(undefined1 **)(puVar11 + 2) = &LAB_042ec4c8;
    *(code **)(puVar11 + 4) = FUN_040b2960;
    *(undefined1 **)(puVar11 + 6) = &LAB_04169bc0;
    *(code **)(puVar11 + 8) = FUN_042e712c;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(int **)(puVar11 + 0xc) = piVar25;
    *(undefined8 *)(puVar11 + 0xe) = uVar10;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(puVar11 + 0x10,*param_2,param_2[1]);
    }
    else {
      uVar10 = *param_2;
      *(undefined8 *)(puVar11 + 0x12) = param_2[1];
      *(undefined8 *)(puVar11 + 0x10) = uVar10;
      *(undefined8 *)(puVar11 + 0x14) = param_2[2];
    }
    plVar12 = (long *)FUN_00008790(400);
    uVar10 = *(undefined8 *)(param_1 + 0x68);
    local_b0 = (undefined8 ******)FUN_00008790(8);
    *local_b0 = (undefined8 *****)0x0;
    sStack_a8 = 0;
    FUN_0786cd50(plVar12,param_2,uVar10,&local_b0,puVar11);
    ppppppuVar7 = local_b0;
    local_b0 = (undefined8 *******)0x0;
    if (ppppppuVar7 != (undefined8 ******)0x0) {
      _objc_destroyWeak(ppppppuVar7);
      FUN_00005810(ppppppuVar7,8);
    }
    *plVar12 = (long)&PTR_FUN_0da7dfc0;
    plVar12[2] = (long)&PTR_LAB_0da7e048;
    *(undefined1 *)(plVar12 + 0x2d) = 0;
    if ((plVar12 != (long *)0x0) &&
       (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7118);
      (*pcVar17)();
    }
  }
  local_78 = (undefined8 *)0x0;
  local_70 = 0;
  local_68 = 0;
  puVar16 = (undefined8 *)FUN_042bfa68(&local_78,param_4);
  puVar15 = local_78;
  uVar30 = (long)puVar16 - (long)local_78 >> 5;
  local_70 = uVar30;
  if ((long)puVar16 - (long)local_78 == 0) goto LAB_042e70cc;
  if ((plVar12 != (long *)0x0) &&
     (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(0,0x42e70f4);
    (*pcVar17)();
  }
  if (uVar30 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  puVar13 = (undefined8 *)FUN_00008790();
  puVar28 = puVar13;
  do {
    if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
    uVar10 = *puVar15;
    puVar28[1] = puVar15[1];
    *puVar28 = uVar10;
    puVar28[2] = puVar15[2];
    puVar18 = (undefined8 *)puVar15[3];
    puVar28[3] = puVar18;
    pcVar17 = (code *)*puVar18;
    if (pcVar17 != (code *)0x0) {
      uVar10 = (*pcVar17)(*puVar15);
      *puVar28 = uVar10;
    }
    puVar15 = puVar15 + 4;
    puVar28 = puVar28 + 4;
  } while (puVar15 != puVar16);
  uVar26 = (long)puVar28 - (long)puVar13 >> 5;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_b0,*param_2,param_2[1]);
  }
  else {
    sStack_a8 = param_2[1];
    local_b0 = (undefined8 ******)*param_2;
    local_a0 = param_2[2];
  }
  pppppppuVar1 = (undefined8 *******)local_b0;
  if (-1 < (long)local_a0._7_1_) {
    pppppppuVar1 = &local_b0;
  }
  sVar2 = sStack_a8;
  if (-1 < (long)local_a0) {
    sVar2 = (long)local_a0._7_1_;
  }
  local_98 = plVar12;
  local_90 = puVar13;
  local_88 = uVar26;
  local_80 = uVar30;
  uVar14 = FUN_00018f3c(pppppppuVar1,sVar2);
  uVar29 = *(ulong *)(param_1 + 0x78);
  if (uVar29 == 0) {
    uVar31 = 0;
  }
  else {
    uVar27 = uVar29 - 1;
    if ((uVar29 & uVar27) == 0) {
      uVar31 = uVar27 & uVar14;
    }
    else {
      uVar31 = uVar14;
      if (uVar29 <= uVar14) {
        uVar31 = 0;
        if (uVar29 != 0) {
          uVar31 = uVar14 / uVar29;
        }
        uVar31 = uVar14 - uVar31 * uVar29;
      }
    }
    plVar20 = *(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8);
    if (plVar20 != (long *)0x0) {
      for (plVar20 = (long *)*plVar20; plVar20 != (long *)0x0; plVar20 = (long *)*plVar20) {
        uVar19 = plVar20[1];
        if (uVar19 == uVar14) {
          bVar4 = *(byte *)((long)plVar20 + 0x27);
          sVar3 = plVar20[3];
          if (-1 < (char)bVar4) {
            sVar3 = (ulong)bVar4;
          }
          if (sVar3 == sVar2) {
            puVar15 = (void *)plVar20[2];
            if (-1 < (char)bVar4) {
              puVar15 = plVar20 + 2;
            }
            iVar8 = _memcmp(puVar15,pppppppuVar1,sVar2);
            if (iVar8 == 0) goto LAB_042e6f54;
          }
        }
        else {
          if ((uVar29 & uVar27) == 0) {
            uVar19 = uVar19 & uVar27;
          }
          else if (uVar29 <= uVar19) {
            uVar6 = 0;
            if (uVar29 != 0) {
              uVar6 = uVar19 / uVar29;
            }
            uVar19 = uVar19 - uVar6 * uVar29;
          }
          if (uVar19 != uVar31) break;
        }
      }
    }
  }
  plVar20 = (long *)FUN_00008790(0x48);
  if (plVar20 == (long *)0x0) goto LAB_042e70cc;
  *plVar20 = 0;
  plVar20[1] = uVar14;
  if (plVar20 + 2 == (long *)0x0) goto LAB_042e70cc;
  plVar20[3] = sStack_a8;
  plVar20[2] = (long)local_b0;
  plVar20[4] = local_a0;
  local_b0 = (undefined8 *******)0x0;
  sStack_a8 = 0;
  local_98 = (long *)0x0;
  local_90 = (undefined8 *)0x0;
  plVar20[5] = (long)plVar12;
  plVar20[6] = (long)puVar13;
  local_88 = 0;
  local_80 = 0;
  local_a0 = 0;
  fVar32 = (float)(*(long *)(param_1 + 0x88) + 1);
  plVar20[7] = uVar26;
  plVar20[8] = uVar30;
  if (fVar32 <= *(float *)(param_1 + 0x90) * (float)uVar29) goto LAB_042e6f2c;
  uVar30 = (ulong)((uVar29 & uVar29 - 1) != 0);
  if (uVar29 < 3) {
    uVar30 = 1;
  }
  uVar30 = uVar30 | uVar29 << 1;
  uVar26 = (ulong)(fVar32 / *(float *)(param_1 + 0x90));
  if (uVar30 <= uVar26) {
    uVar30 = uVar26;
  }
  if (uVar30 - 1 == 0) {
    uVar30 = 2;
  }
  else if ((uVar30 & uVar30 - 1) != 0) {
    uVar30 = FUN_00f76630(uVar30);
    uVar29 = *(ulong *)(param_1 + 0x78);
  }
  if (uVar29 < uVar30) {
LAB_042e6e10:
    if (uVar30 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b087f0();
    }
    uVar10 = FUN_00008790(uVar30 << 3);
    bVar24 = false;
LAB_042e6e24:
    lVar9 = *(long *)(param_1 + 0x70);
    *(undefined8 *)(param_1 + 0x70) = uVar10;
    if (lVar9 != 0) {
      FUN_00005810(lVar9,*(long *)(param_1 + 0x78) << 3);
    }
    *(ulong *)(param_1 + 0x78) = uVar30;
    if (!bVar24) {
      uVar26 = 0;
      do {
        *(undefined8 *)(*(long *)(param_1 + 0x70) + uVar26 * 8) = 0;
        uVar26 = uVar26 + 1;
      } while (uVar30 != uVar26);
      plVar21 = *(long **)(param_1 + 0x80);
      if (plVar21 != (long *)0x0) {
        uVar26 = plVar21[1];
        uVar29 = uVar30 - 1;
        if ((uVar30 & uVar29) == 0) {
          uVar26 = uVar26 & uVar29;
        }
        else if (uVar30 <= uVar26) {
          uVar31 = 0;
          if (uVar30 != 0) {
            uVar31 = uVar26 / uVar30;
          }
          uVar26 = uVar26 - uVar31 * uVar30;
        }
        *(undefined8 **)(*(long *)(param_1 + 0x70) + uVar26 * 8) = (undefined8 *)(param_1 + 0x80);
        plVar22 = (long *)*plVar21;
        while (plVar22 != (long *)0x0) {
          uVar31 = plVar22[1];
          if ((uVar30 & uVar29) == 0) {
            uVar31 = uVar31 & uVar29;
          }
          else if (uVar30 <= uVar31) {
            uVar27 = 0;
            if (uVar30 != 0) {
              uVar27 = uVar31 / uVar30;
            }
            uVar31 = uVar31 - uVar27 * uVar30;
          }
          plVar23 = plVar22;
          if (uVar31 != uVar26) {
            if (*(long *)(*(long *)(param_1 + 0x70) + uVar31 * 8) == 0) {
              *(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8) = plVar21;
              uVar26 = uVar31;
            }
            else {
              *plVar21 = *plVar22;
              *plVar22 = **(undefined8 **)(*(long *)(param_1 + 0x70) + uVar31 * 8);
              **(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8) = (long)plVar22;
              plVar23 = plVar21;
            }
          }
          plVar21 = plVar23;
          plVar22 = (long *)*plVar23;
        }
      }
    }
  }
  else if (uVar30 < uVar29) {
    uVar26 = (ulong)((float)*(ulong *)(param_1 + 0x88) / *(float *)(param_1 + 0x90));
    if ((uVar29 < 3) || ((uVar29 & uVar29 - 1) != 0)) {
      uVar26 = FUN_00f76630();
    }
    else if (1 < uVar26) {
      uVar26 = 1L << (-LZCOUNT(uVar26 - 1) & 0x3fU);
    }
    if (uVar30 <= uVar26) {
      uVar30 = uVar26;
    }
    if (uVar30 < uVar29) {
      if (uVar30 != 0) goto LAB_042e6e10;
      uVar10 = 0;
      bVar24 = true;
      goto LAB_042e6e24;
    }
  }
  uVar29 = *(ulong *)(param_1 + 0x78);
  if ((uVar29 & uVar29 - 1) == 0) {
    uVar31 = uVar29 - 1 & uVar14;
  }
  else {
    uVar31 = uVar14;
    if (uVar29 <= uVar14) {
      uVar30 = 0;
      if (uVar29 != 0) {
        uVar30 = uVar14 / uVar29;
      }
      uVar31 = uVar14 - uVar30 * uVar29;
    }
  }
LAB_042e6f2c:
  lVar9 = *(long *)(param_1 + 0x70);
  plVar21 = *(long **)(lVar9 + uVar31 * 8);
  if (plVar21 == (long *)0x0) {
    plVar21 = (long *)(param_1 + 0x80);
    *plVar20 = *plVar21;
    *plVar21 = (long)plVar20;
    *(long **)(lVar9 + uVar31 * 8) = plVar21;
    if (*plVar20 != 0) {
      uVar30 = *(ulong *)(*plVar20 + 8);
      if ((uVar29 & uVar29 - 1) == 0) {
        uVar30 = uVar30 & uVar29 - 1;
      }
      else if (uVar29 <= uVar30) {
        uVar26 = 0;
        if (uVar29 != 0) {
          uVar26 = uVar30 / uVar29;
        }
        uVar30 = uVar30 - uVar26 * uVar29;
      }
      *(long **)(*(long *)(param_1 + 0x70) + uVar30 * 8) = plVar20;
    }
  }
  else {
    *plVar20 = *plVar21;
    *plVar21 = (long)plVar20;
  }
  *(long *)(param_1 + 0x88) = *(long *)(param_1 + 0x88) + 1;
  puVar13 = local_90;
LAB_042e6f54:
  if (puVar13 != (undefined8 *)0x0) {
    puVar15 = puVar13;
    if ((local_88 & 0x7ffffffffffffff) != 0) {
      lVar9 = local_88 * -0x20;
      puVar16 = puVar13 + local_88 * 4;
      do {
        puVar28 = puVar16 + -4;
        if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar28);
        }
        lVar9 = lVar9 + 0x20;
        puVar15 = local_90;
        puVar16 = puVar28;
      } while (lVar9 != 0);
    }
    local_88 = (long)puVar13 - (long)puVar15 >> 5;
    FUN_00005810(puVar15,local_80 << 5);
  }
  if (local_98 != (long *)0x0) {
    LOAcquire();
    iVar8 = (int)local_98[1];
    *(int *)(local_98 + 1) = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(*local_98 + 0x20))();
    }
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  FUN_0784a050(plVar12);
  puVar15 = local_78;
  if (local_78 != (undefined8 *)0x0) {
    if ((local_70 & 0x7ffffffffffffff) != 0) {
      lVar9 = local_70 * -0x20;
      puVar16 = local_78 + local_70 * 4;
      do {
        puVar28 = puVar16 + -4;
        if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar28);
        }
        lVar9 = lVar9 + 0x20;
        puVar16 = puVar28;
      } while (lVar9 != 0);
    }
    local_70 = 0;
    FUN_00005810(puVar15,local_68 << 5);
  }
  if (plVar12 != (long *)0x0) {
    LOAcquire();
    iVar8 = (int)plVar12[1];
    *(int *)(plVar12 + 1) = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(*plVar12 + 0x20))(plVar12);
    }
  }
  return;
}


