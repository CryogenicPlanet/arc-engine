// ==== 0x4334d2c  FUN_04334d2c  (496 bytes)

undefined8 * FUN_04334d2c(undefined8 *param_1)

{
  undefined8 *puVar1;
  code *pcVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  undefined8 *puVar7;
  
  if (*(char *)((long)param_1 + 0xcf) < '\0') {
    FUN_00005810(param_1[0x17],param_1[0x19] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0xb7) < '\0') {
    FUN_00005810(param_1[0x14],param_1[0x16] & 0x7fffffffffffffff);
  }
  param_1[0xf] = &PTR_FUN_0da7eed0;
  plVar6 = (long *)param_1[0x11];
  if (plVar6 != (long *)0x0) {
    LOAcquire();
    lVar3 = plVar6[1];
    plVar6[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*plVar6 + 0x10))(plVar6);
      if (plVar6[2] != 0) {
        LOAcquire();
        lVar3 = plVar6[2];
        plVar6[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_04334d94;
      }
      pcVar2 = *(code **)(*plVar6 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar6,0x20);
      }
      else {
        (*pcVar2)(plVar6);
      }
    }
  }
LAB_04334d94:
  lVar3 = param_1[0xc];
  if (lVar3 != 0) {
    lVar5 = param_1[0xd] * 0x18;
    lVar4 = lVar3;
    if (lVar5 != 0) {
      lVar4 = param_1[0xd] * -0x18;
      puVar1 = (undefined8 *)(lVar5 + lVar3);
      do {
        puVar7 = puVar1 + -3;
        if (puVar7 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x4334f1c);
          (*pcVar2)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar7,puVar1[-1] << 1);
        }
        lVar4 = lVar4 + 0x18;
        puVar1 = puVar7;
      } while (lVar4 != 0);
      lVar4 = param_1[0xc];
    }
    param_1[0xd] = (lVar3 - lVar4 >> 3) * -0x5555555555555555;
    FUN_00005810(lVar4,param_1[0xe] * 0x18);
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


