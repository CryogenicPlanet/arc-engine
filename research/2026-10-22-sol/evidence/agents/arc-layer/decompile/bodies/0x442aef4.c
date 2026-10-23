// ==== 0x442aef4  FUN_0442aef4  (512 bytes)

undefined8 FUN_0442aef4(long *param_1,long param_2)

{
  int *piVar1;
  long lVar2;
  code *pcVar3;
  int iVar4;
  undefined8 uVar5;
  uint *puVar6;
  long *plVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long local_70;
  int local_68;
  long local_60;
  long local_58;
  long local_50;
  long local_48;
  long local_40;
  long lStack_38;
  
  puVar6 = *(uint **)(param_2 + 0x18);
  if (puVar6[3] != 0x110033d6) {
    return 0;
  }
  if (puVar6[1] < 2) {
    lVar11 = (long)puVar6 + (ulong)*puVar6;
  }
  else {
    lVar8 = *(long *)(puVar6 + 8);
    lVar11 = 0;
    if (lVar8 != 0) {
      lVar11 = (long)(puVar6 + 8) + lVar8;
    }
  }
  local_58 = 0;
  local_50 = 0;
  if (DAT_0e4a4378 == '\0') {
    iVar4 = _getentropy(&local_58,0x10);
    if (iVar4 != 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x442b060);
      (*pcVar3)();
    }
  }
  else {
    FUN_000e7a3c(&local_58,0x10);
  }
  if (local_58 == 0 && local_50 == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x442b0b4);
    (*pcVar3)();
  }
  local_40 = local_58;
  lStack_38 = local_50;
  lVar9 = *(long *)(lVar11 + 8);
  lVar8 = lVar11 + 8 + lVar9;
  lVar10 = local_40;
  lVar2 = lStack_38;
  if (lVar9 != 0 && lVar8 != 0) {
    plVar7 = (long *)(lVar8 + 8);
    lVar10 = *plVar7;
    lVar8 = (long)plVar7 + lVar10;
    if (lVar10 == 0 || lVar8 == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x442b0c0);
      (*pcVar3)();
    }
    lVar10 = *(long *)(lVar8 + 8);
    lVar2 = *(long *)(lVar8 + 0x10);
    if (*(long *)(lVar8 + 0x10) == 0 && *(long *)(lVar8 + 8) == 0) {
      FUN_084a997c(param_2,0x11,"arc.mojom.RenderProcessAgent",0,0);
      return 0;
    }
  }
  lStack_38 = lVar2;
  local_40 = lVar10;
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  FUN_00affafc(lVar11 + 0x10,param_2,&local_58);
  local_70 = local_58;
  local_68 = (int)local_50;
  local_60 = local_48;
  (**(code **)(*param_1 + 0x10))(param_1,&local_40,&local_70);
  if (local_68 == 1) {
    if (local_60 == 0) {
      local_68 = 0;
      goto LAB_0442b010;
    }
    FUN_084a7784();
  }
  lVar11 = local_60;
  local_68 = 0;
  local_60 = 0;
  if (lVar11 != 0) {
    piVar1 = (int *)(lVar11 + 8);
    LOAcquire();
    iVar4 = *piVar1;
    *piVar1 = iVar4 + -1;
    LORelease();
    if (iVar4 == 1) {
      uVar5 = FUN_084a76fc();
      FUN_00005810(uVar5,0x38);
    }
    if (local_60 != 0) {
      LOAcquire();
      iVar4 = *(int *)(local_60 + 8);
      *(int *)(local_60 + 8) = iVar4 + -1;
      LORelease();
      if (iVar4 == 1) {
        uVar5 = FUN_084a76fc();
        FUN_00005810(uVar5,0x38);
      }
    }
  }
LAB_0442b010:
  if (local_70 != 0) {
    FUN_005a359c();
  }
  return 1;
}


