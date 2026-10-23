// ==== 0x4323710  FUN_04323710  (268 bytes)

void FUN_04323710(long param_1)

{
  int iVar1;
  int *piVar2;
  long lVar3;
  undefined8 uVar4;
  long *plVar5;
  long lVar6;
  ulong local_28;
  
  plVar5 = *(long **)(param_1 + 0x20);
  if (plVar5 != (long *)0x0) {
    *(undefined1 *)(plVar5 + 0x26) = 1;
    if (plVar5[0xe] != 0) {
      FUN_041e4780();
    }
    local_28 = FUN_04b37104(5,"Import was cancelled 2.",0x17,0x2a,
                            "../../chrome/browser/importer/external_process_importer_host.cc");
    (**(code **)(*plVar5 + 0xd8))(plVar5,&local_28);
    if ((local_28 & 1) == 0) {
      FUN_04b35898();
    }
    if ((*(ulong *)(param_1 + 0x20) & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    *(undefined8 *)(param_1 + 0x20) = 0;
    *(undefined1 *)(param_1 + 0x28) = 0;
  }
  lVar6 = *(long *)(param_1 + 0x140);
  *(undefined8 *)(param_1 + 0x140) = 0;
  if (lVar6 != 0) {
    piVar2 = *(int **)(lVar6 + 0x30);
    if (piVar2 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar2;
      *piVar2 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        (**(code **)(piVar2 + 4))();
      }
    }
    lVar3 = *(long *)(lVar6 + 0x28);
    *(undefined8 *)(lVar6 + 0x28) = 0;
    if (lVar3 != 0) {
      FUN_00005810(lVar3,0x10);
    }
    uVar4 = FUN_00111f30(lVar6);
    FUN_00005810(uVar4,0x38);
    return;
  }
  return;
}


