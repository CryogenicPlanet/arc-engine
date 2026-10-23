// ==== 0x1f5fc10  FUN_01f5fc10  (332 bytes)

long * FUN_01f5fc10(long *param_1,undefined8 *param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  ulong uVar6;
  undefined8 uVar7;
  long lVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  uVar6 = param_2[1];
  if (uVar6 != 0) {
    if (0x6bca1af286bca1 < uVar6) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar5 = (undefined8 *)*param_2;
    puVar2 = (undefined8 *)FUN_00008790(uVar6 * 0x260);
    *param_1 = (long)puVar2;
    param_1[1] = 0;
    param_1[2] = uVar6;
    puVar3 = puVar2;
    if (uVar6 * 0x260 != 0) {
      puVar3 = puVar5 + uVar6 * 0x4c;
      puVar2 = puVar2 + 0x47;
      do {
        puVar4 = puVar2;
        if (puVar4 + -0x47 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x1f5fd48);
          (*pcVar1)();
        }
        puVar4[-0x47] = *puVar5;
        _memcpy(puVar4 + -0x46,puVar5 + 1,0x1f0);
        uVar9 = puVar5[0x40];
        uVar7 = puVar5[0x3f];
        uVar11 = puVar5[0x42];
        uVar10 = puVar5[0x41];
        uVar13 = puVar5[0x44];
        uVar12 = puVar5[0x43];
        uVar14 = *(undefined8 *)((long)puVar5 + 0x221);
        *(undefined8 *)((long)puVar4 + -0xf) = *(undefined8 *)((long)puVar5 + 0x229);
        *(undefined8 *)((long)puVar4 + -0x17) = uVar14;
        puVar4[-5] = uVar11;
        puVar4[-6] = uVar10;
        puVar4[-3] = uVar13;
        puVar4[-4] = uVar12;
        puVar4[-7] = uVar9;
        puVar4[-8] = uVar7;
        if (*(char *)((long)puVar5 + 0x24f) < '\0') {
          FUN_00060a60(puVar4,puVar5[0x47],puVar5[0x48]);
        }
        else {
          uVar9 = puVar5[0x48];
          uVar7 = puVar5[0x47];
          puVar4[2] = puVar5[0x49];
          puVar4[1] = uVar9;
          *puVar4 = uVar7;
        }
        uVar7 = puVar5[0x4a];
        puVar4[4] = puVar5[0x4b];
        puVar4[3] = uVar7;
        puVar5 = puVar5 + 0x4c;
        puVar2 = puVar4 + 0x4c;
      } while (puVar5 != puVar3);
      puVar2 = (undefined8 *)*param_1;
      puVar3 = puVar4 + 5;
    }
    param_1[1] = ((long)puVar3 - (long)puVar2 >> 5) * -0x79435e50d79435e5;
  }
  lVar8 = param_2[3];
  param_1[4] = param_2[4];
  param_1[3] = lVar8;
  return param_1;
}


