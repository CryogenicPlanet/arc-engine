// ==== 0x384784c  FUN_0384784c  (8416 bytes)

undefined8 * FUN_0384784c(undefined8 *param_1,long *param_2)

{
  ulong uVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  ulong uVar7;
  long lVar8;
  undefined8 *puVar9;
  int *piVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  long *plVar15;
  undefined8 *puVar16;
  long lVar17;
  undefined8 extraout_x1;
  long *plVar18;
  long *plVar19;
  code *pcVar20;
  ulong *puVar21;
  undefined8 *puVar22;
  ulong uVar23;
  undefined1 auVar24 [16];
  int *local_198 [2];
  undefined1 auStack_188 [296];
  
  param_1[1] = &PTR_LAB_0da0d730;
  puVar16 = param_1 + 2;
  *puVar16 = &PTR_LAB_0da0d758;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[6] = &PTR_FUN_0da0d7f8;
  param_1[3] = param_1 + 4;
  *param_1 = &PTR_LAB_0da0d680;
  if (((ulong)param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  param_1[7] = param_2;
  uVar7 = FUN_00014cc0(&PTR_s_UseActionsForBrowserCommands_0d26d553_1_0e2ea2c0);
  if (((uVar7 & 1) == 0) &&
     (iVar3 = FUN_00014cc0(&PTR_s_AppMenuGlowUp_0d26d632_1_0e2ea360), iVar3 == 0)) {
    puVar9 = (undefined8 *)FUN_00008790(0x38);
    puVar22 = (undefined8 *)0x0;
    if (param_1 != (undefined8 *)0x0) {
      puVar22 = param_1 + 1;
    }
    *puVar9 = &PTR_LAB_0d965e78;
    if (((ulong)puVar22 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(puVar22);
    }
    puVar9[1] = puVar22;
    puVar9[3] = 0;
    puVar9[2] = 0;
    puVar9[5] = 0;
    puVar9[4] = 0;
    *(undefined4 *)(puVar9 + 6) = 0x3f800000;
  }
  else {
    lVar8 = (**(code **)(*(long *)param_1[7] + 0x18))();
    for (plVar18 = *(long **)(lVar8 + 8); plVar18 != (long *)0x0; plVar18 = (long *)*plVar18) {
      while ((undefined **)plVar18[4] < (undefined **)0xdf65271) {
        if ((undefined **)plVar18[4] == &PTR_s_BrowserActions_kDataKeyImpl_0df65270) {
          if (plVar18 == (long *)(lVar8 + 8)) goto LAB_0384792c;
          puVar21 = (ulong *)plVar18[5];
          if (((ulong)puVar21 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_0000ffa8(puVar21);
          }
          goto LAB_03847958;
        }
        plVar18 = (long *)plVar18[1];
        if (plVar18 == (long *)0x0) goto LAB_0384792c;
      }
    }
LAB_0384792c:
    puVar21 = (ulong *)0x0;
LAB_03847958:
    if (((ulong)puVar21 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(puVar21);
    }
    uVar7 = *puVar21;
    puVar9 = (undefined8 *)FUN_00008790(0x28);
    *puVar9 = &PTR_LAB_0d9a6600;
    puVar9[1] = 0;
    puVar9[2] = 0;
    puVar9[3] = 0;
    if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar7);
    }
    puVar9[4] = uVar7;
  }
  puVar22 = param_1 + 9;
  *puVar22 = &PTR_FUN_0da8a590;
  param_1[8] = puVar9;
  piVar10 = (int *)FUN_00008790(8);
  *piVar10 = 0;
  *(undefined1 *)(piVar10 + 1) = 0;
  if ((piVar10 != (int *)0x0) && (iVar3 = *piVar10, *piVar10 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar20 = (code *)SoftwareBreakpoint(0,0x38497a0);
    (*pcVar20)();
  }
  puVar9 = param_1 + 0xf;
  *puVar9 = &PTR_FUN_0da8a590;
  param_1[9] = &PTR_FUN_0dabb640;
  param_1[10] = piVar10;
  param_1[0xb] = puVar22;
  param_1[0xc] = 0;
  param_1[0xe] = 0;
  piVar10 = (int *)FUN_00008790(8);
  *piVar10 = 0;
  *(undefined1 *)(piVar10 + 1) = 0;
  if ((piVar10 != (int *)0x0) && (iVar3 = *piVar10, *piVar10 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar20 = (code *)SoftwareBreakpoint(0,0x38497ac);
    (*pcVar20)();
  }
  param_1[0x14] = 0;
  param_1[0xf] = &PTR_FUN_0dabb640;
  param_1[0x10] = piVar10;
  param_1[0x11] = puVar9;
  param_1[0x12] = 0;
  param_1[0x18] = 0;
  param_1[0x15] = 0;
  *(undefined4 *)(param_1 + 0x16) = 0;
  param_1[0x17] = 0;
  param_1[0x19] = 0;
  uVar7 = (**(code **)(*param_2 + 0x18))(param_2);
  param_1[0x1a] = &PTR_LAB_0da0d898;
  if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar7);
  }
  param_1[0x1b] = uVar7;
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
    uVar7 = param_1[0x1b];
  }
  param_1[0x1c] = param_1;
  if (uVar7 == 0) {
                    /* WARNING: Does not return */
    pcVar20 = (code *)SoftwareBreakpoint(0,0x3849788);
    (*pcVar20)();
  }
  if (param_1 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
    pcVar20 = (code *)SoftwareBreakpoint(0,0x3849794);
    (*pcVar20)();
  }
  FUN_01cb4948(uVar7,&PTR_s_chrome_BrowserCommandController_0df545e0,param_1);
  piVar10 = (int *)FUN_00008790(8);
  *piVar10 = 0;
  *(undefined1 *)(piVar10 + 1) = 0;
  if ((piVar10 != (int *)0x0) && (iVar3 = *piVar10, *piVar10 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar20 = (code *)SoftwareBreakpoint(0,0x38497b8);
    (*pcVar20)();
  }
  param_1[0x1d] = piVar10;
  param_1[0x1e] = param_1;
  lVar8 = (**(code **)(*(long *)param_1[7] + 0x78))();
  FUN_0210cd90(lVar8 + 0x20,puVar16);
  FUN_0210cc84(puVar16,lVar8);
  uVar7 = *(ulong *)(DAT_0e481618 + 0x38);
  if (uVar7 != 0) {
    if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar7);
    }
    if ((param_1[0x14] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_000270f0();
    }
    uVar23 = DAT_0e2f2208;
    param_1[0x14] = uVar7;
    if (((ulong)param_1 & 0xfffffffc00000000) == uVar23) {
      FUN_0000ffa8(param_1);
    }
    puVar11 = (undefined4 *)FUN_00008790(0x38);
    *puVar11 = 1;
    *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
    *(code **)(puVar11 + 4) = FUN_01bde140;
    *(code **)(puVar11 + 6) = FUN_00b77080;
    *(code **)(puVar11 + 8) = FUN_07d53e90;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(undefined8 **)(puVar11 + 0xc) = param_1;
    if (puVar11 == (undefined4 *)0x0) {
      puVar12 = (undefined4 *)0x0;
    }
    else {
      puVar12 = (undefined4 *)FUN_00008790(0x30);
      *puVar12 = 1;
      *(code **)(puVar12 + 2) = FUN_00e049e0;
      *(code **)(puVar12 + 4) = FUN_039a5440;
      *(code **)(puVar12 + 6) = FUN_00b77080;
      *(undefined4 **)(puVar12 + 10) = puVar11;
    }
    FUN_017a70c8(puVar9,"select_file_dialogs.allowed",0x1b,puVar12);
  }
  uVar23 = (ulong)param_1 & 0xfffffffc00000000;
  plVar18 = (long *)(**(code **)(*(long *)param_1[7] + 0x38))();
  uVar7 = (**(code **)(*plVar18 + 0x168))();
  if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar7);
  }
  if ((param_1[0xe] & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_000270f0();
  }
  uVar1 = DAT_0e2f2208;
  param_1[0xe] = uVar7;
  if (uVar23 == uVar1) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = FUN_02c6ef58;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"bookmarks.editing_enabled",0x19,puVar12);
  if (uVar23 == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = ArcWebContentsConfiguration::_cxx_destruct;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"bookmark_bar.show_on_all_tabs",0x1d,puVar12);
  if (uVar23 == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = ArcWebContentsConfiguration::_cxx_destruct;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"bookmark_bar.visibility_state",0x1d,puVar12);
  if (uVar23 == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = FUN_0421eda4;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"incognito.mode_availability",0x1b,puVar12);
  if (uVar23 == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = FUN_008813f8;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"printing.enabled",0x10,puVar12);
  if (uVar23 == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar11 = (undefined4 *)FUN_00008790(0x38);
  *puVar11 = 1;
  *(undefined1 **)(puVar11 + 2) = &LAB_00e04a80;
  *(code **)(puVar11 + 4) = FUN_01bde140;
  *(code **)(puVar11 + 6) = FUN_00b77080;
  *(code **)(puVar11 + 8) = FUN_07d53f00;
  *(undefined8 *)(puVar11 + 10) = 0;
  *(undefined8 **)(puVar11 + 0xc) = param_1;
  if (puVar11 == (undefined4 *)0x0) {
    puVar12 = (undefined4 *)0x0;
  }
  else {
    puVar12 = (undefined4 *)FUN_00008790(0x30);
    *puVar12 = 1;
    *(code **)(puVar12 + 2) = FUN_00e049e0;
    *(code **)(puVar12 + 4) = FUN_039a5440;
    *(code **)(puVar12 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar12 + 10) = puVar11;
  }
  FUN_017a70c8(puVar22,"download_restrictions",0x15,puVar12);
  uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
  uVar14 = FUN_02879cfc();
  lVar8 = FUN_0009278c(uVar14,uVar13,1);
  if (lVar8 != 0) {
    if (uVar23 == DAT_0e2f2208) {
      FUN_0000ffa8(param_1);
    }
    puVar11 = (undefined4 *)FUN_00008790(0x38);
    *puVar11 = 1;
    *(code **)(puVar11 + 2) = FUN_04074d00;
    *(code **)(puVar11 + 4) = FUN_01bde140;
    *(code **)(puVar11 + 6) = FUN_00b77080;
    *(undefined **)(puVar11 + 8) = &DAT_07d53f34;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(undefined8 **)(puVar11 + 0xc) = param_1;
    FUN_07354db0(local_198,lVar8 + 0x28,puVar11);
    FUN_01469964(param_1 + 0x18,local_198);
    piVar10 = local_198[0];
    if (local_198[0] != (int *)0x0) {
      local_198[0] = (int *)0x0;
      (**(code **)(piVar10 + 2))(piVar10);
      LOAcquire();
      iVar3 = *piVar10;
      *piVar10 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(piVar10 + 4))(piVar10);
      }
      if (local_198[0] != (int *)0x0) {
        LOAcquire();
        iVar3 = *local_198[0];
        *local_198[0] = iVar3 + -1;
        LORelease();
        if (iVar3 == 1) {
          (**(code **)(local_198[0] + 4))();
        }
      }
    }
  }
  plVar18 = (long *)param_1[7];
  bVar2 = false;
  if (plVar18 != (long *)0x0) {
    iVar3 = (**(code **)(*plVar18 + 0x60))(plVar18);
    if (iVar3 == 3) {
      bVar2 = false;
    }
    else {
      iVar3 = (**(code **)(*plVar18 + 0x60))(plVar18);
      bVar2 = iVar3 != 5;
    }
  }
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x80ea,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x80ef,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x80f1,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8534,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8535,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8536,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8537,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8538,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8539,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x853b,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x853c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d43,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5a,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d68,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d6b,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdfde,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xd305,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xd306,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84dc,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84de,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84df,1);
  plVar18 = (long *)param_1[8];
  iVar3 = (**(code **)(*(long *)param_1[7] + 0x60))();
  (**(code **)(*plVar18 + 0x38))(plVar18,0x84eb,iVar3 != 5);
  uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
  uVar14 = FUN_02302728();
  lVar8 = FUN_0009278c(uVar14,uVar13,1);
  if (lVar8 == 0) {
    bVar2 = false;
  }
  else if ((*(long *)(lVar8 + 0x10) == 0) || ((*(byte *)(*(long *)(lVar8 + 0x10) + 0x30) & 5) == 0))
  {
    bVar2 = *(long *)(lVar8 + 0x48) != 0;
  }
  else {
    bVar2 = true;
  }
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84ec,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84ef,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8501,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850a,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850b,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850d,1);
  plVar18 = (long *)param_1[8];
  (**(code **)(*(long *)param_1[7] + 0x38))();
  (**(code **)(*plVar18 + 0x38))(plVar18,0x8507,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88c2,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdffe,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9471,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9472,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9473,1);
  iVar3 = (**(code **)(*(long *)param_1[7] + 0x60))();
  (**(code **)(*(long *)param_1[7] + 0x38))();
  iVar4 = FUN_005c44e8();
  plVar18 = (long *)param_1[7];
  plVar19 = (long *)param_1[8];
  iVar5 = (**(code **)(*plVar18 + 0x60))(plVar18);
  if (((iVar5 == 3) || (iVar5 = (**(code **)(*plVar18 + 0x60))(plVar18), iVar5 == 2)) ||
     (iVar5 = (**(code **)(*plVar18 + 0x60))(plVar18), iVar5 == 4)) {
    uVar13 = 0;
  }
  else if (*(long *)(DAT_0e481618 + 0x38) == 0) {
    uVar13 = 1;
  }
  else {
    uVar13 = FUN_003bc2f4(*(long *)(DAT_0e481618 + 0x38),"select_file_dialogs.allowed",0x1b);
  }
  (**(code **)(*plVar19 + 0x38))(plVar19,40000,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c44,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c45,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c47,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c57,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d2d,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88ba,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d3d,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c46,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5d,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5e,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5f,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d60,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdff8,1);
  plVar18 = (long *)param_1[8];
  if (iVar4 == 2) {
    uVar6 = 0;
  }
  else {
    (**(code **)(*(long *)param_1[7] + 0x38))();
    uVar6 = FUN_0136b8c4();
    uVar6 = uVar6 ^ 1;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9c4a,uVar6);
  plVar18 = (long *)param_1[8];
  if (iVar4 == 2) {
    uVar6 = 0;
  }
  else {
    (**(code **)(*(long *)param_1[7] + 0x38))();
    uVar6 = FUN_0136b8c4();
    uVar6 = uVar6 ^ 1;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,&DAT_00009c59,uVar6);
  plVar18 = (long *)param_1[8];
  (**(code **)(*(long *)param_1[7] + 0x38))();
  iVar5 = FUN_0147d54c();
  if (iVar5 == 0) {
    uVar13 = 0;
  }
  else {
    uVar13 = FUN_00014cc0(&PTR_s_TabsFromOtherDevicesSidePanel_0d26d9e4_1_0e2ea550);
  }
  bVar2 = iVar4 != 2;
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9d70,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c4c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d69,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdffc,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdffb,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d51,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d6f,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5b,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d5c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88c8,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88cd,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdffd,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88e1,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d58,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88e2,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a19,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a20,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1a,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1b,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1f,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88e9,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88e7,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88e8,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdff2,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],&DAT_00009c53,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9c54,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d3c,1);
  plVar18 = (long *)param_1[8];
  uVar13 = FUN_00014cc0(&PTR_s_EnterpriseReleaseNotes_0d26d951_1_0e2ea500);
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9d71,uVar13);
  uVar6 = 0;
  plVar18 = (long *)param_1[8];
  if (iVar4 != 2) {
    (**(code **)(*(long *)param_1[7] + 0x38))((long *)param_1[7],extraout_x1,0);
    uVar6 = FUN_0136b8c4();
    uVar6 = uVar6 ^ 1;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0xdff5,uVar6);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdff4,1);
  plVar18 = (long *)param_1[8];
  if (iVar4 == 2) {
LAB_03848920:
    bVar2 = false;
  }
  else {
    (**(code **)(*(long *)param_1[7] + 0x38))();
    uVar7 = FUN_0136b8c4();
    if ((uVar7 & 1) != 0) goto LAB_03848920;
    uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
    iVar5 = FUN_005c44e8();
    if (iVar5 == 1) goto LAB_03848920;
    iVar5 = FUN_005c44e8(uVar13);
    bVar2 = iVar5 != 6;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0xdffa,bVar2);
  plVar18 = (long *)param_1[8];
  if (iVar4 == 2) {
LAB_03848990:
    bVar2 = false;
  }
  else {
    (**(code **)(*(long *)param_1[7] + 0x38))();
    uVar7 = FUN_0136b8c4();
    if ((uVar7 & 1) != 0) goto LAB_03848990;
    uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
    iVar5 = FUN_005c44e8();
    if (iVar5 == 1) goto LAB_03848990;
    iVar5 = FUN_005c44e8(uVar13);
    bVar2 = iVar5 != 6;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9d55,bVar2);
  plVar18 = (long *)param_1[8];
  if (iVar4 == 2) {
LAB_03848a00:
    bVar2 = false;
  }
  else {
    (**(code **)(*(long *)param_1[7] + 0x38))();
    uVar7 = FUN_0136b8c4();
    if ((uVar7 & 1) != 0) goto LAB_03848a00;
    uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
    iVar5 = FUN_005c44e8();
    if (iVar5 == 1) goto LAB_03848a00;
    iVar5 = FUN_005c44e8(uVar13);
    bVar2 = iVar5 != 6;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9d6a,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x91e6,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a17,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a18,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1d,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8a1e,1);
  (**(code **)(*(long *)param_1[7] + 0x38))();
  auVar24 = FUN_011bfd80();
  uVar7 = auVar24._0_8_;
  plVar18 = (long *)param_1[8];
  uVar6 = auVar24._0_4_;
  if (iVar4 == 2) {
    uVar6 = 1;
  }
  if ((uVar6 & 1) == 0) {
    (**(code **)(*(long *)param_1[7] + 0x38))((long *)param_1[7],auVar24._8_8_,uVar7);
    uVar6 = FUN_0136b8c4();
    uVar7 = (ulong)(uVar6 ^ 1);
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9c4d,uVar7);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9cc6,iVar3 == 0);
  bVar2 = true;
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d44,1);
  plVar18 = (long *)param_1[8];
  if ((iVar3 != 0) && (iVar4 = (**(code **)(*(long *)param_1[7] + 0x60))(), iVar4 != 2)) {
    iVar4 = (**(code **)(*(long *)param_1[7] + 0x60))();
    bVar2 = iVar4 == 4;
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x80eb,bVar2);
  plVar18 = (long *)param_1[7];
  if (plVar18 != (long *)0x0) {
    if (*(code **)(*plVar18 + 0x20) == FUN_004c9ac8) {
      plVar18 = plVar18 + 0x15;
    }
    else {
      plVar18 = (long *)(**(code **)(*plVar18 + 0x20))();
    }
    for (plVar19 = (long *)plVar18[1]; plVar19 != (long *)0x0; plVar19 = (long *)*plVar19) {
      while ((undefined **)plVar19[4] < (undefined **)0xdf547f9) {
        if ((undefined **)plVar19[4] == &PTR_s_AppBrowserController_kDataKeyIm_0df547f8) {
          if (plVar19 == plVar18 + 1) goto LAB_03848be0;
          uVar7 = plVar19[5];
          if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_0000ffa8(uVar7);
          }
          goto LAB_03848c0c;
        }
        plVar19 = (long *)plVar19[1];
        if (plVar19 == (long *)0x0) goto LAB_03848be0;
      }
    }
