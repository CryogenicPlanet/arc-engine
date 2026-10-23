// ==== 0x433cab0  FUN_0433cab0  (336 bytes)

void FUN_0433cab0(long *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  undefined8 uVar8;
  ulong uVar9;
  ulong uVar10;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  uVar10 = param_2[1];
  if (uVar10 != 0) {
    if (uVar10 >> 0x3d != 0) {
LAB_0433cbf8:
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    lVar7 = uVar10 << 3;
    lVar3 = FUN_00008790(lVar7);
    uVar9 = 0;
    *param_1 = lVar3;
    param_1[1] = 0;
    param_1[2] = uVar10;
    param_2 = (undefined8 *)*param_2;
    do {
      uVar8 = *param_2;
      if (uVar9 == param_1[2]) {
        uVar10 = uVar9 + 1;
        if (uVar10 >> 0x3d != 0) goto LAB_0433cbf8;
        uVar6 = uVar9 * 2;
        if (uVar6 < uVar10 || uVar6 - uVar10 == 0) {
          uVar6 = uVar10;
        }
        if (0xffffffffffffffe < uVar9) {
          uVar6 = 0x1fffffffffffffff;
        }
        if (uVar6 == 0) {
          lVar3 = 0;
        }
        else {
          if (uVar6 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          lVar3 = FUN_00008790(uVar6 << 3);
        }
        puVar1 = (undefined8 *)(lVar3 + uVar9 * 8);
        if (puVar1 == (undefined8 *)0x0) goto LAB_0433cb74;
        *puVar1 = uVar8;
        lVar5 = param_1[1];
        _memcpy(puVar1 + -lVar5,(void *)*param_1,lVar5 << 3);
        lVar4 = *param_1;
        *param_1 = (long)(puVar1 + -lVar5);
        lVar5 = param_1[2];
        param_1[2] = uVar6;
        if (lVar4 != 0) {
          FUN_00005810(lVar4,lVar5 << 3);
        }
      }
      else {
        puVar1 = (undefined8 *)(lVar3 + uVar9 * 8);
        if (puVar1 == (undefined8 *)0x0) goto LAB_0433cb74;
        *puVar1 = uVar8;
      }
      lVar5 = lVar3 + uVar9 * 8;
      lVar3 = *param_1;
      lVar5 = (lVar5 + 8) - lVar3;
      uVar9 = lVar5 >> 3;
      param_1[1] = uVar9;
      if (lVar5 == 0) {
LAB_0433cb74:
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x433cb78);
        (*pcVar2)();
      }
      param_2 = param_2 + 1;
      lVar7 = lVar7 + -8;
    } while (lVar7 != 0);
  }
  return;
}


