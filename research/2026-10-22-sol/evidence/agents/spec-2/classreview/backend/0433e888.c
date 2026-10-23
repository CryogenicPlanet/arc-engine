// ==== 0x433e888  FUN_0433e888  (724 bytes)

void FUN_0433e888(long *param_1,long param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  long *plVar3;
  long lVar4;
  long lVar5;
  code *pcVar6;
  undefined8 *puVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  undefined1 uVar11;
  long *plVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined1 local_a1;
  ulong local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined **local_80;
  undefined **local_78;
  long *local_70;
  long lStack_68;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  uVar8 = *(ulong *)(param_2 + 0x2b0);
  if (uVar8 != 0) {
    if (0x333333333333333 < uVar8) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    lVar5 = FUN_00008790(uVar8 * 0x50);
    FUN_0433f298(0,0,lVar5);
    local_80 = (undefined **)*param_1;
    *param_1 = lVar5;
    param_1[1] = 0;
    lStack_68 = param_1[2];
    local_70 = (long *)0x0;
    param_1[2] = uVar8;
    local_78 = local_80;
    FUN_0433f484(&local_80);
    lVar5 = *(long *)(param_2 + 0x2b0) * 0x68;
    if (lVar5 != 0) {
      lVar9 = 0;
      lVar10 = *(long *)(param_2 + 0x2a8);
      do {
        puVar2 = (undefined8 *)(lVar10 + lVar9);
        FUN_04338dd8(&local_80,puVar2 + 3);
        if (*(char *)(puVar2 + 0xb) == '\0') {
          uVar11 = 0;
          local_90 = local_90 & 0xffffffffffffff;
          local_a0 = local_a0 & 0xffffffffffff0000;
        }
        else if (*(char *)((long)puVar2 + 0x57) < '\0') {
          FUN_00054420(&local_a0,puVar2[8],*(undefined8 *)(lVar10 + lVar9 + 0x48));
          uVar11 = *(undefined1 *)(puVar2 + 0xb);
        }
        else {
          uStack_98 = puVar2[9];
          local_a0 = puVar2[8];
          local_90 = puVar2[10];
          uVar11 = 1;
        }
        local_a1 = uVar11;
        if (param_1[1] == param_1[2]) {
          puVar7 = (undefined8 *)FUN_0433f60c(param_1,lVar10 + lVar9,&local_80,&local_a0,&local_a1);
        }
        else {
          puVar7 = (undefined8 *)(*param_1 + param_1[1] * 0x50);
          if (puVar7 == (undefined8 *)0x0) goto LAB_0433eb38;
          puVar1 = (undefined8 *)(lVar10 + lVar9);
          if (*(char *)((long)puVar1 + 0x17) < '\0') {
            FUN_00054420(puVar7,*puVar1,puVar1[1]);
          }
          else {
            uVar14 = puVar2[1];
            uVar13 = *puVar2;
            puVar7[2] = puVar2[2];
            puVar7[1] = uVar14;
            *puVar7 = uVar13;
          }
          puVar7[3] = &PTR_FUN_0da7eed0;
          puVar7[4] = local_78;
          puVar7[5] = local_70;
          if (local_70 != (long *)0x0) {
            local_70[1] = local_70[1] + 1;
          }
          if ((long)local_90 < 0) {
            FUN_00054420(puVar7 + 6,local_a0,uStack_98);
          }
          else {
            puVar7[8] = local_90;
            puVar7[7] = uStack_98;
            puVar7[6] = local_a0;
          }
          *(undefined1 *)(puVar7 + 9) = uVar11;
          puVar7 = puVar7 + 10;
        }
        param_1[1] = ((long)puVar7 - *param_1 >> 4) * -0x3333333333333333;
        if ((long)puVar7 - *param_1 == 0) {
LAB_0433eb38:
                    /* WARNING: Does not return */
          pcVar6 = (code *)SoftwareBreakpoint(1,0x433eb3c);
          (*pcVar6)();
        }
        if ((long)local_90 < 0) {
          FUN_00005810(local_a0,local_90 << 1);
        }
        plVar3 = local_70;
        local_80 = &PTR_FUN_0da7eed0;
        if (local_70 != (long *)0x0) {
          LOAcquire();
          lVar4 = local_70[1];
          local_70[1] = lVar4 + -1;
          LORelease();
          if (lVar4 == 0) {
            plVar12 = local_70 + 2;
            (**(code **)(*local_70 + 0x10))(local_70);
            if (*plVar12 != 0) {
              LOAcquire();
              lVar4 = plVar3[2];
              plVar3[2] = lVar4 + -1;
              LORelease();
              if (lVar4 != 0) goto LAB_0433ea70;
            }
            pcVar6 = *(code **)(*plVar3 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar3,0x20);
            }
            else {
              (*pcVar6)(plVar3);
            }
          }
        }
LAB_0433ea70:
        lVar9 = lVar9 + 0x68;
      } while (lVar5 - lVar9 != 0);
    }
  }
  return;
}


