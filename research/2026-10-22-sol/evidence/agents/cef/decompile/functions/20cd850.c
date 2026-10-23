// ==== 0x20cd850  FUN_020cd850  (1828 bytes)

void FUN_020cd850(long param_1,short *param_2)

{
  long *plVar1;
  ulong uVar2;
  byte bVar3;
  byte bVar4;
  char cVar5;
  code *pcVar6;
  bool bVar7;
  int iVar8;
  undefined8 uVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  char *pcVar14;
  size_t sVar15;
  short *psVar16;
  short *psVar17;
  undefined8 *puVar18;
  undefined8 *puVar19;
  long *plVar20;
  long lVar21;
  undefined8 *local_e0;
  undefined6 uStack_d8;
  undefined2 uStack_d2;
  undefined6 uStack_d0;
  char cStack_ca;
  char cStack_c9;
  ulong local_c8;
  ulong local_c0;
  undefined8 *local_b0;
  undefined8 uStack_a8;
  long *local_a0;
  ulong local_98;
  ulong local_90;
  char *local_80;
  undefined8 local_78;
  undefined8 local_70;
  char *local_68;
  
  cVar5 = DAT_0e4a44bf;
  local_70 = 0x6f;
  if (DAT_0e4a44bf != '\0') {
    lVar21 = (long)*(char *)((long)param_2 + 0x17);
    psVar16 = param_2;
    if (lVar21 < 0) {
      lVar21 = *(long *)(param_2 + 4);
      psVar16 = *(short **)param_2;
    }
    FUN_00af22f8(&local_e0,psVar16,lVar21);
    uStack_a8 = CONCAT26(uStack_d2,uStack_d8);
    local_b0 = local_e0;
    local_a0 = (long *)CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0));
    uStack_d8 = 0;
    uStack_d2 = 0;
    uStack_d0 = 0;
    cStack_ca = '\0';
    cStack_c9 = '\0';
    local_e0 = (undefined8 *)0x0;
    local_80 = "AutocompleteController::Start";
    local_68 = "text";
    FUN_004d4be0(cVar5,&local_70,&local_80,1,&local_68,&local_b0);
    if ((long)local_a0 < 0) {
      FUN_00005810(local_b0,(ulong)local_a0 & 0x7fffffffffffffff);
    }
    if (cStack_c9 < '\0') {
      FUN_00005810(local_e0,CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0)) & 0x7fffffffffffffff);
    }
  }
  local_80 = (char *)0x0;
  local_78 = 0;
  FUN_01bd6a88(&local_80);
  if (*(int *)(param_2 + 0xda) != 0) {
    *(undefined1 *)(param_1 + 0x5d8) = 1;
  }
  puVar18 = *(undefined8 **)(param_1 + 0x898);
  puVar19 = puVar18 + 1;
  local_b0 = puVar18;
  FUN_0096b910(&local_b0,*puVar19);
  *puVar18 = puVar19;
  puVar18[2] = 0;
  *puVar19 = 0;
  if ((*(byte *)((long)param_2 + 0x1b3) & 1) == 0) {
    local_90 = 0;
    uStack_a8 = 0;
    local_b0 = (undefined8 *)0x0;
    local_98 = 0;
    local_a0 = (long *)0x0;
    if (*(long *)(param_1 + 0x38) != 0) {
      FUN_0455be30(&local_b0,param_1 + 0x10);
    }
    local_c0 = 0;
    uStack_d8 = 0;
    uStack_d2 = 0;
    local_e0 = (undefined8 *)0x0;
    local_c8 = 0;
    uStack_d0 = 0;
    cStack_ca = '\0';
    cStack_c9 = '\0';
LAB_020cdbc8:
    if (local_a0 == (long *)0x0) {
      plVar20 = (long *)CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0));
LAB_020cdc00:
      if (plVar20 != (long *)0x0) {
        uVar11 = plVar20[1];
        if (local_c0 <= (ulong)plVar20[1]) {
          uVar11 = local_c0;
        }
        if (local_c8 != uVar11) goto LAB_020cdc20;
      }
LAB_020cdcc0:
      FUN_025c1eac(&local_e0);
      FUN_025c1eac(&local_b0);
      goto LAB_020cd8d0;
    }
    uVar11 = local_a0[1];
    if (local_90 <= (ulong)local_a0[1]) {
      uVar11 = local_90;
    }
    plVar20 = (long *)CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0));
    if (local_98 == uVar11) goto LAB_020cdc00;
