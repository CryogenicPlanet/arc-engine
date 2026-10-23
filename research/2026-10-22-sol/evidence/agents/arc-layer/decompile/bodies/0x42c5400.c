// ==== 0x42c5400  FUN_042c5400  (7036 bytes)

/* WARNING: Removing unreachable block (ram,0x042c66e8) */

undefined1  [16] FUN_042c5400(long *param_1,undefined4 param_2)

{
  long lVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  long lVar5;
  long *plVar6;
  long *plVar7;
  long *plVar8;
  int *piVar9;
  long *plVar10;
  undefined1 (*pauVar11) [16];
  ulong uVar12;
  long lVar13;
  bool bVar14;
  code *pcVar15;
  undefined8 *puVar16;
  undefined8 *puVar17;
  long *plVar18;
  long lVar19;
  long *plVar20;
  undefined8 *puVar21;
  undefined8 uVar22;
  undefined1 auVar23 [16];
  undefined1 auVar24 [16];
  undefined8 local_d0;
  undefined8 local_c8;
  ulong uStack_c0;
  undefined4 local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined7 local_88;
  char cStack_81;
  undefined8 *local_80;
  long lStack_78;
  ulong local_70;
  byte local_68;
  
  local_b0 = 0;
  uStack_a8 = 0;
  uStack_a0 = 0;
  (**(code **)(*param_1 + 0x30))(&local_80);
  FUN_00df9708(&local_98,&local_80);
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00060a60(&local_b0,local_98,uStack_90);
    if (cStack_81 < '\0') {
      FUN_00005810(local_98,CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
    }
  }
  else {
    uStack_a8 = uStack_90;
    local_b0 = local_98;
    uStack_a0 = CONCAT17(cStack_81,local_88);
  }
  auVar24._0_8_ = FUN_00008790(800);
  uStack_c0 = uStack_a0;
  local_c8 = uStack_a8;
  local_d0 = local_b0;
  local_b0 = 0;
  uStack_a8 = 0;
  uStack_a0 = 0;
  local_b8 = param_2;
  FUN_042c6fb8(auVar24._0_8_,&local_d0);
  plVar6 = (long *)FUN_00008790(0x20);
  plVar18 = plVar6 + 1;
  *plVar18 = 0;
  *plVar6 = (long)&PTR_viewWillStartLiveResize_0da7c830;
  plVar6[2] = 0;
  plVar6[3] = auVar24._0_8_;
  if ((auVar24._0_8_ != 0) &&
     ((plVar7 = *(long **)(auVar24._0_8_ + 0x10), plVar7 == (long *)0x0 || (plVar7[1] == -1)))) {
    if (plVar6 != (long *)0x0) {
      plVar6[1] = plVar6[1] + 1;
      plVar6[2] = plVar6[2] + 1;
    }
    *(ulong *)(auVar24._0_8_ + 8) = auVar24._0_8_;
    *(long **)(auVar24._0_8_ + 0x10) = plVar6;
    if (plVar7 != (long *)0x0) {
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar5 = plVar7[2];
        plVar7[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5558;
      }
      pcVar15 = *(code **)(*plVar7 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5558:
    if (plVar6 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar18;
      *plVar18 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar6 + 0x10))(plVar6);
        if (plVar6[2] != 0) {
          LOAcquire();
          lVar5 = plVar6[2];
          plVar6[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5568;
        }
        pcVar15 = *(code **)(*plVar6 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar6,0x20);
        }
        else {
          (*pcVar15)(plVar6);
        }
      }
    }
  }
LAB_042c5568:
  if ((long)uStack_c0 < 0) {
    FUN_00005810(local_d0,uStack_c0 & 0x7fffffffffffffff);
  }
  if (*(long *)(auVar24._0_8_ + 0xf0) != 0) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f0c);
    (*pcVar15)();
  }
  plVar7 = (long *)FUN_00008790(0xe0);
  plVar20 = plVar7 + 1;
  *plVar20 = 0;
  plVar7[2] = 0;
  *plVar7 = (long)&PTR_viewWillStartLiveResize_0da7c868;
  plVar18 = plVar7 + 3;
  if (plVar18 == (long *)0x0) goto LAB_042c6f10;
  FUN_042ded8c(plVar18);
  if ((plVar7 != (long *)&DAT_ffffffffffffffd8) &&
     ((plVar8 = (long *)plVar7[6], plVar8 == (long *)0x0 || (plVar8[1] == -1)))) {
    if (plVar7 != (long *)0x0) {
      plVar7[1] = plVar7[1] + 1;
      plVar7[2] = plVar7[2] + 1;
    }
    plVar7[5] = (long)plVar18;
    plVar7[6] = (long)plVar7;
    if (plVar8 != (long *)0x0) {
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5620;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5620:
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar20;
      *plVar20 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar5 = plVar7[2];
          plVar7[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5630;
        }
        pcVar15 = *(code **)(*plVar7 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar15)(plVar7);
        }
      }
    }
  }
