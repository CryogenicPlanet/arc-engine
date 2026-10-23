// ==== 0x42b3f34  FUN_042b3f34  (1244 bytes)

void FUN_042b3f34(undefined8 param_1,int param_2,undefined8 *param_3,long param_4,long *param_5)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  long *plVar4;
  long lVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  undefined8 *puVar9;
  undefined1 auVar10 [16];
  undefined8 local_1b0;
  long *plStack_1a8;
  ulong local_198;
  undefined8 local_190;
  long *plStack_188;
  long *local_180;
  undefined8 uStack_178;
  long *local_170 [36];
  
  if (param_5 == (long *)0x0) {
    if (DAT_0e4a3078 < 3) {
      FUN_00f195d8(&local_180,"../../arc/browser/browser_application_impl.cc",0x2e5,2);
      uVar3 = FUN_00040060(local_170,"Failed to create profile named [",0x20);
      cVar1 = *(char *)((long)param_3 + 0x17);
      puVar9 = (undefined8 *)*param_3;
      if (-1 < (long)cVar1) {
        puVar9 = param_3;
      }
      lVar5 = param_3[1];
      if (-1 < cVar1) {
        lVar5 = (long)cVar1;
      }
      uVar3 = FUN_00040060(uVar3,puVar9,lVar5);
      uVar3 = FUN_00040060(uVar3,"], profile: ",0xc);
      FUN_0000f1c0(uVar3,0);
      FUN_00f1950c(&local_180);
    }
    local_180 = (long *)FUN_04b37104(0x35,"failed to create profile on disk",0x20,0x2e7,
                                     "../../arc/browser/browser_application_impl.cc");
    if (local_180 == (long *)((long)&MACH_HEADER.magic + 1)) {
      FUN_04b37cec(&local_180);
    }
    (**(code **)(param_4 + 0x10))(param_4,&local_180);
    plVar4 = local_170[0];
    if (local_180 != (long *)((long)&MACH_HEADER.magic + 1)) {
      if (((ulong)local_180 & 1) != 0) {
        return;
      }
      FUN_04b35898();
      return;
    }
    if (local_170[0] == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar5 = local_170[0][1];
    local_170[0][1] = lVar5 + -1;
    LORelease();
    if (lVar5 != 0) {
      return;
    }
    plVar7 = local_170[0] + 2;
    (**(code **)(*local_170[0] + 0x10))(local_170[0]);
    if (*plVar7 != 0) {
      LOAcquire();
      lVar5 = plVar4[2];
      plVar4[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) {
        return;
      }
    }
    pcVar6 = *(code **)(*plVar4 + 0x20);
    if (((pcVar6 != FUN_00f01bc0) && (pcVar6 != FUN_00f01bc0)) && (pcVar6 != FUN_013efb40)) {
      (*pcVar6)(plVar4);
      return;
    }
    lVar5 = 0x20;
    goto LAB_042b4148;
  }
  FUN_01a29860(DAT_0e481618);
  uVar3 = FUN_01a298f8();
  local_180 = (long *)0x0;
  uStack_178 = 0;
  local_170[0] = (long *)0x0;
  lVar5 = (long)*(char *)((long)param_3 + 0x17);
  puVar9 = param_3;
  if (lVar5 < 0) {
    puVar9 = (undefined8 *)*param_3;
    lVar5 = param_3[1];
  }
  FUN_007cf8e0(&local_180,puVar9,lVar5);
  iVar2 = FUN_03220e78(uVar3,&local_180,0);
  if (iVar2 != 0) {
    (**(code **)(*param_5 + 0x30))(&local_198,param_5);
    lVar5 = FUN_01a299e0(uVar3,&local_198);
    if ((long)plStack_188 < 0) {
      FUN_00005810(local_198,(ulong)plStack_188 & 0x7fffffffffffffff);
    }
    if (lVar5 != 0) {
      FUN_07be1624(lVar5,&local_180,0);
    }
  }
  auVar10 = FUN_042b4cc4(param_1,param_5);
  plVar4 = auVar10._8_8_;
  (**(code **)(**(long **)(*(long *)(DAT_0e481618 + 0x38) + 0x18) + 0xa0))();
  local_1b0 = 0;
  plStack_1a8 = (long *)0x0;
  if (param_2 == 0) {
    FUN_042c8400(&local_1b0,auVar10._0_8_);
  }
  else {
    FUN_042d730c();
  }
  plStack_188 = plStack_1a8;
  local_190 = local_1b0;
  if (plStack_1a8 != (long *)0x0) {
    plStack_1a8[1] = plStack_1a8[1] + 1;
  }
  local_198 = 1;
  (**(code **)(param_4 + 0x10))(param_4,&local_198);
  plVar7 = plStack_188;
  if (local_198 == 1) {
    if (plStack_188 != (long *)0x0) {
      LOAcquire();
      lVar5 = plStack_188[1];
      plStack_188[1] = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        plVar8 = plStack_188 + 2;
        (**(code **)(*plStack_188 + 0x10))(plStack_188);
        if (*plVar8 != 0) {
          LOAcquire();
          lVar5 = plVar7[2];
          plVar7[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042b4108;
        }
        pcVar6 = *(code **)(*plVar7 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar6)(plVar7);
        }
      }
    }
  }
  else if ((local_198 & 1) == 0) {
    FUN_04b35898();
  }
LAB_042b4108:
  plVar7 = plStack_1a8;
  if (plStack_1a8 != (long *)0x0) {
    LOAcquire();
    lVar5 = plStack_1a8[1];
    plStack_1a8[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      plVar8 = plStack_1a8 + 2;
      (**(code **)(*plStack_1a8 + 0x10))(plStack_1a8);
      if (*plVar8 != 0) {
        LOAcquire();
        lVar5 = plVar7[2];
        plVar7[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042b4120;
      }
      pcVar6 = *(code **)(*plVar7 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar6)(plVar7);
      }
    }
  }
LAB_042b4120:
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042b4134;
      }
      pcVar6 = *(code **)(*plVar4 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar6)(plVar4);
      }
    }
  }
LAB_042b4134:
  if (-1 < (long)local_170[0]) {
    return;
  }
  lVar5 = (long)local_170[0] << 1;
  plVar4 = local_180;
LAB_042b4148:
  FUN_00005810(plVar4,lVar5);
  return;
}


