// ==== 0x436d69c  FUN_0436d69c  (584 bytes)

void FUN_0436d69c(long param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined4 *puVar6;
  int *piVar7;
  long lVar8;
  int *piVar9;
  
  if (((*(long *)(param_1 + 0x18) != 0) && (lVar3 = FUN_018efea8(), lVar3 != 0)) &&
     (lVar3 = FUN_004969fc(lVar3 + 0x310,&PTR_s_ThumbnailTabHelper_kDataKeyImpl_0df14bf0),
     lVar3 != 0)) {
    if (*(long *)(param_1 + 0x40) == 0) {
      if (*(long *)(lVar3 + 0x68) == 0) {
        return;
      }
      piVar9 = (int *)(*(long *)(lVar3 + 0x68) + 8);
      iVar2 = *piVar9;
      *piVar9 = iVar2 + 1;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x436d8e4);
        (*pcVar1)();
      }
      LOAcquire();
      iVar2 = *piVar9;
      *piVar9 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        uVar4 = FUN_02d5fc08();
        FUN_00005810(uVar4,0x60);
      }
      lVar8 = *(long *)(lVar3 + 0x68);
      if ((lVar8 != 0) &&
         (iVar2 = *(int *)(lVar8 + 8), *(int *)(lVar8 + 8) = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x436d8fc);
        (*pcVar1)();
      }
      uVar4 = FUN_0731be94(lVar8);
      lVar5 = *(long *)(param_1 + 0x40);
      *(undefined8 *)(param_1 + 0x40) = uVar4;
      if (lVar5 != 0) {
        uVar4 = FUN_0731bd18();
        FUN_00005810(uVar4,0x28);
      }
      if (lVar8 != 0) {
        LOAcquire();
        iVar2 = *(int *)(lVar8 + 8);
        *(int *)(lVar8 + 8) = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          uVar4 = FUN_02d5fc08(lVar8);
          FUN_00005810(uVar4,0x60);
        }
      }
      lVar8 = *(long *)(param_1 + 0x40);
      piVar9 = *(int **)(param_1 + 0x90);
      if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x436d8f0);
        (*pcVar1)();
      }
      uVar4 = *(undefined8 *)(param_1 + 0x98);
      puVar6 = (undefined4 *)FUN_00008790(0x40);
      *puVar6 = 1;
      *(undefined1 **)(puVar6 + 2) = &LAB_0436e96c;
      *(code **)(puVar6 + 4) = FUN_040a6300;
      *(undefined1 **)(puVar6 + 6) = &LAB_04169bc0;
      *(undefined1 **)(puVar6 + 8) = &LAB_0436dff4;
      *(undefined8 *)(puVar6 + 10) = 0;
      *(int **)(puVar6 + 0xc) = piVar9;
      *(undefined8 *)(puVar6 + 0xe) = uVar4;
      piVar9 = *(int **)(lVar8 + 0x18);
      *(undefined4 **)(lVar8 + 0x18) = puVar6;
      if (piVar9 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar9;
        *piVar9 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          if (*(code **)(piVar9 + 4) == FUN_039a66c0) {
            piVar7 = *(int **)(piVar9 + 0xc);
            if (piVar7 != (int *)0x0) {
              LOAcquire();
              iVar2 = *piVar7;
              *piVar7 = iVar2 + -1;
              LORelease();
              if (iVar2 == 1) {
                FUN_00005810(piVar7,8);
              }
            }
            FUN_00005810(piVar9,0x48);
          }
          else {
            (**(code **)(piVar9 + 4))(piVar9);
          }
        }
      }
    }
    lVar3 = *(long *)(lVar3 + 0x68);
    if (lVar3 == 0) {
      FUN_0731c7a8(0);
      return;
    }
    piVar9 = (int *)(lVar3 + 8);
    iVar2 = *piVar9;
    *piVar9 = iVar2 + 1;
    if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x436d8d8);
      (*pcVar1)();
    }
    FUN_0731c7a8(lVar3);
    LOAcquire();
    iVar2 = *piVar9;
    *piVar9 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      uVar4 = FUN_02d5fc08(lVar3);
      FUN_00005810(uVar4,0x60);
      return;
    }
  }
  return;
}