LAB_042c5630:
  plVar20 = *(long **)(auVar24._0_8_ + 0xe8);
  *(long **)(auVar24._0_8_ + 0xe0) = plVar18;
  *(long **)(auVar24._0_8_ + 0xe8) = plVar7;
  if (plVar20 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar20[1];
    plVar20[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar20 + 0x10))(plVar20);
      if (plVar20[2] != 0) {
        LOAcquire();
        lVar5 = plVar20[2];
        plVar20[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c568c;
      }
      pcVar15 = *(code **)(*plVar20 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar20,0x20);
      }
      else {
        (*pcVar15)(plVar20);
      }
    }
  }
LAB_042c568c:
  plVar7 = (long *)FUN_00008790(0x50);
  plVar20 = plVar7 + 1;
  *plVar20 = 0;
  plVar7[2] = 0;
  *plVar7 = (long)&PTR_viewWillStartLiveResize_0da7c8a0;
  plVar18 = plVar7 + 3;
  if (plVar18 == (long *)0x0) goto LAB_042c6f10;
  plVar7[6] = 0;
  plVar7[7] = 0;
  plVar7[3] = (long)&PTR_FUN_0da8a590;
  piVar9 = (int *)FUN_00008790(8);
  *piVar9 = 0;
  *(undefined1 *)(piVar9 + 1) = 0;
  if ((piVar9 != (int *)0x0) && (iVar4 = *piVar9, *piVar9 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f68);
    (*pcVar15)();
  }
  plVar7[4] = (long)piVar9;
  plVar7[5] = (long)plVar18;
  plVar7[3] = (long)&PTR_FUN_0da7cae8;
  if (((ulong)plVar18 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(plVar18);
    plVar8 = (long *)plVar7[7];
    plVar7[8] = (long)plVar18;
    plVar7[9] = 0;
    if (plVar8 == (long *)0x0) goto LAB_042c5748;
    if (plVar8[1] == -1) {
      bVar14 = false;
      goto joined_r0x042c574c;
    }
  }
  else {
    plVar8 = (long *)0x0;
    plVar7[8] = (long)plVar18;
    plVar7[9] = 0;
LAB_042c5748:
    bVar14 = true;
joined_r0x042c574c:
    if (plVar7 != (long *)0x0) {
      plVar7[1] = plVar7[1] + 1;
      plVar7[2] = plVar7[2] + 1;
    }
    plVar7[6] = (long)plVar18;
    plVar7[7] = (long)plVar7;
    if (!bVar14) {
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5794;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5794:
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar20;
      *plVar20 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar5 = plVar7[2];
          plVar7[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c57e4;
        }
        pcVar15 = *(code **)(*plVar7 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar15)(plVar7);
        }
      }
    }
  }
LAB_042c57e4:
  plVar20 = *(long **)(auVar24._0_8_ + 0xf8);
  *(long **)(auVar24._0_8_ + 0xf0) = plVar18;
  *(long **)(auVar24._0_8_ + 0xf8) = plVar7;
  if (plVar20 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar20[1];
    plVar20[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar20 + 0x10))(plVar20);
      if (plVar20[2] != 0) {
        LOAcquire();
        lVar5 = plVar20[2];
        plVar20[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5840;
      }
      pcVar15 = *(code **)(*plVar20 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar20,0x20);
      }
      else {
        (*pcVar15)(plVar20);
      }
    }
  }
