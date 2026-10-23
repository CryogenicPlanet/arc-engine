// ==== 0x4335698  FUN_04335698  (484 bytes)

/* WARNING: Removing unreachable block (ram,0x04335824) */

undefined8 * FUN_04335698(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ulong uVar7;
  long lVar8;
  undefined8 uVar9;
  
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(param_1,*param_2,param_2[1]);
  }
  else {
    uVar9 = param_2[1];
    uVar5 = *param_2;
    param_1[2] = param_2[2];
    param_1[1] = uVar9;
    *param_1 = uVar5;
  }
  if (*(char *)((long)param_2 + 0x2f) < '\0') {
    FUN_00060a60(param_1 + 3,param_2[3],param_2[4]);
  }
  else {
    uVar9 = param_2[4];
    uVar5 = param_2[3];
    param_1[5] = param_2[5];
    param_1[4] = uVar9;
    param_1[3] = uVar5;
  }
  if (*(char *)((long)param_2 + 0x47) < '\0') {
    FUN_00054420(param_1 + 6,param_2[6],param_2[7]);
  }
  else {
    uVar9 = param_2[7];
    uVar5 = param_2[6];
    param_1[8] = param_2[8];
    param_1[7] = uVar9;
    param_1[6] = uVar5;
  }
  if (*(char *)((long)param_2 + 0x5f) < '\0') {
    FUN_00054420(param_1 + 9,param_2[9],param_2[10]);
  }
  else {
    uVar9 = param_2[10];
    uVar5 = param_2[9];
    param_1[0xb] = param_2[0xb];
    param_1[10] = uVar9;
    param_1[9] = uVar5;
  }
  uVar5 = param_2[0xc];
  param_1[0xd] = 0;
  param_1[0xc] = uVar5;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  uVar7 = param_2[0xe];
  if (uVar7 != 0) {
    if (0x492492492492492 < uVar7) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    lVar8 = param_2[0xd];
    lVar2 = uVar7 * 0x38;
    puVar4 = (undefined8 *)FUN_00008790(lVar2);
    param_1[0xd] = puVar4;
    param_1[0xe] = 0;
    param_1[0xf] = uVar7;
    puVar6 = puVar4;
    if (lVar2 != 0) {
      puVar4 = (undefined8 *)(lVar8 + 0x20);
      do {
        if (puVar6 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x4335878);
          (*pcVar3)();
        }
        *puVar6 = puVar4[-4];
        uVar9 = puVar4[-2];
        uVar5 = puVar4[-3];
        puVar6[3] = puVar4[-1];
        puVar6[2] = uVar9;
        puVar6[1] = uVar5;
        if (*(char *)((long)puVar4 + 0x17) < '\0') {
          FUN_00054420(puVar6 + 4,*puVar4,puVar4[1]);
        }
        else {
          uVar9 = puVar4[1];
          uVar5 = *puVar4;
          puVar6[6] = puVar4[2];
          puVar6[5] = uVar9;
          puVar6[4] = uVar5;
        }
        puVar6 = puVar6 + 7;
        puVar1 = puVar4 + 3;
        puVar4 = puVar4 + 7;
      } while (puVar1 != (undefined8 *)(lVar8 + lVar2));
      puVar4 = (undefined8 *)param_1[0xd];
    }
    param_1[0xe] = ((long)puVar6 - (long)puVar4 >> 3) * 0x6db6db6db6db6db7;
  }
  return param_1;
}


