// ==== 0x43334c8  FUN_043334c8  (208 bytes)

undefined8 * FUN_043334c8(undefined8 *param_1,undefined8 param_2,long *param_3)

{
  long *plVar1;
  long lVar2;
  code *pcVar3;
  
  *param_1 = param_2;
  param_1[1] = param_3;
  if (param_3 != (long *)0x0) {
    plVar1 = param_3 + 1;
    *plVar1 = *plVar1 + 1;
    LOAcquire();
    lVar2 = *plVar1;
    *plVar1 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*param_3 + 0x10))(param_3);
      if (param_3[2] != 0) {
        LOAcquire();
        lVar2 = param_3[2];
        param_3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return param_1;
        }
      }
      pcVar3 = *(code **)(*param_3 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(param_3,0x20);
      }
      else {
        (*pcVar3)(param_3);
      }
    }
  }
  return param_1;
}


