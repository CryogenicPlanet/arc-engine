// ==== 0x788520  FUN_00788520  (2368 bytes)

/* WARNING: Removing unreachable block (ram,0x00788ab4) */
/* WARNING: Removing unreachable block (ram,0x00788c6c) */
/* WARNING: Removing unreachable block (ram,0x00788c7c) */
/* WARNING: Removing unreachable block (ram,0x00788e58) */
/* WARNING: Removing unreachable block (ram,0x00788c8c) */
/* WARNING: Removing unreachable block (ram,0x00788c94) */
/* WARNING: Removing unreachable block (ram,0x00788ca0) */
/* WARNING: Removing unreachable block (ram,0x00788ac8) */
/* WARNING: Removing unreachable block (ram,0x00788e44) */
/* WARNING: Removing unreachable block (ram,0x00788a6c) */
/* WARNING: Removing unreachable block (ram,0x00788c68) */

void FUN_00788520(undefined8 *****param_1,long param_2,ulong param_3,long *param_4,ulong param_5)

{
  short *****pppppsVar1;
  short *****pppppsVar2;
  ulong uVar3;
  undefined8 ****ppppuVar4;
  undefined8 ****ppppuVar5;
  code *pcVar6;
  undefined8 *****pppppuVar7;
  short *****pppppsVar8;
  ulong uVar9;
  char *pcVar10;
  long lVar11;
  ulong uVar12;
  ulong uVar13;
  short *****pppppsVar14;
  uint uVar15;
  long lVar16;
  ulong uVar17;
  ushort uVar18;
  ushort uVar20;
  ushort uVar21;
  short ****ppppsVar19;
  ushort uVar22;
  ushort uVar23;
  ushort uVar25;
  ushort uVar26;
  short ****ppppsVar24;
  ushort uVar27;
  short ****local_f0;
  undefined8 ***pppuStack_e8;
  undefined8 local_e0;
  undefined8 ***pppuStack_d8;
  undefined8 ***local_d0;
  undefined8 ****local_c8;
  long lStack_c0;
  undefined8 local_b8;
  undefined8 ****local_b0;
  ulong uStack_a8;
  undefined8 local_a0;
  ulong auStack_98 [3];
  undefined8 ****local_80;
  undefined8 ***pppuStack_78;
  undefined8 local_70;
  
  if (param_4 != (long *)0x0) {
    lVar11 = param_4[1] * 0x18;
    if (lVar11 != 0) {
      lVar16 = param_4[1] * -0x18;
      lVar11 = lVar11 + *param_4;
      do {
        lVar11 = lVar11 + -0x18;
        if (lVar11 == 0) goto LAB_00788dbc;
        lVar16 = lVar16 + 0x18;
      } while (lVar16 != 0);
    }
    param_4[1] = 0;
  }
  local_b0 = (undefined8 *****)0x0;
  uStack_a8 = 0;
  local_a0 = 0;
  if (0x7fffffffffffffee < param_3) {
LAB_00788cdc:
                    /* WARNING: Subroutine does not return */
    FUN_04c31780();
  }
  if (param_3 < 0xb) {
    pppppuVar7 = &local_b0;
  }
  else {
    pppppuVar7 = (undefined8 *****)FUN_00008790((param_3 & 0x7ffffffffffffff8) * 2 + 0x10);
    if (((undefined8 *****)((long)pppppuVar7 + 2) < pppppuVar7) ||
       (&local_b0 < (undefined8 *****)((long)pppppuVar7 + 2) && pppppuVar7 <= &local_b0)) {
LAB_00788dbc:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x788dc0);
      (*pcVar6)();
    }
    *(undefined2 *)pppppuVar7 = 0;
    local_a0 = (param_3 & 0x7ffffffffffffff8) + 0x8000000000000008;
    local_b0 = pppppuVar7;
  }
  uStack_a8 = 0;
  FUN_00786cf8(&local_b0,pppppuVar7,param_2,param_2 + param_3,param_3);
  local_c8 = (undefined8 *****)0x0;
  lStack_c0 = 0;
  local_b8 = 0;
  if (param_3 != 0) {
    pcVar10 = (char *)(param_3 + param_2);
    uVar17 = param_3;
    do {
      pcVar10 = pcVar10 + -1;
      if (*pcVar10 == '.') {
        uVar17 = (long)pcVar10 - param_2;
        if (uVar17 != 0xffffffffffffffff) {
          if (param_3 <= uVar17) {
                    /* WARNING: Subroutine does not return */
            FUN_04b085b0("string_view::substr");
          }
          lVar11 = uVar17 + 1;
          uVar17 = param_3 - lVar11;
          local_f0 = (short ****)0x0;
          pppuStack_e8 = (undefined8 ****)0x0;
          local_e0 = (undefined8 ****)0x0;
          if (uVar17 < 0xb) {
            pppppsVar8 = &local_f0;
          }
          else {
            uVar17 = param_3 - lVar11 & 0x7ffffffffffffff8;
            pppppsVar8 = (short *****)FUN_00008790(uVar17 * 2 + 0x10);
            if (((short *****)((long)pppppsVar8 + 2) < pppppsVar8) ||
               (&local_f0 < (short *****)((long)pppppsVar8 + 2) && pppppsVar8 <= &local_f0))
            goto LAB_00788dbc;
            *(short *)pppppsVar8 = 0;
            local_e0 = (undefined8 ****)(uVar17 + 0x8000000000000008);
            uVar17 = param_3 - lVar11;
            local_f0 = (short ****)pppppsVar8;
          }
          pppuStack_e8 = (undefined8 ****)0x0;
          lVar11 = param_2 + lVar11;
          FUN_00786cf8(&local_f0,pppppsVar8,lVar11,param_2 + param_3,uVar17);
          pppppsVar8 = (short *****)local_f0;
          if (-1 < (long)local_e0._7_1_) {
            pppppsVar8 = &local_f0;
          }
          ppppuVar5 = (undefined8 ****)pppuStack_e8;
          if (-1 < (long)local_e0) {
            ppppuVar5 = (undefined8 ****)(long)local_e0._7_1_;
          }
          local_80 = (undefined8 ****)0x0;
          pppuStack_78 = (undefined8 ***)0x0;
          local_70 = (undefined8 ****)0x0;
          auStack_98[0] = 0;
          auStack_98[1] = 0;
          auStack_98[2] = 0;
          FUN_00788e60(pppppsVar8,ppppuVar5,&local_80,0,auStack_98,0,1,&local_c8);
          if ((long)auStack_98[2] < 0) {
            FUN_00005810(auStack_98[0],auStack_98[2] << 1);
          }
          if ((long)local_e0 < 0) {
            FUN_00005810(local_f0,(long)local_e0 << 1);
          }
          goto LAB_0078861c;
        }
        break;
      }
      uVar17 = uVar17 - 1;
    } while (uVar17 != 0);
  }
  lVar11 = 0;
  uVar17 = 0;
