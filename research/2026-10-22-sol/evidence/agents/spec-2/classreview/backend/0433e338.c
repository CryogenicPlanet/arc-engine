// ==== 0x433e338  FUN_0433e338  (256 bytes)

undefined8 * FUN_0433e338(undefined8 *param_1)

{
  long lVar1;
  code *pcVar2;
  long *plVar3;
  
  if (*(char *)((long)param_1 + 0x47) < '\0') {
    FUN_00005810(param_1[6],param_1[8] << 1);
  }
  param_1[3] = &PTR_FUN_0da7eed0;
  plVar3 = (long *)param_1[5];
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0433e388;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_0433e388:
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] << 1);
  }
  return param_1;
}


