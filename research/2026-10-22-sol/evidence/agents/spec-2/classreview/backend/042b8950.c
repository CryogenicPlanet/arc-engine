// ==== 0x42b8950  FUN_042b8950  (284 bytes)

void FUN_042b8950(long param_1,undefined8 *param_2)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  code *pcVar7;
  undefined8 uVar8;
  int *piVar9;
  
  if (*(long *)(param_1 + 0x1a0) == *(long *)(param_1 + 0x1a8)) {
    puVar5 = (undefined8 *)FUN_042bfa68(param_1 + 0x198);
  }
  else {
    puVar5 = (undefined8 *)(*(long *)(param_1 + 0x198) + *(long *)(param_1 + 0x1a0) * 0x20);
    if (puVar5 == (undefined8 *)0x0) goto LAB_042b8a58;
    uVar3 = *param_2;
    puVar5[1] = param_2[1];
    *puVar5 = uVar3;
    puVar5[2] = param_2[2];
    puVar6 = (undefined8 *)param_2[3];
    puVar5[3] = puVar6;
    pcVar7 = (code *)*puVar6;
    if (pcVar7 != (code *)0x0) {
      uVar3 = (*pcVar7)(*param_2);
      *puVar5 = uVar3;
    }
    puVar5 = puVar5 + 4;
  }
  lVar1 = (long)puVar5 - *(long *)(param_1 + 0x198);
  *(long *)(param_1 + 0x1a0) = lVar1 >> 5;
  if (lVar1 != 0) {
    if ((*(byte *)(param_1 + 0x341) & 1) != 0) {
      return;
    }
    *(undefined1 *)(param_1 + 0x341) = 1;
    uVar3 = FUN_043210c8();
    piVar9 = *(int **)(param_1 + 0x468);
    if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x42b8a6c);
      (*pcVar7)();
    }
    uVar8 = *(undefined8 *)(param_1 + 0x470);
    puVar4 = (undefined4 *)FUN_00008790(0x40);
    *puVar4 = 1;
    *(undefined1 **)(puVar4 + 2) = &LAB_040c5ad0;
    *(code **)(puVar4 + 4) = FUN_040a6300;
    *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
    *(code **)(puVar4 + 8) = FUN_042b8a74;
    *(undefined8 *)(puVar4 + 10) = 0;
    *(int **)(puVar4 + 0xc) = piVar9;
    *(undefined8 *)(puVar4 + 0xe) = uVar8;
    FUN_04321384(uVar3,puVar4);
    return;
  }
LAB_042b8a58:
                    /* WARNING: Does not return */
  pcVar7 = (code *)SoftwareBreakpoint(1,0x42b8a5c);
  (*pcVar7)();
}


