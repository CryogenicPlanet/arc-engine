// ==== 0x1b92718  FUN_01b92718  (228 bytes)

undefined8 * FUN_01b92718(undefined8 *param_1,undefined8 *param_2)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  int *piVar4;
  
  if (param_1 != param_2) {
    piVar4 = (int *)*param_1;
    if (piVar4 != (int *)0x0) {
      *param_1 = 0;
      (**(code **)(piVar4 + 2))(piVar4);
      LOAcquire();
      iVar1 = *piVar4;
      *piVar4 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        (**(code **)(piVar4 + 4))(piVar4);
      }
    }
    uVar3 = *param_2;
    *param_2 = 0;
    piVar4 = (int *)*param_1;
    *param_1 = uVar3;
    if (piVar4 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar4;
      *piVar4 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        if (*(code **)(piVar4 + 4) == FUN_039a66c0) {
          piVar2 = *(int **)(piVar4 + 0xc);
          if (piVar2 != (int *)0x0) {
            LOAcquire();
            iVar1 = *piVar2;
            *piVar2 = iVar1 + -1;
            LORelease();
            if (iVar1 == 1) {
              FUN_00005810(piVar2,8);
            }
          }
          FUN_00005810(piVar4,0x48);
        }
        else {
          (**(code **)(piVar4 + 4))(piVar4);
        }
      }
    }
  }
  return param_1;
}