LAB_042c5840:
  plVar18 = *(long **)(auVar24._0_8_ + 0x10);
  if (plVar18 == (long *)0x0) goto LAB_042c66fc;
  uVar22 = *(undefined8 *)(auVar24._0_8_ + 8);
  lVar5 = plVar18[1];
  do {
    if (lVar5 == -1) goto LAB_042c66fc;
    lVar19 = lVar5 + 1;
    lVar13 = plVar18[1];
    bVar14 = lVar13 != lVar5;
    lVar5 = lVar13;
  } while (bVar14);
  plVar18[1] = lVar19;
  plVar20 = (long *)FUN_00008790(0x158);
  plVar8 = plVar20 + 1;
  *plVar8 = 0;
  plVar20[2] = 0;
  *plVar20 = (long)&PTR_viewWillStartLiveResize_0da7c8d8;
  plVar7 = plVar20 + 3;
  if (plVar7 == (long *)0x0) goto LAB_042c6f10;
  lVar5 = plVar18[2];
  plVar18[2] = lVar5 + 1;
  FUN_042e5b60(lVar5,plVar7,uVar22,plVar18);
  if ((plVar20 != (long *)0xffffffffffffffe0) &&
     ((plVar10 = (long *)plVar20[5], plVar10 == (long *)0x0 || (plVar10[1] == -1)))) {
    if (plVar20 != (long *)0x0) {
      plVar20[1] = plVar20[1] + 1;
      plVar20[2] = plVar20[2] + 1;
    }
    plVar20[4] = (long)plVar7;
    plVar20[5] = (long)plVar20;
    if (plVar10 != (long *)0x0) {
      if (plVar10[2] != 0) {
        LOAcquire();
        lVar5 = plVar10[2];
        plVar10[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5924;
      }
      pcVar15 = *(code **)(*plVar10 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar10,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5924:
    if (plVar20 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar8;
      *plVar8 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar20 + 0x10))(plVar20);
        if (plVar20[2] != 0) {
          LOAcquire();
          lVar5 = plVar20[2];
          plVar20[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5934;
        }
        pcVar15 = *(code **)(*plVar20 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar20,0x20);
        }
        else {
          (*pcVar15)(plVar20);
        }
      }
    }
  }
LAB_042c5934:
  plVar8 = *(long **)(auVar24._0_8_ + 0x108);
  *(long **)(auVar24._0_8_ + 0x100) = plVar7;
  *(long **)(auVar24._0_8_ + 0x108) = plVar20;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar8[1];
    plVar8[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5990;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)(plVar8);
      }
    }
  }
LAB_042c5990:
  LOAcquire();
  lVar5 = plVar18[1];
  plVar18[1] = lVar5 + -1;
  LORelease();
  if (lVar5 == 0) {
    (**(code **)(*plVar18 + 0x10))(plVar18);
    if (plVar18[2] != 0) {
      LOAcquire();
      lVar5 = plVar18[2];
      plVar18[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042c59e0;
    }
    pcVar15 = *(code **)(*plVar18 + 0x20);
    if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
      FUN_00005810(plVar18,0x20);
    }
    else {
      (*pcVar15)(plVar18);
    }
  }
LAB_042c59e0:
  plVar18 = *(long **)(auVar24._0_8_ + 0x10);
  if (plVar18 == (long *)0x0) goto LAB_042c66fc;
  uVar22 = *(undefined8 *)(auVar24._0_8_ + 8);
  lVar5 = plVar18[1];
  do {
    if (lVar5 == -1) goto LAB_042c66fc;
    lVar19 = lVar5 + 1;
    lVar13 = plVar18[1];
    bVar14 = lVar13 != lVar5;
    lVar5 = lVar13;
  } while (bVar14);
  plVar18[1] = lVar19;
  plVar20 = (long *)FUN_00008790(0xd0);
  plVar8 = plVar20 + 1;
  *plVar8 = 0;
  plVar20[2] = 0;
  *plVar20 = (long)&PTR_viewWillStartLiveResize_0da7c910;
  plVar7 = plVar20 + 3;
  if (plVar7 == (long *)0x0) goto LAB_042c6f10;
  lVar5 = plVar18[2];
  plVar18[2] = lVar5 + 1;
  FUN_042ed030(lVar5,plVar7,uVar22,plVar18);
  if ((plVar20 != (long *)0xffffffffffffffe0) &&
     ((plVar10 = (long *)plVar20[5], plVar10 == (long *)0x0 || (plVar10[1] == -1)))) {
    if (plVar20 != (long *)0x0) {
      plVar20[1] = plVar20[1] + 1;
      plVar20[2] = plVar20[2] + 1;
    }
    plVar20[4] = (long)plVar7;
    plVar20[5] = (long)plVar20;
    if (plVar10 != (long *)0x0) {
      if (plVar10[2] != 0) {
        LOAcquire();
        lVar5 = plVar10[2];
        plVar10[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5ac4;
      }
      pcVar15 = *(code **)(*plVar10 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar10,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5ac4:
    if (plVar20 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar8;
      *plVar8 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar20 + 0x10))(plVar20);
        if (plVar20[2] != 0) {
          LOAcquire();
          lVar5 = plVar20[2];
          plVar20[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5ad4;
        }
        pcVar15 = *(code **)(*plVar20 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar20,0x20);
        }
        else {
          (*pcVar15)(plVar20);
        }
      }
    }
  }
LAB_042c5ad4:
  plVar8 = *(long **)(auVar24._0_8_ + 0x118);
  *(long **)(auVar24._0_8_ + 0x110) = plVar7;
  *(long **)(auVar24._0_8_ + 0x118) = plVar20;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar8[1];
    plVar8[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5b30;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)(plVar8);
      }
    }
  }
