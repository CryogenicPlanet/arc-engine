// ==== 0x42b0850  FUN_042b0850  (1168 bytes)

/* WARNING: Removing unreachable block (ram,0x042b0958) */

undefined8 FUN_042b0850(ulong param_1,undefined4 *param_2)

{
  undefined1 *puVar1;
  long *plVar2;
  undefined4 uVar3;
  uint uVar4;
  byte bVar5;
  byte bVar6;
  undefined1 uVar7;
  int *piVar8;
  code *pcVar9;
  int iVar10;
  undefined8 *puVar11;
  long lVar12;
  long *plVar13;
  long lVar14;
  undefined4 uVar15;
  size_t sVar16;
  long *plVar17;
  long lVar18;
  undefined8 uVar19;
  undefined8 *puVar20;
  ulong uVar21;
  int *local_f8;
  undefined4 local_f0;
  long *local_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  int *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  int *local_88;
  int *local_80;
  undefined8 uStack_78;
  ulong local_70;
  
  if (*(long *)(param_1 + 0x98) == 0) {
    uVar3 = param_2[0xe];
    uVar15 = 1;
    if (*(char *)(param_2 + 0x16) == '\0') {
      uVar15 = 2;
    }
    uVar4 = param_2[0x17];
    bVar5 = *(byte *)(param_2 + 0x18);
    bVar6 = *(byte *)((long)param_2 + 0x61);
    if (*(char *)((long)param_2 + 0x57) < '\0') {
      FUN_00060a60(&local_80,*(undefined8 *)(param_2 + 0x10),*(undefined8 *)(param_2 + 0x12));
    }
    else {
      uStack_78 = *(undefined8 *)(param_2 + 0x12);
      local_80 = *(int **)(param_2 + 0x10);
      local_70 = *(ulong *)(param_2 + 0x14);
    }
    uVar19 = FUN_00008790(0x58);
    uStack_b8 = uStack_78;
    local_c0 = local_80;
    local_b0 = local_70;
    local_80 = (int *)0x0;
    uStack_78 = 0;
    local_70 = 0;
    DAT_0e4ac750 = FUN_044d3828(uVar19,uVar3,uVar15,(ulong)uVar4 | 0x100000000,
                                (ulong)bVar5 ^ 1 | 0x100000000,(ulong)bVar6 | 0x100000000,&local_c0)
    ;
    if ((long)local_b0 < 0) {
      FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
    }
    if (*(int *)(DAT_0e4ac750 + 0x14) == 0) {
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(0,0x42b0cdc);
      (*pcVar9)();
    }
    FUN_0354e8e8();
    puVar1 = (undefined1 *)(param_1 + 0x148);
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1 + 0x148);
    }
    uVar7 = *(undefined1 *)(param_1 + 0x148);
    *(undefined1 *)(param_1 + 0x148) = 1;
    uVar19 = FUN_01f0d54c();
    FUN_028d08b8(uVar19,1);
    uVar19 = FUN_07bf4c40();
    FUN_04d32b00(uVar19,param_1 + 0x28);
    if ((param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1);
    }
    local_88 = (int *)FUN_00008790(0x38);
    *local_88 = 1;
    *(undefined1 **)(local_88 + 2) = &LAB_040c4380;
    *(code **)(local_88 + 4) = FUN_039a7300;
    *(code **)(local_88 + 6) = FUN_00b77080;
    *(code **)(local_88 + 8) = FUN_042b0ce4;
    local_88[10] = 0;
    local_88[0xb] = 0;
    *(ulong *)(local_88 + 0xc) = param_1;
    FUN_07bf4b40(&local_c0,&local_88);
    FUN_01469964(param_1 + 0x460,&local_c0);
    piVar8 = local_c0;
    if (local_c0 != (int *)0x0) {
      local_c0 = (int *)0x0;
      (**(code **)(piVar8 + 2))(piVar8);
      LOAcquire();
      iVar10 = *piVar8;
      *piVar8 = iVar10 + -1;
      LORelease();
      if (iVar10 == 1) {
        (**(code **)(piVar8 + 4))(piVar8);
      }
      if (local_c0 != (int *)0x0) {
        LOAcquire();
        iVar10 = *local_c0;
        *local_c0 = iVar10 + -1;
        LORelease();
        if (iVar10 == 1) {
          (**(code **)(local_c0 + 4))();
        }
      }
    }
    if (local_88 != (int *)0x0) {
      LOAcquire();
      iVar10 = *local_88;
      *local_88 = iVar10 + -1;
      LORelease();
      if (iVar10 == 1) {
        (**(code **)(local_88 + 4))();
      }
    }
    puVar11 = (undefined8 *)FUN_00008790(0x38);
    *(undefined1 *)(puVar11 + 5) = 0;
    puVar11[6] = 0;
    puVar11[1] = 0;
    *puVar11 = 0;
    puVar11[3] = 0;
    puVar11[2] = 0;
    *(undefined1 *)(puVar11 + 4) = 0;
    lVar12 = *(long *)(param_1 + 0x98);
    *(undefined8 **)(param_1 + 0x98) = puVar11;
    if (lVar12 != 0) {
      uVar19 = FUN_042aa684();
      FUN_00005810(uVar19,0x38);
    }
    uStack_90 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    local_98 = 0;
    local_a0 = 0;
    uStack_b8 = 0;
    local_c0 = (int *)0x0;
    FUN_0421b464(&local_c0,0);
    plVar17 = (long *)(param_1 + 0x318);
    lVar12 = *(long *)(param_1 + 0x318);
    if (lVar12 != 0) {
      lVar18 = *(long *)(param_1 + 800) * 0x18;
      lVar14 = lVar12;
      if (lVar18 != 0) {
        lVar14 = *(long *)(param_1 + 800) * -0x18;
        puVar11 = (undefined8 *)(lVar18 + lVar12);
        do {
          puVar20 = puVar11 + -3;
          if (puVar20 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x42b0ce4);
            (*pcVar9)();
          }
          if (*(char *)((long)puVar11 + -1) < '\0') {
            FUN_00005810(*puVar20,puVar11[-1] & 0x7fffffffffffffff);
          }
          lVar14 = lVar14 + 0x18;
          puVar11 = puVar20;
        } while (lVar14 != 0);
        lVar14 = *plVar17;
      }
      *(long *)(param_1 + 800) = (lVar12 - lVar14 >> 3) * -0x5555555555555555;
      FUN_00005810(lVar14,*(long *)(param_1 + 0x328) * 0x18);
      *plVar17 = 0;
      *(undefined8 *)(param_1 + 800) = 0;
      *(undefined8 *)(param_1 + 0x328) = 0;
    }
    *(undefined8 *)(param_1 + 0x318) = *(undefined8 *)(param_2 + 2);
    *(undefined8 *)(param_1 + 800) = *(undefined8 *)(param_2 + 4);
    *(undefined8 *)(param_1 + 0x328) = *(undefined8 *)(param_2 + 6);
    *(undefined8 *)(param_2 + 2) = 0;
    *(undefined8 *)(param_2 + 4) = 0;
    *(undefined8 *)(param_2 + 6) = 0;
    uVar21 = *(ulong *)(param_1 + 800);
    sVar16 = uVar21 << 3;
    if (uVar21 >> 0x3d != 0) {
      sVar16 = 0xffffffffffffffff;
    }
    plVar13 = (long *)FUN_00008790(sVar16);
    _bzero(plVar13,sVar16);
    lVar12 = *(long *)(param_1 + 0x330);
    *(long **)(param_1 + 0x330) = plVar13;
    if (lVar12 != 0) {
      FUN_00010ba4();
      uVar21 = *(ulong *)(param_1 + 800);
      plVar13 = *(long **)(param_1 + 0x330);
    }
    lVar12 = uVar21 * 0x18;
    if (lVar12 != 0) {
      plVar17 = (long *)*plVar17;
      do {
        plVar2 = (long *)*plVar17;
        if (-1 < *(char *)((long)plVar17 + 0x17)) {
          plVar2 = plVar17;
        }
        *plVar13 = (long)plVar2;
        plVar17 = plVar17 + 3;
        lVar12 = lVar12 + -0x18;
        plVar13 = plVar13 + 1;
      } while (lVar12 != 0);
      plVar13 = *(long **)(param_1 + 0x330);
    }
    uStack_b8 = CONCAT44(uStack_b8._4_4_,*param_2);
    if (((ulong)plVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(plVar13);
    }
    if ((local_b0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    uStack_d8 = local_a0;
    local_e0 = uStack_a8;
    local_f8 = local_c0;
    local_f0 = (undefined4)uStack_b8;
    local_c0 = (int *)0x0;
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
    uStack_c8 = uStack_90;
    uStack_d0 = local_98;
    local_e8 = plVar13;
    uVar19 = FUN_042aa7ec(*(undefined8 *)(param_1 + 0x98),&local_f8,0,0,0,param_2 + 8);
    FUN_02440774(&local_f8);
    FUN_02440774(&local_c0);
    if (puVar1 != (undefined1 *)0x0) {
      *puVar1 = uVar7;
    }
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(puVar1);
    }
  }
  else {
    uVar19 = 0xffffffff;
  }
  return uVar19;
}


