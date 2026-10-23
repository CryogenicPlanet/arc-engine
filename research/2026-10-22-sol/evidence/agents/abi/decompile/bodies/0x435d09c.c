// ==== 0x435d09c  FUN_0435d09c  (188 bytes)

void FUN_0435d09c(long param_1)

{
  long lVar1;
  int iVar2;
  int *piVar3;
  long *plVar4;
  code *pcVar5;
  long lVar6;
  
  if (param_1 == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x435d154);
    (*pcVar5)();
  }
  lVar6 = *(long *)(param_1 + 8);
  *(undefined8 *)(param_1 + 8) = 0;
  if (lVar6 != 0) {
    FUN_0435c74c(lVar6);
    *(undefined8 *)(lVar6 + 0x40) = 0;
    if (*(long *)(lVar6 + 0x38) != 0) {
      *(undefined1 *)(*(long *)(lVar6 + 0x38) + 4) = 1;
      piVar3 = *(int **)(lVar6 + 0x38);
      if (piVar3 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar3;
        *piVar3 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(piVar3,8);
        }
      }
    }
    plVar4 = *(long **)(lVar6 + 0x28);
    if (plVar4 != (long *)0x0) {
      lVar1 = plVar4[1];
      iVar2 = (int)lVar1 + -1;
      *(int *)(plVar4 + 1) = iVar2;
      if ((int)lVar1 == 0) {
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(0,0x435d158);
        (*pcVar5)();
      }
      if (iVar2 == 0) {
        (**(code **)(*plVar4 + 0xe8))();
      }
    }
    pcVar5 = *(code **)(*(long *)(lVar6 + 0x20) + 8);
    if (pcVar5 != (code *)0x0) {
      (*pcVar5)(*(undefined8 *)(lVar6 + 8));
    }
    FUN_00005810(lVar6,0x48);
    return;
  }
  return;
}


