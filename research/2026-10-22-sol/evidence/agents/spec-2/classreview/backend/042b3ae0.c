// ==== 0x42b3ae0  FUN_042b3ae0  (660 bytes)

void FUN_042b3ae0(undefined8 param_1)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  long *plVar4;
  code *pcVar5;
  long *plVar6;
  undefined8 local_78;
  undefined8 uStack_70;
  ulong local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  
  FUN_01966060(&local_78);
  plVar3 = (long *)FUN_00008790(0x3c0);
  plVar6 = plVar3 + 1;
  *plVar6 = 0;
  plVar3[2] = 0;
  *plVar3 = (long)&PTR_viewWillStartLiveResize_0da7bdc0;
  plVar1 = plVar3 + 3;
  if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x42b3d74);
    (*pcVar5)();
  }
  uStack_58 = uStack_70;
  local_60 = local_78;
  local_50 = local_68;
  uStack_70 = 0;
  local_68 = 0;
  local_78 = 0;
  FUN_04297d70(plVar1,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((plVar3 == (long *)0xffffffffffffffd0) ||
     ((plVar4 = (long *)plVar3[7], plVar4 != (long *)0x0 && (plVar4[1] != -1)))) goto LAB_042b3be0;
  if (plVar3 != (long *)0x0) {
    plVar3[1] = plVar3[1] + 1;
    plVar3[2] = plVar3[2] + 1;
  }
  plVar3[6] = (long)plVar1;
  plVar3[7] = (long)plVar3;
  if (plVar4 != (long *)0x0) {
    if (plVar4[2] != 0) {
      LOAcquire();
      lVar2 = plVar4[2];
      plVar4[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) goto LAB_042b3bd0;
    }
    pcVar5 = *(code **)(*plVar4 + 0x20);
    if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
      FUN_00005810(plVar4,0x20);
    }
    else {
      (*pcVar5)();
    }
  }
LAB_042b3bd0:
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar2 = *plVar6;
    *plVar6 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_042b3be0;
      }
      pcVar5 = *(code **)(*plVar3 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar5)(plVar3);
      }
    }
  }
LAB_042b3be0:
  if ((long)local_68 < 0) {
    FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
  }
  FUN_04298600(param_1,plVar1);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar2 = *plVar6;
    *plVar6 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return;
        }
      }
      pcVar5 = *(code **)(*plVar3 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar5)(plVar3);
      }
    }
  }
  return;
}


