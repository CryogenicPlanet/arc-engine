// ==== 0x432c5e4  FUN_0432c5e4  (168 bytes)

void FUN_0432c5e4(long *param_1,long param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 *puVar6;
  ulong uVar7;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  uVar7 = *(ulong *)(param_2 + 0x18);
  if (uVar7 != 0) {
    if (uVar7 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    puVar6 = *(undefined8 **)(param_2 + 0x10);
    puVar1 = puVar6 + uVar7 * 2;
    puVar3 = (undefined8 *)FUN_00008790(uVar7 << 4);
    *param_1 = (long)puVar3;
    param_1[2] = uVar7;
    puVar4 = puVar3;
    do {
      if (puVar4 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x432c688);
        (*pcVar2)();
      }
      *puVar4 = *puVar6;
      lVar5 = puVar6[1];
      puVar4[1] = lVar5;
      if (lVar5 != 0) {
        *(long *)(lVar5 + 8) = *(long *)(lVar5 + 8) + 1;
      }
      puVar6 = puVar6 + 2;
      puVar4 = puVar4 + 2;
    } while (puVar6 != puVar1);
    param_1[1] = (long)puVar4 - (long)puVar3 >> 4;
  }
  return;
}