LAB_042c5b30:
  LOAcquire();
  lVar5 = plVar18[1];
  plVar18[1] = lVar5 + -1;
  LORelease();
  if (lVar5 == 0) {
    (**(code **)(*plVar18 + 0x10))(plVar18);
    if (plVar18[2] != 0) {
      LOAcquire();
      lVar5 = plVar18[2];
      plVar18[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042c5b80;
    }
    pcVar15 = *(code **)(*plVar18 + 0x20);
    if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
      FUN_00005810(plVar18,0x20);
    }
    else {
      (*pcVar15)(plVar18);
    }
  }
LAB_042c5b80:
  plVar7 = (long *)FUN_00008790(0x48);
  plVar7[1] = 0;
  plVar7[2] = 0;
  *plVar7 = (long)&PTR_viewWillStartLiveResize_0da7c948;
  plVar18 = plVar7 + 3;
  if (plVar18 == (long *)0x0) goto LAB_042c6f10;
  plVar7[6] = 0;
  plVar7[7] = 0;
  plVar7[3] = (long)&PTR_FUN_0da8a590;
  piVar9 = (int *)FUN_00008790(8);
  *piVar9 = 0;
  *(undefined1 *)(piVar9 + 1) = 0;
  if ((piVar9 != (int *)0x0) && (iVar4 = *piVar9, *piVar9 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f74);
    (*pcVar15)();
  }
  plVar7[4] = (long)piVar9;
  plVar7[5] = (long)plVar18;
  plVar7[3] = (long)&PTR_FUN_0da7cd78;
  plVar7[8] = 0;
  if (plVar7 == (long *)0x0) {
    segment_command_00000020.segname[8] = '\x18';
    segment_command_00000020.segname[9] = '\0';
    segment_command_00000020.segname[10] = '\0';
    segment_command_00000020.segname[0xb] = '\0';
    segment_command_00000020.segname[0xc] = '\0';
    segment_command_00000020.segname[0xd] = '\0';
    segment_command_00000020.segname[0xe] = '\0';
    segment_command_00000020.segname[0xf] = '\0';
    segment_command_00000020.vmaddr = 0;
  }
  else {
    plVar20 = plVar7 + 1;
    *plVar20 = *plVar20 + 1;
    plVar7[2] = plVar7[2] + 1;
    plVar7[6] = (long)plVar18;
    plVar7[7] = (long)plVar7;
    LOAcquire();
    lVar5 = *plVar20;
    *plVar20 = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar5 = plVar7[2];
        plVar7[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5c58;
      }
      pcVar15 = *(code **)(*plVar7 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar15)(plVar7);
      }
    }
  }
LAB_042c5c58:
  plVar20 = *(long **)(auVar24._0_8_ + 0x128);
  *(long **)(auVar24._0_8_ + 0x120) = plVar18;
  *(long **)(auVar24._0_8_ + 0x128) = plVar7;
  if (plVar20 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar20[1];
    plVar20[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar20 + 0x10))(plVar20);
      if (plVar20[2] != 0) {
        LOAcquire();
        lVar5 = plVar20[2];
        plVar20[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5cb4;
      }
      pcVar15 = *(code **)(*plVar20 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar20,0x20);
      }
      else {
        (*pcVar15)(plVar20);
      }
    }
  }
