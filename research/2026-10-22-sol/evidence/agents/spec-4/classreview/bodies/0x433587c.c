// ==== 0x433587c  FUN_0433587c  (332 bytes)

/* WARNING: Removing unreachable block (ram,0x043358b8) */
/* WARNING: Removing unreachable block (ram,0x043358d8) */

undefined8 * FUN_0433587c(undefined8 *param_1)

{
  long lVar1;
  code *pcVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar3 = param_1[0xd];
  if (lVar3 != 0) {
    lVar1 = param_1[0xe] * 0x38;
    lVar4 = lVar3;
    if (lVar1 != 0) {
      lVar4 = 0;
      lVar5 = lVar3;
      do {
        if (lVar5 + lVar1 == 0x38) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x43359c8);
          (*pcVar2)();
        }
        lVar5 = lVar5 + -0x38;
        lVar4 = lVar4 + 0x38;
      } while (lVar1 != lVar4);
      lVar4 = param_1[0xd];
    }
    param_1[0xe] = (lVar3 - lVar4 >> 3) * 0x6db6db6db6db6db7;
    FUN_00005810(lVar4,param_1[0xf] * 0x38);
  }
  if (*(char *)((long)param_1 + 0x5f) < '\0') {
    FUN_00005810(param_1[9],param_1[0xb] << 1);
  }
  if (*(char *)((long)param_1 + 0x47) < '\0') {
    FUN_00005810(param_1[6],param_1[8] << 1);
  }
  if (*(char *)((long)param_1 + 0x2f) < '\0') {
    FUN_00005810(param_1[3],param_1[5] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
  }
  return param_1;
}


