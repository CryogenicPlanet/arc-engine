// ==== 0x4367748  FUN_04367748  (788 bytes)

void FUN_04367748(long *param_1,long param_2)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  ulong uVar4;
  code *pcVar5;
  long *plVar6;
  long lVar7;
  undefined1 auStack_a8 [24];
  undefined1 *local_90;
  long lStack_88;
  ulong local_80;
  undefined1 local_78;
  undefined1 *local_70;
  long lStack_68;
  ulong local_60;
  int *local_50;
  ulong local_48;
  
  local_50 = (int *)0x0;
  local_48 = 0;
  lVar3 = *(long *)(param_2 + 0x18);
  if (lVar3 == 0) {
    local_50 = (int *)FUN_04b37104(0x25,"managed webcontents is null",0x1b,0x40,
                                   "../../arc/browser/web_contents/navigation_controller_impl.cc");
    if ((local_50 == (int *)((long)&MACH_HEADER.magic + 1)) &&
       (FUN_04b37cec(&local_50), local_50 == (int *)((long)&MACH_HEADER.magic + 1))) {
      uVar4 = local_48 & 1;
      goto joined_r0x04367a50;
    }
    *param_1 = (long)local_50;
    if (((ulong)local_50 & 1) != 0) goto LAB_04367970;
    *local_50 = *local_50 + 1;
    lVar3 = *param_1;
  }
  else {
    iVar1 = *(int *)(lVar3 + 0x2b8);
    if (iVar1 == -1) {
      iVar1 = *(int *)(lVar3 + 0x2b4);
    }
    uVar2 = FUN_00881f80(lVar3 + 0x260,iVar1,1,0);
    uVar4 = uVar2 >> 0x20;
    local_48 = CONCAT71(local_48._1_7_,(char)(uVar2 >> 0x20));
    local_50 = (int *)((long)&MACH_HEADER.magic + 1);
joined_r0x04367a50:
    if (uVar4 != 0) {
      local_70 = (undefined1 *)0x0;
      lStack_68 = 0;
      local_60 = 0;
      lVar3 = *(long *)(param_2 + 0x18);
      iVar1 = *(int *)(lVar3 + 0x2b8);
      if (iVar1 == -1) {
        iVar1 = *(int *)(lVar3 + 0x2b4);
      }
      uVar4 = FUN_00881f80(lVar3 + 0x260,iVar1,1,0);
      if (((uVar4 >> 0x20 == 0) || ((int)uVar4 < 0)) ||
         ((int)*(ulong *)(lVar3 + 0x280) <= (int)uVar4)) {
LAB_04367804:
        local_60 = local_60 & 0xffffffffffffff;
        local_70 = (undefined1 *)((ulong)local_70 & 0xffffffffffffff00);
      }
      else {
        if (*(ulong *)(lVar3 + 0x280) <= (uVar4 & 0x7fffffff)) goto LAB_04367a28;
        lVar3 = *(long *)(*(long *)(lVar3 + 0x278) + (uVar4 & 0x7fffffff) * 8);
        if (lVar3 == 0) goto LAB_04367804;
        plVar6 = (long *)FUN_007cf864(*(long *)(*(long *)(lVar3 + 0x18) + 8) + 0x60);
        if (*(char *)((long)plVar6 + 0x17) < '\0') {
          FUN_00060a60(&local_90,*plVar6,plVar6[1]);
        }
        else {
          lStack_88 = plVar6[1];
          local_90 = (undefined1 *)*plVar6;
          local_80 = plVar6[2];
        }
        local_78 = 1;
        lStack_68 = lStack_88;
        local_70 = local_90;
        local_60 = local_80;
      }
      FUN_04345994(&local_90,&local_70);
      if ((*(char *)(param_2 + 0x58) != '\0') &&
         (plVar6 = *(long **)(param_2 + 0x50), plVar6 != (long *)0x0)) {
        LOAcquire();
        lVar3 = plVar6[1];
        plVar6[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*plVar6 + 0x10))(plVar6);
          if (plVar6[2] != 0) {
            LOAcquire();
            lVar3 = plVar6[2];
            plVar6[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) goto LAB_0436787c;
          }
          pcVar5 = *(code **)(*plVar6 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar6,0x20);
          }
          else {
            (*pcVar5)(plVar6);
          }
        }
      }
LAB_0436787c:
      if (param_2 == -0x48) {
LAB_04367a28:
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(1,0x4367a2c);
        (*pcVar5)();
      }
      *(long *)(param_2 + 0x50) = lStack_88;
      *(undefined1 **)(param_2 + 0x48) = local_90;
      *(undefined1 *)(param_2 + 0x58) = 1;
      FUN_0665e8a0(auStack_a8,*(long *)(param_2 + 0x18) + 0x260);
      local_90 = auStack_a8;
      FUN_064540c0(&local_90);
      if ((*(byte *)(param_2 + 0x58) & 1) == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04b08a00();
      }
      lVar3 = *(long *)(param_2 + 0x50);
      lVar7 = *(long *)(param_2 + 0x48);
      param_1[2] = *(long *)(param_2 + 0x50);
      param_1[1] = lVar7;
      if (lVar3 != 0) {
        plVar6 = (long *)(lVar3 + 8);
        *plVar6 = *plVar6 + 1;
      }
      *param_1 = 1;
      if ((long)local_60 < 0) {
        FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
      }
      goto LAB_04367970;
    }
    lVar3 = FUN_04b37104(0x25,"cannot go back",0xe,0xc9,
                         "../../arc/browser/web_contents/navigation_controller_impl.cc");
    *param_1 = lVar3;
  }
  if (lVar3 == 1) {
    FUN_04b37cec(param_1);
  }
LAB_04367970:
  if (((ulong)local_50 & 1) == 0) {
    FUN_04b35898();
  }
  return;
}


