// ==== 0x4357078  FUN_04357078  (1696 bytes)

/* WARNING: Removing unreachable block (ram,0x04357200) */
/* WARNING: Removing unreachable block (ram,0x043571e8) */
/* WARNING: Removing unreachable block (ram,0x04357348) */

void FUN_04357078(long param_1,undefined8 param_2,undefined8 *param_3)

{
  uint uVar1;
  undefined1 uVar2;
  undefined3 uVar3;
  code *pcVar4;
  int iVar5;
  long lVar6;
  long *plVar7;
  ulong uVar8;
  undefined4 *puVar9;
  undefined8 *puVar10;
  int *piVar11;
  undefined8 uVar12;
  long lVar13;
  undefined8 uVar14;
  long *plVar15;
  int *piVar16;
  undefined4 local_104;
  undefined8 local_100;
  undefined4 local_f8;
  undefined1 local_f4;
  undefined4 uStack_f3;
  undefined3 uStack_ef;
  undefined1 local_ec;
  undefined8 local_e8 [2];
  undefined7 local_d8;
  char cStack_d1;
  byte local_d0;
  undefined8 local_c8 [2];
  undefined7 local_b8;
  char cStack_b1;
  byte local_b0;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  byte local_90;
  char local_88 [23];
  undefined1 uStack_71;
  undefined7 local_70;
  undefined4 uStack_69;
  char cStack_65;
  undefined4 uStack_64;
  undefined8 local_60;
  undefined1 uStack_51;
  
  if (*(long *)(param_1 + 0x50) == 0) {
    return;
  }
  if (*(char *)(*(long *)(param_1 + 0x50) + 4) != '\0') {
    return;
  }
  if (*(long *)(param_1 + 0x58) == 0) {
    return;
  }
  if (*(long *)(param_1 + 0x60) != 0) {
    uVar14 = *(undefined8 *)(*(long *)(param_1 + 0x60) + 0xe0);
    local_60 = CONCAT17(0xb,(undefined7)local_60);
    if (&cStack_65 <= "DevToolsAPI" || "DevToolsAPI" < &local_70) {
      local_70 = 0x6c6f6f54766544;
      uStack_69 = 0x49504173;
      cStack_65 = '\0';
      uStack_71 = 9;
      if (local_88 + 9 <= "showPanel" || "showPanel" < local_88) {
        builtin_strncpy(local_88,"showPanel",10);
        lVar13 = (long)*(char *)((long)param_3 + 0x17);
        puVar10 = param_3;
        if (lVar13 < 0) {
          puVar10 = (undefined8 *)*param_3;
          lVar13 = param_3[1];
        }
        FUN_00c87c40(local_a8,puVar10,lVar13);
        local_b0 = 0;
        local_d0 = 0;
        FUN_0c0f84c0(uVar14,&local_70,local_88,local_a8,local_c8,local_e8,0);
        if ((local_d0 != 0xff) && ((local_d0 & 0xfd) != 0)) {
          if (local_d0 == 4) {
            if (cStack_d1 < '\0') {
              FUN_00005810(local_e8[0],CONCAT17(cStack_d1,local_d8) & 0x7fffffffffffffff);
            }
          }
          else {
            (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_d0])(&uStack_51,local_e8);
          }
        }
        local_d0 = 0xff;
        if ((local_b0 != 0xff) && ((local_b0 & 0xfd) != 0)) {
          if (local_b0 == 4) {
            if (cStack_b1 < '\0') {
              FUN_00005810(local_c8[0],CONCAT17(cStack_b1,local_b8) & 0x7fffffffffffffff);
            }
          }
          else {
            (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_b0])(&uStack_51,local_c8);
          }
        }
        local_b0 = 0xff;
        if (local_90 == 0xff) {
          return;
        }
        if ((local_90 & 0xfd) == 0) {
          return;
        }
        if (local_90 != 4) {
          (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_90])(&uStack_51,local_a8);
          return;
        }
        if (-1 < cStack_91) {
          return;
        }
        FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
        return;
      }
    }
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4357708);
    (*pcVar4)();
  }
  if ((*(long *)(param_1 + 0x50) == 0) || (*(char *)(*(long *)(param_1 + 0x50) + 4) != '\0')) {
    uVar14 = 0;
  }
  else {
    uVar14 = *(undefined8 *)(param_1 + 0x58);
  }
  plVar7 = (long *)FUN_06477134(uVar14);
  uVar14 = (**(code **)(*plVar7 + 0x68))();
  if ((*(long *)(param_1 + 0x50) == 0) || (*(char *)(*(long *)(param_1 + 0x50) + 4) != '\0')) {
    uVar12 = 0;
  }
  else {
    uVar12 = *(undefined8 *)(param_1 + 0x58);
  }
  local_70 = 0;
  uStack_69._0_1_ = 0;
  uStack_69._1_3_ = 0;
  cStack_65 = '\0';
  uStack_64 = 0;
  local_60 = 0;
  uVar8 = FUN_0c1057c0(uVar14,uVar12,0,&local_70,1,param_2,param_3,plVar7[6] != 0,0);
  if ((uVar8 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar8);
  }
  if ((*(ulong *)(param_1 + 0x60) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_000270f0();
  }
  *(ulong *)(param_1 + 0x60) = uVar8;
  if (uVar8 == 0) {
    if ((*(long *)(param_1 + 0x50) == 0) || (*(char *)(*(long *)(param_1 + 0x50) + 4) != '\0')) {
      uVar14 = 0;
    }
    else {
      uVar14 = *(undefined8 *)(param_1 + 0x58);
    }
    FUN_0c12b550(uVar14);
    FUN_043586ac(param_1);
    goto LAB_04357518;
  }
  piVar16 = *(int **)(param_1 + 0x128);
  if ((piVar16 != (int *)0x0) && (iVar5 = *piVar16, *piVar16 = iVar5 + 1, iVar5 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0,0x4357718);
    (*pcVar4)();
  }
  uVar14 = *(undefined8 *)(param_1 + 0x130);
  puVar9 = (undefined4 *)FUN_00008790(0x40);
  *puVar9 = 1;
  *(undefined1 **)(puVar9 + 2) = &LAB_040c5ad0;
  *(code **)(puVar9 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar9 + 6) = &LAB_04169bc0;
  *(code **)(puVar9 + 8) = FUN_043586ac;
  *(undefined8 *)(puVar9 + 10) = 0;
  *(int **)(puVar9 + 0xc) = piVar16;
  *(undefined8 *)(puVar9 + 0xe) = uVar14;
  piVar16 = *(int **)(*(long *)(param_1 + 0x60) + 0x140);
  *(undefined4 **)(*(long *)(param_1 + 0x60) + 0x140) = puVar9;
  if (piVar16 != (int *)0x0) {
    LOAcquire();
    iVar5 = *piVar16;
    *piVar16 = iVar5 + -1;
    LORelease();
    if (iVar5 == 1) {
      if (*(code **)(piVar16 + 4) == FUN_039a66c0) {
        piVar11 = *(int **)(piVar16 + 0xc);
        if (piVar11 != (int *)0x0) {
          LOAcquire();
          iVar5 = *piVar11;
          *piVar11 = iVar5 + -1;
          LORelease();
          if (iVar5 == 1) {
            FUN_00005810(piVar11,8);
          }
        }
        FUN_00005810(piVar16,0x48);
      }
      else {
        (**(code **)(piVar16 + 4))(piVar16);
      }
    }
  }
  uVar14 = *(undefined8 *)(*(long *)(param_1 + 0x60) + 0xe0);
  local_70 = SUB87(plVar7,0);
  uStack_69._0_1_ = (undefined1)((ulong)plVar7 >> 0x38);
  if ((plVar7 != (long *)0x0) &&
     (uVar1 = *(uint *)(plVar7 + 1), *(uint *)(plVar7 + 1) = uVar1 + 1, 0xfffffffe < uVar1)) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0,0x4357724);
    (*pcVar4)();
  }
  FUN_0c101218(uVar14,&local_70);
  plVar15 = (long *)CONCAT17((undefined1)uStack_69,local_70);
  if (plVar15 != (long *)0x0) {
    lVar13 = plVar15[1];
    iVar5 = (int)lVar13 + -1;
    *(int *)(plVar15 + 1) = iVar5;
    if ((int)lVar13 == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0x4357730);
      (*pcVar4)();
    }
    if (iVar5 == 0) {
      (**(code **)(*plVar15 + 0xe8))();
    }
  }
  uVar8 = *(ulong *)(*(long *)(param_1 + 0x60) + 0xa0);
  if ((uVar8 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar8);
  }
  uVar2 = (undefined1)uStack_69;
  uVar3 = uStack_69._1_3_;
  if (uVar8 != 0) {
    uStack_ef = 0;
    uStack_f3 = 0;
    local_104 = 1;
    local_100 = 0;
    local_f8 = 0;
    local_f4 = 0;
    local_ec = 0;
    FUN_04346100(&local_70,uVar8,&local_104);
    puVar10 = (undefined8 *)FUN_00008790(0x10);
    puVar10[1] = CONCAT44(uStack_64,CONCAT13(cStack_65,uStack_69._1_3_));
    *puVar10 = CONCAT17((undefined1)uStack_69,local_70);
    local_70 = 0;
    uStack_69._0_1_ = 0;
    uVar2 = (undefined1)uStack_69;
    uStack_69._1_3_ = 0;
    uVar3 = uStack_69._1_3_;
    uStack_69._0_1_ = 0;
    uStack_69._1_3_ = 0;
    cStack_65 = '\0';
    uStack_64 = 0;
    lVar13 = *(long *)(param_1 + 0x68);
    *(undefined8 **)(param_1 + 0x68) = puVar10;
    if (lVar13 != 0) {
      plVar15 = *(long **)(lVar13 + 8);
      if (plVar15 != (long *)0x0) {
        LOAcquire();
        lVar6 = plVar15[1];
        plVar15[1] = lVar6 + -1;
        LORelease();
        if (lVar6 == 0) {
          uStack_69._0_1_ = uVar2;
          uStack_69._1_3_ = uVar3;
          (**(code **)(*plVar15 + 0x10))(plVar15);
          if (plVar15[2] != 0) {
            LOAcquire();
            lVar6 = plVar15[2];
            plVar15[2] = lVar6 + -1;
            LORelease();
            if (lVar6 != 0) goto LAB_043574b0;
          }
          pcVar4 = *(code **)(*plVar15 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(plVar15,0x20);
          }
          else {
            (*pcVar4)(plVar15);
          }
        }
      }
LAB_043574b0:
      FUN_00005810(lVar13,0x10);
      plVar15 = (long *)CONCAT44(uStack_64,CONCAT13(cStack_65,uStack_69._1_3_));
      uVar2 = (undefined1)uStack_69;
      uVar3 = uStack_69._1_3_;
      if (plVar15 != (long *)0x0) {
        LOAcquire();
        lVar13 = plVar15[1];
        plVar15[1] = lVar13 + -1;
        LORelease();
        if (lVar13 == 0) {
          (**(code **)(*plVar15 + 0x10))(plVar15);
          if (plVar15[2] != 0) {
            LOAcquire();
            lVar13 = plVar15[2];
            plVar15[2] = lVar13 + -1;
            LORelease();
            uVar2 = (undefined1)uStack_69;
            uVar3 = uStack_69._1_3_;
            if (lVar13 != 0) goto LAB_043574d4;
          }
          pcVar4 = *(code **)(*plVar15 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(plVar15,0x20);
            uVar2 = (undefined1)uStack_69;
            uVar3 = uStack_69._1_3_;
          }
          else {
            (*pcVar4)(plVar15);
            uVar2 = (undefined1)uStack_69;
            uVar3 = uStack_69._1_3_;
          }
        }
      }
    }
  }
LAB_043574d4:
  uStack_69._1_3_ = uVar3;
  uStack_69._0_1_ = uVar2;
  if ((uVar8 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(uVar8);
  }
LAB_04357518:
  if (plVar7 != (long *)0x0) {
    lVar13 = plVar7[1];
    iVar5 = (int)lVar13 + -1;
    *(int *)(plVar7 + 1) = iVar5;
    if ((int)lVar13 == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(0,0x435770c);
      (*pcVar4)();
    }
    if (iVar5 == 0) {
      (**(code **)(*plVar7 + 0xe8))(plVar7);
    }
  }
  return;
}


