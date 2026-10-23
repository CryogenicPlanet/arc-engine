// ==== 0x42a44a8  FUN_042a44a8  (188 bytes)

undefined8 * FUN_042a44a8(undefined8 *param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = param_1[6];
  if (lVar2 != 0) {
    lVar4 = param_1[7] * 4;
    if (lVar4 != 0) {
      lVar3 = param_1[7] * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) goto LAB_042a4560;
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    param_1[7] = 0;
    FUN_00005810(lVar2,param_1[8] << 2);
  }
  lVar2 = param_1[3];
  if (lVar2 != 0) {
    lVar4 = param_1[4] * 4;
    if (lVar4 != 0) {
      lVar3 = param_1[4] * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) {
LAB_042a4560:
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x42a4564);
          (*pcVar1)();
        }
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    param_1[4] = 0;
    FUN_00005810(lVar2,param_1[5] << 2);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
  }
  return param_1;
}