LAB_03848be0:
    uVar7 = 0;
LAB_03848c0c:
    if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar7);
    }
    if (uVar7 != 0) {
      uVar14 = 1;
      uVar13 = 1;
      goto LAB_03848c30;
    }
  }
  plVar18 = (long *)(**(code **)(*(long *)param_1[7] + 0x38))();
  uVar13 = (**(code **)(*plVar18 + 0x168))();
  uVar13 = FUN_003bc2f4(uVar13,"sharing_hub.desktop_sharing_hub_enabled",0x27);
  uVar14 = 0;
LAB_03848c30:
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8517,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8519,uVar14);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x851a,uVar14);
  pcVar20 = *(code **)(*(long *)param_1[7] + 0x18);
  if (pcVar20 == FUN_004c9ac8) {
    plVar18 = (long *)param_1[7] + 0x15;
  }
  else {
    plVar18 = (long *)(*pcVar20)();
  }
  for (plVar19 = (long *)plVar18[1]; plVar19 != (long *)0x0; plVar19 = (long *)*plVar19) {
    while ((undefined **)plVar19[4] < (undefined **)0xdf41201) {
      if ((undefined **)plVar19[4] == &PTR_s_WindowFeatureController_kDataKe_0df41200) {
        if (plVar19 == plVar18 + 1) goto LAB_03848cc8;
        uVar7 = plVar19[5];
        if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0000ffa8(uVar7);
        }
        goto LAB_03848cf4;
      }
      plVar19 = (long *)plVar19[1];
      if (plVar19 == (long *)0x0) goto LAB_03848cc8;
    }
  }
