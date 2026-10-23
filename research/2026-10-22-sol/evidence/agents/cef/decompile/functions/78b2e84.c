// ==== 0x78b2e84  FUN_078b2e84  (900 bytes)

/* WARNING: Removing unreachable block (ram,0x078b3128) */
/* WARNING: Removing unreachable block (ram,0x078b31c0) */
/* WARNING: Removing unreachable block (ram,0x078b3170) */

void FUN_078b2e84(long param_1,undefined8 param_2,undefined8 param_3)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  long *plVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  ulong uVar10;
  int *piVar11;
  undefined8 uVar12;
  undefined1 auVar13 [16];
  undefined1 auStack_118 [151];
  undefined1 uStack_81;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  byte local_68;
  
  lVar3 = FUN_09f810c4(*(undefined8 *)(param_1 + 0xc0));
  if (lVar3 != 0) {
    FUN_06bdbd94(&local_80);
    if ((local_68 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    FUN_078b2d50(param_1,param_2,param_3,&local_80);
  }
  FUN_04596df4(param_2,param_3,*(undefined8 *)(param_1 + 0xc0));
  lVar3 = FUN_08205c6c(*(undefined8 *)(*(long *)(param_1 + 0x18) + 0x270));
  if (lVar3 != 0) {
    FUN_08203ff8();
    iVar2 = FUN_078b3210(param_1);
    if ((iVar2 != 0) && (*(long *)(lVar3 + 0x58) != 0)) {
      *(undefined1 *)(*(long *)(lVar3 + 0x58) + 0xc) = 1;
      FUN_08204cec(lVar3,*(undefined4 *)(*(long *)(lVar3 + 0x58) + 8),1);
    }
  }
  if (((*(long **)(param_1 + 0xc0) != (long *)0x0) &&
      (lVar3 = (**(code **)(**(long **)(param_1 + 0xc0) + 0x38))(), lVar3 != 0)) &&
     (plVar4 = *(long **)(param_1 + 0xa00), plVar4 != (long *)0x0)) {
    if (*(code **)(*plVar4 + 0x10) == (code *)&LAB_0444efc0) {
      if ((plVar4[4] == 0) || (*(int *)plVar4[3] == 1)) goto LAB_078b2f48;
    }
    else {
      iVar2 = (**(code **)(*plVar4 + 0x10))();
      if (iVar2 == 0) goto LAB_078b2f48;
    }
    uVar5 = 0;
    if (*(long **)(param_1 + 0xc0) != (long *)0x0) {
      uVar5 = (**(code **)(**(long **)(param_1 + 0xc0) + 0x38))();
    }
    FUN_09f7a064(uVar5,2);
  }
LAB_078b2f48:
  FUN_009de658(param_1 + 0x9d0);
  (**(code **)(**(long **)(param_1 + 0xc0) + 0x68))();
  lVar3 = FUN_0372d4c4(*(undefined8 *)(param_1 + 0x960));
  iVar2 = FUN_03708768(*(undefined8 *)(lVar3 + 0x18));
  if (iVar2 != 0) {
    uVar5 = FUN_0275dda8(*(undefined8 *)(*(long *)(param_1 + 0x18) + 0x270));
    FUN_00b89b80(&local_80,"passwords_account_storage_used",&uStack_81);
    FUN_09f424ac(uVar5,&local_80);
  }
  auVar13 = (**(code **)(**(long **)(param_1 + 0xc0) + 0x48))();
  uVar5 = FUN_00008790(0x930);
  thunk_FUN_06c060f4(uVar5,auVar13._0_8_,auVar13._8_8_,param_2);
  lVar3 = *(long *)(param_1 + 0x9c0);
  *(undefined8 *)(param_1 + 0x9c0) = uVar5;
  if (lVar3 != 0) {
    uVar5 = FUN_06c06278();
    FUN_00005810(uVar5,0x930);
    uVar5 = *(undefined8 *)(param_1 + 0x9c0);
  }
  uVar6 = FUN_018d09d0(*(undefined8 *)(param_1 + 0x960));
  uVar7 = FUN_01993fd0(*(undefined8 *)(param_1 + 0x960));
  uVar8 = (**(code **)(**(long **)(*(long *)(param_1 + 0x18) + 0x270) + 0x168))();
  piVar11 = *(int **)(param_1 + 0xa60);
  if ((piVar11 != (int *)0x0) && (iVar2 = *piVar11, *piVar11 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x78b3208);
    (*pcVar1)();
  }
  uVar12 = *(undefined8 *)(param_1 + 0xa68);
  uVar9 = FUN_00008790(0x40);
  lVar3 = FUN_0105a98c(uVar9,&SUB_040a6540,FUN_040742c0,&LAB_04d0a140);
  *(code **)(lVar3 + 0x20) = FUN_078b32a4;
  *(undefined8 *)(lVar3 + 0x28) = 0;
  *(int **)(lVar3 + 0x30) = piVar11;
  *(undefined8 *)(lVar3 + 0x38) = uVar12;
  FUN_06c0633c(uVar5,uVar6,uVar7,uVar8,uVar9);
  FUN_078ad5cc(param_1 + 0x70,4);
  *(undefined4 *)(param_1 + 0x9c8) = 3;
  plVar4 = (long *)FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),
                                *(undefined8 *)(param_1 + 0x18));
  if (plVar4 != (long *)0x0) {
    (**(code **)(*plVar4 + 0x38))();
    uVar10 = FUN_07c1aec0();
    if ((uVar10 & 1) == 0) {
      thunk_FUN_042348ec(plVar4);
      local_80 = 0;
      uStack_78 = 0;
      local_70 = 0;
      FUN_030e1478(auStack_118,&PTR_s_IPH_PasswordsManagementBubbleAft_0e363a88,&local_80);
      thunk_FUN_030e12c0(auStack_118);
      FUN_078b1b28(plVar4);
    }
  }
  return;
}


