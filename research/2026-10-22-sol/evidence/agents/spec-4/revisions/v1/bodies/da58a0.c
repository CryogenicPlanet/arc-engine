// ==== 0xda58a0  FUN_00da58a0  (1432 bytes)

/* WARNING: Removing unreachable block (ram,0x00da5bc0) */

void FUN_00da58a0(long *param_1,long param_2,ulong param_3,undefined4 param_4,undefined4 param_5)

{
  ulong uVar1;
  bool bVar2;
  ulong uVar3;
  undefined **ppuVar4;
  size_t sVar5;
  size_t sVar6;
  undefined8 ***pppuVar7;
  ulong uVar8;
  code *pcVar9;
  int iVar10;
  void *pvVar11;
  ulong uVar12;
  ulong *puVar13;
  undefined8 ****ppppuVar14;
  ulong uVar15;
  long lVar16;
  ulong uVar17;
  ulong *puVar18;
  ulong *puVar19;
  long lVar20;
  ulong uVar21;
  ulong uVar22;
  uint local_10c;
  ulong local_108;
  long local_100;
  undefined **local_e8;
  size_t local_e0;
  undefined8 local_d8;
  ulong local_d0;
  undefined8 ***local_c8;
  undefined8 ***local_c0;
  size_t local_b8;
  undefined8 local_b0;
  undefined **local_a8;
  long local_a0;
  ulong local_98;
  ulong local_90;
  undefined8 ***pppuStack_88;
  undefined8 ***local_80;
  ulong local_78;
  undefined8 local_70;
  
  local_80 = (undefined8 ****)0x0;
  local_78 = 0;
  local_70 = 0;
  if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (param_3 < 0x17) {
    pppuStack_88 = &local_80;
    local_90 = 0;
    local_a8 = &PTR_FUN_0da99310;
    local_a0 = 0;
    local_98 = 0;
    if (param_3 == 0) {
      puVar19 = (ulong *)0x0;
      uVar22 = 0;
      lVar20 = 0;
      goto LAB_00da5a80;
    }
  }
  else {
    ppppuVar14 = (undefined8 ****)FUN_00008790((param_3 & 0x7ffffffffffffff0) + 0x10);
    if (((undefined8 ****)((long)ppppuVar14 + 1) < ppppuVar14) ||
       (&local_80 < (undefined8 ****)((long)ppppuVar14 + 1) && ppppuVar14 <= &local_80)) {
LAB_00da5c94:
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(1,0xda5c98);
      (*pcVar9)();
    }
    *(undefined1 *)ppppuVar14 = 0;
    local_70 = (param_3 & 0x7ffffffffffffff0) + 0x8000000000000010;
    local_80 = ppppuVar14;
  }
  pppuStack_88 = &local_80;
  local_78 = 0;
  local_90 = 0;
  local_98 = 0;
  local_a0 = 0;
  local_a8 = &PTR_FUN_0da99310;
  uVar22 = 0;
  uVar17 = 0;
  uVar21 = 0;
  puVar18 = (ulong *)0x0;
  do {
    pvVar11 = _memchr((void *)(param_2 + uVar21),0x2e,param_3 - uVar21);
    uVar8 = local_90;
    uVar15 = (long)pvVar11 - param_2;
    if (pvVar11 == (void *)0x0) {
      uVar15 = 0xffffffffffffffff;
    }
    uVar3 = param_3;
    if (uVar15 != 0xffffffffffffffff) {
      uVar3 = uVar15;
    }
    uVar15 = param_3 - uVar21;
    if (uVar3 - uVar21 <= param_3 - uVar21) {
      uVar15 = uVar3 - uVar21;
    }
    uVar12 = FUN_00da5e38(param_2 + uVar21,uVar15,&local_a8);
    bVar2 = (uVar12 & 1) == 0;
    if (bVar2) {
      FUN_00121ec0(&local_a8,param_2 + uVar21,uVar15);
    }
    uVar15 = local_90;
    uVar12 = (ulong)!bVar2;
    if (uVar22 == uVar17) {
      uVar1 = uVar22 + 1;
      if (0x666666666666666 < uVar1) {
                    /* WARNING: Subroutine does not return */
        FUN_040416c0();
      }
      uVar17 = uVar22 * 2;
      if (uVar17 < uVar1 || uVar17 - uVar1 == 0) {
        uVar17 = uVar1;
      }
      if (0x333333333333332 < uVar22) {
        uVar17 = 0x666666666666666;
      }
      if (uVar17 == 0) {
        puVar13 = (ulong *)0x0;
      }
      else {
        if (0x666666666666666 < uVar17) {
                    /* WARNING: Subroutine does not return */
          FUN_04b087f0();
        }
        puVar13 = (ulong *)FUN_00008790(uVar17 * 0x28);
      }
      puVar19 = puVar13 + uVar22 * 5;
      if (puVar19 == (ulong *)0x0) goto LAB_00da5c94;
      *puVar19 = uVar21;
      puVar19[1] = uVar3;
      puVar19[2] = uVar8;
      puVar19[3] = uVar15;
      puVar19[4] = uVar12;
      puVar19 = puVar19 + uVar22 * -5;
      _memcpy(puVar19,puVar18,uVar22 * 0x28);
      if (puVar18 != (ulong *)0x0) {
        FUN_00005810(puVar18,uVar22 * 0x28);
      }
    }
    else {
      puVar19 = puVar18 + uVar22 * 5;
      if (puVar19 == (ulong *)0x0) goto LAB_00da5c94;
      *puVar19 = uVar21;
      puVar19[1] = uVar3;
      puVar19[2] = uVar8;
      puVar19[3] = local_90;
      puVar19[4] = uVar12;
      puVar19 = puVar18;
      puVar13 = puVar18;
    }
    lVar20 = (long)puVar13 + ((uVar22 * 0x28 + 0x28) - (long)puVar19);
    if (lVar20 == 0) goto LAB_00da5c94;
    if (uVar3 < param_3) {
      if (local_98 <= local_90) {
        uVar22 = 0x10;
        if (local_98 != 0) {
          uVar22 = local_98;
        }
        do {
          if (uVar22 >> 0x1e != 0) goto LAB_00da5a5c;
          uVar22 = uVar22 << 1;
        } while (uVar22 < local_98 + 1);
        (*(code *)local_a8[2])(&local_a8);
      }
      *(undefined1 *)(local_a0 + local_90) = 0x2e;
      local_90 = local_90 + 1;
    }
LAB_00da5a5c:
    uVar22 = (lVar20 >> 3) * -0x3333333333333333;
    uVar21 = uVar3 + 1;
    puVar18 = puVar19;
  } while (uVar21 < param_3);
  lVar20 = uVar17 * 0x28;
LAB_00da5a80:
  FUN_003417f0(&local_a8);
  ppppuVar14 = (undefined8 ****)local_80;
  if (-1 < (long)local_70._7_1_) {
    ppppuVar14 = &local_80;
  }
  uVar21 = local_78;
  if (-1 < (long)local_70) {
    uVar21 = (long)local_70._7_1_;
  }
  FUN_00da6074(&local_e8,ppppuVar14,uVar21,param_4,param_5);
  sVar5 = local_e0;
  ppuVar4 = local_e8;
  if ((char)local_d0 == '\0') {
    ppuVar4 = (undefined **)0x0;
    sVar5 = 0;
  }
  if ((local_d0 & 1) == 0) {
    *(undefined1 *)param_1 = 0;
    *(undefined1 *)(param_1 + 2) = 0;
  }
  else {
    if (local_e0 != 0) {
      uVar21 = local_78;
      if ((local_70 & 0x8000000000000000) == 0) {
        uVar21 = local_70 >> 0x38;
      }
      if (uVar22 != 0) {
        uVar21 = uVar21 - local_e0;
        puVar18 = puVar19;
        do {
          if (uVar21 == puVar18[2]) {
            uVar21 = *puVar18;
            if (param_3 < uVar21) {
LAB_00da5e00:
                    /* WARNING: Subroutine does not return */
              FUN_04b085b0("string_view::substr");
            }
            *param_1 = param_2 + uVar21;
            param_1[1] = param_3 - uVar21;
            *(undefined1 *)(param_1 + 2) = 1;
            goto LAB_00da5b7c;
          }
          if (((uVar21 < puVar18[3]) && ((puVar18[4] & 1) != 0)) &&
             (iVar10 = (int)puVar18[1] + -1, (int)*puVar18 <= iVar10)) {
            lVar16 = -(long)iVar10;
            local_100 = param_2 + iVar10;
            local_108 = (long)iVar10;
            do {
              local_c0 = (undefined8 ****)0x0;
              local_b8 = 0;
              local_b0 = 0;
              local_e8 = &PTR_FUN_0da99310;
              local_c8 = &local_c0;
              local_d8 = 0;
              local_d0 = 0;
              local_e0 = 0;
              if (param_3 < local_108) goto LAB_00da5e00;
              uVar17 = param_3 + lVar16;
              if (lVar16 + puVar18[1] <= param_3 + lVar16) {
                uVar17 = lVar16 + puVar18[1];
              }
              uVar17 = FUN_00da5e38(local_100,uVar17,&local_e8);
              if ((uVar17 & 1) == 0) {
                local_10c = (uint)local_b0._7_1_;
              }
              else {
                FUN_003417f0(&local_e8);
                pppuVar7 = local_c0;
                local_10c = (uint)(char)local_b0._7_1_;
                sVar6 = local_b8;
                if (-1 < (int)local_10c) {
                  sVar6 = (ulong)local_b0._7_1_;
                }
                if (sVar6 == sVar5) {
                  ppppuVar14 = (undefined8 ****)local_c0;
                  if (-1 < (int)local_10c) {
                    ppppuVar14 = &local_c0;
                  }
                  iVar10 = _memcmp(ppppuVar14,ppuVar4,sVar5);
                  if (iVar10 == 0) {
                    *param_1 = local_100;
                    param_1[1] = param_3 + lVar16;
                    *(undefined1 *)(param_1 + 2) = 1;
                    if ((int)local_10c < 0) {
                      FUN_00005810(pppuVar7,local_b0 & 0x7fffffffffffffff);
                    }
                    goto LAB_00da5b7c;
                  }
                }
              }
              if ((local_10c >> 7 & 1) != 0) {
                FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
              }
              lVar16 = lVar16 + 1;
              local_100 = local_100 + -1;
              bVar2 = (long)(int)*puVar18 < (long)local_108;
              local_108 = local_108 - 1;
            } while (bVar2);
          }
          puVar18 = puVar18 + 5;
        } while (puVar18 != puVar19 + uVar22 * 5);
      }
    }
    *param_1 = 0;
    param_1[1] = 0;
    *(undefined1 *)(param_1 + 2) = 1;
  }
LAB_00da5b7c:
  if (puVar19 != (ulong *)0x0) {
    if (uVar22 * 0x28 != 0) {
      lVar16 = uVar22 * -0x28;
      puVar18 = puVar19 + uVar22 * 5;
      do {
        puVar18 = puVar18 + -5;
        if (puVar18 == (ulong *)0x0) goto LAB_00da5c94;
        lVar16 = lVar16 + 0x28;
      } while (lVar16 != 0);
    }
    FUN_00005810(puVar19,lVar20);
  }
  return;
}


