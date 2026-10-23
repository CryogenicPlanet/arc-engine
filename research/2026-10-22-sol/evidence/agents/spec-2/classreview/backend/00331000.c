// ==== 0x331000  FUN_00331000  (2152 bytes)

void FUN_00331000(undefined8 *param_1,byte *param_2,long param_3,long param_4,ulong param_5)

{
  byte *pbVar1;
  byte *pbVar2;
  ulong *puVar3;
  undefined8 *puVar4;
  long lVar5;
  char cVar6;
  int iVar7;
  bool bVar8;
  undefined8 **ppuVar9;
  code *pcVar10;
  int iVar11;
  uint uVar12;
  byte *pbVar13;
  undefined8 *puVar14;
  ulong uVar15;
  ulong uVar16;
  size_t sVar17;
  ulong uVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  undefined8 *puVar22;
  long lVar23;
  byte bVar24;
  ulong uVar25;
  undefined8 local_4b8;
  uint local_4b0;
  byte local_4ac;
  byte local_4ab;
  byte bStack_4aa;
  undefined1 uStack_4a9;
  ulong local_4a8;
  uint local_49c;
  uint local_498;
  uint local_494;
  undefined8 *local_490;
  code *pcStack_488;
  long local_480;
  byte *local_478;
  byte local_470 [1024];
  long local_70 [2];
  
  local_70[0] = *(long *)PTR____stack_chk_guard_0d824020;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  ppuVar9 = &local_490;
  _bzero(local_470,0x400);
  pcStack_488 = FUN_003334c4;
  local_480 = 0;
  local_490 = param_1;
  local_478 = local_470;
  if (param_3 == -1) {
    local_4b0 = (uint)param_4;
    local_4ac = (byte)((ulong)param_4 >> 0x20);
    local_4ab = (byte)((ulong)param_4 >> 0x28);
    bStack_4aa = (byte)((ulong)param_4 >> 0x30);
    uStack_4a9 = (undefined1)((ulong)param_4 >> 0x38);
    lVar19 = *(long *)(param_2 + 0x18) << 5;
    local_4a8 = param_5;
    if (lVar19 != 0) {
      lVar20 = 0;
      lVar5 = *(long *)(param_2 + 8);
      lVar21 = *(long *)(param_2 + 0x10) + 0x10;
      lVar23 = lVar5;
      do {
        lVar23 = lVar23 + lVar20;
        lVar20 = (lVar5 + *(long *)(lVar21 + -8)) - lVar23;
        if ((lVar20 < 0) || ((lVar23 == 0 && (lVar20 != 0)))) goto LAB_00331774;
        local_4b8 = ppuVar9;
        if (*(char *)(lVar21 + -0x10) == '\0') {
          FUN_040473c4(&local_490,lVar23,lVar20);
          ppuVar9 = local_4b8;
        }
        else {
          uVar25 = FUN_04047a40(&local_4b8,lVar21);
          ppuVar9 = local_4b8;
          if ((uVar25 & 1) == 0) {
            bVar24 = 0;
            goto LAB_00331418;
          }
        }
        lVar21 = lVar21 + 0x20;
        lVar19 = lVar19 + -0x20;
      } while (lVar19 != 0);
    }
    bVar24 = *param_2 ^ 1;
  }
  else {
    ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
    if ((param_3 < 0) ||
       ((param_3 != 0 &&
        (ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8), param_2 == (byte *)0x0))
       )) goto LAB_00331774;
    local_49c = 0;
    if (param_3 != 0) {
      pbVar2 = param_2 + param_3;
LAB_00331160:
      uVar25 = (long)pbVar2 - (long)param_2;
      pbVar13 = _memchr(param_2,0x25,(long)pbVar2 - (long)param_2);
      if (pbVar13 == (byte *)0x0) {
        ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
        if (((long)uVar25 < 0) ||
           (ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8),
           param_2 == (byte *)0x0)) goto LAB_00331774;
        local_480 = local_480 + uVar25;
        if (uVar25 < (ulong)((long)local_70 - (long)local_478)) {
          if (uVar25 == 1) {
            *local_478 = *param_2;
          }
          else {
            _memcpy(local_478,param_2,uVar25);
          }
          local_478 = local_478 + uVar25;
          goto LAB_003313ec;
        }
        lVar19 = (long)local_478 - (long)local_470;
        ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
        if (lVar19 < 0) goto LAB_00331774;
        (*pcStack_488)(local_490,local_470,lVar19);
        local_478 = local_470;
        (*pcStack_488)(local_490,param_2,uVar25);
        goto LAB_0033140c;
      }
      uVar25 = (long)pbVar13 - (long)param_2;
      ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
      if (((long)uVar25 < 0) ||
         (ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8), param_2 == (byte *)0x0)
         ) goto LAB_00331774;
      if (pbVar13 != param_2) {
        local_480 = local_480 + uVar25;
        if (uVar25 < (ulong)((long)local_70 - (long)local_478)) {
          if (uVar25 == 1) {
            *local_478 = *param_2;
          }
          else {
            _memcpy(local_478,param_2,uVar25);
          }
          local_478 = local_478 + uVar25;
        }
        else {
          lVar19 = (long)local_478 - (long)local_470;
          ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
          if (lVar19 < 0) goto LAB_00331774;
          (*pcStack_488)(local_490,local_470,lVar19);
          local_478 = local_470;
          (*pcStack_488)(local_490,param_2,uVar25);
        }
      }
      pbVar1 = pbVar13 + 1;
      if (pbVar1 < pbVar2) {
        uVar25 = (ulong)(char)(&DAT_0c4b3020)[*pbVar1];
        if (-1 < (long)uVar25) {
          uVar15 = (ulong)local_49c;
          if ((int)local_49c < 0) goto LAB_003316b0;
          local_49c = local_49c + 1;
          ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
          if (pbVar1 == (byte *)0x0) goto LAB_00331774;
          if (param_5 <= uVar15) goto LAB_003316b0;
          param_2 = pbVar13 + 2;
          puVar3 = (ulong *)(param_4 + uVar15 * 0x10);
          uVar15 = *puVar3;
          pcVar10 = (code *)puVar3[1];
          if (pcVar10 == (code *)&DAT_01114a80) {
            if (((0x9fffaU >> (uVar25 & 0x3f)) >> 1 & 1) != 0) {
              uVar25 = uVar25 | 0xffffffff00090000;
              uVar16 = 0xffffffff;
LAB_00331150:
              uVar25 = FUN_003321d8(uVar15,uVar25,uVar16,&local_490);
joined_r0x00331154:
              if ((uVar25 & 1) != 0) goto LAB_00331158;
            }
            goto LAB_003316b0;
          }
          if (pcVar10 == (code *)&DAT_01115580) {
            if (((0x9fffaU >> (uVar25 & 0x3f)) >> 1 & 1) == 0) goto LAB_003316b0;
            uVar25 = uVar25 | 0xffffffff00090000;
            uVar16 = 0xffffffff;
LAB_00331118:
            uVar25 = FUN_00332b28(uVar15,uVar25,uVar16,&local_490);
          }
          else {
            if (pcVar10 != (code *)&DAT_01114618) {
              uVar25 = (*pcVar10)(uVar15,uVar25 | 0xffffffff00090000,0xffffffff,&local_490);
              goto joined_r0x00331154;
            }
            if (((0x9fffaU >> (uVar25 & 0x3f)) >> 1 & 1) == 0) goto LAB_003316b0;
            uVar25 = uVar25 | 0xffffffff00090000;
            uVar16 = 0xffffffff;
LAB_003310ec:
            uVar25 = FUN_00331db0(uVar15,uVar25,uVar16,&local_490);
          }
          if ((uVar25 & 1) == 0) goto LAB_003316b0;
LAB_00331158:
          if (param_2 == pbVar2) goto LAB_0033140c;
          goto LAB_00331160;
        }
        if (*pbVar1 == 0x25) {
          local_480 = local_480 + 1;
          if ((ulong)((long)local_70 - (long)local_478) < 2) {
            lVar19 = (long)local_478 - (long)local_470;
            ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
            if (lVar19 < 0) goto LAB_00331774;
            (*pcStack_488)(local_490,local_470,lVar19);
            local_478 = local_470;
            (*pcStack_488)(local_490,"%",1);
          }
          else {
            *local_478 = 0x25;
            local_478 = local_478 + 1;
          }
          param_2 = pbVar13 + 2;
          goto LAB_00331158;
        }
        local_4b8._0_4_ = 0;
        local_4ac = 0;
        uStack_4a9 = 0;
        local_4b8._4_4_ = 0xffffffff;
        local_4b0 = 0xffffffff;
        local_4ab = 9;
        bStack_4aa = 0x13;
        param_2 = (byte *)FUN_00331868(pbVar1,pbVar2,&local_4b8,&local_49c);
        if (param_2 != (byte *)0x0) {
          ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
          if ((-1 < (long)param_2 - (long)pbVar1) &&
             (ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8),
             pbVar1 != (byte *)0x0)) {
            iVar7 = (int)local_4b8 + -1;
            if ((ulong)(long)iVar7 < param_5) {
              if (local_4ac == 0) {
                uVar16 = 0xffffffff;
                uVar25 = 0xffffffff00090000;
LAB_0033127c:
                puVar3 = (ulong *)(param_4 + (long)iVar7 * 0x10);
                uVar18 = (ulong)bStack_4aa;
                uVar15 = *puVar3;
                pcVar10 = (code *)puVar3[1];
                uVar25 = uVar25 | uVar18;
                if (pcVar10 == (code *)&DAT_01114a80) {
                  if (uVar18 == 0x13) {
                    uVar12 = (uint)uVar15;
                    if (0x7ffffffe < uVar12) {
                      uVar12 = 0x7fffffff;
                    }
                    uVar15 = (ulong)uVar12;
LAB_0033163c:
                    local_490 = (undefined8 *)CONCAT44(local_490._4_4_,(int)uVar15);
                    goto LAB_00331158;
                  }
                  if (((0x9fffaU >> (uVar18 & 0x3f)) >> 1 & 1) != 0) goto LAB_00331150;
                }
                else if (pcVar10 == (code *)&DAT_01115580) {
                  if (uVar18 == 0x13) {
                    if (0x7ffffffe < uVar15) {
                      uVar15 = 0x7fffffff;
                    }
                    goto LAB_0033163c;
                  }
                  if (((0x9fffaU >> (uVar18 & 0x3f)) >> 1 & 1) != 0) goto LAB_00331118;
                }
                else {
                  if (pcVar10 != (code *)&DAT_01114618) {
                    uVar25 = (*pcVar10)();
                    goto joined_r0x00331154;
                  }
                  if (uVar18 == 0x13) goto LAB_0033163c;
                  if (((0x9fffaU >> (uVar18 & 0x3f)) >> 1 & 1) != 0) goto LAB_003310ec;
                }
              }
              else {
                local_494 = local_4b8._4_4_;
                if ((int)local_4b8._4_4_ < -1) {
                  if (~local_4b8._4_4_ <= param_5) {
                    uVar25 = (ulong)~local_4b8._4_4_ - 1;
                    if (param_5 <= uVar25) {
                    /* WARNING: Does not return */
                      pcVar10 = (code *)SoftwareBreakpoint(1,0x3317dc);
                      (*pcVar10)();
                    }
                    puVar4 = (undefined8 *)(param_4 + uVar25 * 0x10);
                    iVar11 = (*(code *)puVar4[1])(*puVar4,0x90013,0,&local_494);
                    if (iVar11 != 0) {
                      if (-1 < (int)local_494) goto LAB_00331250;
                      if (local_494 < 0x80000002) {
                        local_494 = 0x80000001;
                      }
                      local_494 = -local_494;
                      bVar8 = true;
                      goto LAB_00331254;
                    }
                  }
                }
                else {
LAB_00331250:
                  bVar8 = false;
LAB_00331254:
                  uVar16 = (ulong)local_4b0;
                  local_498 = local_4b0;
                  if (-2 < (int)local_4b0) {
LAB_00331264:
                    uVar25 = (ulong)local_4ac;
                    if (bVar8) {
                      uVar25 = uVar25 | 1;
                    }
                    uVar25 = (ulong)local_494 << 0x20 | (ulong)local_4ab << 0x10 | uVar25 << 8;
                    goto LAB_0033127c;
                  }
                  if ((uVar16 ^ 0xffffffff) <= param_5) {
                    uVar25 = (uVar16 ^ 0xffffffff) - 1;
                    if (param_5 <= uVar25) {
                    /* WARNING: Does not return */
                      pcVar10 = (code *)SoftwareBreakpoint(1,0x33177c);
                      (*pcVar10)();
                    }
                    puVar4 = (undefined8 *)(param_4 + uVar25 * 0x10);
                    iVar11 = (*(code *)puVar4[1])(*puVar4,0x90013,0,&local_498);
                    if (iVar11 != 0) {
                      uVar16 = (ulong)local_498;
                      goto LAB_00331264;
                    }
                  }
                }
              }
            }
            goto LAB_003316b0;
          }
          goto LAB_00331774;
        }
      }
