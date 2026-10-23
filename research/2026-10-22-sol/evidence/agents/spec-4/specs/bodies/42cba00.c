// ==== 0x42cba00  FUN_042cba00  (1304 bytes)

void FUN_042cba00(long param_1,undefined8 *param_2,undefined8 *param_3)

{
  char cVar1;
  int iVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  undefined4 *puVar6;
  ulong uVar7;
  undefined1 uVar8;
  long lVar9;
  undefined8 *puVar10;
  code *pcVar11;
  long lVar12;
  int *piVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined1 local_180 [24];
  undefined1 local_168;
  ulong local_160;
  long lStack_158;
  ulong local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  ulong local_e8;
  undefined4 local_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  int iStack_d4;
  undefined4 uStack_d0;
  int iStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70 [2];
  
  if (*(long *)(param_1 + 0x88) == 0) {
    local_160 = 0;
    lStack_158 = 0;
    local_150 = 0;
    (*(code *)param_3[2])(param_3,&local_160);
    uVar7 = local_160;
    if (local_160 == 0) {
      return;
    }
    if (lStack_158 * 0x10 != 0) {
      lVar5 = lStack_158 * -0x10;
      lVar9 = lStack_158 * 0x10 + local_160;
      do {
        if (lVar9 + -0x10 == 0) goto LAB_042cbf0c;
        plVar4 = *(long **)(lVar9 + -8);
        if (plVar4 != (long *)0x0) {
          LOAcquire();
          lVar12 = plVar4[1];
          plVar4[1] = lVar12 + -1;
          LORelease();
          if (lVar12 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4);
            if (plVar4[2] != 0) {
              LOAcquire();
              lVar12 = plVar4[2];
              plVar4[2] = lVar12 + -1;
              LORelease();
              if (lVar12 != 0) goto LAB_042cbe08;
            }
            pcVar11 = *(code **)(*plVar4 + 0x20);
            if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) ||
               (pcVar11 == FUN_013efb40)) {
              FUN_00005810(plVar4,0x20);
            }
            else {
              (*pcVar11)(plVar4);
            }
          }
        }
LAB_042cbe08:
        lVar5 = lVar5 + 0x10;
        lVar9 = lVar9 + -0x10;
      } while (lVar5 != 0);
    }
    lStack_158 = (long)(uVar7 - local_160) >> 4;
    uVar7 = local_150 << 4;
  }
  else {
    FUN_0119426c();
    plVar4 = (long *)FUN_02c31954();
    cVar1 = *(char *)((long)param_2 + 0x17);
    lVar9 = (long)cVar1;
    lVar5 = lVar9;
    if (lVar9 < 0) {
      lVar5 = param_2[1];
    }
    if (lVar5 == 0) {
      piVar13 = *(int **)(param_1 + 0x310);
      if ((piVar13 != (int *)0x0) && (iVar2 = *piVar13, *piVar13 = iVar2 + 1, iVar2 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar11 = (code *)SoftwareBreakpoint(0,0x42cbf14);
        (*pcVar11)();
      }
      uVar14 = *(undefined8 *)(param_1 + 0x318);
      puVar6 = (undefined4 *)FUN_00008790(0x60);
      *puVar6 = 1;
      *(undefined **)(puVar6 + 2) = &DAT_042da57c;
      *(code **)(puVar6 + 4) = FUN_042da658;
      *(undefined1 **)(puVar6 + 6) = &LAB_04169bc0;
      *(code **)(puVar6 + 8) = FUN_042cbf28;
      *(undefined8 *)(puVar6 + 10) = 0;
      *(int **)(puVar6 + 0xc) = piVar13;
      *(undefined8 *)(puVar6 + 0xe) = uVar14;
      puVar10 = (undefined8 *)param_3[3];
      uVar14 = *param_3;
      uVar16 = param_3[3];
      uVar15 = param_3[2];
      *(undefined8 *)(puVar6 + 0x12) = param_3[1];
      *(undefined8 *)(puVar6 + 0x10) = uVar14;
      *(undefined8 *)(puVar6 + 0x16) = uVar16;
      *(undefined8 *)(puVar6 + 0x14) = uVar15;
      if ((code *)*puVar10 != (code *)0x0) {
        uVar14 = (*(code *)*puVar10)(*param_3);
        *(undefined8 *)(puVar6 + 0x10) = uVar14;
      }
                    /* WARNING: Could not recover jumptable at 0x042cbc20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar4 + 0x10))(plVar4,puVar6);
      return;
    }
    puVar10 = (undefined8 *)*param_2;
    if (-1 < cVar1) {
      puVar10 = param_2;
    }
    uStack_f8 = 0;
    local_100 = 0;
    local_e8 = 0;
    uStack_f0 = 0;
    uStack_118 = 0;
    local_120 = 0;
    uStack_108 = 0;
    uStack_110 = 0;
    uStack_138 = 0;
    local_140 = 0;
    uStack_128 = 0;
    uStack_130 = 0;
    lStack_158 = 0;
    local_160 = 0;
    uStack_148 = 0;
    local_150 = 0;
    local_70[0] = 0;
    uStack_88 = 0;
    local_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    uStack_98 = 0;
    uStack_a0 = 0;
    uStack_c8 = 0;
    uStack_c4 = 0;
    uStack_d0 = 0;
    iStack_cc = 0;
    uStack_b8 = 0;
    local_c0 = 0;
    uStack_bc = 0;
    lVar5 = param_2[1];
    if (-1 < cVar1) {
      lVar5 = lVar9;
    }
    uStack_d8 = 0;
    iStack_d4 = 0;
    local_e0 = 0;
    uStack_dc = 0;
    FUN_00c28450(&local_e0,puVar10,lVar5);
    if (((char)uStack_c8 == '\0') ||
       ((lVar5 = FUN_007cf864(&local_e0), *(char *)(lVar5 + 0x17) < '\0' &&
        (0x200000 < *(ulong *)(lVar5 + 8))))) {
      uVar8 = 0;
      local_160 = local_160 & 0xffffffffffffff00;
    }
    else {
      FUN_00827294(&local_160,&local_e0);
      uVar8 = 1;
    }
    local_e8 = CONCAT71(local_e8._1_7_,uVar8);
    FUN_000282bc(local_70,0);
    FUN_0002827c(&local_c0);
    if (iStack_cc < 0) {
      FUN_00005810(CONCAT44(uStack_dc,local_e0),CONCAT44(iStack_cc,uStack_d0) & 0x7fffffffffffffff);
    }
    if ((local_e8 & 1) == 0) {
      local_e0 = 0;
      uStack_dc = 0;
      uStack_d8 = 0;
      iStack_d4 = 0;
      uStack_d0 = 0;
      iStack_cc = 0;
      (*(code *)param_3[2])(param_3,&local_e0);
      lVar5 = CONCAT44(uStack_dc,local_e0);
      if (lVar5 != 0) {
        lVar12 = CONCAT44(iStack_d4,uStack_d8) * 0x10;
        lVar9 = lVar5;
        if (lVar12 != 0) {
          lVar9 = CONCAT44(iStack_d4,uStack_d8) * -0x10;
          lVar12 = lVar12 + lVar5;
          do {
            if (lVar12 + -0x10 == 0) {
LAB_042cbf0c:
                    /* WARNING: Does not return */
              pcVar11 = (code *)SoftwareBreakpoint(1,0x42cbf10);
              (*pcVar11)();
            }
            plVar4 = *(long **)(lVar12 + -8);
            if (plVar4 != (long *)0x0) {
              LOAcquire();
              lVar3 = plVar4[1];
              plVar4[1] = lVar3 + -1;
              LORelease();
              if (lVar3 == 0) {
                (**(code **)(*plVar4 + 0x10))(plVar4);
                if (plVar4[2] != 0) {
                  LOAcquire();
                  lVar3 = plVar4[2];
                  plVar4[2] = lVar3 + -1;
                  LORelease();
                  if (lVar3 != 0) goto LAB_042cbe90;
                }
                pcVar11 = *(code **)(*plVar4 + 0x20);
                if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) ||
                   (pcVar11 == FUN_013efb40)) {
                  FUN_00005810(plVar4,0x20);
                }
                else {
                  (*pcVar11)(plVar4);
                }
              }
            }
