// ==== 0x42eb57c  FUN_042eb57c  (1400 bytes)

/* WARNING: Removing unreachable block (ram,0x042eb888) */

void FUN_042eb57c(long param_1,long *param_2)

{
  undefined1 *puVar1;
  size_t sVar2;
  size_t sVar3;
  undefined8 *puVar4;
  undefined1 uVar5;
  byte bVar6;
  char cVar7;
  ulong uVar8;
  code *pcVar9;
  bool bVar10;
  int iVar11;
  ulong uVar12;
  ulong extraout_x1;
  long *plVar13;
  ulong uVar14;
  long lVar15;
  long lVar16;
  ulong uVar17;
  undefined8 *puVar18;
  ulong uVar19;
  undefined8 *puVar20;
  undefined8 *puVar21;
  ulong uVar22;
  long lVar23;
  undefined8 *puVar24;
  undefined8 uVar25;
  undefined8 uVar26;
  long *local_b8;
  ulong local_b0;
  long local_a8;
  long local_a0;
  ulong uStack_98;
  long *local_90;
  long local_88;
  undefined8 local_80;
  long local_78;
  long local_70;
  ulong local_68;
  
  if (*(long *)(param_1 + 0x68) == 0) {
    return;
  }
  if (*(long *)(param_1 + 0x108) == 0) {
    return;
  }
  puVar1 = (undefined1 *)(param_1 + 0x100);
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1 + 0x100);
  }
  uVar5 = *puVar1;
  *puVar1 = 1;
  local_78 = 0;
  local_70 = 0;
  local_68 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  local_88 = 0;
  local_90 = (long *)0x0;
  local_80 = 0x3f800000;
  uVar19 = param_2[1];
  if (uVar19 != 0) {
    if (0xaaaaaaaaaaaaaaa < uVar19) goto LAB_042eba64;
    lVar15 = uVar19 * 0x18;
    local_78 = FUN_00008790(lVar15);
    local_70 = 0;
    lVar23 = *param_2;
    local_68 = uVar19;
    do {
      local_b8 = &local_a0;
      FUN_010d8c54(&local_b8,lVar23,lVar23);
      if ((extraout_x1 & 1) != 0) {
        FUN_00ad9ee8(&local_78,lVar23);
      }
      lVar23 = lVar23 + 0x18;
      lVar15 = lVar15 + -0x18;
    } while (lVar15 != 0);
  }
  lVar23 = *(long *)(*(long *)(param_1 + 0x108) + 200) * 0x18;
  if (lVar23 == 0) {
    puVar24 = (undefined8 *)0x0;
    puVar18 = (undefined8 *)0x0;
LAB_042eb620:
    bVar10 = true;
  }
  else {
    if (0xaaaaaaaaaaaaaaa < (ulong)((lVar23 >> 3) * -0x5555555555555555)) {
LAB_042eba64:
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    lVar16 = *(long *)(*(long *)(param_1 + 0x108) + 0xc0);
    puVar18 = (undefined8 *)FUN_00008790(lVar23);
    lVar15 = 0;
    do {
      puVar24 = (undefined8 *)((long)puVar18 + lVar15);
      if (puVar24 == (undefined8 *)0x0) goto LAB_042eba60;
      puVar21 = (undefined8 *)(lVar16 + lVar15);
      if (*(char *)((long)puVar21 + 0x17) < '\0') {
        puVar21 = (undefined8 *)(lVar16 + lVar15);
        FUN_00060a60(puVar24,*puVar21,puVar21[1]);
      }
      else {
        uVar26 = puVar21[1];
        uVar25 = *puVar21;
        puVar24[2] = puVar21[2];
        puVar24[1] = uVar26;
        *puVar24 = uVar25;
      }
      lVar15 = lVar15 + 0x18;
    } while (lVar23 - lVar15 != 0);
    puVar24 = (undefined8 *)((long)puVar18 + lVar15);
    puVar21 = puVar18;
    if (lVar15 == 0) goto LAB_042eb620;
    do {
      uVar19 = uStack_98;
      if (uStack_98 != 0 && local_88 != 0) {
        cVar7 = *(char *)((long)puVar21 + 0x17);
        puVar20 = (void *)*puVar21;
        if (-1 < (long)cVar7) {
          puVar20 = puVar21;
        }
        sVar2 = puVar21[1];
        if (-1 < cVar7) {
          sVar2 = (long)cVar7;
        }
        uVar12 = FUN_00018f3c(puVar20,sVar2);
        uVar22 = uVar19 - 1;
        if ((uVar19 & uVar22) == 0) {
          uVar17 = uVar12 & uVar22;
        }
        else {
          uVar17 = uVar12;
          if (uVar19 <= uVar12) {
            uVar17 = 0;
            if (uVar19 != 0) {
              uVar17 = uVar12 / uVar19;
            }
            uVar17 = uVar12 - uVar17 * uVar19;
          }
        }
        plVar13 = *(long **)(local_a0 + uVar17 * 8);
        if (plVar13 != (long *)0x0) {
          for (plVar13 = (long *)*plVar13; plVar13 != (long *)0x0; plVar13 = (long *)*plVar13) {
            uVar14 = plVar13[1];
            if (uVar12 == uVar14) {
              bVar6 = *(byte *)((long)plVar13 + 0x27);
              sVar3 = plVar13[3];
              if (-1 < (char)bVar6) {
                sVar3 = (ulong)bVar6;
              }
              if (sVar3 == sVar2) {
                puVar4 = (void *)plVar13[2];
                if (-1 < (char)bVar6) {
                  puVar4 = plVar13 + 2;
                }
                iVar11 = _memcmp(puVar4,puVar20,sVar2);
                if (iVar11 == 0) goto LAB_042eb9f8;
              }
            }
            else {
              if ((uVar19 & uVar22) == 0) {
                uVar14 = uVar14 & uVar22;
              }
              else if (uVar19 <= uVar14) {
                uVar8 = 0;
                if (uVar19 != 0) {
                  uVar8 = uVar14 / uVar19;
                }
                uVar14 = uVar14 - uVar8 * uVar19;
              }
              if (uVar14 != uVar17) break;
            }
          }
        }
      }
      FUN_07cd94c0(*(undefined8 *)(param_1 + 0x108),puVar21,0);
LAB_042eb9f8:
      bVar10 = puVar24 + -3 != puVar21;
      puVar21 = puVar21 + 3;
    } while (bVar10);
    bVar10 = false;
  }
  local_b8 = (long *)0x0;
  local_b0 = 0;
  local_a8 = 0;
  lVar15 = local_70 * 0x18;
  lVar16 = local_78;
  if (lVar15 != 0) {
    do {
      uVar19 = FUN_07cd9ea0(*(undefined8 *)(param_1 + 0x108),lVar16);
      if ((uVar19 & 1) == 0) {
        FUN_07cd94c0(*(undefined8 *)(param_1 + 0x108),lVar16,1);
      }
      iVar11 = FUN_07cd9ea0(*(undefined8 *)(param_1 + 0x108),lVar16);
      if (iVar11 != 0) {
        FUN_00ad9ee8(&local_b8,lVar16);
      }
      lVar15 = lVar15 + -0x18;
      lVar16 = lVar16 + 0x18;
    } while (lVar15 != 0);
    if (local_b0 != 0) {
      lVar15 = 0;
      uVar19 = 0;
      do {
        FUN_07cd9f40(*(undefined8 *)(param_1 + 0x108),(long)local_b8 + lVar15,uVar19);
        uVar19 = uVar19 + 1;
        lVar15 = lVar15 + 0x18;
      } while (uVar19 < local_b0);
      lVar15 = local_b0 * 0x18;
      goto LAB_042eb754;
    }
  }
  lVar15 = 0;
LAB_042eb754:
  plVar13 = local_b8;
  if (local_b8 != (long *)0x0) {
    if (lVar15 != 0) {
      puVar21 = (undefined8 *)(lVar15 + (long)local_b8);
      do {
        puVar20 = puVar21 + -3;
        if (puVar20 == (undefined8 *)0x0) goto LAB_042eba60;
        if (*(char *)((long)puVar21 + -1) < '\0') {
          FUN_00005810(*puVar20,puVar21[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + -0x18;
        puVar21 = puVar20;
      } while (lVar15 != 0);
    }
    local_b0 = ((long)plVar13 - (long)local_b8 >> 3) * -0x5555555555555555;
    FUN_00005810(local_b8,local_a8 * 0x18);
  }
  lVar15 = local_a0;
  plVar13 = local_90;
  if (puVar18 != (undefined8 *)0x0) {
    if (!bVar10) {
      do {
        puVar24 = puVar24 + -3;
        if (puVar24 == (undefined8 *)0x0) goto LAB_042eba60;
      } while (puVar24 != puVar18);
    }
    FUN_00005810(puVar18,lVar23);
    lVar15 = local_a0;
    plVar13 = local_90;
  }
  while (plVar13 != (long *)0x0) {
    local_a0 = lVar15;
    if (plVar13 == (long *)0xfffffffffffffff0) goto LAB_042eba60;
    lVar23 = *plVar13;
    if (*(char *)((long)plVar13 + 0x27) < '\0') {
      FUN_00005810(plVar13[2],plVar13[4] & 0x7fffffffffffffff);
    }
    FUN_00005810(plVar13,0x28);
    lVar15 = local_a0;
    plVar13 = (long *)lVar23;
  }
  local_a0 = 0;
  if (lVar15 != 0) {
    FUN_00005810(lVar15,uStack_98 << 3);
  }
  lVar23 = local_78;
  if (local_78 != 0) {
    if (local_70 * 0x18 != 0) {
      lVar15 = local_70 * -0x18;
      puVar18 = (undefined8 *)(local_70 * 0x18 + local_78);
      do {
        puVar24 = puVar18 + -3;
        if (puVar24 == (undefined8 *)0x0) {
LAB_042eba60:
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x42eba64);
          (*pcVar9)();
        }
        if (*(char *)((long)puVar18 + -1) < '\0') {
          FUN_00005810(*puVar24,puVar18[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + 0x18;
        puVar18 = puVar24;
      } while (lVar15 != 0);
    }
    local_70 = (lVar23 - local_78 >> 3) * -0x5555555555555555;
    FUN_00005810(local_78,local_68 * 0x18);
  }
  if (puVar1 != (undefined1 *)0x0) {
    *puVar1 = uVar5;
  }
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(puVar1);
  }
  return;
}


