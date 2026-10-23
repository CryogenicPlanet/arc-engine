// ==== 0x4367d74  FUN_04367d74  (904 bytes)

void FUN_04367d74(long *param_1,long param_2,long *param_3)

{
  long lVar1;
  bool bVar2;
  long lVar3;
  char *pcVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  code *pcVar8;
  long *plVar9;
  long *plVar10;
  long lVar11;
  undefined1 auStack_98 [24];
  undefined1 *local_80;
  undefined8 uStack_78;
  ulong local_70;
  undefined8 local_60;
  undefined8 uStack_58;
  
  if (*(long *)(param_2 + 0x18) == 0) {
    pcVar4 = "managed webcontents is null";
    uVar5 = 0x1b;
    uVar6 = 0xf0;
LAB_04367de4:
    lVar3 = FUN_04b37104(0x25,pcVar4,uVar5,uVar6,
                         "../../arc/browser/web_contents/navigation_controller_impl.cc");
    *param_1 = lVar3;
    if (lVar3 != 1) {
      return;
    }
    FUN_04b37cec(param_1);
    return;
  }
  plVar9 = *(long **)(param_2 + 0x38);
  if (plVar9 == (long *)0x0) {
LAB_04367da8:
    pcVar4 = "parent webcontents was cleared";
    uVar5 = 0x1e;
    uVar6 = 0xf6;
    goto LAB_04367de4;
  }
  lVar3 = plVar9[1];
  do {
    if (lVar3 == -1) goto LAB_04367da8;
    lVar11 = lVar3 + 1;
    lVar1 = plVar9[1];
    bVar2 = lVar1 != lVar3;
    lVar3 = lVar1;
  } while (bVar2);
  plVar9[1] = lVar11;
  if (*(long *)(param_2 + 0x30) == 0) {
    pcVar4 = "parent webcontents was cleared";
    uVar5 = 0x25;
    uVar6 = 0x1e;
    uVar7 = 0xf6;
  }
  else if ((*(byte *)(*param_3 + 0x30) & 1) == 0) {
    pcVar4 = "item is not valid";
    uVar5 = 0xd;
    uVar6 = 0x11;
    uVar7 = 0xfc;
  }
  else {
    uVar5 = FUN_042aebb8(*(undefined8 *)(*(long *)(param_2 + 0x30) + 0x2f8),param_3);
    if (-1 < (int)uVar5) {
      lVar3 = *param_3;
      if (*(char *)(lVar3 + 0x2f) < '\0') {
        FUN_00060a60(&local_80,*(undefined8 *)(lVar3 + 0x18),*(undefined8 *)(lVar3 + 0x20));
      }
      else {
        uStack_78 = *(undefined8 *)(lVar3 + 0x20);
        local_80 = *(undefined1 **)(lVar3 + 0x18);
        local_70 = *(ulong *)(lVar3 + 0x28);
      }
      FUN_04345994(&local_60,&local_80);
      if ((*(char *)(param_2 + 0x58) != '\0') &&
         (plVar10 = *(long **)(param_2 + 0x50), plVar10 != (long *)0x0)) {
        LOAcquire();
        lVar3 = plVar10[1];
        plVar10[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*plVar10 + 0x10))(plVar10);
          if (plVar10[2] != 0) {
            LOAcquire();
            lVar3 = plVar10[2];
            plVar10[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) goto LAB_04368004;
          }
          pcVar8 = *(code **)(*plVar10 + 0x20);
          if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40)) {
            FUN_00005810(plVar10,0x20);
          }
          else {
            (*pcVar8)(plVar10);
          }
        }
      }
LAB_04368004:
      if (param_2 == -0x48) {
                    /* WARNING: Does not return */
        pcVar8 = (code *)SoftwareBreakpoint(1,0x43680f8);
        (*pcVar8)();
      }
      *(undefined8 *)(param_2 + 0x50) = uStack_58;
      *(undefined8 *)(param_2 + 0x48) = local_60;
      local_60 = 0;
      uStack_58 = 0;
      *(undefined1 *)(param_2 + 0x58) = 1;
      if ((long)local_70 < 0) {
        FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
      }
      FUN_0665ea00(auStack_98,*(long *)(param_2 + 0x18) + 0x260,uVar5);
      local_80 = auStack_98;
      FUN_064540c0(&local_80);
      if ((*(byte *)(param_2 + 0x58) & 1) == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04b08a00();
      }
      lVar3 = *(long *)(param_2 + 0x50);
      lVar11 = *(long *)(param_2 + 0x48);
      param_1[2] = *(long *)(param_2 + 0x50);
      param_1[1] = lVar11;
      if (lVar3 != 0) {
        plVar10 = (long *)(lVar3 + 8);
        *plVar10 = *plVar10 + 1;
      }
      *param_1 = 1;
      goto LAB_04367e74;
    }
    pcVar4 = "item is not present in webcontents backforwardlist";
    uVar5 = 0x2d;
    uVar6 = 0x32;
    uVar7 = 0x102;
  }
  lVar3 = FUN_04b37104(uVar5,pcVar4,uVar6,uVar7,
                       "../../arc/browser/web_contents/navigation_controller_impl.cc");
  *param_1 = lVar3;
  if (lVar3 == 1) {
    FUN_04b37cec(param_1);
  }
LAB_04367e74:
  LOAcquire();
  lVar3 = plVar9[1];
  plVar9[1] = lVar3 + -1;
  LORelease();
  if (lVar3 != 0) {
    return;
  }
  (**(code **)(*plVar9 + 0x10))(plVar9);
  if (plVar9[2] != 0) {
    LOAcquire();
    lVar3 = plVar9[2];
    plVar9[2] = lVar3 + -1;
    LORelease();
    if (lVar3 != 0) {
      return;
    }
  }
  pcVar8 = *(code **)(*plVar9 + 0x20);
  if (((pcVar8 != FUN_00f01bc0) && (pcVar8 != FUN_00f01bc0)) && (pcVar8 != FUN_013efb40)) {
    (*pcVar8)(plVar9);
    return;
  }
  FUN_00005810(plVar9,0x20);
  return;
}