LAB_03848cc8:
  uVar7 = 0;
LAB_03848cf4:
  if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(uVar7);
  }
  bVar2 = iVar3 == 0;
  uVar13 = FUN_0064f70c(uVar7,2,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e0,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e1,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850e,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x850f,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84f0,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84f1,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e2,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e3,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e4,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e5,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e6,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e7,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e8,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84e9,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x84ea,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88d0,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xd04c,uVar13);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],&DAT_00009c58,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xd034,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d57,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88c4,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88c5,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88c6,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88ce,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88cf,bVar2);
  iVar3 = (**(code **)(*(long *)param_1[7] + 0x60))();
  bVar2 = iVar3 == 0;
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd14,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd15,bVar2);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xd049,bVar2);
  iVar3 = FUN_00014cc0(&PTR_s_ui_debug_tools_0e3001a8);
  if (iVar3 != 0) {
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd1e,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd1f,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd20,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd21,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xcd22,1);
  }
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d52,1);
  iVar3 = (**(code **)(*(long *)param_1[7] + 0x60))();
  if (iVar3 == 0) {
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0xdff7,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d4f,1);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d50,1);
  }
  iVar3 = FUN_03b0c4ac();
  if (iVar3 != 0) {
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d54,1);
  }
  uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
  uVar14 = FUN_02879cfc();
  lVar8 = FUN_0009278c(uVar14,uVar13,1);
  if (lVar8 != 0) {
    lVar8 = *(long *)(lVar8 + 0x20);
    (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d72,lVar8 == 0);
    uVar7 = FUN_00014cc0(&PTR_s_UseActionsForBrowserCommands_0d26d553_1_0e2ea2c0);
    if (((uVar7 & 1) == 0) &&
       (uVar7 = FUN_00014cc0(&PTR_s_AppMenuGlowUp_0d26d632_1_0e2ea360), (uVar7 & 1) == 0)) {
      lVar17 = (**(code **)(*(long *)param_1[7] + 0x18))();
      for (plVar18 = *(long **)(lVar17 + 8); plVar18 != (long *)0x0; plVar18 = (long *)*plVar18) {
        while ((undefined **)plVar18[4] < (undefined **)0xdf65271) {
          if ((undefined **)plVar18[4] == &PTR_s_BrowserActions_kDataKeyImpl_0df65270) {
            if (plVar18 == (long *)(lVar17 + 8)) goto LAB_0384984c;
            plVar18 = (long *)plVar18[5];
            if (((ulong)plVar18 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(plVar18);
            }
            goto LAB_0384987c;
          }
          plVar18 = (long *)plVar18[1];
          if (plVar18 == (long *)0x0) goto LAB_0384984c;
        }
      }
LAB_0384984c:
      plVar18 = (long *)0x0;
LAB_0384987c:
      if (((ulong)plVar18 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(plVar18);
      }
      lVar17 = *plVar18;
      if (lVar17 != 0) {
        uVar13 = FUN_0088307c();
        lVar17 = FUN_00882ea0(uVar13,0xb9,lVar17);
        if ((lVar17 != 0) && ((bool)*(char *)(lVar17 + 0xc1) != (lVar8 == 0))) {
          *(bool *)(lVar17 + 0xc1) = lVar8 == 0;
          if (*(int *)(lVar17 + 0x78) < 1) {
            *(undefined1 *)(lVar17 + 0x7c) = 0;
            FUN_00ccf484(lVar17,lVar17);
          }
          else {
            *(undefined1 *)(lVar17 + 0x7c) = 1;
          }
        }
      }
    }
  }
  FUN_0421f038(param_1);
  uVar7 = FUN_008814c0(param_1[7]);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8ca1,(uVar7 & 1) == 0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],36000,(uVar7 & 2) == 0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x8ca3,(uVar7 & 4) == 0);
  plVar18 = (long *)param_1[8];
  uVar13 = FUN_00881c08(param_1[7]);
  (**(code **)(*plVar18 + 0x38))(plVar18,0x88bc,uVar13);
  FUN_008813f8(param_1);
  plVar18 = (long *)param_1[8];
  uVar13 = FUN_00881390(param_1[7]);
  (**(code **)(*plVar18 + 0x38))(plVar18,35000,uVar13);
  plVar18 = (long *)param_1[7];
  plVar19 = (long *)param_1[8];
  plVar15 = (long *)(**(code **)(*plVar18 + 0x78))(plVar18);
  if (*(int *)(*plVar15 + 0x18) < 2) {
    uVar13 = 0;
  }
  else {
    uVar13 = FUN_00881390(plVar18);
  }
  (**(code **)(*plVar19 + 0x38))(plVar19,0x88b9,uVar13);
  FUN_0421eda4(param_1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d4c,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d4d,1);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x9d67,1);
  if (DAT_0e4a3090 == 0) {
    uVar6 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
  }
  else {
    uVar6 = FUN_083a6f28(DAT_0e4a3090,"../../arc/browser/ui/browser_commands.cc",0x28);
  }
  if (0 < (int)uVar6) {
    FUN_00f195d8(local_198,"../../arc/browser/ui/browser_commands.cc",10,0xffffffff);
    FUN_00040060(auStack_188,
                 "std::optional<int> chrome::GetKeyboardFocusedTabIndex(const BrowserWindowInterface *)"
                 ,0x55);
    FUN_00f1950c(local_198);
  }
  plVar18 = (long *)param_1[8];
  iVar3 = (**(code **)(*(long *)param_1[7] + 0x60))();
  if (iVar3 != 2) {
    (**(code **)(*(long *)param_1[7] + 0x60))();
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x88ed,0);
  (**(code **)(*(long *)param_1[7] + 0x60))();
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88ea,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88eb,0);
  (**(code **)(*(long *)param_1[8] + 0x38))((long *)param_1[8],0x88ec,0);
  plVar18 = (long *)param_1[8];
  lVar8 = (**(code **)(*(long *)param_1[7] + 0x78))();
  if ((((*(long *)(lVar8 + 0x90) == 0) ||
       (lVar8 = *(long *)(*(long *)(lVar8 + 0x90) + 0x40), lVar8 == 0)) ||
      (*(long *)(lVar8 + 0x8d8) == 0)) || (plVar19 = (long *)FUN_010f9b60(), plVar19 == (long *)0x0)
     ) {
    uVar13 = 0;
  }
  else {
    uVar13 = (**(code **)(*plVar19 + 0x70))();
  }
  (**(code **)(*plVar18 + 0x38))(plVar18,0x9d44,uVar13);
  FUN_03b25024(param_1);
  (**(code **)(*(long *)param_1[7] + 0x38))();
  lVar8 = FUN_00881874();
  if (lVar8 != 0) {
    piVar10 = (int *)param_1[0x1d];
    if ((piVar10 != (int *)0x0) && (iVar3 = *piVar10, *piVar10 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar20 = (code *)SoftwareBreakpoint(0,0x38497dc);
      (*pcVar20)();
    }
    uVar13 = param_1[0x1e];
    puVar11 = (undefined4 *)FUN_00008790(0x40);
    *puVar11 = 1;
    *(code **)(puVar11 + 2) = FUN_00dfc980;
    *(code **)(puVar11 + 4) = FUN_039a31c0;
    *(undefined1 **)(puVar11 + 6) = &LAB_04d0a140;
    *(code **)(puVar11 + 8) = FUN_02c6ef58;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(int **)(puVar11 + 0xc) = piVar10;
    *(undefined8 *)(puVar11 + 0xe) = uVar13;
    FUN_03222b84(lVar8,puVar11);
  }
  uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
  uVar14 = FUN_02302728();
  lVar8 = FUN_0009278c(uVar14,uVar13,1);
  if (lVar8 != 0) {
    FUN_008ef5e0(lVar8 + 0x58,param_1 + 6);
    if ((*(long *)(lVar8 + 0x10) != 0) && ((*(byte *)(*(long *)(lVar8 + 0x10) + 0x30) & 5) != 0)) {
      FUN_0328b690();
    }
  }
  uVar13 = (**(code **)(*(long *)param_1[7] + 0x38))();
  puVar16 = (undefined8 *)FUN_00008790(0x20);
  *puVar16 = &PTR_LAB_0da0d830;
  FUN_00029e40(local_198,"ExtensionStateObserver",
               "../../chrome/browser/ui/browser_command_controller.cc",0x12e);
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar16[1] = param_1;
  if (((ulong)puVar16 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(puVar16);
  }
  puVar16[2] = puVar16;
  puVar16[3] = 0;
  uVar7 = FUN_014baf1c(uVar13);
  if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar7);
  }
  if ((puVar16[3] & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  puVar16[3] = uVar7;
  FUN_0171bfe0(uVar7 + 0x98,puVar16[2]);
  puVar9 = (undefined8 *)param_1[0x19];
  param_1[0x19] = puVar16;
  if (puVar9 != (undefined8 *)0x0) {
    *puVar9 = &PTR_LAB_0da0d830;
    FUN_018e1e00(puVar9 + 2);
    if ((puVar9[1] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    FUN_00005810(puVar9,0x20);
  }
  return param_1;
}


