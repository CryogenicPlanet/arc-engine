// ==== 0x42b5c28  FUN_042b5c28  (776 bytes)

void FUN_042b5c28(long param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)

{
  long *plVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined4 *puVar5;
  long lVar6;
  code *pcVar7;
  long *plVar8;
  int *piVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  ulong local_88;
  undefined8 local_80;
  long *plStack_78;
  undefined8 local_70;
  long *plStack_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  ulong local_48;
  
  uVar3 = FUN_01a29860(DAT_0e481618);
  lVar6 = (long)*(char *)((long)param_3 + 0x17);
  puVar4 = param_3;
  if (lVar6 < 0) {
    lVar6 = param_3[1];
    puVar4 = (undefined8 *)*param_3;
  }
  local_58 = 0;
  uStack_50 = 0;
  local_48 = 0;
  FUN_01c39a30(&local_58,puVar4,lVar6);
  local_70 = 0;
  plStack_68 = (long *)0x0;
  local_60 = 0;
  FUN_042b4410(&local_70,param_1,param_2,param_3);
  if ((char)local_60 == '\0') {
    piVar9 = *(int **)(param_1 + 0x468);
    if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x42b5f30);
      (*pcVar7)();
    }
    uVar10 = *(undefined8 *)(param_1 + 0x470);
    puVar5 = (undefined4 *)FUN_00008790(0x80);
    *puVar5 = 1;
    *(undefined1 **)(puVar5 + 2) = &LAB_042bf4cc;
    *(code **)(puVar5 + 4) = FUN_042bf628;
    *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
    *(code **)(puVar5 + 8) = FUN_042b5f38;
    *(undefined8 *)(puVar5 + 10) = 0;
    *(int **)(puVar5 + 0xc) = piVar9;
    *(undefined8 *)(puVar5 + 0xe) = uVar10;
    puVar5[0x10] = (int)param_2;
    if ((long)local_48 < 0) {
      FUN_00060a60(puVar5 + 0x12,local_58,uStack_50);
    }
    else {
      *(undefined8 *)(puVar5 + 0x14) = uStack_50;
      *(undefined8 *)(puVar5 + 0x12) = local_58;
      *(ulong *)(puVar5 + 0x16) = local_48;
    }
    lVar6 = param_4[3];
    uVar10 = *param_4;
    uVar12 = param_4[3];
    uVar11 = param_4[2];
    *(undefined8 *)(puVar5 + 0x1a) = param_4[1];
    *(undefined8 *)(puVar5 + 0x18) = uVar10;
    *(undefined8 *)(puVar5 + 0x1e) = uVar12;
    *(undefined8 *)(puVar5 + 0x1c) = uVar11;
    if (*(long *)(lVar6 + 8) != 0) {
      param_4[2] = 0;
      param_4[3] = &DAT_0c4b3500;
    }
    FUN_07bdb8c4(uVar3,&local_58,0,puVar5);
    goto LAB_042b5d80;
  }
  plStack_78 = plStack_68;
  local_80 = local_70;
  if (plStack_68 != (long *)0x0) {
    plStack_68[1] = plStack_68[1] + 1;
  }
  local_88 = 1;
  (*(code *)param_4[2])(param_4,&local_88);
  plVar1 = plStack_78;
  if (local_88 == 1) {
    if (plStack_78 != (long *)0x0) {
      LOAcquire();
      lVar6 = plStack_78[1];
      plStack_78[1] = lVar6 + -1;
      LORelease();
      if (lVar6 == 0) {
        plVar8 = plStack_78 + 2;
        (**(code **)(*plStack_78 + 0x10))(plStack_78);
        if (*plVar8 != 0) {
          LOAcquire();
          lVar6 = plVar1[2];
          plVar1[2] = lVar6 + -1;
          LORelease();
          if (lVar6 != 0) goto LAB_042b5d60;
        }
        pcVar7 = *(code **)(*plVar1 + 0x20);
        if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
          FUN_00005810(plVar1,0x20);
        }
        else {
          (*pcVar7)(plVar1);
        }
      }
    }
  }
  else if ((local_88 & 1) == 0) {
    FUN_04b35898();
  }
LAB_042b5d60:
  plVar1 = plStack_68;
  if (((char)local_60 != '\0') && (plStack_68 != (long *)0x0)) {
    LOAcquire();
    lVar6 = plStack_68[1];
    plStack_68[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar8 = plStack_68 + 2;
      (**(code **)(*plStack_68 + 0x10))(plStack_68);
      if (*plVar8 != 0) {
        LOAcquire();
        lVar6 = plVar1[2];
        plVar1[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_042b5d80;
      }
      pcVar7 = *(code **)(*plVar1 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar7)(plVar1);
      }
    }
  }
LAB_042b5d80:
  if ((long)local_48 < 0) {
    FUN_00005810(local_58,local_48 & 0x7fffffffffffffff);
  }
  return;
}


