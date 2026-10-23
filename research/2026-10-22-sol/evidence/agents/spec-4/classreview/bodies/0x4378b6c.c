// ==== 0x4378b6c  FUN_04378b6c  (696 bytes)

void FUN_04378b6c(long param_1,undefined8 *param_2)

{
  long *plVar1;
  long lVar2;
  undefined4 uVar3;
  long lVar4;
  int iVar5;
  ulong uVar6;
  long *plVar7;
  undefined4 *puVar8;
  undefined8 *puVar9;
  long lVar10;
  code *pcVar11;
  long *plVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  long local_70;
  long lStack_68;
  long local_60;
  long lStack_58;
  ulong local_50;
  long local_48;
  
  if (*(long *)(param_1 + 0x130) == 0) {
                    /* WARNING: Could not recover jumptable at 0x04378c34. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)param_2[2])(param_2,0);
    return;
  }
  lStack_58 = 0;
  local_60 = 0;
  local_48 = 0;
  local_50 = 0;
  lStack_68 = 0;
  local_70 = 0;
  FUN_043755a4(&local_70);
  lVar10 = lStack_68;
  if (local_60 == 0 && lStack_68 == 0) {
LAB_04378bd0:
    (*(code *)param_2[2])(param_2,0);
  }
  else {
    if (lStack_68 != 0) {
      uVar6 = FUN_001c5f40(lStack_68 + 0x1f8,0);
      if ((uVar6 & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar11 = (code *)SoftwareBreakpoint(0,0x4378e20);
        (*pcVar11)();
      }
      if (*(char *)(lVar10 + 0x1a0) != '\0') goto LAB_04378bd0;
    }
    plVar7 = (long *)FUN_00008790(0x48);
    lVar4 = local_48;
    lVar2 = lStack_68;
    lVar10 = local_70;
    plVar12 = plVar7 + 1;
    *plVar12 = 0;
    plVar7[2] = 0;
    *plVar7 = (long)&PTR_viewWillStartLiveResize_0da82600;
    plVar1 = plVar7 + 3;
    if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(1,0x4378e28);
      (*pcVar11)();
    }
    local_48 = 0;
    plVar7[8] = lVar4;
    local_70 = 0;
    lStack_68 = 0;
    plVar7[4] = lVar2;
    plVar7[3] = lVar10;
    plVar7[5] = local_60;
    plVar7[6] = lStack_58;
    local_60 = 0;
    lStack_58 = 0;
    uVar3 = (undefined4)local_50;
    local_50 = local_50 & 0xffffffff00000000;
    *(undefined4 *)(plVar7 + 7) = uVar3;
    puVar8 = (undefined4 *)FUN_00008790(0x58);
    *puVar8 = 1;
    *(undefined1 **)(puVar8 + 2) = &LAB_0439f40c;
    *(code **)(puVar8 + 4) = FUN_0439f540;
    *(code **)(puVar8 + 6) = FUN_00b77080;
    *(long **)(puVar8 + 10) = plVar1;
    *(long **)(puVar8 + 0xc) = plVar7;
    if (plVar7 != (long *)0x0) {
      *plVar12 = *plVar12 + 1;
    }
    uVar13 = *param_2;
    uVar15 = param_2[3];
    uVar14 = param_2[2];
    *(undefined8 *)(puVar8 + 0x10) = param_2[1];
    *(undefined8 *)(puVar8 + 0xe) = uVar13;
    lVar10 = param_2[3];
    *(undefined8 *)(puVar8 + 0x14) = uVar15;
    *(undefined8 *)(puVar8 + 0x12) = uVar14;
    if (*(long *)(lVar10 + 8) != 0) {
      param_2[2] = 0;
      param_2[3] = &DAT_0c4b3500;
    }
    if (lVar4 == 0) {
      puVar9 = (undefined8 *)FUN_00008790(8);
      *puVar9 = &PTR__cxx_destruct_0da88798;
      iVar5 = FUN_009180c0(plVar1,0,0,0,puVar9,"arc.mojom.RenderFrameAgent",&LAB_04429bcc,
                           &LAB_04051dc0);
      if (iVar5 != 0) {
        lVar10 = plVar7[4];
        puVar9 = (undefined8 *)FUN_00008790(0x10);
        *puVar9 = &PTR__cxx_destruct_0da88710;
        puVar9[1] = lVar10;
        lVar10 = plVar7[8];
        plVar7[8] = (long)puVar9;
        if (lVar10 != 0) {
          FUN_00005810(lVar10,0x10);
        }
      }
    }
    FUN_0442a5b8(plVar7[8],puVar8);
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar10 = *plVar12;
      *plVar12 = lVar10 + -1;
      LORelease();
      if (lVar10 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar10 = plVar7[2];
          plVar7[2] = lVar10 + -1;
          LORelease();
          if (lVar10 != 0) goto LAB_04378be0;
        }
        pcVar11 = *(code **)(*plVar7 + 0x20);
        if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar11)(plVar7);
        }
      }
    }
  }
LAB_04378be0:
  lVar10 = local_48;
  local_48 = 0;
  if (lVar10 != 0) {
    FUN_00005810(lVar10,0x10);
  }
  FUN_00111f30(&local_70);
  return;
}


