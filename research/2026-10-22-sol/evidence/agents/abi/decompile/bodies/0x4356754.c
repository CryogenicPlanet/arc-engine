// ==== 0x4356754  FUN_04356754  (456 bytes)

void FUN_04356754(long param_1,int param_2)

{
  ulong uVar1;
  long *plVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  ulong uVar7;
  undefined8 *puVar8;
  ulong uVar9;
  long *plVar10;
  long *plVar11;
  
  uVar4 = *(ulong *)(param_1 + 0x108);
  if ((uVar4 != 0) && (*(long *)(param_1 + 0x118) != 0)) {
    uVar3 = (ulong)param_2;
    uVar5 = uVar4 - 1;
    if ((uVar4 & uVar5) == 0) {
      uVar7 = uVar5 & uVar3;
    }
    else {
      uVar7 = uVar3;
      if (uVar4 <= uVar3) {
        uVar7 = 0;
        if (uVar4 != 0) {
          uVar7 = uVar3 / uVar4;
        }
        uVar7 = uVar3 - uVar7 * uVar4;
      }
    }
    lVar6 = *(long *)(param_1 + 0x100);
    puVar8 = *(undefined8 **)(lVar6 + uVar7 * 8);
    if (puVar8 != (undefined8 *)0x0) {
      for (plVar11 = (long *)*puVar8; plVar11 != (long *)0x0; plVar11 = (long *)*plVar11) {
        uVar9 = plVar11[1];
        if (uVar9 == uVar3) {
          if ((int)plVar11[2] == param_2) {
            if ((uVar4 & uVar5) == 0) {
              uVar3 = uVar5 & uVar3;
            }
            else if (uVar4 <= uVar3) {
              uVar7 = 0;
              if (uVar4 != 0) {
                uVar7 = uVar3 / uVar4;
              }
              uVar3 = uVar3 - uVar7 * uVar4;
            }
            plVar2 = *(long **)(lVar6 + uVar3 * 8);
            do {
              plVar10 = plVar2;
              plVar2 = (long *)*plVar10;
            } while ((long *)*plVar10 != plVar11);
            if (plVar10 != (long *)(param_1 + 0x110)) {
              uVar7 = plVar10[1];
              if ((uVar4 & uVar5) == 0) {
                uVar7 = uVar7 & uVar5;
              }
              else if (uVar4 <= uVar7) {
                uVar9 = 0;
                if (uVar4 != 0) {
                  uVar9 = uVar7 / uVar4;
                }
                uVar7 = uVar7 - uVar9 * uVar4;
              }
              if (uVar7 == uVar3) goto LAB_043568ac;
            }
            if (*plVar11 != 0) {
              uVar7 = *(ulong *)(*plVar11 + 8);
              if ((uVar4 & uVar5) == 0) {
                uVar7 = uVar7 & uVar5;
              }
              else if (uVar4 <= uVar7) {
                uVar9 = 0;
                if (uVar4 != 0) {
                  uVar9 = uVar7 / uVar4;
                }
                uVar7 = uVar7 - uVar9 * uVar4;
              }
              if (uVar7 == uVar3) goto LAB_043568ac;
            }
            *(undefined8 *)(lVar6 + uVar3 * 8) = 0;
LAB_043568ac:
            lVar6 = *plVar11;
            if (lVar6 != 0) {
              uVar7 = *(ulong *)(lVar6 + 8);
              if ((uVar4 & uVar5) == 0) {
                uVar7 = uVar7 & uVar5;
              }
              else if (uVar4 <= uVar7) {
                uVar5 = 0;
                if (uVar4 != 0) {
                  uVar5 = uVar7 / uVar4;
                }
                uVar7 = uVar7 - uVar5 * uVar4;
              }
              if (uVar7 != uVar3) {
                *(long **)(*(long *)(param_1 + 0x100) + uVar7 * 8) = plVar10;
                lVar6 = *plVar11;
              }
            }
            *plVar10 = lVar6;
            *plVar11 = 0;
            *(long *)(param_1 + 0x118) = *(long *)(param_1 + 0x118) + -1;
            FUN_0435d09c(plVar11 + 2);
            FUN_00005810(plVar11,0x20);
            return;
          }
        }
        else {
          if ((uVar4 & uVar5) == 0) {
            uVar9 = uVar9 & uVar5;
          }
          else if (uVar4 <= uVar9) {
            uVar1 = 0;
            if (uVar4 != 0) {
              uVar1 = uVar9 / uVar4;
            }
            uVar9 = uVar9 - uVar1 * uVar4;
          }
          if (uVar9 != uVar7) {
            return;
          }
        }
      }
    }
  }
  return;
}