LAB_042c5cb4:
  plVar18 = *(long **)(auVar24._0_8_ + 0x10);
  if (plVar18 == (long *)0x0) goto LAB_042c66fc;
  lVar13 = *(long *)(auVar24._0_8_ + 0x10);
  lVar19 = *(long *)(auVar24._0_8_ + 8);
  lVar5 = plVar18[1];
  do {
    if (lVar5 == -1) goto LAB_042c66fc;
    lVar1 = lVar5 + 1;
    lVar2 = plVar18[1];
    bVar14 = lVar2 != lVar5;
    lVar5 = lVar2;
  } while (bVar14);
  plVar18[1] = lVar1;
  plVar20 = (long *)FUN_00008790(0x48);
  plVar8 = plVar20 + 1;
  *plVar8 = 0;
  plVar20[2] = 0;
  *plVar20 = (long)&PTR_viewWillStartLiveResize_0da7c980;
  plVar7 = plVar20 + 3;
  if (plVar7 == (long *)0x0) goto LAB_042c6f10;
  plVar10 = plVar18 + 2;
  *plVar10 = *plVar10 + 1;
  *plVar7 = 0;
  plVar20[4] = 0;
  plVar20[6] = lVar13;
  plVar20[5] = lVar19;
  *plVar10 = *plVar10 + 1;
  plVar20[7] = 0;
  plVar20[8] = 0;
  if (plVar18[2] == 0) {
LAB_042c5d44:
    pcVar15 = *(code **)(*plVar18 + 0x20);
    if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
      FUN_00005810(plVar18,0x20);
    }
    else {
      (*pcVar15)(plVar18);
    }
  }
  else {
    LOAcquire();
    lVar5 = *plVar10;
    *plVar10 = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) goto LAB_042c5d44;
  }
  plVar10 = (long *)plVar20[4];
  if ((plVar10 == (long *)0x0) || (plVar10[1] == -1)) {
    if (plVar20 != (long *)0x0) {
      plVar20[1] = plVar20[1] + 1;
      plVar20[2] = plVar20[2] + 1;
    }
    plVar20[3] = (long)(plVar20 + 3);
    plVar20[4] = (long)plVar20;
    if (plVar10 != (long *)0x0) {
      if (plVar10[2] != 0) {
        LOAcquire();
        lVar5 = plVar10[2];
        plVar10[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5dc8;
      }
      pcVar15 = *(code **)(*plVar10 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar10,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5dc8:
    if (plVar20 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar8;
      *plVar8 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar20 + 0x10))(plVar20);
        if (plVar20[2] != 0) {
          LOAcquire();
          lVar5 = plVar20[2];
          plVar20[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5e18;
        }
        pcVar15 = *(code **)(*plVar20 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar20,0x20);
        }
        else {
          (*pcVar15)(plVar20);
        }
      }
    }
  }
LAB_042c5e18:
  plVar8 = *(long **)(auVar24._0_8_ + 0x138);
  *(long **)(auVar24._0_8_ + 0x130) = plVar7;
  *(long **)(auVar24._0_8_ + 0x138) = plVar20;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar8[1];
    plVar8[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5e74;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)(plVar8);
      }
    }
  }
LAB_042c5e74:
  LOAcquire();
  lVar5 = plVar18[1];
  plVar18[1] = lVar5 + -1;
  LORelease();
  if (lVar5 == 0) {
    (**(code **)(*plVar18 + 0x10))(plVar18);
    if (plVar18[2] != 0) {
      LOAcquire();
      lVar5 = plVar18[2];
      plVar18[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042c5ec4;
    }
    pcVar15 = *(code **)(*plVar18 + 0x20);
    if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
      FUN_00005810(plVar18,0x20);
    }
    else {
      (*pcVar15)(plVar18);
    }
  }
LAB_042c5ec4:
  plVar18 = *(long **)(auVar24._0_8_ + 0x10);
  if (plVar18 == (long *)0x0) {
LAB_042c66fc:
                    /* WARNING: Subroutine does not return */
    FUN_050d342c();
  }
  uVar22 = *(undefined8 *)(auVar24._0_8_ + 8);
  lVar5 = plVar18[1];
  do {
    if (lVar5 == -1) goto LAB_042c66fc;
    lVar19 = lVar5 + 1;
    lVar13 = plVar18[1];
    bVar14 = lVar13 != lVar5;
    lVar5 = lVar13;
  } while (bVar14);
  plVar18[1] = lVar19;
  plVar20 = (long *)FUN_00008790(0x180);
  plVar8 = plVar20 + 1;
  *plVar8 = 0;
  plVar20[2] = 0;
  *plVar20 = (long)&PTR_viewWillStartLiveResize_0da7c9b8;
  plVar7 = plVar20 + 3;
  if (plVar7 == (long *)0x0) {
LAB_042c6f10:
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(1,0x42c6f14);
    (*pcVar15)();
  }
  lVar5 = plVar18[2];
  plVar18[2] = lVar5 + 1;
  FUN_042f1b08(lVar5,plVar7,uVar22,plVar18);
  if ((plVar20 != (long *)0xffffffffffffffe0) &&
     ((plVar10 = (long *)plVar20[5], plVar10 == (long *)0x0 || (plVar10[1] == -1)))) {
    if (plVar20 != (long *)0x0) {
      plVar20[1] = plVar20[1] + 1;
      plVar20[2] = plVar20[2] + 1;
    }
    plVar20[4] = (long)plVar7;
    plVar20[5] = (long)plVar20;
    if (plVar10 != (long *)0x0) {
      if (plVar10[2] != 0) {
        LOAcquire();
        lVar5 = plVar10[2];
        plVar10[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c5fa8;
      }
      pcVar15 = *(code **)(*plVar10 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar10,0x20);
      }
      else {
        (*pcVar15)();
      }
    }
LAB_042c5fa8:
    if (plVar20 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar8;
      *plVar8 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar20 + 0x10))(plVar20);
        if (plVar20[2] != 0) {
          LOAcquire();
          lVar5 = plVar20[2];
          plVar20[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042c5fb8;
        }
        pcVar15 = *(code **)(*plVar20 + 0x20);
        if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
          FUN_00005810(plVar20,0x20);
        }
        else {
          (*pcVar15)(plVar20);
        }
      }
    }
  }
