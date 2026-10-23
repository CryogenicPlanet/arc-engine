// ==== 0x7bdba94  FUN_07bdba94  (1120 bytes)

void FUN_07bdba94(ulong param_1,undefined8 *param_2,int *param_3,int *param_4)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  long *plVar5;
  int *piVar6;
  undefined8 uVar7;
  long lVar8;
  code *pcVar9;
  long lVar10;
  int *piVar11;
  int *local_b8;
  int *local_b0;
  undefined8 local_a8;
  int *local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined1 local_80 [16];
  ulong local_70;
  char *local_60;
  char *pcStack_58;
  
  cVar1 = DAT_0e4a4571;
  local_a8 = 0x121;
  local_b8 = param_4;
  local_b0 = param_3;
  if (DAT_0e4a4571 != '\0') {
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(&local_a0,*param_2,param_2[1]);
    }
    else {
      uStack_98 = param_2[1];
      local_a0 = (int *)*param_2;
      local_90 = param_2[2];
    }
    local_80._8_8_ = uStack_98;
    local_80._0_8_ = local_a0;
    local_70 = local_90;
    uStack_98 = 0;
    local_90 = 0;
    local_a0 = (int *)0x0;
    local_60 = "profile_path";
    pcStack_58 = "ProfileManager::CreateProfileAsync";
    FUN_004d4be0(cVar1,&local_a8,&pcStack_58,1,&local_60,local_80);
    if ((long)local_70 < 0) {
      FUN_00005810(local_80._0_8_,local_70 & 0x7fffffffffffffff);
    }
    if ((long)local_90 < 0) {
      FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
    }
  }
  if (*(char *)(param_1 + 0x179) == '\0') {
    local_80 = FUN_04210644(param_1 + 0x198,param_2);
    lVar8 = local_80._8_8_;
    uStack_98 = 0;
    uVar4 = FUN_005be840(local_80,&local_a0);
    if ((uVar4 & 1) == 0) {
      FUN_013c3580(local_80,"operator->");
      FUN_013c3580(local_80,"operator*()");
      piVar11 = *(int **)(param_1 + 0x1a8);
      if ((piVar11 != (int *)0x0) && (iVar3 = *piVar11, *piVar11 = iVar3 + 1, iVar3 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(0,0x7bdbef4);
        (*pcVar9)();
      }
      uVar7 = *(undefined8 *)(param_1 + 0x1b0);
      piVar6 = (int *)FUN_00008790(0x68);
      *piVar6 = 1;
      *(undefined1 **)(piVar6 + 2) = &LAB_07bddab8;
      *(code **)(piVar6 + 4) = FUN_07bddaf0;
      *(undefined1 **)(piVar6 + 6) = &LAB_04d21600;
      *(code **)(piVar6 + 8) = FUN_07bdba94;
      piVar6[10] = 0;
      piVar6[0xb] = 0;
      *(int **)(piVar6 + 0xc) = piVar11;
      *(undefined8 *)(piVar6 + 0xe) = uVar7;
      if (*(char *)((long)param_2 + 0x17) < '\0') {
        FUN_00060a60(piVar6 + 0x10,*param_2,param_2[1]);
      }
      else {
        uVar7 = *param_2;
        *(undefined8 *)(piVar6 + 0x12) = param_2[1];
        *(undefined8 *)(piVar6 + 0x10) = uVar7;
        *(undefined8 *)(piVar6 + 0x14) = param_2[2];
      }
      local_b8 = (int *)0x0;
      local_b0 = (int *)0x0;
      *(int **)(piVar6 + 0x16) = param_3;
      *(int **)(piVar6 + 0x18) = param_4;
      local_a0 = piVar6;
      FUN_00dfccb0(lVar8 + 0x18,&local_a0);
      param_3 = local_a0;
      goto joined_r0x07bdbd18;
    }
    uVar4 = FUN_027e08cc(param_1,param_2);
    if ((uVar4 & 1) == 0) {
      if (param_3 == (int *)0x0) goto LAB_07bdbd1c;
      pcVar9 = *(code **)(param_3 + 2);
      lVar10 = 0;
    }
    else {
      plVar5 = (long *)FUN_0417df10(param_1 + 0xa8,param_2);
      lVar8 = *plVar5;
      bVar2 = param_3 == (int *)0x0 && param_4 == (int *)0x0;
      if ((lVar8 == 0) || (lVar8 == param_1 + 0xb0)) {
        uVar7 = FUN_02ec9880(param_2,param_1,1);
        lVar8 = FUN_07bdc02c(param_1,uVar7);
        if (bVar2) goto LAB_07bdbd1c;
LAB_07bdbb54:
        if (param_3 != (int *)0x0) {
          FUN_07977444(lVar8 + 0x18,&local_b0);
          param_4 = local_b8;
        }
        if (param_4 != (int *)0x0) {
          FUN_07977444(lVar8 + 0x30,&local_b8);
        }
        goto LAB_07bdbd1c;
      }
      if (bVar2) goto LAB_07bdbd1c;
      lVar8 = *(long *)(lVar8 + 0x38);
      if (((*(byte *)(lVar8 + 0x60) & 1) == 0) || (lVar10 = *(long *)(lVar8 + 0x48), lVar10 == 0))
      goto LAB_07bdbb54;
      uVar7 = FUN_005c44e8(lVar10);
      if ((int)uVar7 == 2) {
LAB_07bdbe18:
        FUN_07bdc1c4(uVar7,lVar10);
        lVar10 = FUN_0317d0d4(lVar10,1);
      }
      else {
        uVar7 = FUN_0136b8c4(lVar10);
        if ((int)uVar7 != 0) goto LAB_07bdbe18;
      }
      if (param_3 == (int *)0x0) goto LAB_07bdbd1c;
      pcVar9 = *(code **)(param_3 + 2);
    }
    local_b0 = (int *)0x0;
    (*pcVar9)(param_3,lVar10);
    LOAcquire();
    iVar3 = *param_3;
    *param_3 = iVar3 + -1;
    LORelease();
    if (iVar3 != 1) goto LAB_07bdbd1c;
    pcVar9 = *(code **)(param_3 + 4);
  }
  else {
    if ((param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1);
    }
    piVar11 = (int *)FUN_00008790(0x60);
    *piVar11 = 1;
    *(undefined1 **)(piVar11 + 2) = &LAB_07bdd994;
    *(code **)(piVar11 + 4) = FUN_07bdd9f4;
    *(code **)(piVar11 + 6) = FUN_00b77080;
    *(code **)(piVar11 + 8) = FUN_07bdba94;
    piVar11[10] = 0;
    piVar11[0xb] = 0;
    *(ulong *)(piVar11 + 0xc) = param_1;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(piVar11 + 0xe,*param_2,param_2[1]);
    }
    else {
      uVar7 = *param_2;
      *(undefined8 *)(piVar11 + 0x10) = param_2[1];
      *(undefined8 *)(piVar11 + 0xe) = uVar7;
      *(undefined8 *)(piVar11 + 0x12) = param_2[2];
    }
    local_b8 = (int *)0x0;
    local_b0 = (int *)0x0;
    *(int **)(piVar11 + 0x14) = param_3;
    *(int **)(piVar11 + 0x16) = param_4;
    local_80._0_8_ = piVar11;
    FUN_00dfccb0(param_1 + 0x180,local_80);
    param_3 = (int *)local_80._0_8_;
joined_r0x07bdbd18:
    if (param_3 == (int *)0x0) goto LAB_07bdbd1c;
    LOAcquire();
    iVar3 = *param_3;
    *param_3 = iVar3 + -1;
    LORelease();
    if (iVar3 != 1) goto LAB_07bdbd1c;
    pcVar9 = *(code **)(param_3 + 4);
  }
  (*pcVar9)(param_3);
LAB_07bdbd1c:
  local_a0 = (int *)0x121;
  if (DAT_0e4a4571 != '\0') {
    local_80._0_8_ = 0;
    FUN_0000a280(DAT_0e4a4571,&local_a0,local_80,2);
  }
  if (local_b8 != (int *)0x0) {
    LOAcquire();
    iVar3 = *local_b8;
    *local_b8 = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      (**(code **)(local_b8 + 4))();
    }
  }
  if (local_b0 != (int *)0x0) {
    LOAcquire();
    iVar3 = *local_b0;
    *local_b0 = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      (**(code **)(local_b0 + 4))();
    }
  }
  return;
}


