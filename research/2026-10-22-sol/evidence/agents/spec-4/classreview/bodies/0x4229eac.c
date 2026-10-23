// ==== 0x4229eac  FUN_04229eac  (456 bytes)

void FUN_04229eac(long param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  int *piVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 local_48 [2];
  undefined7 local_38;
  char cStack_31;
  
  piVar5 = *(int **)(param_1 + 0x158);
  if (piVar5 != (int *)0x0) {
    if (*(long *)(param_1 + 0x160) != 0) {
      FUN_00217a74(local_48,&PTR_s_TabSharingUIViews_StopSharing_0df55878,1);
      piVar5 = *(int **)(param_1 + 0x160);
      if (piVar5 == (int *)0x0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x422a058);
        (*pcVar1)();
      }
      iVar2 = *piVar5;
      *piVar5 = iVar2 + 1;
      if (iVar2 < 1) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x422a070);
        (*pcVar1)();
      }
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x422a064);
        (*pcVar1)();
      }
      (**(code **)(*(long *)(param_1 + 0x160) + 8))(piVar5,local_48);
      LOAcquire();
      iVar2 = *piVar5;
      *piVar5 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar5 + 4))(piVar5);
      }
      if (cStack_31 < '\0') {
        FUN_00005810(local_48[0],CONCAT17(cStack_31,local_38) & 0x7fffffffffffffff);
      }
      piVar5 = *(int **)(param_1 + 0x158);
      if (piVar5 == (int *)0x0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x422a07c);
        (*pcVar1)();
      }
    }
    *(undefined8 *)(param_1 + 0x158) = 0;
    (**(code **)(piVar5 + 2))(piVar5);
    LOAcquire();
    iVar2 = *piVar5;
    *piVar5 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      (**(code **)(piVar5 + 4))(piVar5);
    }
  }
  uVar6 = *(ulong *)(param_1 + 0x180);
  if (uVar6 != 0) {
    *(undefined8 *)(param_1 + 0x180) = 0;
    FUN_01fbf654(uVar6,*(undefined8 *)(param_1 + 0x178));
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar6);
    }
  }
  while (*(long *)(param_1 + 0x40) != 0) {
    FUN_01e7c404(*(undefined8 *)(*(long *)(param_1 + 0x30) + 0x20),param_1 + 0x28);
  }
  FUN_018f0070(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),local_48,FUN_0422a520,0,0);
  puVar7 = (undefined8 *)(param_1 + 0x80);
  FUN_07e0af40(*puVar7);
  *puVar7 = 0;
  *(undefined8 *)(param_1 + 0x88) = 0;
  *(undefined8 **)(param_1 + 0x78) = puVar7;
  lVar3 = *(long *)(param_1 + 0x148);
  *(undefined8 *)(param_1 + 0x148) = 0;
  if (lVar3 != 0) {
    uVar4 = FUN_071054a4();
    FUN_00005810(uVar4,0x2b0);
  }
  if ((*(ulong *)(param_1 + 0x138) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_000270f0();
  }
  *(undefined8 *)(param_1 + 0x138) = 0;
  return;
}


