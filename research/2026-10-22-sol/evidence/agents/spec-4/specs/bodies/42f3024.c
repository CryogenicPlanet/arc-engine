// ==== 0x42f3024  FUN_042f3024  (784 bytes)

void FUN_042f3024(undefined8 *param_1,ulong param_2,long *param_3)

{
  long *plVar1;
  ulong uVar2;
  code *pcVar3;
  bool bVar4;
  int iVar5;
  int *piVar6;
  undefined8 uVar7;
  char *pcVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  long lVar11;
  long lVar12;
  undefined8 local_90;
  long lStack_88;
  undefined8 local_80;
  int *local_70;
  ulong uStack_68;
  undefined8 local_60;
  long *local_50;
  ulong uStack_48;
  undefined8 local_40;
  
  if (*(long *)(param_2 + 0x70) == 0) {
    pcVar8 = "managed profile is null";
    uVar7 = 0x25;
    uVar9 = 0x17;
    uVar10 = 0xbe;
    goto LAB_042f32b4;
  }
  lVar11 = *param_3;
  if (*(char *)(lVar11 + 0x3cf) < '\0') {
    FUN_00054420(&local_50,*(undefined8 *)(lVar11 + 0x3b8),*(undefined8 *)(lVar11 + 0x3c0));
  }
  else {
    uStack_48 = *(ulong *)(lVar11 + 0x3c0);
    local_50 = *(long **)(lVar11 + 0x3b8);
    local_40 = *(long *)(lVar11 + 0x3c8);
  }
  uVar2 = uStack_48;
  if (-1 < (char)local_40._7_1_) {
    uVar2 = (ulong)local_40._7_1_;
  }
  if (uVar2 == 0) {
    bVar4 = true;
    if (((uint)(int)(char)local_40._7_1_ >> 7 & 1) != 0) goto LAB_042f3128;
  }
  else {
    lVar11 = *param_3;
    if (*(char *)(lVar11 + 999) < '\0') {
      FUN_00054420(&local_70,*(undefined8 *)(lVar11 + 0x3d0),*(undefined8 *)(lVar11 + 0x3d8));
    }
    else {
      uStack_68 = *(ulong *)(lVar11 + 0x3d8);
      local_70 = *(int **)(lVar11 + 0x3d0);
      local_60 = *(long *)(lVar11 + 0x3e0);
    }
    uVar2 = uStack_68;
    if (-1 < (char)local_60._7_1_) {
      uVar2 = (ulong)local_60._7_1_;
    }
    if (uVar2 == 0) {
      bVar4 = true;
      if (((uint)(int)(char)local_60._7_1_ >> 7 & 1) != 0) goto LAB_042f3110;
    }
    else {
      lVar11 = *param_3;
      if (*(char *)(lVar11 + 0x3ff) < '\0') {
        FUN_00060a60(&local_90,*(undefined8 *)(lVar11 + 1000),*(undefined8 *)(lVar11 + 0x3f0));
      }
      else {
        lStack_88 = *(long *)(lVar11 + 0x3f0);
        local_90 = *(undefined8 *)(lVar11 + 1000);
        local_80 = *(ulong *)(lVar11 + 0x3f8);
      }
      lVar11 = lStack_88;
      if (-1 < (long)local_80) {
        lVar11 = (long)local_80._7_1_;
      }
      bVar4 = lVar11 == 0;
      if ((long)local_80._7_1_ < 0) {
        FUN_00005810(local_90,local_80 & 0x7fffffffffffffff);
      }
      if (local_60 < 0) {
LAB_042f3110:
        FUN_00005810(local_70,local_60 << 1);
      }
    }
    if (local_40 < 0) {
LAB_042f3128:
      FUN_00005810(local_50,local_40 << 1);
    }
  }
  if (!bVar4) {
    if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_2);
    }
    piVar6 = (int *)FUN_00008790(0x40);
    *piVar6 = 1;
    *(code **)(piVar6 + 2) = FUN_042f5834;
    *(code **)(piVar6 + 4) = FUN_042f5fe4;
    *(code **)(piVar6 + 6) = FUN_00b77080;
    lVar12 = param_3[1];
    lVar11 = *param_3;
    *(long *)(piVar6 + 0xc) = param_3[1];
    *(long *)(piVar6 + 10) = lVar11;
    lVar11 = 0;
    if (lVar12 != 0) {
      plVar1 = (long *)(lVar12 + 8);
      lVar11 = *plVar1;
      *plVar1 = lVar11 + 1;
    }
    *(ulong *)(piVar6 + 0xe) = param_2;
    lVar11 = FUN_01a9dba4(lVar11,*(undefined8 *)(param_2 + 0x70));
    if ((*(byte *)(lVar11 + 0x170) & 1) == 0) {
      FUN_01469bb4(&local_70,lVar11 + 0x1e8,piVar6);
      local_50 = param_3;
      lVar11 = FUN_042f6c18(param_2 + 0x108,&local_50);
      if (lVar11 != 0) {
        FUN_01469964(lVar11 + 0x20,&local_70);
        piVar6 = local_70;
        if (local_70 != (int *)0x0) {
          local_70 = (int *)0x0;
          (**(code **)(piVar6 + 2))(piVar6);
          LOAcquire();
          iVar5 = *piVar6;
          *piVar6 = iVar5 + -1;
          LORelease();
          if (iVar5 == 1) {
            (**(code **)(piVar6 + 4))(piVar6);
          }
          if (local_70 != (int *)0x0) {
            LOAcquire();
            iVar5 = *local_70;
            *local_70 = iVar5 + -1;
            LORelease();
            if (iVar5 == 1) {
              (**(code **)(local_70 + 4))();
            }
          }
        }
        *param_1 = 1;
        return;
      }
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x42f3334);
      (*pcVar3)();
    }
    *param_1 = 1;
    if (piVar6 == (int *)0x0) {
      return;
    }
    FUN_042f5834(piVar6);
    LOAcquire();
    iVar5 = *piVar6;
    *piVar6 = iVar5 + -1;
    LORelease();
    if (iVar5 != 1) {
      return;
    }
    (**(code **)(piVar6 + 4))(piVar6);
    return;
  }
  pcVar8 = "Search engine cannot be added - Name, Keyword and URL must not be empty";
  uVar7 = 0xd;
  uVar9 = 0x47;
  uVar10 = 0xc4;
LAB_042f32b4:
  uVar7 = FUN_04b37104(uVar7,pcVar8,uVar9,uVar10,
                       "../../arc/browser/browser_context/preferences_impl.cc");
  *param_1 = uVar7;
  return;
}