LAB_020cdc20:
    if (local_98 == local_c8 && local_a0 == plVar20) goto LAB_020cdcc0;
    if (local_98 < (ulong)local_a0[1]) {
      plVar20 = (long *)(*local_a0 + local_98 * 0x10);
      lVar21 = *plVar20;
      if ((lVar21 == 0) || (*(char *)(lVar21 + 4) != '\0')) {
        plVar20 = (long *)0x0;
      }
      else {
        plVar20 = (long *)plVar20[1];
      }
      (**(code **)(*plVar20 + 0x10))(plVar20,param_1,param_2);
      if (local_a0 != (long *)0x0) {
        uVar11 = local_a0[1];
        if (local_90 <= (ulong)local_a0[1]) {
          uVar11 = local_90;
        }
        do {
          local_98 = local_98 + 1;
          if (uVar11 <= local_98) break;
          if ((ulong)local_a0[1] <= local_98) goto LAB_020cdcd8;
          iVar8 = FUN_026da700(*local_a0 + local_98 * 0x10);
        } while (iVar8 != 0);
      }
      goto LAB_020cdbc8;
    }
LAB_020cdcd8:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x20cdcdc);
    (*pcVar6)();
  }
LAB_020cd8d0:
  FUN_024e3ca4(param_1 + 0x860);
  *(undefined1 *)(param_1 + 0x880) = 0;
  uVar9 = (*(code *)PTR_FUN_0e2f1ea8)();
  *(undefined8 *)(param_1 + 0x868) = uVar9;
  *(undefined8 *)(param_1 + 0x870) = uVar9;
  *(undefined8 *)(param_1 + 0x878) = uVar9;
  bVar3 = *(byte *)(param_1 + 0x117);
  uVar11 = *(ulong *)(param_1 + 0x108);
  if (-1 < (char)bVar3) {
    uVar11 = (ulong)bVar3;
  }
  bVar4 = *(byte *)((long)param_2 + 0x17);
  uVar2 = *(ulong *)(param_2 + 4);
  if (-1 < (char)bVar4) {
    uVar2 = (ulong)bVar4;
  }
  if (uVar11 == uVar2) {
    if (uVar11 != 0) {
      psVar16 = *(short **)param_2;
      if (-1 < (char)bVar4) {
        psVar16 = param_2;
      }
      psVar17 = *(short **)(param_1 + 0x100);
      if (-1 < (char)bVar3) {
        psVar17 = (short *)(param_1 + 0x100);
      }
      do {
        if (*psVar16 != *psVar17) {
          bVar7 = false;
          goto LAB_020cd95c;
        }
        uVar11 = uVar11 - 1;
        psVar16 = psVar16 + 1;
        psVar17 = psVar17 + 1;
      } while (uVar11 != 0);
    }
    if ((*(char *)(param_1 + 0x2b1) != *(char *)((long)param_2 + 0x1b1)) ||
       (*(char *)(param_1 + 0x2b3) != *(char *)((long)param_2 + 0x1b3))) goto LAB_020cd9a8;
    bVar7 = *(int *)(param_1 + 0x2b4) == *(int *)(param_2 + 0xda);
  }
  else {
LAB_020cd9a8:
    bVar7 = false;
  }
