// ==== 0x44d8594  FUN_044d8594  (1064 bytes)

/* WARNING: Type propagation algorithm not settling */

long * FUN_044d8594(long *param_1)

{
  int **ppiVar1;
  int iVar2;
  uint uVar3;
  undefined8 *******pppppppuVar4;
  code *pcVar5;
  bool bVar6;
  undefined *puVar7;
  int *piVar8;
  int *piVar9;
  long lVar10;
  ulong uVar11;
  ulong uVar12;
  int *piVar13;
  undefined **ppuVar14;
  undefined8 *******pppppppuVar15;
  undefined8 *******pppppppuVar16;
  undefined1 auVar17 [16];
  int *local_a0;
  ulong local_98;
  undefined8 local_90;
  undefined7 local_88;
  char cStack_81;
  byte local_80;
  undefined8 *******local_78;
  undefined8 *******local_70;
  ulong local_68;
  byte local_60;
  undefined8 *******local_58;
  
  piVar13 = (int *)0x0;
  param_1[3] = 0;
  param_1[2] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  param_1[6] = (long)(param_1 + 6);
  param_1[7] = (long)(param_1 + 6);
  param_1[8] = 0;
  ppuVar14 = (undefined **)&DAT_0dff3480;
  *(undefined4 *)(param_1 + 9) = 0;
  do {
    (*(code *)ppuVar14[1])(&local_78);
    local_a0 = (int *)CONCAT44(local_a0._4_4_,*(undefined4 *)ppuVar14);
    FUN_01891db8(&local_98,&local_78);
    if ((local_60 != 0xff) && ((local_60 & 0xfd) != 0)) {
      if (local_60 == 4) {
        if ((long)local_68 < 0) {
          FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
        }
      }
      else {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_60])(&local_58,&local_78);
      }
    }
    piVar8 = (int *)*param_1;
    if ((piVar13 == piVar8) || (piVar13[-10] < (int)local_a0)) {
      lVar10 = param_1[1];
      if ((piVar13 == piVar8 + lVar10 * 10) || ((int)local_a0 < *piVar13)) goto LAB_044d8664;
      if (*piVar13 < (int)local_a0) goto LAB_044d86f0;
    }
    else {
      lVar10 = param_1[1];
LAB_044d86f0:
      piVar13 = piVar8;
      if (lVar10 * 0x28 != 0) {
        uVar11 = (lVar10 * 0x28 >> 3) * -0x3333333333333333;
        piVar9 = piVar8;
        do {
          uVar12 = uVar11 >> 1;
          piVar13 = piVar9 + uVar12 * 10 + 10;
          uVar11 = uVar11 + ~(uVar11 >> 1);
          if ((int)local_a0 <= piVar9[uVar12 * 10]) {
            piVar13 = piVar9;
            uVar11 = uVar12;
          }
          piVar9 = piVar13;
        } while (uVar11 != 0);
      }
      if ((piVar13 == piVar8 + lVar10 * 10) || ((int)local_a0 < *piVar13)) {
LAB_044d8664:
        piVar13 = (int *)FUN_044da24c(param_1,piVar13,&local_a0);
      }
    }
    if ((local_80 != 0xff) && ((local_80 & 0xfd) != 0)) {
      if (local_80 == 4) {
        if (cStack_81 < '\0') {
          FUN_00005810(local_98,CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
        }
      }
      else {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_80])(&local_78,&local_98);
      }
    }
    piVar13 = piVar13 + 10;
    ppuVar14 = ppuVar14 + 2;
  } while (ppuVar14 != &PTR_s_kAlwaysImportToBookmarkBarSubfol_0dff3750);
  if (*(int *)(DAT_0e4ac750 + 0x10) == 1) {
    FUN_044d9734(param_1,0xb,1);
  }
  lVar10 = DAT_0e4a2fd8;
  local_a0 = (int *)0x0;
  local_98 = 0;
  local_90 = 0;
  FUN_00cad330(&local_a0,DAT_0e4a2fd8,&DAT_0cb213d8,4);
  uVar3 = (uint)(char)local_90._7_1_;
  uVar11 = local_98;
  if (-1 < (int)uVar3) {
    uVar11 = (ulong)local_90._7_1_;
  }
  if (uVar11 == 7) {
    ppiVar1 = (int **)local_a0;
    if (-1 < (int)uVar3) {
      ppiVar1 = &local_a0;
    }
    if (*(int *)ppiVar1 == 0x6c697475 && *(int *)((long)ppiVar1 + 3) == 0x7974696c) {
      local_70 = (undefined8 *******)0x0;
      local_68 = 0;
      local_78 = &local_70;
      if (*(long *)(lVar10 + 0x28) != 0) {
        local_70 = (undefined8 *******)
                   FUN_00f8e130(&local_78,*(undefined8 *)(lVar10 + 0x20),&local_78);
        local_70[2] = &local_70;
        pppppppuVar4 = local_70;
        do {
          pppppppuVar16 = pppppppuVar4;
          pppppppuVar4 = (undefined8 *******)*pppppppuVar16;
        } while ((undefined8 *******)*pppppppuVar16 != (undefined8 *******)0x0);
        local_68 = *(ulong *)(lVar10 + 0x28);
        local_78 = pppppppuVar16;
        while ((undefined8 ********)pppppppuVar16 != &local_70) {
          auVar17 = FUN_044da780(&PTR_s_kAlwaysImportToBookmarkBarSubfol_0dff3750,pppppppuVar16 + 4)
          ;
          puVar7 = auVar17._0_8_;
          if (puVar7 != &DAT_0dff3ba0 && puVar7 != auVar17._8_8_) {
            iVar2 = *(int *)(puVar7 + 0x10);
            piVar8 = (int *)*param_1;
            lVar10 = param_1[1] * 0x28;
            piVar13 = piVar8;
            if (lVar10 != 0) {
              uVar11 = (lVar10 >> 3) * -0x3333333333333333;
              piVar9 = piVar8;
              do {
                uVar12 = uVar11 >> 1;
                piVar13 = piVar9 + uVar12 * 10 + 10;
                uVar11 = uVar11 + ~(uVar11 >> 1);
                if (iVar2 <= piVar9[uVar12 * 10]) {
                  piVar13 = piVar9;
                  uVar11 = uVar12;
                }
                piVar9 = piVar13;
              } while (uVar11 != 0);
            }
            if ((piVar13 == piVar8 + param_1[1] * 10) || (iVar2 < *piVar13)) {
                    /* WARNING: Does not return */
              pcVar5 = (code *)SoftwareBreakpoint(0,0x44d89bc);
              (*pcVar5)();
            }
            if ((char)piVar13[8] == '\x01') {
              FUN_044d9734(param_1,iVar2,1);
            }
          }
          pppppppuVar4 = (undefined8 *******)pppppppuVar16[1];
          pppppppuVar15 = pppppppuVar16;
          if ((undefined8 *******)pppppppuVar16[1] == (undefined8 *******)0x0) {
            do {
              pppppppuVar16 = (undefined8 *******)pppppppuVar15[2];
              bVar6 = pppppppuVar15 != (undefined8 *******)*pppppppuVar16;
              pppppppuVar15 = pppppppuVar16;
            } while (bVar6);
          }
          else {
            do {
              pppppppuVar16 = pppppppuVar4;
              pppppppuVar4 = (undefined8 *******)*pppppppuVar16;
            } while ((undefined8 *******)*pppppppuVar16 != (undefined8 *******)0x0);
          }
        }
      }
      local_58 = &local_78;
      FUN_00ed0b9c(&local_58,local_70);
      if (-1 < (long)local_90) {
        return param_1;
      }
      goto LAB_044d8828;
    }
  }
  if ((uVar3 >> 7 & 1) == 0) {
    return param_1;
  }
LAB_044d8828:
  FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
  return param_1;
}


