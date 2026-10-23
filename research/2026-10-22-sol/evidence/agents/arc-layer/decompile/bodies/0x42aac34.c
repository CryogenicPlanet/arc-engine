// ==== 0x42aac34  FUN_042aac34  (560 bytes)

undefined8 FUN_042aac34(long param_1)

{
  long lVar1;
  int iVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  int *piVar5;
  code *pcVar6;
  undefined4 *puVar7;
  int *piVar8;
  undefined1 auStack_80 [32];
  ulong local_60 [2];
  long *local_50;
  int *local_48;
  undefined8 local_40;
  
  local_40 = 0;
  local_60[1] = 0;
  local_60[0] = 0;
  local_48 = (int *)0x0;
  local_50 = (long *)0x0;
  FUN_01f19054(local_60,0);
  puVar3 = (undefined8 *)FUN_00008790(8);
  *puVar3 = 0;
  uVar4 = FUN_02736300();
  FUN_02736440(uVar4,0,0);
  puVar7 = *(undefined4 **)(param_1 + 8);
  *(undefined8 **)(param_1 + 8) = puVar3;
  if (puVar7 != (undefined4 *)0x0) {
    uVar4 = FUN_02736300();
    FUN_02dd6dc0(uVar4,*puVar7,puVar7[1]);
    FUN_00005810(puVar7,8);
  }
  lVar1 = DAT_0e481618;
  piVar8 = *(int **)(DAT_0e481618 + 0x1f0);
  *(undefined8 *)(DAT_0e481618 + 0x1f0) = 0;
  if (piVar8 != (int *)0x0) {
    LOAcquire();
    iVar2 = *piVar8;
    *piVar8 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      if (*(code **)(piVar8 + 4) == FUN_039a6ef8) {
        piVar5 = *(int **)(piVar8 + 10);
        if (piVar5 != (int *)0x0) {
          LOAcquire();
          iVar2 = *piVar5;
          *piVar5 = iVar2 + -1;
          LORelease();
          if (iVar2 == 1) {
            (**(code **)(piVar5 + 4))();
          }
        }
        FUN_00005810(piVar8,0x30);
      }
      else {
        (**(code **)(piVar8 + 4))(piVar8);
      }
    }
  }
  uVar4 = FUN_02fa43fc(local_60);
  piVar8 = *(int **)(lVar1 + 0x1f0);
  *(undefined8 *)(lVar1 + 0x1f0) = uVar4;
  if (piVar8 != (int *)0x0) {
    LOAcquire();
    iVar2 = *piVar8;
    *piVar8 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      if (*(code **)(piVar8 + 4) == FUN_039a66c0) {
        piVar5 = *(int **)(piVar8 + 0xc);
        if (piVar5 != (int *)0x0) {
          LOAcquire();
          iVar2 = *piVar5;
          *piVar5 = iVar2 + -1;
          LORelease();
          if (iVar2 == 1) {
            FUN_00005810(piVar5,8);
          }
        }
        FUN_00005810(piVar8,0x48);
      }
      else {
        (**(code **)(piVar8 + 4))(piVar8);
      }
    }
  }
  FUN_00029e40(auStack_80,"RunMainLoop","../../arc/browser/arc_browser_runner.cc",0xf1);
  FUN_01f197e8(local_60,auStack_80);
  local_40 = 0;
  if (local_48 != (int *)0x0) {
    *(undefined1 *)(local_48 + 1) = 1;
    if (local_48 != (int *)0x0) {
      LOAcquire();
      iVar2 = *local_48;
      *local_48 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00005810(local_48,8);
      }
    }
  }
  if (local_50 != (long *)0x0) {
    LOAcquire();
    iVar2 = (int)local_50[1];
    *(int *)(local_50 + 1) = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      pcVar6 = *(code **)(*local_50 + 0x18);
      if (pcVar6 == (code *)&LAB_044511c4) {
        pcVar6 = *(code **)(*local_50 + 0x10);
      }
      (*pcVar6)();
    }
  }
  if ((local_60[0] & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_000270f0();
  }
  return 0;
}


