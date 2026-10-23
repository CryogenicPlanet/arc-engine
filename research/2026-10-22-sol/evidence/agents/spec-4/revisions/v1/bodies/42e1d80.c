// ==== 0x42e1d80  FUN_042e1d80  (1176 bytes)

void FUN_042e1d80(long param_1,undefined8 *param_2,undefined8 param_3,undefined8 *param_4,
                 int param_5)

{
  size_t sVar1;
  size_t sVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  ulong uVar5;
  byte bVar6;
  byte bVar7;
  code *pcVar8;
  int iVar9;
  undefined8 *puVar10;
  int *piVar11;
  undefined8 *puVar12;
  long *plVar13;
  undefined4 *puVar14;
  long lVar15;
  undefined8 uVar16;
  undefined8 *puVar17;
  long lVar18;
  long lVar19;
  undefined8 local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  ulong local_98;
  ulong local_90;
  undefined1 auStack_80 [32];
  
  lVar18 = *(long *)(param_1 + 0x30);
  if (lVar18 != 0) {
    puVar10 = (undefined8 *)FUN_000805e0(lVar18,&DAT_0c9d67c8);
    if (puVar10 == (undefined8 *)0x0) {
      puVar10 = (undefined8 *)FUN_00008790(0x50);
      puVar10[3] = 0;
      puVar10[2] = 0;
      puVar17 = puVar10 + 4;
      puVar10[5] = 0;
      *puVar17 = 0;
      puVar10[7] = 0;
      puVar10[6] = 0;
      puVar10[9] = 0;
      puVar10[8] = 0;
      *puVar10 = &PTR_FUN_0da7cb20;
      puVar10[1] = 0;
      *(undefined4 *)(puVar10 + 2) = 1;
      *puVar17 = 0;
      puVar10[5] = 0;
      puVar10[6] = 0;
      puVar10[3] = puVar17;
      puVar10[7] = 0;
      piVar11 = (int *)FUN_00008790(8);
      *piVar11 = 0;
      *(undefined1 *)(piVar11 + 1) = 0;
      if ((piVar11 != (int *)0x0) && (iVar9 = *piVar11, *piVar11 = iVar9 + 1, iVar9 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar8 = (code *)SoftwareBreakpoint(0,0x42e222c);
        (*pcVar8)();
      }
      puVar10[8] = piVar11;
      puVar10[9] = puVar10;
      FUN_011db6c0(lVar18,&DAT_0c9d67c8,puVar10);
    }
    if (puVar10[1] == 0) {
      puVar17 = (undefined8 *)FUN_00008790(0x18);
      puVar17[1] = 0;
      puVar17[2] = 0;
      *puVar17 = 0;
      plVar13 = (long *)puVar10[1];
      if (plVar13 == (long *)0x0) {
        puVar10[1] = puVar17;
      }
      else {
        lVar18 = *plVar13;
        if (lVar18 != 0) {
          lVar15 = plVar13[1] * 0x20;
          lVar19 = lVar18;
          if (lVar15 != 0) {
            lVar19 = plVar13[1] * -0x20;
            lVar15 = lVar15 + lVar18;
            do {
              lVar15 = lVar15 + -0x20;
              FUN_044294f4(lVar15);
              lVar19 = lVar19 + 0x20;
            } while (lVar19 != 0);
            lVar19 = *plVar13;
          }
          plVar13[1] = lVar18 - lVar19 >> 5;
          FUN_00005810(lVar19,plVar13[2] << 5);
        }
        FUN_00005810(plVar13,0x18);
        plVar13 = (long *)puVar10[1];
        puVar10[1] = puVar17;
        if (plVar13 != (long *)0x0) {
          lVar18 = *plVar13;
          if (lVar18 != 0) {
            lVar15 = plVar13[1] * 0x20;
            lVar19 = lVar18;
            if (lVar15 != 0) {
              lVar19 = plVar13[1] * -0x20;
              lVar15 = lVar15 + lVar18;
              do {
                lVar15 = lVar15 + -0x20;
                FUN_044294f4(lVar15);
                lVar19 = lVar19 + 0x20;
              } while (lVar19 != 0);
              lVar19 = *plVar13;
            }
            plVar13[1] = lVar18 - lVar19 >> 5;
            FUN_00005810(lVar19,plVar13[2] << 5);
          }
          FUN_00005810(plVar13,0x18);
        }
      }
    }
    uStack_a8 = 0;
    local_b0 = 0;
    local_98 = 0;
    uStack_a0 = 0;
    uStack_b8 = 0;
    local_c0 = 0;
    local_90 = 1;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00359998(&local_c0,*param_2,param_2[1]);
      if ((int)local_90 != 1) {
                    /* WARNING: Does not return */
        pcVar8 = (code *)SoftwareBreakpoint(0,0x42e2238);
        (*pcVar8)();
      }
    }
    else {
      uStack_b8 = param_2[1];
      local_c0 = *param_2;
      local_b0 = param_2[2];
    }
    bVar6 = *(byte *)((long)param_4 + 0x17);
    if ((long)local_98 < 0) {
      uVar5 = param_4[1];
      puVar17 = (undefined8 *)*param_4;
      if (-1 < (char)bVar6) {
        uVar5 = (ulong)bVar6;
        puVar17 = param_4;
      }
      FUN_00359ad8(&uStack_a8,puVar17,uVar5);
    }
    else if ((char)bVar6 < '\0') {
      FUN_00359998(&uStack_a8,*param_4,param_4[1]);
    }
    else {
      uStack_a0 = param_4[1];
      uStack_a8 = *param_4;
      local_98 = param_4[2];
    }
    puVar12 = (undefined8 *)FUN_042e29c0(puVar10[1],param_3);
    puVar17 = (undefined8 *)*puVar12;
    if (puVar17 == (undefined8 *)0x0) {
      puVar17 = (undefined8 *)FUN_00008790(0x58);
      *(undefined1 *)(puVar17 + 9) = 0;
      *(undefined1 *)(puVar17 + 10) = 0;
      *(undefined1 *)((long)puVar17 + 0x54) = 0;
      puVar17[1] = 0;
      *puVar17 = 0;
      puVar17[3] = 0;
      puVar17[2] = 0;
      puVar17[5] = 0;
      puVar17[4] = 0;
      *(undefined1 *)(puVar17 + 6) = 0;
      *puVar12 = puVar17;
    }
    if (param_5 != 0) {
      if (param_5 != 1) {
                    /* WARNING: Does not return */
        pcVar8 = (code *)SoftwareBreakpoint(0x550b,0x42e2204);
        (*pcVar8)();
      }
      puVar17 = puVar17 + 3;
    }
    lVar18 = puVar17[1] * 0x38;
    if (lVar18 != 0) {
      puVar12 = (undefined8 *)*puVar17;
      do {
        if (*(int *)(puVar12 + 6) != 1) {
                    /* WARNING: Does not return */
          pcVar8 = (code *)SoftwareBreakpoint(0,0x42e2208);
          (*pcVar8)();
        }
        bVar6 = *(byte *)((long)puVar12 + 0x17);
        sVar1 = puVar12[1];
        if (-1 < (char)bVar6) {
          sVar1 = (ulong)bVar6;
        }
        bVar7 = *(byte *)((long)param_2 + 0x17);
        sVar2 = param_2[1];
        if (-1 < (char)bVar7) {
          sVar2 = (ulong)bVar7;
        }
        if (sVar1 == sVar2) {
          puVar3 = (void *)*puVar12;
          if (-1 < (char)bVar6) {
            puVar3 = puVar12;
          }
          puVar4 = (void *)*param_2;
          if (-1 < (char)bVar7) {
            puVar4 = param_2;
          }
          iVar9 = _memcmp(puVar3,puVar4,sVar1);
          if (iVar9 == 0) {
            FUN_042e4104(puVar12,&local_c0);
            goto LAB_042e1fd0;
          }
        }
        puVar12 = puVar12 + 7;
        lVar18 = lVar18 + -0x38;
      } while (lVar18 != 0);
    }
    FUN_042e5a3c(puVar17,&local_c0);
LAB_042e1fd0:
    plVar13 = (long *)(*(code *)PTR___tlv_bootstrap_0e379840)();
    if (((undefined8 *)*plVar13 == (undefined8 *)0x0) ||
       (puVar17 = *(undefined8 **)*plVar13, puVar17 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
      pcVar8 = (code *)SoftwareBreakpoint(0,0x42e2214);
      (*pcVar8)();
    }
    FUN_00029e40(auStack_80,"NotifyBoostConfigChanged",
                 "../../arc/browser/browser_context/effects_impl.cc",0x2d);
    piVar11 = (int *)puVar10[8];
    if ((piVar11 != (int *)0x0) && (iVar9 = *piVar11, *piVar11 = iVar9 + 1, iVar9 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar8 = (code *)SoftwareBreakpoint(0,0x42e2220);
      (*pcVar8)();
    }
    uVar16 = puVar10[9];
    puVar14 = (undefined4 *)FUN_00008790(0x40);
    *puVar14 = 1;
    *(undefined1 **)(puVar14 + 2) = &LAB_040c5ad0;
    *(code **)(puVar14 + 4) = FUN_040a6300;
    *(undefined1 **)(puVar14 + 6) = &LAB_04169bc0;
    *(code **)(puVar14 + 8) = FUN_042e47c4;
    *(undefined8 *)(puVar14 + 10) = 0;
    *(int **)(puVar14 + 0xc) = piVar11;
    *(undefined8 *)(puVar14 + 0xe) = uVar16;
    (**(code **)*puVar17)(puVar17,auStack_80,puVar14,0);
    if ((int)local_90 != 0) {
      local_90 = local_90 & 0xffffffff00000000;
      if ((long)local_98 < 0) {
        FUN_00005810(uStack_a8,local_98 & 0x7fffffffffffffff);
      }
      if ((long)local_b0 < 0) {
        FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
      }
    }
  }
  return;
}


