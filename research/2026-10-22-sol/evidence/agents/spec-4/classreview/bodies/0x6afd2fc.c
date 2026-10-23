// ==== 0x6afd2fc  FUN_06afd2fc  (352 bytes)

void FUN_06afd2fc(long param_1,undefined8 param_2)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  int *piVar5;
  undefined8 uVar6;
  long lVar7;
  long *plVar8;
  int *local_38;
  
  for (plVar8 = *(long **)(param_1 + 0xe0); plVar8 != (long *)0x0; plVar8 = (long *)*plVar8) {
    uVar3 = FUN_00dbc280(param_2,plVar8 + 4);
    if ((uVar3 >> 7 & 1) == 0) {
      if ((char)uVar3 == '\0') {
        return;
      }
      plVar8 = plVar8 + 1;
    }
  }
  uVar4 = FUN_06afd464(param_1,param_2);
  if ((((uVar4 & 1) == 0) &&
      (iVar2 = FUN_02b25980(param_2,*(undefined8 *)(param_1 + 0x88)), iVar2 != 1)) &&
     (uVar4 = FUN_00080ffc(*(long *)(param_1 + 0x90) + 0x68,param_2), (uVar4 & 1) == 0)) {
    piVar5 = (int *)FUN_0194a118(*(long *)(param_1 + 0x90) + 0x20,param_2);
    if (piVar5 == (int *)0x0) {
      FUN_06af8464(*(undefined8 *)(param_1 + 0x88),param_2);
      return;
    }
    iVar2 = FUN_07811d58(*(undefined8 *)(param_1 + 0x40),piVar5);
    if (iVar2 != 0) {
      FUN_06af8464(*(undefined8 *)(param_1 + 0x88),param_2);
      lVar7 = *(long *)(param_1 + 0x90);
      iVar2 = *piVar5;
      *piVar5 = iVar2 + 1;
      local_38 = piVar5;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x6afd45c);
        (*pcVar1)();
      }
      FUN_01fcf068(lVar7 + 8,&local_38);
      if (local_38 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_38;
        *local_38 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          uVar6 = thunk_FUN_02d613c8();
          FUN_00005810(uVar6,0x268);
        }
      }
      FUN_06aff854(*(undefined8 *)(param_1 + 0x90),*(undefined8 *)(piVar5 + 0x8a));
      FUN_03020f64(param_1,piVar5,0);
      FUN_06aff4ec(*(undefined8 *)(param_1 + 0x90),piVar5);
    }
  }
  return;
}