LAB_0078861c:
  param_1[5] = (undefined8 ****)0x0;
  param_1[4] = (undefined8 ****)0x0;
  param_1[8] = (undefined8 ****)0x0;
  param_1[7] = (undefined8 ****)0x0;
  param_1[6] = (undefined8 ****)0x0;
  param_1[1] = (undefined8 ****)0x0;
  *param_1 = (undefined8 ****)0x0;
  param_1[3] = (undefined8 ****)0x0;
  param_1[2] = (undefined8 ****)0x0;
  local_80 = (undefined8 *****)0x0;
  pppuStack_78 = (undefined8 ****)0x0;
  local_70 = (undefined8 ****)0x0;
  auStack_98[0] = 0;
  uVar9 = uStack_a8;
  if (-1 < local_a0) {
    uVar9 = (long)(char)local_a0._7_1_;
  }
  if (uVar9 != 0) {
    uVar13 = (ulong)(long)(char)local_a0._7_1_ >> 0x3f;
    do {
      pppppuVar7 = (undefined8 *****)local_b0;
      if ((int)uVar13 == 0) {
        pppppuVar7 = &local_b0;
      }
      pppppsVar1 = (short *****)((long)pppppuVar7 + auStack_98[0] * 2);
      pppppsVar2 = (short *****)((long)pppppuVar7 + uVar9 * 2);
      uVar13 = uVar9 * 2 + auStack_98[0] * -2;
      pppppsVar8 = pppppsVar1;
      while (0x3e < uVar13) {
        ppppsVar24 = pppppsVar8[1];
        ppppsVar19 = *pppppsVar8;
        uVar18 = -(ushort)((short)ppppsVar19 == 0x2e);
        uVar20 = -(ushort)((short)((ulong)ppppsVar19 >> 0x10) == 0x2e);
        uVar21 = -(ushort)((short)((ulong)ppppsVar19 >> 0x20) == 0x2e);
        uVar22 = -(ushort)((short)((ulong)ppppsVar19 >> 0x30) == 0x2e);
        uVar23 = -(ushort)((short)ppppsVar24 == 0x2e);
        uVar25 = -(ushort)((short)((ulong)ppppsVar24 >> 0x10) == 0x2e);
        uVar26 = -(ushort)((short)((ulong)ppppsVar24 >> 0x20) == 0x2e);
        uVar27 = -(ushort)((short)((ulong)ppppsVar24 >> 0x30) == 0x2e);
        if (CONCAT26(uVar22,CONCAT24(uVar21,CONCAT22(uVar20,uVar18))) +
            CONCAT26(uVar27,CONCAT24(uVar26,CONCAT22(uVar25,uVar23))) != 0) {
          lVar16 = 0;
          uVar15 = (uint)(ushort)((uVar18 & 1) + (uVar20 & 2) + (uVar21 & 4) + (uVar22 & 8) +
                                  (uVar23 & 0x10) + (uVar25 & 0x20) + (uVar26 & 0x40) +
                                 (uVar27 & 0x80));
LAB_007887cc:
          uVar15 = (uVar15 & 0xaaaaaaaa) >> 1 | (uVar15 & 0x55555555) << 1;
          uVar15 = (uVar15 & 0xcccccccc) >> 2 | (uVar15 & 0x33333333) << 2;
          uVar13 = lVar16 + LZCOUNT((uVar15 >> 4 | (uVar15 & 0xf0f0f0f) << 4) << 0x18) * 2;
          goto LAB_00788800;
        }
        ppppsVar24 = pppppsVar8[3];
        ppppsVar19 = pppppsVar8[2];
        uVar18 = -(ushort)((short)ppppsVar19 == 0x2e);
        uVar20 = -(ushort)((short)((ulong)ppppsVar19 >> 0x10) == 0x2e);
        uVar21 = -(ushort)((short)((ulong)ppppsVar19 >> 0x20) == 0x2e);
        uVar22 = -(ushort)((short)((ulong)ppppsVar19 >> 0x30) == 0x2e);
        uVar23 = -(ushort)((short)ppppsVar24 == 0x2e);
        uVar25 = -(ushort)((short)((ulong)ppppsVar24 >> 0x10) == 0x2e);
        uVar26 = -(ushort)((short)((ulong)ppppsVar24 >> 0x20) == 0x2e);
        uVar27 = -(ushort)((short)((ulong)ppppsVar24 >> 0x30) == 0x2e);
        if (CONCAT26(uVar22,CONCAT24(uVar21,CONCAT22(uVar20,uVar18))) +
            CONCAT26(uVar27,CONCAT24(uVar26,CONCAT22(uVar25,uVar23))) != 0) {
          uVar15 = (uint)(ushort)((uVar18 & 1) + (uVar20 & 2) + (uVar21 & 4) + (uVar22 & 8) +
                                  (uVar23 & 0x10) + (uVar25 & 0x20) + (uVar26 & 0x40) +
                                 (uVar27 & 0x80));
          lVar16 = 0x10;
          goto LAB_007887cc;
        }
        ppppsVar24 = pppppsVar8[5];
        ppppsVar19 = pppppsVar8[4];
        uVar18 = -(ushort)((short)ppppsVar19 == 0x2e);
        uVar20 = -(ushort)((short)((ulong)ppppsVar19 >> 0x10) == 0x2e);
        uVar21 = -(ushort)((short)((ulong)ppppsVar19 >> 0x20) == 0x2e);
        uVar22 = -(ushort)((short)((ulong)ppppsVar19 >> 0x30) == 0x2e);
        uVar23 = -(ushort)((short)ppppsVar24 == 0x2e);
        uVar25 = -(ushort)((short)((ulong)ppppsVar24 >> 0x10) == 0x2e);
        uVar26 = -(ushort)((short)((ulong)ppppsVar24 >> 0x20) == 0x2e);
        uVar27 = -(ushort)((short)((ulong)ppppsVar24 >> 0x30) == 0x2e);
        if (CONCAT26(uVar22,CONCAT24(uVar21,CONCAT22(uVar20,uVar18))) +
            CONCAT26(uVar27,CONCAT24(uVar26,CONCAT22(uVar25,uVar23))) != 0) {
          uVar15 = (uint)(ushort)((uVar18 & 1) + (uVar20 & 2) + (uVar21 & 4) + (uVar22 & 8) +
                                  (uVar23 & 0x10) + (uVar25 & 0x20) + (uVar26 & 0x40) +
                                 (uVar27 & 0x80));
          lVar16 = 0x20;
          goto LAB_007887cc;
        }
        ppppsVar24 = pppppsVar8[7];
        ppppsVar19 = pppppsVar8[6];
        uVar18 = -(ushort)((short)ppppsVar19 == 0x2e);
        uVar20 = -(ushort)((short)((ulong)ppppsVar19 >> 0x10) == 0x2e);
        uVar21 = -(ushort)((short)((ulong)ppppsVar19 >> 0x20) == 0x2e);
        uVar22 = -(ushort)((short)((ulong)ppppsVar19 >> 0x30) == 0x2e);
        uVar23 = -(ushort)((short)ppppsVar24 == 0x2e);
        uVar25 = -(ushort)((short)((ulong)ppppsVar24 >> 0x10) == 0x2e);
        uVar26 = -(ushort)((short)((ulong)ppppsVar24 >> 0x20) == 0x2e);
        uVar27 = -(ushort)((short)((ulong)ppppsVar24 >> 0x30) == 0x2e);
        if (CONCAT26(uVar22,CONCAT24(uVar21,CONCAT22(uVar20,uVar18))) +
            CONCAT26(uVar27,CONCAT24(uVar26,CONCAT22(uVar25,uVar23))) != 0) {
          uVar15 = (uint)(ushort)((uVar18 & 1) + (uVar20 & 2) + (uVar21 & 4) + (uVar22 & 8) +
                                  (uVar23 & 0x10) + (uVar25 & 0x20) + (uVar26 & 0x40) +
                                 (uVar27 & 0x80));
          lVar16 = 0x30;
          goto LAB_007887cc;
        }
        pppppsVar8 = pppppsVar8 + 8;
        uVar13 = (long)pppppsVar2 - (long)pppppsVar8;
      }
      while (0xe < uVar13) {
        ppppsVar24 = pppppsVar8[1];
        ppppsVar19 = *pppppsVar8;
        uVar18 = -(ushort)((short)ppppsVar19 == 0x2e);
        uVar20 = -(ushort)((short)((ulong)ppppsVar19 >> 0x10) == 0x2e);
        uVar21 = -(ushort)((short)((ulong)ppppsVar19 >> 0x20) == 0x2e);
        uVar22 = -(ushort)((short)((ulong)ppppsVar19 >> 0x30) == 0x2e);
        uVar23 = -(ushort)((short)ppppsVar24 == 0x2e);
        uVar25 = -(ushort)((short)((ulong)ppppsVar24 >> 0x10) == 0x2e);
        uVar26 = -(ushort)((short)((ulong)ppppsVar24 >> 0x20) == 0x2e);
        uVar27 = -(ushort)((short)((ulong)ppppsVar24 >> 0x30) == 0x2e);
        if (CONCAT26(uVar22,CONCAT24(uVar21,CONCAT22(uVar20,uVar18))) +
            CONCAT26(uVar27,CONCAT24(uVar26,CONCAT22(uVar25,uVar23))) != 0) {
          uVar15 = (uint)(ushort)((uVar18 & 1) + (uVar20 & 2) + (uVar21 & 4) + (uVar22 & 8) +
                                  (uVar23 & 0x10) + (uVar25 & 0x20) + (uVar26 & 0x40) +
                                 (uVar27 & 0x80));
          uVar15 = (uVar15 & 0xaaaaaaaa) >> 1 | (uVar15 & 0x55555555) << 1;
          uVar15 = (uVar15 & 0xcccccccc) >> 2 | (uVar15 & 0x33333333) << 2;
          pppppsVar14 = (short *****)
                        ((long)pppppsVar8 +
                        LZCOUNT((uVar15 >> 4 | (uVar15 & 0xf0f0f0f) << 4) << 0x18) * 2);
          goto LAB_00788830;
        }
        pppppsVar8 = pppppsVar8 + 2;
        uVar13 = (long)pppppsVar2 - (long)pppppsVar8;
      }
      pppppsVar14 = pppppsVar2;
      if (pppppsVar2 != pppppsVar8) {
        if ((ulong)((long)pppppsVar8 - (long)pppppsVar1) < 0xf) {
          if ((((ulong)pppppsVar8 & 1) != 0) || (((ulong)pppppsVar2 & 1) != 0)) goto LAB_00788dbc;
          do {
            pppppsVar14 = pppppsVar8;
            if (*(short *)pppppsVar8 == 0x2e) break;
            pppppsVar8 = (short *****)((long)pppppsVar8 + 2);
            pppppsVar14 = pppppsVar2;
          } while (pppppsVar8 != pppppsVar2);
        }
        else {
          pppppsVar8 = pppppsVar2 + -2;
          ppppsVar24 = pppppsVar2[-1];
          ppppsVar19 = *pppppsVar8;
          uVar13 = CONCAT17(-((short)((ulong)ppppsVar24 >> 0x30) == 0x2e),
                            CONCAT16(-((short)((ulong)ppppsVar24 >> 0x20) == 0x2e),
                                     CONCAT15(-((short)((ulong)ppppsVar24 >> 0x10) == 0x2e),
                                              CONCAT14(-((short)ppppsVar24 == 0x2e),
                                                       CONCAT13(-((short)((ulong)ppppsVar19 >> 0x30)
                                                                 == 0x2e),
                                                                CONCAT12(-((short)((ulong)ppppsVar19
                                                                                  >> 0x20) == 0x2e),
                                                                         CONCAT11(-((short)((ulong)
                                                  ppppsVar19 >> 0x10) == 0x2e),
                                                  -((short)ppppsVar19 == 0x2e))))))));
          uVar13 = (uVar13 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar13 & 0x5555555555555555) << 1;
          uVar13 = (uVar13 & 0xcccccccccccccccc) >> 2 | (uVar13 & 0x3333333333333333) << 2;
          uVar13 = (uVar13 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar13 & 0xf0f0f0f0f0f0f0f) << 4;
          uVar13 = (uVar13 & 0xff00ff00ff00ff00) >> 8 | (uVar13 & 0xff00ff00ff00ff) << 8;
          uVar13 = (uVar13 & 0xffff0000ffff0000) >> 0x10 | (uVar13 & 0xffff0000ffff) << 0x10;
          uVar13 = (ulong)LZCOUNT(uVar13 >> 0x20 | uVar13 << 0x20) >> 2 & 0x1e;
LAB_00788800:
          pppppsVar14 = (short *****)((long)pppppsVar8 + uVar13);
        }
      }
LAB_00788830:
      pppppsVar8 = (short *****)0x0;
      if (pppppsVar14 != pppppsVar2) {
        pppppsVar8 = pppppsVar14;
      }
      uVar13 = (long)pppppsVar8 - (long)pppppuVar7 >> 1;
      if (pppppsVar8 == (short *****)0x0) {
        uVar13 = 0xffffffffffffffff;
      }
      uVar3 = uVar9;
      if (uVar13 != 0xffffffffffffffff) {
        uVar3 = uVar13;
      }
      ppppuVar5 = (undefined8 ****)pppuStack_78;
      if (((ulong)local_70 & 0x8000000000000000) == 0) {
        ppppuVar5 = (undefined8 ****)((ulong)local_70 >> 0x38);
      }
      local_d0 = (undefined8 ***)(uVar3 - auStack_98[0]);
      if (uVar3 < auStack_98[0] || (undefined8 ****)local_d0 == (undefined8 ****)0x0) {
        uVar9 = 0;
      }
      else {
        ppppuVar4 = (undefined8 ****)(uVar9 - auStack_98[0]);
        if (local_d0 <= (undefined8 ****)(uVar9 - auStack_98[0])) {
          ppppuVar4 = (undefined8 ****)local_d0;
        }
        if ((undefined8 ****)0x7fffffffffffffee < ppppuVar4) goto LAB_00788cdc;
        if (ppppuVar4 < (undefined8 ****)0xb) {
          local_e0 = (undefined8 ****)CONCAT17((char)ppppuVar4,(undefined7)local_e0);
          pppppsVar8 = &local_f0;
        }
        else {
          pppppsVar8 = (short *****)FUN_00008790(((ulong)ppppuVar4 & 0x7ffffffffffffff8) * 2 + 0x10)
          ;
          local_e0 = (undefined8 ****)(((ulong)ppppuVar4 & 0x7ffffffffffffff8) + 0x8000000000000008)
          ;
          local_f0 = (short ****)pppppsVar8;
          pppuStack_e8 = ppppuVar4;
        }
        pppppsVar2 = (short *****)((long)pppppsVar8 + (long)ppppuVar4 * 2);
        if ((pppppsVar2 < pppppsVar8) || (pppppsVar8 <= pppppsVar1 && pppppsVar1 < pppppsVar2))
        goto LAB_00788dbc;
        _memmove(pppppsVar8,pppppsVar1,(long)ppppuVar4 << 1);
        *(short *)((long)pppppsVar8 + (long)ppppuVar4 * 2) = 0;
        pppppsVar8 = (short *****)local_f0;
        if (-1 < (long)local_e0._7_1_) {
          pppppsVar8 = &local_f0;
        }
        ppppuVar4 = (undefined8 ****)pppuStack_e8;
        if (-1 < (long)local_e0) {
          ppppuVar4 = (undefined8 ****)(long)local_e0._7_1_;
        }
        pppppuVar7 = (undefined8 *****)local_c8;
        if (-1 < (long)local_b8._7_1_) {
          pppppuVar7 = &local_c8;
        }
        lVar16 = lStack_c0;
        if (-1 < local_b8) {
          lVar16 = (long)local_b8._7_1_;
        }
        uVar9 = FUN_00788e60(pppppsVar8,ppppuVar4,lVar11,uVar17,pppppuVar7,lVar16,param_5,&local_80)
        ;
        if ((long)local_e0 < 0) {
          FUN_00005810(local_f0,(long)local_e0 << 1);
        }
        *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) | (byte)(uVar9 >> 8);
        if ((uVar9 >> 0x20 != 0) && (*(uint *)(param_1 + 8) < 2)) {
          *(int *)(param_1 + 8) = (int)(uVar9 >> 0x20);
        }
      }
      ppppuVar4 = (undefined8 ****)pppuStack_78;
      if (((ulong)local_70 & 0x8000000000000000) == 0) {
        ppppuVar4 = (undefined8 ****)((ulong)local_70 >> 0x38);
      }
      local_f0 = (short ****)((long)ppppuVar4 - (long)ppppuVar5);
      if ((param_4 != (long *)0x0) && ((uVar9 & 1) != 0)) {
        thunk_FUN_08bfc8fc(param_4,auStack_98,&local_d0,&local_f0);
      }
      uVar12 = (ulong)local_a0._7_1_;
      uVar9 = uStack_a8;
      if (-1 < local_a0) {
        uVar9 = uVar12;
      }
      if (uVar3 < uVar9) {
        uVar9 = (ulong)local_70._7_1_;
        if (local_70._7_1_ == 10) {
          pppppuVar7 = (undefined8 *****)FUN_00008790(0x30);
          if (((undefined8 *****)((long)pppppuVar7 + 0x14U) < pppppuVar7) ||
             ((pppppuVar7 <= &local_80 && (&local_80 < (undefined8 *****)((long)pppppuVar7 + 0x14U))
              ))) goto LAB_00788dbc;
          _memmove(pppppuVar7,&local_80,0x14);
          uVar9 = 10;
          local_80 = pppppuVar7;
          local_70 = (undefined8 ****)&DAT_8000000000000018;
          pppuStack_78 = (undefined8 ****)0xb;
        }
        else {
          local_70 = (undefined8 ****)
                     (CONCAT17(local_70._7_1_ + 1,(undefined7)local_70) & 0x7fffffffffffffff);
          pppppuVar7 = &local_80;
        }
        *(undefined4 *)((long)pppppuVar7 + uVar9 * 2) = 0x2e;
        uVar12 = (ulong)local_a0._7_1_;
      }
      auStack_98[0] = uVar3 + 1;
      uVar13 = (ulong)((char)local_a0._7_1_ < '\0');
      uVar9 = uStack_a8;
      if ((char)local_a0._7_1_ >= '\0') {
        uVar9 = uVar12;
      }
    } while (uVar3 + 1 < uVar9);
  }
  if (param_1 != &local_80) {
    if (*(char *)((long)param_1 + 0x17) < '\0') {
      ppppuVar5 = (undefined8 ****)pppuStack_78;
      pppppuVar7 = (undefined8 *****)local_80;
      if (-1 < (long)local_70) {
        ppppuVar5 = (undefined8 ****)((ulong)local_70 >> 0x38);
        pppppuVar7 = &local_80;
      }
      FUN_007ab764(param_1,pppppuVar7,ppppuVar5);
    }
    else if ((long)local_70 < 0) {
      FUN_007ab624(param_1,local_80,pppuStack_78);
    }
    else {
      param_1[1] = (undefined8 ****)pppuStack_78;
      *param_1 = local_80;
      param_1[2] = local_70;
    }
  }
  if (((ulong)param_1[3] & 1) != 0) {
    thunk_FUN_08bfc4fc();
    pppppuVar7 = (undefined8 *****)local_80;
    if (-1 < (long)(char)local_70._7_1_) {
      pppppuVar7 = &local_80;
    }
    ppppuVar5 = (undefined8 ****)pppuStack_78;
    if (-1 < (long)local_70) {
      ppppuVar5 = (undefined8 ****)(long)(char)local_70._7_1_;
    }
    thunk_FUN_08bfb564(&local_f0,&DAT_0e3d42b8,pppppuVar7,ppppuVar5);
    if (*(char *)((long)param_1 + 0x37) < '\0') {
      FUN_00005810(param_1[4],(ulong)param_1[6] & 0x7fffffffffffffff);
    }
    param_1[5] = (undefined8 ****)pppuStack_e8;
    param_1[4] = (undefined8 ****)local_f0;
    param_1[6] = local_e0;
    param_1[7] = (undefined8 ****)pppuStack_d8;
    if ((param_5 & 1) == 0) {
      ppppuVar5 = param_1[5];
      if ((*(byte *)((long)param_1 + 0x37) & 0x80) == 0) {
        ppppuVar5 = (undefined8 ****)(ulong)*(byte *)((long)param_1 + 0x37);
      }
      if (ppppuVar5 != (undefined8 ****)0x0) {
        if (param_4 != (long *)0x0) {
          lVar11 = param_4[1] * 0x18;
          if (lVar11 != 0) {
            lVar16 = param_4[1] * -0x18;
            lVar11 = lVar11 + *param_4;
            do {
              lVar11 = lVar11 + -0x18;
              if (lVar11 == 0) goto LAB_00788dbc;
              lVar16 = lVar16 + 0x18;
            } while (lVar16 != 0);
          }
          param_4[1] = 0;
        }
        FUN_007ab5d4(param_1,&local_b0);
      }
    }
  }
  if (local_b8 < 0) {
    FUN_00005810(local_c8,local_b8 << 1);
  }
  if (local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 << 1);
  }
  return;
}