LAB_020cd95c:
  FUN_020ce4e8(param_1 + 0x100,param_2);
  *(undefined4 *)(param_1 + 0x85c) = 0;
  lVar10 = (*(code *)PTR_FUN_0e2f1ea8)();
  lVar21 = *(long *)(param_1 + 0x58) << 3;
  if (lVar21 != 0) {
    plVar20 = *(long **)(param_1 + 0x50);
    do {
      uVar11 = FUN_020ce020(param_1,*plVar20);
      if ((uVar11 & 1) == 0) {
        (**(code **)(*(long *)*plVar20 + 0x10))((long *)*plVar20,0);
      }
      else {
        lVar12 = (*(code *)PTR_FUN_0e2f1ea8)();
        (**(code **)(*(long *)*plVar20 + 8))((long *)*plVar20,param_1 + 0x100,bVar7);
        lVar13 = (*(code *)PTR_FUN_0e2f1ea8)();
        cStack_c9 = '\x16';
        if (("Omnibox.ProviderTime2." >= &local_e0 && "" < &cStack_ca) &&
            ("Omnibox.ProviderTime2." < &local_e0 || &cStack_ca != "Omnibox.ProviderTime2."))
        goto LAB_020cdcd8;
        uStack_d8 = 0x6469766f7250;
        local_e0 = (undefined8 *)0x2e786f62696e6d4f;
        uStack_d2 = 0x7265;
        uStack_d0 = 0x2e32656d6954;
        cStack_ca = '\0';
        pcVar14 = (char *)FUN_020cdf74(*(undefined4 *)(*plVar20 + 0x94));
        sVar15 = _strlen(pcVar14);
        puVar18 = (undefined8 *)FUN_000b8fc0(&local_e0,pcVar14,sVar15);
        local_a0 = (long *)puVar18[2];
        uStack_a8 = puVar18[1];
        local_b0 = (undefined8 *)*puVar18;
        puVar18[1] = 0;
        puVar18[2] = 0;
        *puVar18 = 0;
        uVar11 = lVar13 - lVar12 >> 0x3f ^ 0x8000000000000000;
        if (!SBORROW8(lVar13,lVar12)) {
          uVar11 = lVar13 - lVar12;
        }
        uVar9 = FUN_00a587c0(&local_b0,1000,5000000,0x14,1);
        FUN_008f6640(uVar9,uVar11);
        if ((long)local_a0 < 0) {
          FUN_00005810(local_b0,(ulong)local_a0 & 0x7fffffffffffffff);
        }
        if (cStack_c9 < '\0') {
          FUN_00005810(local_e0,CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0)) &
                                0x7fffffffffffffff);
        }
      }
      plVar20 = plVar20 + 1;
      lVar21 = lVar21 + -8;
    } while (lVar21 != 0);
  }
  if ((*(byte *)((long)param_2 + 0x1b3) & 1) == 0) {
    lVar21 = (*(code *)PTR_FUN_0e2f1ea8)();
    uVar11 = lVar21 - lVar10 >> 0x3f ^ 0x8000000000000000;
    if (!SBORROW8(lVar21,lVar10)) {
      uVar11 = lVar21 - lVar10;
    }
    FUN_02b75c1c("Omnibox.QueryTime2",uVar11,1000,1000000,0x32);
    if ((char)*(byte *)((long)param_2 + 0x17) < '\0') {
      if (5 < *(ulong *)(param_2 + 4)) goto LAB_020cdad0;
    }
    else if (5 < *(byte *)((long)param_2 + 0x17)) goto LAB_020cdad0;
    FUN_007a86f8(&local_e0);
    puVar18 = (undefined8 *)FUN_00f24ba0(&local_e0,0,"Omnibox.QueryTime2.",0x13);
    local_a0 = (long *)puVar18[2];
    uStack_a8 = puVar18[1];
    local_b0 = (undefined8 *)*puVar18;
    puVar18[1] = 0;
    puVar18[2] = 0;
    *puVar18 = 0;
    uVar9 = FUN_00a587c0(&local_b0,1000,1000000,0x32,1);
    FUN_008f6640(uVar9,uVar11);
    if ((long)local_a0 < 0) {
      FUN_00005810(local_b0,(ulong)local_a0 & 0x7fffffffffffffff);
    }
    if (cStack_c9 < '\0') {
      FUN_00005810(local_e0,CONCAT17(cStack_c9,CONCAT16(cStack_ca,uStack_d0)) & 0x7fffffffffffffff);
    }
  }
LAB_020cdad0:
  uVar9 = 1;
  FUN_007aa0e0("Omnibox.Start.WantAsyncMatches",(*(byte *)((long)param_2 + 0x1b3) ^ 0xff) & 1);
  if ((*(ulong *)(param_1 + 0x58) & 0x1fffffffffffffff) != 0) {
    bVar7 = false;
    plVar20 = *(long **)(param_1 + 0x50);
    plVar1 = plVar20 + *(ulong *)(param_1 + 0x58);
    do {
      while ((iVar8 = FUN_020ce020(param_1,*plVar20), iVar8 == 0 ||
             ((*(byte *)(*plVar20 + 0x90) & 1) != 0))) {
        plVar20 = plVar20 + 1;
        if (plVar20 == plVar1) {
          if (!bVar7) {
            uVar9 = 1;
            goto LAB_020cdb38;
          }
          goto LAB_020cdd1c;
        }
      }
      if (*(int *)(*plVar20 + 0x94) != 0x200) {
        uVar9 = 2;
        goto LAB_020cdb38;
      }
      plVar20 = plVar20 + 1;
      bVar7 = true;
    } while (plVar20 != plVar1);
LAB_020cdd1c:
    uVar9 = 2;
  }
LAB_020cdb38:
  FUN_02835204(param_1,uVar9);
  if ((((*(int *)(param_2 + 0x5a) == 4) && ((*(byte *)(param_1 + 0x888) & 1) == 0)) &&
      (*(long *)(param_1 + 0x578) * 0x6b0 != 0)) &&
     ((lVar21 = *(long *)(param_1 + 0x570), lVar21 != 0 && ((*(byte *)(lVar21 + 100) & 1) != 0)))) {
    *(undefined1 *)(param_1 + 0x888) = 1;
    if (*(char *)(lVar21 + 100) == '\0') {
      lVar21 = 0;
    }
    FUN_080e8554(*(undefined8 *)(param_1 + 0x48),lVar21 + 0x68);
  }
  if ((char)local_78 != '\0') {
    *(int *)(local_80 + 0x48) = *(int *)(local_80 + 0x48) + -1;
  }
  local_e0 = (undefined8 *)0x6f;
  if (DAT_0e4a44bf != '\0') {
    local_b0 = (undefined8 *)0x0;
    FUN_0000a280(DAT_0e4a44bf,&local_e0,&local_b0,2);
  }
  return;
}


