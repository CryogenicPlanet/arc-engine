// ==== 0x4331e20  FUN_04331e20  (224 bytes)

undefined8 * FUN_04331e20(undefined8 *param_1,undefined8 *param_2)

{
  long lVar1;
  code *pcVar2;
  long *plVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  uVar5 = param_2[1];
  uVar4 = *param_2;
  if (param_2[1] != 0) {
    plVar3 = (long *)(param_2[1] + 8);
    *plVar3 = *plVar3 + 1;
  }
  plVar3 = (long *)param_1[1];
  param_1[1] = uVar5;
  *param_1 = uVar4;
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
        if (lVar1 != 0) {
          return param_1;
        }
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
  return param_1;
}


