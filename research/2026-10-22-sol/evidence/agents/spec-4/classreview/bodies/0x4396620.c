// ==== 0x4396620  FUN_04396620  (380 bytes)

void FUN_04396620(long param_1)

{
  int **ppiVar1;
  int iVar2;
  long *plVar3;
  int *piVar4;
  int *piVar5;
  long lVar6;
  undefined8 local_40;
  int *local_38;
  
  lVar6 = *(long *)(param_1 + 0x130);
  plVar3 = *(long **)(lVar6 + 0xe8);
  local_40 = 0;
  if (plVar3 != (long *)0x0) {
    local_40 = (**(code **)(*plVar3 + 0x70))();
  }
  FUN_0415c9c0(&local_38,lVar6,&local_40,0,0,1);
  ppiVar1 = (int **)(param_1 + 0x5f8);
  if (ppiVar1 != &local_38) {
    piVar4 = *ppiVar1;
    if (piVar4 == (int *)0x0) {
      *ppiVar1 = local_38;
      return;
    }
    *ppiVar1 = (int *)0x0;
    (**(code **)(piVar4 + 2))(piVar4);
    LOAcquire();
    iVar2 = *piVar4;
    *piVar4 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      (**(code **)(piVar4 + 4))(piVar4);
    }
    piVar4 = local_38;
    piVar5 = *ppiVar1;
    local_38 = (int *)0x0;
    *ppiVar1 = piVar4;
    if (piVar5 == (int *)0x0) {
      return;
    }
    LOAcquire();
    iVar2 = *piVar5;
    *piVar5 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      if (*(code **)(piVar5 + 4) == FUN_039a66c0) {
        piVar4 = *(int **)(piVar5 + 0xc);
        if (piVar4 != (int *)0x0) {
          LOAcquire();
          iVar2 = *piVar4;
          *piVar4 = iVar2 + -1;
          LORelease();
          if (iVar2 == 1) {
            FUN_00005810(piVar4,8);
          }
        }
        FUN_00005810(piVar5,0x48);
      }
      else {
        (**(code **)(piVar5 + 4))(piVar5);
      }
    }
  }
  piVar4 = local_38;
  if (local_38 != (int *)0x0) {
    local_38 = (int *)0x0;
    (**(code **)(piVar4 + 2))(piVar4);
    LOAcquire();
    iVar2 = *piVar4;
    *piVar4 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      (**(code **)(piVar4 + 4))(piVar4);
    }
    if (local_38 != (int *)0x0) {
      LOAcquire();
      iVar2 = *local_38;
      *local_38 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(local_38 + 4))();
      }
    }
  }
  return;
}


