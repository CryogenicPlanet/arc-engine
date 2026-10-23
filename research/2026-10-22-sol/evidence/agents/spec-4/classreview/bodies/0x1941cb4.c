// ==== 0x1941cb4  FUN_01941cb4  (120 bytes)

undefined8 * FUN_01941cb4(undefined8 *param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)*param_1;
  if (piVar2 != (int *)0x0) {
    *param_1 = 0;
    (**(code **)(piVar2 + 2))(piVar2);
    LOAcquire();
    iVar1 = *piVar2;
    *piVar2 = iVar1 + -1;
    LORelease();
    if (iVar1 == 1) {
      (**(code **)(piVar2 + 4))(piVar2);
    }
    piVar2 = (int *)*param_1;
    if (piVar2 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar2;
      *piVar2 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        (**(code **)(piVar2 + 4))();
      }
    }
  }
  return param_1;
}


