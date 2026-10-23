// ==== 0x42aaaa0  FUN_042aaaa0  (396 bytes)

void FUN_042aaaa0(long param_1,undefined8 *param_2)

{
  code *pcVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  int *piVar6;
  long lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  
  puVar3 = (undefined4 *)FUN_00008790(0x28);
  *puVar3 = 1;
  *(code **)(puVar3 + 2) = FUN_042ab710;
  *(code **)(puVar3 + 4) = FUN_00b6d3c0;
  *(code **)(puVar3 + 6) = FUN_00b77080;
  puVar4 = (undefined4 *)FUN_00008790(0x48);
  *puVar4 = 1;
  *(code **)(puVar4 + 2) = FUN_042ab844;
  *(code **)(puVar4 + 4) = FUN_041a82e0;
  *(code **)(puVar4 + 6) = FUN_00b77080;
  uVar9 = *param_2;
  uVar11 = param_2[3];
  uVar10 = param_2[2];
  *(undefined8 *)(puVar4 + 0xc) = param_2[1];
  *(undefined8 *)(puVar4 + 10) = uVar9;
  lVar7 = param_2[3];
  *(undefined8 *)(puVar4 + 0x10) = uVar11;
  *(undefined8 *)(puVar4 + 0xe) = uVar10;
  if (*(long *)(lVar7 + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  puVar5 = (undefined8 *)FUN_00008790(0x20);
  *puVar5 = puVar3;
  puVar5[1] = puVar4;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar2 = *piVar6, *piVar6 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42aac2c);
    (*pcVar1)();
  }
  puVar5[2] = piVar6;
  puVar5[3] = puVar5;
  puVar8 = *(undefined8 **)(param_1 + 0x30);
  *(undefined8 **)(param_1 + 0x30) = puVar5;
  if (puVar8 != (undefined8 *)0x0) {
    puVar8[3] = 0;
    if (puVar8[2] != 0) {
      *(undefined1 *)(puVar8[2] + 4) = 1;
      piVar6 = (int *)puVar8[2];
      if (piVar6 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar6;
        *piVar6 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(piVar6,8);
        }
      }
    }
    piVar6 = (int *)puVar8[1];
    if (piVar6 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar6;
      *piVar6 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar6 + 4))();
      }
    }
    piVar6 = (int *)*puVar8;
    if (piVar6 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar6;
      *piVar6 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar6 + 4))();
      }
    }
    FUN_00005810(puVar8,0x20);
    puVar5 = *(undefined8 **)(param_1 + 0x30);
  }
  FUN_083696c0(puVar5);
  return;
}


