// ==== 0x435c6f4  FUN_0435c6f4  (80 bytes)

void FUN_0435c6f4(long param_1)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  long *plVar4;
  
  plVar4 = *(long **)(param_1 + 0x28);
  *(undefined8 *)(param_1 + 0x28) = 0;
  if (plVar4 != (long *)0x0) {
    lVar2 = plVar4[1];
    iVar1 = (int)lVar2 + -1;
    *(int *)(plVar4 + 1) = iVar1;
    if ((int)lVar2 == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x435c744);
      (*pcVar3)();
    }
    if (iVar1 == 0) {
      (**(code **)(*plVar4 + 0xe8))();
    }
  }
  FUN_0435c74c(param_1);
  return;
}


