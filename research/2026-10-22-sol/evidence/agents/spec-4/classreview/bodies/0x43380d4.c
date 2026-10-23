// ==== 0x43380d4  FUN_043380d4  (224 bytes)

long FUN_043380d4(long param_1,long param_2)

{
  long lVar1;
  code *pcVar2;
  long *plVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  uVar5 = *(undefined8 *)(param_2 + 0x10);
  uVar4 = *(undefined8 *)(param_2 + 8);
  if (*(long *)(param_2 + 0x10) != 0) {
    plVar3 = (long *)(*(long *)(param_2 + 0x10) + 8);
    *plVar3 = *plVar3 + 1;
  }
  plVar3 = *(long **)(param_1 + 0x10);
  *(undefined8 *)(param_1 + 0x10) = uVar5;
  *(undefined8 *)(param_1 + 8) = uVar4;
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


