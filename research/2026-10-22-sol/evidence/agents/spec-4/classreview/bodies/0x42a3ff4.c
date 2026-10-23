// ==== 0x42a3ff4  FUN_042a3ff4  (220 bytes)

undefined8 * FUN_042a3ff4(undefined8 *param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = param_1[0x12];
  if (lVar2 != 0) {
    lVar4 = param_1[0x13] * 4;
    if (lVar4 != 0) {
      lVar3 = param_1[0x13] * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x42a40d0);
          (*pcVar1)();
        }
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    param_1[0x13] = 0;
    FUN_00005810(lVar2,param_1[0x14] << 2);
  }
  if (*(char *)((long)param_1 + 0x77) < '\0') {
    FUN_00005810(param_1[0xc],param_1[0xe] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x5f) < '\0') {
    FUN_00005810(param_1[9],param_1[0xb] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x47) < '\0') {
    FUN_00005810(param_1[6],param_1[8] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x2f) < '\0') {
    FUN_00005810(param_1[3],param_1[5] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
  }
  return param_1;
}