LAB_042c5fb8:
  plVar8 = *(long **)(auVar24._0_8_ + 0x148);
  *(long **)(auVar24._0_8_ + 0x140) = plVar7;
  *(long **)(auVar24._0_8_ + 0x148) = plVar20;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar8[1];
    plVar8[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042c6014;
      }
      pcVar15 = *(code **)(*plVar8 + 0x20);
      if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar15)(plVar8);
      }
    }
  }
LAB_042c6014:
  LOAcquire();
  lVar5 = plVar18[1];
  plVar18[1] = lVar5 + -1;
  LORelease();
  if (lVar5 == 0) {
    (**(code **)(*plVar18 + 0x10))(plVar18);
    if (plVar18[2] != 0) {
      LOAcquire();
      lVar5 = plVar18[2];
      plVar18[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042c6064;
    }
    pcVar15 = *(code **)(*plVar18 + 0x20);
    if (((pcVar15 == FUN_00f01bc0) || (pcVar15 == FUN_00f01bc0)) || (pcVar15 == FUN_013efb40)) {
      FUN_00005810(plVar18,0x20);
    }
    else {
      (*pcVar15)(plVar18);
    }
  }
LAB_042c6064:
  if (param_1 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f80);
    (*pcVar15)();
  }
  if (*(long *)(auVar24._0_8_ + 0x88) != 0) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f8c);
    (*pcVar15)();
  }
  if (*(int *)(auVar24._0_8_ + 0xa8) == 0) {
    pauVar11 = (undefined1 (*) [16])FUN_00008790(0x18);
    auVar23 = FUN_032be594(param_1);
    *pauVar11 = auVar23;
    *(undefined4 *)pauVar11[1] = 0x2f;
    FUN_02b99544(param_1,0x2f);
    lVar5 = *(long *)(auVar24._0_8_ + 200);
    *(undefined1 (**) [16])(auVar24._0_8_ + 200) = pauVar11;
    if (lVar5 != 0) {
      uVar22 = FUN_02846350();
      FUN_00005810(uVar22,0x18);
    }
  }
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  if ((*(ulong *)(auVar24._0_8_ + 0x88) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  uVar12 = DAT_0e2f2208;
  *(long **)(auVar24._0_8_ + 0x88) = param_1;
  if (((ulong)param_1 & 0xfffffffc00000000) == uVar12) {
    FUN_0000ffa8(param_1);
  }
  if ((*(ulong *)(auVar24._0_8_ + 0x1c8) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(long **)(auVar24._0_8_ + 0x1c8) = param_1;
  FUN_00575e00(param_1 + 0x11,*(undefined8 *)(auVar24._0_8_ + 0x1c0));
  FUN_01a29860(DAT_0e481618);
  uVar12 = FUN_01a298f8();
  if ((uVar12 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar12);
  }
  if ((*(ulong *)(auVar24._0_8_ + 0x1b8) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(ulong *)(auVar24._0_8_ + 0x1b8) = uVar12;
  FUN_01efc9a0(uVar12 + 0x18,*(undefined8 *)(auVar24._0_8_ + 0x1b0));
  (**(code **)(**(long **)(auVar24._0_8_ + 0x88) + 0x30))(&local_98);
  FUN_042c8aa0(&local_80,auVar24._0_8_,&local_98);
  if (*(char *)(auVar24._0_8_ + 199) < '\0') {
    FUN_00005810(*(undefined8 *)(auVar24._0_8_ + 0xb0),
                 *(ulong *)(auVar24._0_8_ + 0xc0) & 0x7fffffffffffffff);
  }
  *(long *)(auVar24._0_8_ + 0xb8) = lStack_78;
  *(undefined8 **)(auVar24._0_8_ + 0xb0) = local_80;
  *(ulong *)(auVar24._0_8_ + 0xc0) = local_70;
  local_70 = local_70 & 0xffffffffffffff;
  local_80 = (undefined8 *)((ulong)local_80 & 0xffffffffffffff00);
  if (cStack_81 < '\0') {
    FUN_00005810(local_98,CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  lVar5 = FUN_0119426c(*(undefined8 *)(auVar24._0_8_ + 0x88));
  if (*(long **)(lVar5 + 0x278) == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6f98);
    (*pcVar15)();
  }
  lVar5 = **(long **)(lVar5 + 0x278);
  if ((lVar5 != 0) &&
     (uVar3 = *(uint *)(lVar5 + 8), *(uint *)(lVar5 + 8) = uVar3 + 1, 0xfffffffe < uVar3)) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6fa4);
    (*pcVar15)();
  }
  uVar22 = FUN_00008790(0x48);
  FUN_0434f080(uVar22,lVar5,&DAT_0c9d6714);
  lVar5 = *(long *)(auVar24._0_8_ + 0x288);
  *(undefined8 *)(auVar24._0_8_ + 0x288) = uVar22;
  if (lVar5 != 0) {
    uVar22 = FUN_0434f19c();
    FUN_00005810(uVar22,0x48);
  }
  uVar12 = FUN_003bc2f4(*(undefined8 *)(DAT_0e481618 + 0x38),"arc.widevine_component_installed",0x20
                       );
  if (((uVar12 & 1) != 0) || (lVar5 = FUN_025d8a28(DAT_0e481618), lVar5 == 0)) goto LAB_042c62d8;
  local_80 = (undefined8 *)0x0;
  lStack_78 = 0;
  local_70 = 0;
  lVar19 = *(long *)(lVar5 + 0x50) * 0xc0;
  if (lVar19 == 0) {
    puVar21 = (undefined8 *)0x0;
    lVar5 = 0;
LAB_042c62c4:
    bVar14 = true;
  }
  else {
    lVar5 = *(long *)(lVar5 + 0x48);
    do {
      FUN_00ad9ee8(&local_80,lVar5);
      puVar17 = local_80;
      lVar5 = lVar5 + 0xc0;
      lVar19 = lVar19 + -0xc0;
    } while (lVar19 != 0);
    lVar19 = lStack_78 * 0x18;
    lVar5 = local_70 * 0x18;
    puVar21 = local_80 + lStack_78 * 3;
    puVar16 = local_80;
    if (lVar19 == 0) goto LAB_042c62c4;
    do {
      if (((*(char *)((long)puVar16 + 0x17) < '\0') && (puVar16[1] == 0x20)) &&
         (plVar18 = (long *)*puVar16,
         ((*plVar18 == 0x6365706d6f6d696f && plVar18[1] == 0x6a65646a616e6761) &&
         plVar18[2] == 0x626f6a696a6e6e67) && plVar18[3] == 0x6b65676965616265)) {
        local_80 = (undefined8 *)CONCAT71(local_80._1_7_,1);
        local_68 = 1;
        FUN_0167ca68(*(undefined8 *)(DAT_0e481618 + 0x38),"arc.widevine_component_installed",0x20,
                     &local_80);
        if ((local_68 != 0xff) && ((local_68 & 0xfd) != 0)) {
          if (local_68 == 4) {
            if ((long)local_70 < 0) {
              FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
            }
          }
          else {
            (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_68])(&local_98,&local_80);
          }
        }
        if (puVar17 != (undefined8 *)0x0) goto LAB_042c66bc;
        goto LAB_042c62d8;
      }
      lVar19 = lVar19 + -0x18;
      puVar16 = puVar16 + 3;
    } while (lVar19 != 0);
    bVar14 = false;
  }
  puVar17 = local_80;
  FUN_025d8a28(DAT_0e481618);
  FUN_08129c80();
  if (puVar17 != (undefined8 *)0x0) {
    if (!bVar14) {
LAB_042c66bc:
      do {
        puVar21 = puVar21 + -3;
        if (puVar21 == (undefined8 *)0x0) goto LAB_042c6f10;
      } while (puVar21 != puVar17);
    }
    FUN_00005810(puVar17,lVar5);
  }
LAB_042c62d8:
  lVar5 = FUN_0031e1c0();
  lVar19 = *(long *)(auVar24._0_8_ + 0x88);
  if (lVar19 != 0) {
    uVar22 = FUN_03b3b01c();
    lVar13 = FUN_0009278c(uVar22,lVar19,1);
    lVar19 = 0;
    if (lVar13 != 0) {
      lVar19 = lVar13 + 8;
    }
    if (*(long *)(lVar19 + 0x20) != 0) {
      FUN_045a1f80(*(long *)(lVar19 + 0x20),*(int *)(lVar5 + 100) == 2);
    }
  }
  uVar22 = (**(code **)(**(long **)(auVar24._0_8_ + 0x88) + 0x168))();
  uVar12 = FUN_003bc2f4(uVar22,"arc.dnt_disabled_migration_done",0x1f);
  if ((uVar12 & 1) == 0) {
    uVar22 = (**(code **)(**(long **)(auVar24._0_8_ + 0x88) + 0x168))();
    local_80 = (undefined8 *)((ulong)local_80 & 0xffffffffffffff00);
    local_68 = 1;
    FUN_0167ca68(uVar22,"enable_do_not_track",0x13,&local_80);
    if ((local_68 != 0xff) && ((local_68 & 0xfd) != 0)) {
      if (local_68 == 4) {
        if ((long)local_70 < 0) {
          FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
        }
      }
      else {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_68])(&local_98,&local_80);
      }
    }
    uVar22 = (**(code **)(**(long **)(auVar24._0_8_ + 0x88) + 0x168))();
    local_80 = (undefined8 *)CONCAT71(local_80._1_7_,1);
    local_68 = 1;
    FUN_0167ca68(uVar22,"arc.dnt_disabled_migration_done",0x1f,&local_80);
    if ((local_68 != 0xff) && ((local_68 & 0xfd) != 0)) {
      if (local_68 == 4) {
        if ((long)local_70 < 0) {
          FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
        }
      }
      else {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_68])(&local_98,&local_80);
      }
    }
  }
  lVar5 = FUN_042c3c9c(*(undefined8 *)(auVar24._0_8_ + 0x88));
  if ((auVar24._0_8_ & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(auVar24._0_8_);
  }
  if ((*(ulong *)(lVar5 + 0x10) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(ulong *)(lVar5 + 0x10) = auVar24._0_8_;
  FUN_042df0b8(*(undefined8 *)(auVar24._0_8_ + 0xe0),auVar24._0_8_ + 0x70);
  FUN_042e4000(*(undefined8 *)(auVar24._0_8_ + 0xf0),*(undefined8 *)(auVar24._0_8_ + 0x88));
  FUN_042e8260(*(undefined8 *)(auVar24._0_8_ + 0x100),*(undefined8 *)(auVar24._0_8_ + 0x88));
  FUN_042edd20(*(undefined8 *)(auVar24._0_8_ + 0x110),*(undefined8 *)(auVar24._0_8_ + 0x88));
  FUN_042efe0c(*(undefined8 *)(auVar24._0_8_ + 0x120),*(undefined8 *)(auVar24._0_8_ + 0x88));
  FUN_042f0178(*(undefined8 *)(auVar24._0_8_ + 0x130),*(undefined8 *)(auVar24._0_8_ + 0x88));
  FUN_042f3794(*(undefined8 *)(auVar24._0_8_ + 0x140),*(undefined8 *)(auVar24._0_8_ + 0x88));
  lVar5 = FUN_00881874(*(undefined8 *)(auVar24._0_8_ + 0x88));
  if (lVar5 == 0) {
                    /* WARNING: Does not return */
    pcVar15 = (code *)SoftwareBreakpoint(0,0x42c6fb0);
    (*pcVar15)();
  }
  FUN_00575e00(lVar5 + 0x70,auVar24._0_8_ + 0x20);
  uVar22 = FUN_04296d5c();
  FUN_04297504(uVar22,*(undefined8 *)(auVar24._0_8_ + 0x88));
  lVar19 = *(long *)(*(long *)(auVar24._0_8_ + 0x88) + 0x18);
  lVar5 = *(long *)(lVar19 + 0x50);
  if (lVar5 == 0) {
    lVar5 = FUN_00008790(0x90);
    FUN_0416ac00();
    lVar13 = *(long *)(lVar19 + 0x50);
    *(long *)(lVar19 + 0x50) = lVar5;
    if (lVar13 != 0) {
      uVar22 = FUN_0416ac88();
      FUN_00005810(uVar22,0x90);
      lVar5 = *(long *)(lVar19 + 0x50);
    }
  }
  if ((auVar24._0_8_ + 0x78 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(auVar24._0_8_ + 0x78);
  }
  if ((*(ulong *)(lVar5 + 0x78) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(ulong *)(lVar5 + 0x78) = auVar24._0_8_ + 0x78;
  lVar19 = *(long *)(*(long *)(auVar24._0_8_ + 0x88) + 0x18);
  lVar5 = *(long *)(lVar19 + 0x58);
  if (lVar5 == 0) {
    lVar5 = FUN_00008790(0x90);
    FUN_04171cc8();
    lVar13 = *(long *)(lVar19 + 0x58);
    *(long *)(lVar19 + 0x58) = lVar5;
    if (lVar13 != 0) {
      uVar22 = FUN_04171d50();
      FUN_00005810(uVar22,0x90);
      lVar5 = *(long *)(lVar19 + 0x58);
    }
  }
  if ((auVar24._0_8_ + 0x80 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(auVar24._0_8_ + 0x80);
  }
  if ((*(ulong *)(lVar5 + 0x78) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(ulong *)(lVar5 + 0x78) = auVar24._0_8_ + 0x80;
  if ((long)uStack_a0 < 0) {
    FUN_00005810(local_b0,uStack_a0 & 0x7fffffffffffffff);
  }
  auVar24._8_8_ = plVar6;
  return auVar24;
}


