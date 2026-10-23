// ==== 0x433eb60  FUN_0433eb60  (616 bytes)

void FUN_0433eb60(undefined8 *param_1,long param_2)

{
  undefined1 uVar1;
  long lVar2;
  long *plVar3;
  code *pcVar4;
  long *plVar5;
  undefined1 auVar6 [16];
  undefined8 local_98;
  undefined8 uStack_90;
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *plStack_78;
  ulong local_70;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  
  uVar1 = *(undefined1 *)(*(long *)(param_2 + 0x368) + 0x48);
  local_80 = 0;
  plStack_78 = (long *)0x0;
  local_70 = 0;
  auVar6 = FUN_042b17e8();
  plVar3 = auVar6._8_8_;
  (**(code **)(**(long **)(param_2 + 0x368) + 0x30))(&local_60);
  FUN_00df9708(&local_98,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00060a60(&local_60,local_98,uStack_90);
  }
  else {
    uStack_58 = uStack_90;
    local_60 = local_98;
    local_50 = CONCAT17(cStack_81,local_88);
  }
  FUN_042b4410(&local_80,auVar6._0_8_,uVar1,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98,CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar3[1];
    plVar3[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_0433ec24;
      }
      pcVar4 = *(code **)(*plVar3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar4)(plVar3);
      }
    }
  }
LAB_0433ec24:
  plVar3 = plStack_78;
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0,0x433edc8);
    (*pcVar4)();
  }
  param_1[1] = plStack_78;
  *param_1 = local_80;
  if (((plStack_78 != (long *)0x0) && (plStack_78[1] = plStack_78[1] + 1, (char)local_70 != '\0'))
     && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar2 = plStack_78[1];
    plStack_78[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*plVar3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar4)(plVar3);
      }
    }
  }
  return;
}