LAB_042cbe90:
            lVar9 = lVar9 + 0x10;
            lVar12 = lVar12 + -0x10;
          } while (lVar9 != 0);
          lVar9 = CONCAT44(uStack_dc,local_e0);
        }
        uStack_d8 = (undefined4)(lVar5 - lVar9 >> 4);
        iStack_d4 = (int)(lVar5 - lVar9 >> 0x24);
        FUN_00005810(lVar9,CONCAT44(iStack_cc,uStack_d0) << 4);
      }
    }
    else {
      local_e0 = 0;
      uStack_dc = 3;
      uStack_d8 = 3;
      iStack_d4 = 0;
      uStack_d0 = 0;
      iStack_cc = 0;
      uStack_c8 = 0;
      uStack_c4 = 0;
      local_c0 = 0;
      uStack_bc = 0;
      local_180[0] = 0;
      local_168 = 0;
      piVar13 = *(int **)(param_1 + 0x310);
      if ((piVar13 != (int *)0x0) && (iVar2 = *piVar13, *piVar13 = iVar2 + 1, iVar2 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar11 = (code *)SoftwareBreakpoint(0,0x42cbf20);
        (*pcVar11)();
      }
      uVar14 = *(undefined8 *)(param_1 + 0x318);
      puVar6 = (undefined4 *)FUN_00008790(0x60);
      *puVar6 = 1;
      *(undefined1 **)(puVar6 + 2) = &LAB_042da6bc;
      *(code **)(puVar6 + 4) = FUN_042da658;
      *(undefined1 **)(puVar6 + 6) = &LAB_04169bc0;
      *(undefined1 **)(puVar6 + 8) = &LAB_042cc18c;
      *(undefined8 *)(puVar6 + 10) = 0;
      *(int **)(puVar6 + 0xc) = piVar13;
      *(undefined8 *)(puVar6 + 0xe) = uVar14;
      puVar10 = (undefined8 *)param_3[3];
      uVar14 = *param_3;
      uVar16 = param_3[3];
      uVar15 = param_3[2];
      *(undefined8 *)(puVar6 + 0x12) = param_3[1];
      *(undefined8 *)(puVar6 + 0x10) = uVar14;
      *(undefined8 *)(puVar6 + 0x16) = uVar16;
      *(undefined8 *)(puVar6 + 0x14) = uVar15;
      if ((code *)*puVar10 != (code *)0x0) {
        uVar14 = (*(code *)*puVar10)(*param_3);
        *(undefined8 *)(puVar6 + 0x10) = uVar14;
      }
      (**(code **)(*plVar4 + 0x20))(plVar4,&local_160,&local_e0,local_180,puVar6);
      FUN_00af3ca4(local_180);
    }
    if ((char)local_e8 == '\0') {
      return;
    }
    FUN_000282bc(&uStack_f0,0);
    FUN_0002827c(&local_140);
    if (-1 < (long)local_150) {
      return;
    }
    uVar7 = local_150 & 0x7fffffffffffffff;
  }
  FUN_00005810(local_160,uVar7);
  return;
}


