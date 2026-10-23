// ==== 0x42fa5c8  FUN_042fa5c8  (1068 bytes)

void FUN_042fa5c8(undefined8 *param_1,long param_2,undefined8 param_3,undefined8 *param_4)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  long *plVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  code *pcVar9;
  undefined8 *puVar10;
  long *plVar11;
  ulong uVar12;
  int *piVar13;
  undefined8 local_70;
  undefined1 local_61;
  char *local_60;
  int *local_58;
  
  local_70 = 10;
  if (DAT_0e4a445a != '\0') {
    local_60 = "skip_beforeunload";
    local_58 = (int *)0xd37f57c;
    local_61 = (undefined1)param_3;
    FUN_01c8343c(DAT_0e4a445a,&local_70,&local_58,1,&local_60,&local_61);
  }
  plVar6 = *(long **)(param_2 + 0x10);
  if (plVar6 == (long *)0x0) {
    plVar11 = (long *)0x0;
  }
  else {
    plVar6[2] = plVar6[2] + 1;
    lVar5 = plVar6[1];
    do {
      if (lVar5 == -1) {
        plVar11 = (long *)0x0;
        goto LAB_042fa64c;
      }
      lVar1 = lVar5 + 1;
      lVar2 = plVar6[1];
      bVar3 = lVar2 != lVar5;
      lVar5 = lVar2;
    } while (bVar3);
    plVar6[1] = lVar1;
    plVar11 = plVar6;
LAB_042fa64c:
    if (plVar6[2] != 0) {
      LOAcquire();
      lVar5 = plVar6[2];
      plVar6[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042fa6c0;
    }
    pcVar9 = *(code **)(*plVar6 + 0x20);
    if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
      FUN_00005810(plVar6,0x20);
    }
    else {
      (*pcVar9)();
    }
  }
LAB_042fa6c0:
  if (((*(long *)(param_2 + 0xf0) == 0) || (*(char *)(*(long *)(param_2 + 0xf0) + 4) != '\0')) ||
     ((*(long *)(param_2 + 0xf8) == 0 || (*(char *)(param_2 + 0x178) != '\0')))) {
    if ((*(byte *)(param_4[3] + 0x10) & 1) == 0) {
      local_58 = (int *)CONCAT44(local_58._4_4_,1);
      (*(code *)param_4[2])(param_4,&local_58);
    }
  }
  else {
    lVar5 = *(long *)(param_2 + 0x168);
    if (lVar5 == *(long *)(param_2 + 0x170)) {
      puVar8 = (undefined8 *)FUN_042bfa68(param_2 + 0x160,param_4);
    }
    else {
      puVar8 = (undefined8 *)(*(long *)(param_2 + 0x160) + lVar5 * 0x20);
      if (puVar8 == (undefined8 *)0x0) goto LAB_042fa990;
      uVar7 = *param_4;
      puVar8[1] = param_4[1];
      *puVar8 = uVar7;
      puVar8[2] = param_4[2];
      puVar10 = (undefined8 *)param_4[3];
      puVar8[3] = puVar10;
      pcVar9 = (code *)*puVar10;
      if (pcVar9 != (code *)0x0) {
        uVar7 = (*pcVar9)(*param_4);
        *puVar8 = uVar7;
      }
      puVar8 = puVar8 + 4;
    }
    lVar1 = (long)puVar8 - *(long *)(param_2 + 0x160);
    *(long *)(param_2 + 0x168) = lVar1 >> 5;
    if (lVar1 == 0) {
LAB_042fa990:
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(1,0x42fa994);
      (*pcVar9)();
    }
    if (((uint)(lVar5 != 0) & ((uint)param_3 ^ 0xffffffff)) == 0) {
      if ((*(long *)(param_2 + 0xf0) == 0) || (*(char *)(*(long *)(param_2 + 0xf0) + 4) != '\0')) {
        plVar6 = (long *)0x0;
      }
      else {
        plVar6 = *(long **)(param_2 + 0xf8);
      }
      lVar5 = (**(code **)(*plVar6 + 0x18))();
      for (plVar6 = *(long **)(lVar5 + 8); plVar6 != (long *)0x0; plVar6 = (long *)*plVar6) {
        while ((undefined **)plVar6[4] < (undefined **)0xdf54711) {
          if ((undefined **)plVar6[4] == &PTR_s_UnloadController_kDataKeyImpl_0df54710) {
            if (plVar6 == (long *)(lVar5 + 8)) goto LAB_042fa89c;
            uVar12 = plVar6[5];
            if ((uVar12 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(uVar12);
            }
            goto LAB_042fa8c8;
          }
          plVar6 = (long *)plVar6[1];
          if (plVar6 == (long *)0x0) goto LAB_042fa89c;
        }
      }
LAB_042fa89c:
      uVar12 = 0;
LAB_042fa8c8:
      if ((uVar12 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(uVar12);
      }
      piVar13 = *(int **)(param_2 + 0x298);
      if ((piVar13 != (int *)0x0) && (iVar4 = *piVar13, *piVar13 = iVar4 + 1, iVar4 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(0,0x42fa9f4);
        (*pcVar9)();
      }
      uVar7 = *(undefined8 *)(param_2 + 0x2a0);
      local_58 = (int *)FUN_00008790(0x40);
      *local_58 = 1;
      *(undefined1 **)(local_58 + 2) = &LAB_04076980;
      *(code **)(local_58 + 4) = FUN_040a6300;
      *(undefined1 **)(local_58 + 6) = &LAB_04169bc0;
      *(code **)(local_58 + 8) = FUN_042f9f84;
      local_58[10] = 0;
      local_58[0xb] = 0;
      *(int **)(local_58 + 0xc) = piVar13;
      *(undefined8 *)(local_58 + 0xe) = uVar7;
      uVar12 = FUN_07d621f4(uVar12,param_3,&local_58);
      if (local_58 != (int *)0x0) {
        LOAcquire();
        iVar4 = *local_58;
        *local_58 = iVar4 + -1;
        LORelease();
        if (iVar4 == 1) {
          (**(code **)(local_58 + 4))();
        }
      }
      if ((uVar12 & 1) == 0) {
        FUN_042f9f84(param_2,1);
      }
    }
  }
  *param_1 = 1;
  if (plVar11 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar11[1];
    plVar11[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar11 + 0x10))(plVar11);
      if (plVar11[2] != 0) {
        LOAcquire();
        lVar5 = plVar11[2];
        plVar11[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042fa710;
      }
      pcVar9 = *(code **)(*plVar11 + 0x20);
      if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
        FUN_00005810(plVar11,0x20);
      }
      else {
        (*pcVar9)(plVar11);
      }
    }
  }
LAB_042fa710:
  local_60 = "";
  if (DAT_0e4a445a != '\0') {
    local_58 = (int *)0x0;
    FUN_0000a280(DAT_0e4a445a,&local_60,&local_58,2);
  }
  return;
}


