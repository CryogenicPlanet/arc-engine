// ==== 0x4377d70  FUN_04377d70  (732 bytes)

void FUN_04377d70(long param_1,undefined8 *param_2,undefined8 *param_3,char *param_4)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  long lVar4;
  int *piVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  ulong uVar8;
  undefined8 *puVar9;
  char *pcVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  char *local_98 [2];
  undefined7 local_88;
  char cStack_81;
  int *local_80;
  undefined8 uStack_78;
  int *local_70;
  undefined8 uStack_68;
  int *local_60;
  long local_58;
  ulong local_50;
  undefined8 local_48;
  char *local_40;
  char *pcStack_38;
  
  if (*(long *)(param_1 + 0x130) == 0) {
LAB_04377e68:
    local_70 = (int *)((ulong)local_70 & 0xffffffffffffff);
    local_80 = (int *)((ulong)local_80 & 0xffffffffffffff00);
    (*(code *)param_3[2])(param_3,&local_80,0);
    if (-1 < (long)local_70) {
      return;
    }
    uVar8 = (ulong)local_70 & 0x7fffffffffffffff;
    piVar5 = local_80;
  }
  else {
    uVar1 = *(uint *)(*(long *)(param_1 + 0x130) + 0x958);
    if (uVar1 != 4) {
      if (0xb < uVar1) {
LAB_04378044:
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x4378048);
        (*pcVar2)();
      }
      if ((0x58eU >> (ulong)(uVar1 & 0x1f) & 1) != 0) goto LAB_04377e68;
      if (uVar1 != 0) {
        if (uVar1 == 0xb) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x4378054);
          (*pcVar2)();
        }
        goto LAB_04378044;
      }
    }
    local_50 = 0;
    uStack_68 = 0;
    local_70 = (int *)0x0;
    local_58 = 0;
    local_60 = (int *)0x0;
    uStack_78 = 0;
    local_80 = (int *)0x0;
    FUN_02ceacd0(&local_80);
    if (((((((local_50 & 1) == 0) || (local_60 == (int *)0x0)) || ((char)local_60[1] != '\0')) ||
         ((local_58 == 0 || (local_60 == (int *)0x0)))) ||
        (((char)local_60[1] != '\0' || ((local_58 == 0 || (lVar4 = FUN_01b12e04(), lVar4 == 0))))))
       || (*(char *)(lVar4 + 0x51) != '\0')) {
      cStack_81 = '\0';
      local_98[0] = (char *)((ulong)local_98[0] & 0xffffffffffffff00);
      (*(code *)param_3[2])(param_3,local_98,0);
      if (cStack_81 < '\0') {
        FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
      }
    }
    else {
      local_48 = 10;
      if (DAT_0e4a445a != '\0') {
        if (param_4[0x17] < '\0') {
          if (*(long *)(param_4 + 8) == 0) {
            pcVar10 = "unknown Arc event";
          }
          else {
            pcVar10 = *(char **)param_4;
          }
        }
        else {
          pcVar10 = "unknown Arc event";
          if (param_4[0x17] != '\0') {
            pcVar10 = param_4;
          }
        }
        local_40 = "NULL";
        if (pcVar10 != (char *)0x0) {
          local_40 = pcVar10;
        }
        local_98[0] = "arc::WebContents::Impl::ExecuteJavascript";
        pcStack_38 = "reason";
        FUN_0439e700(DAT_0e4a445a,&local_48,local_98,*(undefined8 *)(param_1 + 0x5e8),&pcStack_38,
                     &local_40);
      }
      uVar11 = *(undefined8 *)(*(long *)(param_1 + 0x130) + 0x8d8);
      lVar4 = (long)*(char *)((long)param_2 + 0x17);
      puVar9 = param_2;
      if (lVar4 < 0) {
        puVar9 = (undefined8 *)*param_2;
        lVar4 = param_2[1];
      }
      FUN_007cf8e0(local_98,puVar9,lVar4);
      puVar6 = (undefined4 *)FUN_00008790(0x50);
      uVar7 = *param_3;
      uVar13 = param_3[3];
      uVar12 = param_3[2];
      *(undefined8 *)(puVar6 + 0xc) = param_3[1];
      *(undefined8 *)(puVar6 + 10) = uVar7;
      *puVar6 = 1;
      *(undefined1 **)(puVar6 + 2) = &LAB_0439ed14;
      *(code **)(puVar6 + 4) = FUN_0439edb8;
      *(code **)(puVar6 + 6) = FUN_00b77080;
      *(code **)(puVar6 + 8) = FUN_0437805c;
      puVar9 = (undefined8 *)param_3[3];
      *(undefined8 *)(puVar6 + 0x10) = uVar13;
      *(undefined8 *)(puVar6 + 0xe) = uVar12;
      if ((code *)*puVar9 != (code *)0x0) {
        uVar7 = (*(code *)*puVar9)(*param_3);
        *(undefined8 *)(puVar6 + 10) = uVar7;
      }
      *(undefined8 *)(puVar6 + 0x12) = *(undefined8 *)(param_1 + 0x5e8);
      FUN_066776dc(uVar11,local_98,puVar6,4);
      if (cStack_81 < '\0') {
        FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) << 1);
      }
      *(long *)(param_1 + 0x5e8) = *(long *)(param_1 + 0x5e8) + 1;
    }
    if ((local_50 & 1) == 0) {
      return;
    }
    if (local_60 != (int *)0x0) {
      LOAcquire();
      iVar3 = *local_60;
      *local_60 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        FUN_00005810(local_60,8);
      }
    }
    if (local_70 == (int *)0x0) {
      return;
    }
    LOAcquire();
    iVar3 = *local_70;
    *local_70 = iVar3 + -1;
    LORelease();
    if (iVar3 != 1) {
      return;
    }
    uVar8 = 8;
    piVar5 = local_70;
  }
  FUN_00005810(piVar5,uVar8);
  return;
}


