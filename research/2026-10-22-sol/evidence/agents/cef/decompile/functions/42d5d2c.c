// ==== 0x42d5d2c  FUN_042d5d2c  (460 bytes)

void FUN_042d5d2c(long param_1,undefined8 *param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ulong uVar8;
  long *plVar9;
  undefined8 *puVar10;
  int *piVar11;
  undefined1 *local_60;
  long lStack_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  local_48 = 0;
  uStack_40 = 0;
  local_38 = 0;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_60,*param_2,param_2[1]);
  }
  else {
    lStack_58 = param_2[1];
    local_60 = (undefined1 *)*param_2;
    local_50 = param_2[2];
  }
  ppuVar1 = (undefined1 **)local_60;
  if (-1 < (long)local_50._7_1_) {
    ppuVar1 = &local_60;
  }
  lVar5 = lStack_58;
  if (-1 < (long)local_50) {
    lVar5 = (long)local_50._7_1_;
  }
  FUN_008a7fc4(&local_48,ppuVar1,lVar5);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  puVar10 = *(undefined8 **)(param_1 + 0x250);
  if (puVar10 == (undefined8 *)0x0) {
    puVar10 = (undefined8 *)FUN_00008790(0x10);
    uVar8 = *(ulong *)(param_1 + 0x88);
    if ((uVar8 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar8);
    }
    FUN_0836a940(puVar10,uVar8);
    plVar9 = *(long **)(param_1 + 0x250);
    *(undefined8 **)(param_1 + 0x250) = puVar10;
    if (plVar9 != (long *)0x0) {
      lVar5 = plVar9[1];
      plVar9[1] = 0;
      if (lVar5 != 0) {
        uVar7 = FUN_06c06af8();
        FUN_00005810(uVar7,0x178);
      }
      lVar5 = *plVar9;
      *plVar9 = 0;
      if (lVar5 != 0) {
        uVar7 = FUN_078ca584();
        FUN_00005810(uVar7,0x80);
      }
      FUN_00005810(plVar9,0x10);
      puVar10 = *(undefined8 **)(param_1 + 0x250);
    }
  }
  uVar7 = *puVar10;
  piVar11 = *(int **)(param_1 + 0x310);
  if ((piVar11 != (int *)0x0) && (iVar3 = *piVar11, *piVar11 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x42d5ef8);
    (*pcVar2)();
  }
  uVar6 = *(undefined8 *)(param_1 + 0x318);
  puVar4 = (undefined4 *)FUN_00008790(0x40);
  *puVar4 = 1;
  *(undefined1 **)(puVar4 + 2) = &LAB_04076980;
  *(code **)(puVar4 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
  *(code **)(puVar4 + 8) = PiPNSView::viewWillStartLiveResize;
  *(undefined8 *)(puVar4 + 10) = 0;
  *(int **)(puVar4 + 0xc) = piVar11;
  *(undefined8 *)(puVar4 + 0xe) = uVar6;
  FUN_078cab40(uVar7,&local_48,1,puVar4);
  if ((long)local_38 < 0) {
    FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
  }
  return;
}


