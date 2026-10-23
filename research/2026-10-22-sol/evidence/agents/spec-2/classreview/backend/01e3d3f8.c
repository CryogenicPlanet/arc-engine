// ==== 0x1e3d3f8  FUN_01e3d3f8  (436 bytes)

ulong FUN_01e3d3f8(long param_1,long *param_2,int param_3)

{
  long *plVar1;
  code *pcVar2;
  bool bVar3;
  int iVar4;
  uint uVar5;
  long *plVar6;
  int *piVar7;
  undefined8 uVar8;
  ulong uVar9;
  long *plVar10;
  long lVar11;
  long *plVar12;
  
  plVar6 = param_2;
  if (param_3 == 0) {
    plVar6 = (long *)0x0;
  }
  iVar4 = _pthread_mutex_trylock((pthread_mutex_t *)(param_1 + 0x30));
  if (iVar4 != 0) {
    FUN_002b39c8(param_1 + 0x30,&PTR_s_BaseLock_0df6cbd8);
  }
  plVar12 = *(long **)(param_1 + 8);
  while( true ) {
    if (plVar12 == (long *)(param_1 + 0x10)) {
      uVar5 = _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x30));
      if (plVar6 != (long *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x01e3d5a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar9 = (**(code **)(*plVar6 + 8))(param_2);
        return uVar9;
      }
      return (ulong)uVar5;
    }
    lVar11 = plVar12[4];
    if (*(long **)(lVar11 + 0x10) == param_2) break;
    plVar1 = (long *)plVar12[1];
    plVar10 = plVar12;
    if ((long *)plVar12[1] == (long *)0x0) {
      do {
        plVar12 = (long *)plVar10[2];
        bVar3 = plVar10 != (long *)*plVar12;
        plVar10 = plVar12;
      } while (bVar3);
    }
    else {
      do {
        plVar12 = plVar1;
        plVar1 = (long *)*plVar12;
      } while ((long *)*plVar12 != (long *)0x0);
    }
  }
  if (param_3 != 0) {
    plVar6 = *(long **)(lVar11 + 8);
    *(long **)(lVar11 + 8) = param_2;
    if (plVar6 != (long *)0x0) {
      (**(code **)(*plVar6 + 8))();
    }
  }
  *(undefined1 *)(plVar12[4] + 0x40) = 1;
  plVar6 = plVar12;
  plVar1 = (long *)plVar12[1];
  if ((long *)plVar12[1] == (long *)0x0) {
    do {
      plVar10 = (long *)plVar6[2];
      bVar3 = plVar6 != (long *)*plVar10;
      plVar6 = plVar10;
    } while (bVar3);
  }
  else {
    do {
      plVar10 = plVar1;
      plVar1 = (long *)*plVar10;
    } while ((long *)*plVar10 != (long *)0x0);
  }
  if (*(long **)(param_1 + 8) == plVar12) {
    *(long **)(param_1 + 8) = plVar10;
  }
  *(long *)(param_1 + 0x18) = *(long *)(param_1 + 0x18) + -1;
  FUN_0011d638(*(undefined8 *)(param_1 + 0x10),plVar12);
  if (plVar12 + 4 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x1e3d57c);
    (*pcVar2)();
  }
  piVar7 = (int *)plVar12[4];
  if (piVar7 != (int *)0x0) {
    LOAcquire();
    iVar4 = *piVar7;
    *piVar7 = iVar4 + -1;
    LORelease();
    if (iVar4 == 1) {
      uVar8 = FUN_01e3d5ac();
      FUN_00005810(uVar8,0x48);
    }
  }
  FUN_00005810(plVar12,0x28);
  uVar5 = _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x30));
  return (ulong)uVar5;
}


