// ==== 0x43aa39c  FUN_043aa39c  (944 bytes)

void FUN_043aa39c(undefined8 *param_1,long *param_2)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  int *piVar6;
  undefined4 *puVar7;
  ulong uVar8;
  int *piVar9;
  undefined8 uVar10;
  long lVar11;
  undefined8 *puVar12;
  long lVar13;
  long local_68;
  int local_60;
  long local_58;
  
  puVar4 = (undefined8 *)FUN_00008790(0x20);
  *puVar4 = &PTR_FUN_0da82d28;
  uVar10 = *param_1;
  puVar4[2] = param_1[1];
  puVar4[1] = uVar10;
  puVar4[3] = 0;
  lVar13 = *param_2;
  *param_2 = 0;
  iVar3 = (int)param_2[1];
  lVar11 = param_2[2];
  param_2[2] = 0;
  puVar5 = (undefined8 *)FUN_00008790(0x58);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = puVar4;
  puVar12 = puVar5 + 3;
  *puVar12 = 0;
  puVar5[4] = 0;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar2 = *piVar6, *piVar6 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x43aa734);
    (*pcVar1)();
  }
  puVar5[7] = &PTR__cxx_destruct_0da82d80;
  puVar5[5] = piVar6;
  puVar5[6] = puVar12;
  puVar5[8] = puVar4;
  local_68 = lVar13;
  local_60 = iVar3;
  local_58 = lVar11;
  if (lVar13 == 0) {
    FUN_00365e78(puVar12);
  }
  else {
    puVar4 = (undefined8 *)FUN_00008790(8);
    *puVar4 = &PTR__cxx_destruct_0da88768;
    FUN_0092d8c0(puVar12,&local_68,0,"arc.mojom.RenderFrameAgent",puVar4,0,0,0,puVar5 + 7,0,
                 &LAB_04429bcc,&LAB_04051dc0);
    lVar11 = local_58;
    iVar3 = local_60;
  }
  if (iVar3 == 1) {
    if (lVar11 == 0) {
      local_60 = 0;
      local_58 = 0;
      goto LAB_043aa4dc;
    }
    FUN_084a7784(lVar11);
    lVar11 = local_58;
  }
  local_60 = 0;
  local_58 = 0;
  if (lVar11 != 0) {
    LOAcquire();
    iVar3 = *(int *)(lVar11 + 8);
    *(int *)(lVar11 + 8) = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      uVar10 = FUN_084a76fc(lVar11);
      FUN_00005810(uVar10,0x38);
    }
    if (local_58 != 0) {
      LOAcquire();
      iVar3 = *(int *)(local_58 + 8);
      *(int *)(local_58 + 8) = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        uVar10 = FUN_084a76fc();
        FUN_00005810(uVar10,0x38);
      }
    }
  }
LAB_043aa4dc:
  if (local_68 != 0) {
    FUN_005a359c();
  }
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar3 = *piVar6, *piVar6 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x43aa740);
    (*pcVar1)();
  }
  puVar5[9] = piVar6;
  puVar5[10] = puVar5;
  if (((ulong)puVar5 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(puVar5);
  }
  puVar7 = (undefined4 *)FUN_00008790(0x38);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_04076a00;
  *(code **)(puVar7 + 4) = FUN_01bde140;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  *(code **)(puVar7 + 8) = FUN_043aacd0;
  *(undefined8 *)(puVar7 + 10) = 0;
  *(undefined8 **)(puVar7 + 0xc) = puVar5;
  lVar11 = puVar5[4];
  uVar8 = FUN_001c5f40(lVar11 + 0x1f8,0);
  if ((uVar8 & 1) != 0) {
    piVar6 = *(int **)(lVar11 + 0x198);
    *(undefined4 **)(lVar11 + 0x198) = puVar7;
    if (piVar6 != (int *)0x0) {
      LOAcquire();
      iVar3 = *piVar6;
      *piVar6 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        if (*(code **)(piVar6 + 4) == FUN_039a66c0) {
          piVar9 = *(int **)(piVar6 + 0xc);
          if (piVar9 != (int *)0x0) {
            LOAcquire();
            iVar3 = *piVar9;
            *piVar9 = iVar3 + -1;
            LORelease();
            if (iVar3 == 1) {
              FUN_00005810(piVar9,8);
            }
          }
          FUN_00005810(piVar6,0x48);
        }
        else {
          (**(code **)(piVar6 + 4))(piVar6);
        }
      }
    }
    piVar6 = *(int **)(lVar11 + 400);
    *(undefined8 *)(lVar11 + 400) = 0;
    if (piVar6 != (int *)0x0) {
      LOAcquire();
      iVar3 = *piVar6;
      *piVar6 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        if (*(code **)(piVar6 + 4) == FUN_039a6ef8) {
          piVar9 = *(int **)(piVar6 + 10);
          if (piVar9 != (int *)0x0) {
            LOAcquire();
            iVar3 = *piVar9;
            *piVar9 = iVar3 + -1;
            LORelease();
            if (iVar3 == 1) {
              (**(code **)(piVar9 + 4))();
            }
          }
          FUN_00005810(piVar6,0x30);
        }
        else {
          (**(code **)(piVar6 + 4))(piVar6);
        }
      }
    }
    piVar6 = (int *)puVar5[9];
    if (piVar6 != (int *)0x0) {
      iVar3 = *piVar6;
      *piVar6 = iVar3 + 1;
      if (iVar3 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x43aa728);
        (*pcVar1)();
      }
      LOAcquire();
      iVar3 = *piVar6;
      *piVar6 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        FUN_00005810(piVar6,8);
      }
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x43aa71c);
  (*pcVar1)();
}