LAB_003316b0:
      bVar24 = 0;
      goto LAB_00331410;
    }
LAB_003313ec:
    bVar24 = 1;
    ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
  }
  goto LAB_00331418;
LAB_0033140c:
  bVar24 = 1;
LAB_00331410:
  ppuVar9 = (undefined8 **)CONCAT44(local_4b8._4_4_,(int)local_4b8);
LAB_00331418:
  puVar4 = local_490;
  uVar25 = (long)local_478 - (long)local_470;
  if ((long)uVar25 < 0) goto LAB_00331774;
  local_4b8 = ppuVar9;
  if (pcStack_488 != FUN_003334c4) {
    (*pcStack_488)(local_490,local_470,uVar25);
    goto LAB_003314a8;
  }
  cVar6 = *(char *)((long)local_490 + 0x17);
  sVar17 = (size_t)cVar6;
  if ((long)sVar17 < 0) {
    sVar17 = local_490[1];
    lVar19 = (local_490[2] & 0x7fffffffffffffff) - 1;
  }
  else {
    lVar19 = 0x16;
  }
  if (uVar25 <= lVar19 - sVar17) {
    if (local_478 != local_470) {
      puVar22 = local_490;
      if (cVar6 < '\0') {
        puVar22 = (undefined8 *)*local_490;
      }
      pbVar2 = (byte *)((long)puVar22 + sVar17);
      if ((pbVar2 + uVar25 < pbVar2) || ((pbVar2 <= local_470 && (local_470 < pbVar2 + uVar25))))
      goto LAB_00331774;
      _memmove((undefined1 *)((long)puVar22 + sVar17),local_470,uVar25);
      lVar19 = sVar17 + uVar25;
      if (*(char *)((long)puVar4 + 0x17) < '\0') {
        puVar4[1] = lVar19;
      }
      else {
        *(byte *)((long)puVar4 + 0x17) = (byte)lVar19 & 0x7f;
      }
      *(undefined1 *)((long)puVar22 + lVar19) = 0;
    }
    goto LAB_003314a8;
  }
  uVar15 = sVar17 + uVar25;
  if (0x7fffffffffffffee < uVar15) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (cVar6 < '\0') {
    uVar18 = (local_490[2] & 0x7fffffffffffffff) - 1;
    uVar16 = uVar15;
    if (uVar15 <= uVar18 * 2) {
      uVar16 = uVar18 * 2;
    }
    if (uVar18 < 0x3fffffffffffffe8) {
      uVar16 = uVar16 & 0xfffffffffffffff0;
      goto LAB_00331518;
    }
    uVar16 = 0x7ffffffffffffff0;
  }
  else {
    uVar16 = uVar15;
    if (uVar15 < 0x2d) {
      uVar16 = 0x2c;
    }
    uVar16 = uVar16 & 0x7ffffffffffffff0;
LAB_00331518:
    uVar16 = uVar16 + 0x10;
  }
  puVar14 = (undefined8 *)FUN_00008790(uVar16);
  cVar6 = *(char *)((long)puVar4 + 0x17);
  puVar22 = puVar4;
  if (cVar6 < '\0') {
    puVar22 = (undefined8 *)*puVar4;
  }
  if (sVar17 != 0) {
    ppuVar9 = local_4b8;
    if (((undefined8 *)((long)puVar14 + sVar17) < puVar14) ||
       ((puVar14 <= puVar22 && (puVar22 < (undefined8 *)((long)puVar14 + sVar17)))))
    goto LAB_00331774;
    _memmove(puVar14,puVar22,sVar17);
  }
  pbVar2 = (byte *)((long)puVar14 + sVar17);
  ppuVar9 = local_4b8;
  if ((pbVar2 <= pbVar2 + uVar25) && ((local_470 < pbVar2 || (pbVar2 + uVar25 <= local_470)))) {
    _memcpy(pbVar2,local_470,uVar25);
    *(undefined1 *)((long)puVar14 + uVar15) = 0;
    if (cVar6 < '\0') {
      FUN_00005810(*puVar4,puVar4[2] & 0x7fffffffffffffff);
    }
    *puVar4 = puVar14;
    puVar4[1] = uVar15;
    puVar4[2] = uVar16 | 0x8000000000000000;
LAB_003314a8:
    if ((bVar24 & 1) == 0) {
      if (*(char *)((long)param_1 + 0x17) < '\0') {
        *(undefined1 *)*param_1 = 0;
        param_1[1] = 0;
      }
      else {
        *(undefined1 *)param_1 = 0;
        *(undefined1 *)((long)param_1 + 0x17) = 0;
      }
    }
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_70[0]) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
LAB_00331774:
                    /* WARNING: Does not return */
  pcVar10 = (code *)SoftwareBreakpoint(1,0x331778);
  local_4b8 = ppuVar9;
  (*pcVar10)();
}


