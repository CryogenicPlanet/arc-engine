// ==== 0x42cc3f0  FUN_042cc3f0  (932 bytes)

/* WARNING: Removing unreachable block (ram,0x042cc504) */
/* WARNING: Removing unreachable block (ram,0x042cc724) */

void FUN_042cc3f0(long param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)

{
  code *pcVar1;
  int iVar2;
  long *plVar3;
  undefined8 *puVar4;
  undefined4 *puVar5;
  long lVar6;
  undefined8 uVar7;
  int *piVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  long local_1e8;
  undefined4 local_1e0;
  long local_1d8;
  int local_1d0;
  long local_1c8;
  undefined1 local_1c0;
  undefined7 uStack_1bf;
  undefined8 local_1b8;
  undefined7 local_1b0;
  char cStack_1a9;
  char local_1a8;
  undefined6 uStack_1a7;
  char cStack_1a1;
  undefined8 local_1a0;
  undefined7 local_190;
  char cStack_189;
  undefined8 local_168;
  undefined7 local_158;
  char cStack_151;
  undefined8 local_150;
  undefined7 local_140;
  char cStack_139;
  undefined8 local_118;
  undefined7 local_108;
  char cStack_101;
  undefined8 local_100;
  undefined7 local_f0;
  char cStack_e9;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  ulong local_60;
  
  if (*(long *)(param_1 + 0x88) == 0) {
    local_1c0 = 0;
    local_1a8 = '\0';
    (*(code *)param_4[2])(param_4,&local_1c0);
    if ((local_1a8 != '\0') && (cStack_1a9 < '\0')) {
      FUN_00005810(CONCAT71(uStack_1bf,local_1c0),
                   CONCAT17(cStack_1a9,local_1b0) & 0x7fffffffffffffff);
    }
  }
  else {
    local_60 = 0;
    uStack_78 = 0;
    local_80 = 0;
    uStack_68 = 0;
    uStack_70 = 0;
    uStack_98 = 0;
    local_a0 = 0;
    uStack_88 = 0;
    local_90 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    FUN_042c9420(param_2);
    if ((local_60 & 1) != 0) {
      FUN_0119426c(*(undefined8 *)(param_1 + 0x88));
      plVar3 = (long *)FUN_067688b0();
      if (plVar3 != (long *)0x0) {
        puVar4 = (undefined8 *)FUN_00008790(0x30);
        puVar4[1] = 0;
        *puVar4 = 0;
        puVar4[3] = 0;
        puVar4[2] = 0;
        puVar4[5] = 0;
        puVar4[4] = 0;
        FUN_01996f10(&local_1c0,&local_b0);
        FUN_07a82090(&local_1d8,puVar4,0);
        (**(code **)(*plVar3 + 0x10))(plVar3,&local_1c0,&local_1d8);
        if ((local_1d0 == 1) && (local_1c8 != 0)) {
          FUN_084a7784();
        }
        lVar6 = local_1c8;
        local_1d0 = 0;
        local_1c8 = 0;
        if (lVar6 != 0) {
          piVar8 = (int *)(lVar6 + 8);
          LOAcquire();
          iVar2 = *piVar8;
          *piVar8 = iVar2 + -1;
          LORelease();
          if (iVar2 == 1) {
            uVar7 = FUN_084a76fc();
            FUN_00005810(uVar7,0x38);
          }
          if (local_1c8 != 0) {
            LOAcquire();
            iVar2 = *(int *)(local_1c8 + 8);
            *(int *)(local_1c8 + 8) = iVar2 + -1;
            LORelease();
            if (iVar2 == 1) {
              uVar7 = FUN_084a76fc();
              FUN_00005810(uVar7,0x38);
            }
          }
        }
        if (local_1d8 != 0) {
          FUN_005a359c();
        }
        if (cStack_e9 < '\0') {
          FUN_00005810(local_100,CONCAT17(cStack_e9,local_f0) & 0x7fffffffffffffff);
        }
        if (cStack_101 < '\0') {
          FUN_00005810(local_118,CONCAT17(cStack_101,local_108) & 0x7fffffffffffffff);
        }
        if (cStack_139 < '\0') {
          FUN_00005810(local_150,CONCAT17(cStack_139,local_140) & 0x7fffffffffffffff);
        }
        if (cStack_151 < '\0') {
          FUN_00005810(local_168,CONCAT17(cStack_151,local_158) & 0x7fffffffffffffff);
        }
        if (cStack_189 < '\0') {
          FUN_00005810(local_1a0,CONCAT17(cStack_189,local_190) & 0x7fffffffffffffff);
        }
        if (cStack_1a1 < '\0') {
          FUN_00005810(local_1b8,
                       CONCAT17(cStack_1a1,CONCAT61(uStack_1a7,local_1a8)) & 0x7fffffffffffffff);
        }
        FUN_07a821a4(puVar4);
        uVar7 = puVar4[5];
        local_1e8 = 0;
        local_1e0 = 0;
        piVar8 = *(int **)(param_1 + 0x310);
        if ((piVar8 != (int *)0x0) && (iVar2 = *piVar8, *piVar8 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x42cc734);
          (*pcVar1)();
        }
        uVar9 = *(undefined8 *)(param_1 + 0x318);
        puVar5 = (undefined4 *)FUN_00008790(0x80);
        *puVar5 = 1;
        *(code **)(puVar5 + 2) = FUN_042da798;
        *(code **)(puVar5 + 4) = FUN_042da900;
        *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
        *(undefined1 **)(puVar5 + 8) = &LAB_042cc79c;
        *(undefined8 *)(puVar5 + 10) = 0;
        *(int **)(puVar5 + 0xc) = piVar8;
        *(undefined8 *)(puVar5 + 0xe) = uVar9;
        lVar6 = param_4[3];
        uVar9 = *param_4;
        uVar11 = param_4[3];
        uVar10 = param_4[2];
        *(undefined8 *)(puVar5 + 0x12) = param_4[1];
        *(undefined8 *)(puVar5 + 0x10) = uVar9;
        *(undefined8 *)(puVar5 + 0x16) = uVar11;
        *(undefined8 *)(puVar5 + 0x14) = uVar10;
        if (*(long *)(lVar6 + 8) != 0) {
          param_4[2] = 0;
          param_4[3] = &DAT_0c4b3500;
        }
        if (*(char *)((long)param_3 + 0x17) < '\0') {
          FUN_00060a60(puVar5 + 0x18,*param_3,param_3[1]);
        }
        else {
          uVar9 = *param_3;
          *(undefined8 *)(puVar5 + 0x1a) = param_3[1];
          *(undefined8 *)(puVar5 + 0x18) = uVar9;
          *(undefined8 *)(puVar5 + 0x1c) = param_3[2];
        }
        *(undefined8 **)(puVar5 + 0x1e) = puVar4;
        FUN_0578d4cc(uVar7,&local_1e8,puVar5);
        if (local_1e8 == 0) {
          return;
        }
        FUN_005a359c();
        return;
      }
    }
    local_1c0 = 0;
    local_1a8 = '\0';
    (*(code *)param_4[2])(param_4,&local_1c0);
    if ((local_1a8 != '\0') && (cStack_1a9 < '\0')) {
      FUN_00005810(CONCAT71(uStack_1bf,local_1c0),
                   CONCAT17(cStack_1a9,local_1b0) & 0x7fffffffffffffff);
    }
  }
  return;
}


