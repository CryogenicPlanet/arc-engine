// ==== 0x9fc1b54  FUN_09fc1b54  (1284 bytes)

/* WARNING: Removing unreachable block (ram,0x09fc2024) */

undefined8 FUN_09fc1b54(double param_1,long param_2,long *param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  code *pcVar3;
  byte bVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  long *plVar8;
  ulong *puVar9;
  ulong uVar10;
  ulong uVar11;
  long lVar12;
  byte bVar13;
  long lVar14;
  double dVar15;
  undefined8 local_160;
  ulong uStack_158;
  ulong local_150;
  ulong uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  ulong local_130;
  double dStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 local_e0;
  undefined8 uStack_d8;
  long *local_d0;
  ulong local_c8;
  ulong local_c0;
  undefined8 local_b0;
  undefined8 uStack_a8;
  long *local_a0;
  ulong local_98;
  ulong local_90;
  ulong local_88;
  undefined8 local_80;
  double dStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  
  lVar6 = thunk_FUN_00882dd0(*(undefined8 *)(param_2 + 0x18),*(undefined4 *)(param_2 + 0x20));
  if (*(int *)(param_2 + 0x28) == 3) {
    return 0;
  }
  thunk_FUN_00882dd0(*(undefined8 *)(param_2 + 0x18),*(undefined4 *)(param_2 + 0x20));
  uVar7 = thunk_FUN_01d926b0();
  if ((int)uVar7 == 0) {
    return uVar7;
  }
  lVar14 = *param_3;
  if (lVar14 != 0) {
    thunk_FUN_003d26f0(lVar14 + 8);
  }
  lVar12 = *(long *)(param_2 + 0x40);
  *(long *)(param_2 + 0x40) = lVar14;
  if ((lVar12 != 0) && (thunk_FUN_003d2710(lVar12 + 8), *(int *)(lVar12 + 8) == 0)) {
    uVar7 = FUN_06b04d78(lVar12);
    thunk_FUN_00005810(uVar7,0x18);
  }
  if (*(int *)(param_2 + 0x28) == 2) {
    dVar15 = *(double *)(param_2 + 0x30);
    if (ABS(dVar15 - param_1) <= 0.001) {
      return 1;
    }
    *(double *)(param_2 + 0x30) = param_1;
    local_120 = 0;
    uStack_138 = 0;
    local_140 = 0;
    local_130 = 0;
    uStack_158 = 0;
    uStack_148 = 0;
    local_150 = 0;
    local_160 = 2;
    dStack_128 = param_1;
    uVar7 = FUN_032e87a4(*(undefined8 *)(param_2 + 0x80));
    FUN_0252703c(uVar7,&local_160);
    bVar13 = *(byte *)(param_2 + 0x24);
    if (*param_3 != 0) {
      bVar4 = FUN_06b04d04();
      bVar13 = bVar13 & (bVar4 ^ 1);
    }
    uStack_68 = 0;
    local_70 = 0;
    dStack_78 = 0.0;
    local_80 = 0;
    uVar11 = *(ulong *)(param_2 + 0x18);
    uVar1 = *(undefined4 *)(*(long *)(lVar6 + 0x140) + 0x10);
    uVar7 = *(undefined8 *)(param_2 + 0x30);
    uVar2 = *(undefined4 *)(param_2 + 0x28);
    if ((uVar11 & 0xfffffffc00000000) == DAT_0e2f2208) {
      thunk_FUN_0000ffa8(uVar11);
    }
    local_90 = 0;
    local_80 = CONCAT44(local_80._4_4_,uVar1);
    uStack_68._0_5_ = CONCAT14(bVar13,uVar2);
    uStack_a8 = 0;
    local_b0 = 0;
    local_98 = 0;
    local_a0 = (long *)0x0;
    local_88 = uVar11;
    dStack_78 = dVar15;
    local_70 = uVar7;
    if (*(long *)(param_2 + 0x70) != 0) {
      thunk_FUN_000dec00(&local_b0,param_2 + 0x48);
    }
    local_c0 = 0;
    uStack_d8 = 0;
    local_e0 = 0;
    local_c8 = 0;
    local_d0 = (long *)0x0;
    do {
      if (local_a0 == (long *)0x0) {
LAB_09fc1d0c:
        if (local_d0 != (long *)0x0) {
          uVar11 = local_d0[1];
          if (local_c0 <= (ulong)local_d0[1]) {
            uVar11 = local_c0;
          }
          if (local_c8 != uVar11) goto LAB_09fc1d2c;
        }
LAB_09fc1de4:
        FUN_0236ae80(&local_e0);
        FUN_0236ae80(&local_b0);
        lVar6 = *(long *)(param_2 + 0x40);
        *(undefined8 *)(param_2 + 0x40) = 0;
        if ((lVar6 != 0) && (thunk_FUN_003d2710(lVar6 + 8), *(int *)(lVar6 + 8) == 0)) {
          uVar7 = FUN_06b04d78(lVar6);
          thunk_FUN_00005810(uVar7,0x18);
        }
        if ((local_88 & 0xfffffffc00000000) == DAT_0e2f2208) {
          thunk_FUN_0001fee4();
        }
        if ((long)local_130 < 0) {
          thunk_FUN_00005810(local_140,local_130 & 0x7fffffffffffffff);
        }
        if (-1 < (long)uStack_148) {
          return 1;
        }
        uVar10 = uStack_148 & 0x7fffffffffffffff;
        uVar11 = uStack_158;
        goto LAB_09fc1e54;
      }
      uVar11 = local_a0[1];
      if (local_90 <= (ulong)local_a0[1]) {
        uVar11 = local_90;
      }
      if (local_98 == uVar11) goto LAB_09fc1d0c;
LAB_09fc1d2c:
      if (local_98 == local_c8 && local_a0 == local_d0) goto LAB_09fc1de4;
      if ((ulong)local_a0[1] <= local_98) {
LAB_09fc1f50:
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(1,0x9fc1f54);
        (*pcVar3)();
      }
      plVar8 = (long *)(*local_a0 + local_98 * 0x10);
      lVar6 = *plVar8;
      if ((lVar6 == 0) || (*(char *)(lVar6 + 4) != '\0')) {
        plVar8 = (long *)0x0;
      }
      else {
        plVar8 = (long *)plVar8[1];
      }
      (**(code **)(*plVar8 + 0x18))(plVar8,&local_88);
      if (local_a0 != (long *)0x0) {
        uVar11 = local_a0[1];
        if (local_90 <= (ulong)local_a0[1]) {
          uVar11 = local_90;
        }
        do {
          local_98 = local_98 + 1;
          if (uVar11 <= local_98) break;
          if ((ulong)local_a0[1] <= local_98) goto LAB_09fc1f50;
          iVar5 = FUN_026da700(*local_a0 + local_98 * 0x10);
        } while (iVar5 != 0);
      }
    } while( true );
  }
  lVar14 = FUN_03a708b0(*(undefined8 *)(lVar6 + 0x120));
  if (*(long *)(param_2 + 0x88) != lVar14) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x9fc2058);
    (*pcVar3)();
  }
  uVar11 = *(ulong *)(param_2 + 0x18);
  uVar1 = *(undefined4 *)(*(long *)(lVar6 + 0x140) + 0x10);
  uVar10 = FUN_04597a58(param_2);
  puVar9 = (ulong *)thunk_FUN_00008790(0x28);
  uVar2 = *(undefined4 *)(param_2 + 0x28);
  if ((uVar11 & 0xfffffffc00000000) == DAT_0e2f2208) {
    thunk_FUN_0000ffa8(uVar11);
  }
  *puVar9 = uVar11;
  *(undefined4 *)(puVar9 + 1) = uVar1;
  puVar9[2] = uVar10;
  puVar9[3] = (ulong)param_1;
  *(undefined4 *)(puVar9 + 4) = uVar2;
  *(undefined1 *)((long)puVar9 + 0x24) = 0;
  local_160 = 0;
  FUN_04597cdc(param_2 + 0x38,puVar9);
  FUN_04597cdc(&local_160,0);
  if (*(int *)(param_2 + 0x28) != 1) {
    local_160 = FUN_0259be70(lVar6);
    uVar11 = FUN_04146834(lVar14,&local_160);
    if ((uVar11 & 1) == 0) {
      local_f0 = 0;
      uStack_108 = 0;
      local_110 = 0;
      uStack_f8 = 0;
      uStack_100 = 0;
      dStack_128 = 0.0;
      local_130 = 0;
      uStack_118 = 0;
      local_120 = 0;
      uStack_148 = 0;
      local_150 = 0;
      uStack_138 = 0;
      local_140 = 0;
      uStack_158 = 0;
      local_160 = 0;
      FUN_0259be70(lVar6);
      FUN_0236b038(&local_160);
      uVar11 = uStack_158;
      if ((local_150 & 0x8000000000000000) == 0) {
        uVar11 = local_150 >> 0x38;
      }
      if (uVar11 == 0) {
        lVar6 = *(long *)(param_2 + 0x40);
        *(undefined8 *)(param_2 + 0x40) = 0;
        if ((lVar6 != 0) && (thunk_FUN_003d2710(lVar6 + 8), *(int *)(lVar6 + 8) == 0)) {
          uVar7 = FUN_06b04d78(lVar6);
          thunk_FUN_00005810(uVar7,0x18);
        }
        FUN_04597cdc(param_2 + 0x38,0);
        thunk_FUN_0002822c(&local_160);
        return 0;
      }
      local_88 = 0;
      local_80 = 0;
      dStack_78 = 0.0;
      thunk_FUN_00883690(&local_88,&local_160);
      FUN_064fee44(param_1,lVar14,&local_88);
      thunk_FUN_0002822c(&local_160);
      goto LAB_09fc1f30;
    }
  }
  local_160 = FUN_0259be70(lVar6);
  FUN_02bb3db0(param_1,lVar14,&local_160);
LAB_09fc1f30:
  lVar6 = *(long *)(param_2 + 0x40);
  *(undefined8 *)(param_2 + 0x40) = 0;
  if ((lVar6 != 0) && (thunk_FUN_003d2710(lVar6 + 8), *(int *)(lVar6 + 8) == 0)) {
    uVar11 = FUN_06b04d78(lVar6);
    uVar10 = 0x18;
LAB_09fc1e54:
    thunk_FUN_00005810(uVar11,uVar10);
  }
  return 1;
}


