// ==== 0x42e3140  FUN_042e3140  (784 bytes)

void FUN_042e3140(long param_1,int param_2,undefined8 param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 *puVar3;
  int *piVar4;
  undefined8 *puVar5;
  long *plVar6;
  undefined4 *puVar7;
  long lVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  long lVar11;
  undefined8 *puVar12;
  long lVar13;
  undefined1 auStack_70 [32];
  
  lVar11 = *(long *)(param_1 + 0x30);
  if (lVar11 != 0) {
    puVar3 = (undefined8 *)FUN_000805e0(lVar11,&DAT_0c9d67c8);
    if (puVar3 == (undefined8 *)0x0) {
      puVar3 = (undefined8 *)FUN_00008790(0x50);
      puVar3[3] = 0;
      puVar3[2] = 0;
      puVar12 = puVar3 + 4;
      puVar3[5] = 0;
      *puVar12 = 0;
      puVar3[7] = 0;
      puVar3[6] = 0;
      puVar3[9] = 0;
      puVar3[8] = 0;
      *puVar3 = &PTR_FUN_0da7cb20;
      puVar3[1] = 0;
      *(undefined4 *)(puVar3 + 2) = 1;
      *puVar12 = 0;
      puVar3[5] = 0;
      puVar3[6] = 0;
      puVar3[3] = puVar12;
      puVar3[7] = 0;
      piVar4 = (int *)FUN_00008790(8);
      *piVar4 = 0;
      *(undefined1 *)(piVar4 + 1) = 0;
      if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42e3460);
        (*pcVar1)();
      }
      puVar3[8] = piVar4;
      puVar3[9] = puVar3;
      FUN_011db6c0(lVar11,&DAT_0c9d67c8,puVar3);
    }
    puVar12 = (undefined8 *)puVar3[1];
    if (puVar12 == (undefined8 *)0x0) {
      puVar12 = (undefined8 *)FUN_00008790(0x18);
      puVar12[1] = 0;
      puVar12[2] = 0;
      *puVar12 = 0;
      plVar6 = (long *)puVar3[1];
      if (plVar6 == (long *)0x0) {
        puVar3[1] = puVar12;
      }
      else {
        lVar11 = *plVar6;
        if (lVar11 != 0) {
          lVar8 = plVar6[1] * 0x20;
          lVar13 = lVar11;
          if (lVar8 != 0) {
            lVar13 = plVar6[1] * -0x20;
            lVar8 = lVar8 + lVar11;
            do {
              lVar8 = lVar8 + -0x20;
              FUN_044294f4(lVar8);
              lVar13 = lVar13 + 0x20;
            } while (lVar13 != 0);
            lVar13 = *plVar6;
          }
          plVar6[1] = lVar11 - lVar13 >> 5;
          FUN_00005810(lVar13,plVar6[2] << 5);
        }
        FUN_00005810(plVar6,0x18);
        plVar6 = (long *)puVar3[1];
        puVar3[1] = puVar12;
        if (plVar6 != (long *)0x0) {
          lVar11 = *plVar6;
          if (lVar11 != 0) {
            lVar8 = plVar6[1] * 0x20;
            lVar13 = lVar11;
            if (lVar8 != 0) {
              lVar13 = plVar6[1] * -0x20;
              lVar8 = lVar8 + lVar11;
              do {
                lVar8 = lVar8 + -0x20;
                FUN_044294f4(lVar8);
                lVar13 = lVar13 + 0x20;
              } while (lVar13 != 0);
              lVar13 = *plVar6;
            }
            plVar6[1] = lVar11 - lVar13 >> 5;
            FUN_00005810(lVar13,plVar6[2] << 5);
          }
          FUN_00005810(plVar6,0x18);
          puVar12 = (undefined8 *)puVar3[1];
        }
      }
    }
    puVar12 = (undefined8 *)FUN_042e29c0(puVar12,param_3);
    puVar5 = (undefined8 *)*puVar12;
    if (puVar5 == (undefined8 *)0x0) {
      puVar5 = (undefined8 *)FUN_00008790(0x58);
      *(undefined1 *)(puVar5 + 9) = 0;
      *(undefined1 *)(puVar5 + 10) = 0;
      *(undefined1 *)((long)puVar5 + 0x54) = 0;
      puVar5[1] = 0;
      *puVar5 = 0;
      puVar5[3] = 0;
      puVar5[2] = 0;
      puVar5[5] = 0;
      puVar5[4] = 0;
      *(undefined1 *)(puVar5 + 6) = 0;
      *puVar12 = puVar5;
    }
    uVar9 = 1;
    if (param_2 != 0) {
      if (param_2 != 1) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x42e3444);
        (*pcVar1)();
      }
      uVar9 = 0;
    }
    *(undefined4 *)(puVar5 + 10) = uVar9;
    *(undefined1 *)((long)puVar5 + 0x54) = 1;
    plVar6 = (long *)(*(code *)PTR___tlv_bootstrap_0e379840)();
    if (((undefined8 *)*plVar6 == (undefined8 *)0x0) ||
       (puVar12 = *(undefined8 **)*plVar6, puVar12 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x42e3448);
      (*pcVar1)();
    }
    FUN_00029e40(auStack_70,"NotifyBoostConfigChanged",
                 "../../arc/browser/browser_context/effects_impl.cc",0x2d);
    piVar4 = (int *)puVar3[8];
    if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x42e3454);
      (*pcVar1)();
    }
    uVar10 = puVar3[9];
    puVar7 = (undefined4 *)FUN_00008790(0x40);
    *puVar7 = 1;
    *(undefined1 **)(puVar7 + 2) = &LAB_040c5ad0;
    *(code **)(puVar7 + 4) = FUN_040a6300;
    *(undefined1 **)(puVar7 + 6) = &LAB_04169bc0;
    *(code **)(puVar7 + 8) = FUN_042e47c4;
    *(undefined8 *)(puVar7 + 10) = 0;
    *(int **)(puVar7 + 0xc) = piVar4;
    *(undefined8 *)(puVar7 + 0xe) = uVar10;
    (**(code **)*puVar12)(puVar12,auStack_70,puVar7,0);
  }
  return;
}


