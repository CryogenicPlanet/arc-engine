// ==== 0x2bd55c0  FUN_02bd55c0  (844 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_02bd55c0(long param_1,long *param_2)

{
  code *pcVar1;
  int iVar2;
  long *plVar3;
  long lVar4;
  int *piVar5;
  long *plVar6;
  long lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  ID self;
  long local_a8;
  long *local_a0;
  long local_98;
  undefined1 auStack_90 [8];
  undefined1 auStack_88 [8];
  undefined4 local_80;
  undefined1 auStack_78 [8];
  undefined8 local_70;
  undefined1 auStack_68 [8];
  undefined8 local_60;
  ulong local_58;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (((ulong)param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  if ((*(ulong *)(param_1 + 0x68) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(long **)(param_1 + 0x68) = param_2;
  if (*(int *)(*(long *)(param_1 + 0x28) + 0x9d8) + *(int *)(*(long *)(param_1 + 0x28) + 0x9d4) < 1
      || param_2 == (long *)0x0) {
    piVar5 = *(int **)(param_1 + 0x70);
    if (piVar5 != (int *)0x0) {
      *(undefined8 *)(param_1 + 0x70) = 0;
      (**(code **)(piVar5 + 2))(piVar5);
      LOAcquire();
      iVar2 = *piVar5;
      *piVar5 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar5 + 4))(piVar5);
      }
    }
  }
  else if (*(long *)(param_1 + 0x70) == 0) {
    (**(code **)(*param_2 + 0x20))(&local_a8,param_2);
    FUN_01b92718((long *)(param_1 + 0x70),&local_a8);
    FUN_01941cb4(&local_a8);
  }
  plVar3 = (long *)(**(code **)(**(long **)(param_1 + 0x68) + 8))();
  if (plVar3 != (long *)0x0) {
    local_a8 = 0;
    local_a0 = (long *)0x0;
    FUN_02a4387c(&local_a8);
    local_a0 = (long *)((ulong)local_a0 & 0xffffffff00000000);
    FUN_0684be68(auStack_48,&local_a8);
    FUN_068547d8(param_1 + 0xc0,auStack_48);
    FUN_00279dc0(auStack_48);
    local_50 = 0;
    FUN_068548a8(&local_50,param_1 + 0xa8);
    local_60 = 0;
    local_58 = 0;
    FUN_01861a5c(auStack_68,&local_a8);
    FUN_01861a5c(&local_60,auStack_68);
    local_58 = local_58 & 0xffffffff00000000;
    FUN_00279dc0(auStack_68);
    local_70 = 0;
    FUN_01861a5c(auStack_78,&local_50);
    FUN_01861a5c(&local_70,auStack_78);
    FUN_00279dc0(auStack_78);
    uVar8 = *(undefined8 *)(param_1 + 0x88);
    FUN_01861a5c(auStack_88,&local_60);
    local_80 = (undefined4)local_58;
    FUN_01861a5c(auStack_90,&local_70);
    (**(code **)(*plVar3 + 0x30))(plVar3,uVar8,auStack_88,auStack_90);
    FUN_00279dc0(auStack_90);
    FUN_00279dc0(auStack_88);
    if (*(int *)(*(long *)(param_1 + 0x28) + 0xae0) == 0) {
      FUN_0598f3f0(*(undefined8 *)(param_1 + 0xb8),0);
    }
    uVar9 = *(undefined8 *)(param_1 + 0xb8);
    uVar8 = (**(code **)(**(long **)(param_1 + 0x68) + 0x10))();
    FUN_0598f19c(uVar9,uVar8);
    if (*(long *)(param_1 + 0xa0) == 0) {
      self = 0;
    }
    else {
      self = *(ID *)(*(long *)(param_1 + 0xa0) + 8);
    }
    _objc_msgSend(self,"setHost:",0);
    FUN_00279dc0(&local_70);
    FUN_00279dc0(&local_60);
    FUN_00279dc0(&local_50);
    FUN_00279dc0(&local_a8);
  }
  FUN_022f2940(&local_a8,param_1);
  for (plVar6 = local_a0; plVar6 != &local_a8; plVar6 = (long *)plVar6[1]) {
    lVar7 = plVar6[2];
    FUN_0295d2d4(lVar7,plVar3,*(undefined8 *)(param_1 + 0x88));
    lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x68))();
    if (lVar4 != 0) {
      (**(code **)(**(long **)(lVar7 + 0x438) + 0x20))();
    }
    if (*(long *)(lVar7 + 0x1f0) != 0) {
      FUN_024983a0(*(long *)(lVar7 + 0x1f0),lVar4);
    }
  }
  if (local_98 != 0) {
    plVar3 = *(long **)(local_a8 + 8);
    *(long **)(*local_a0 + 8) = plVar3;
    *plVar3 = *local_a0;
    local_98 = 0;
    plVar3 = local_a0;
    while (plVar3 != &local_a8) {
      if ((plVar3 == (long *)0xfffffffffffffff0) || (plVar3 == (long *)0x0)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x2bd5770);
        (*pcVar1)();
      }
      plVar6 = (long *)plVar3[1];
      FUN_00005810(plVar3,0x18);
      plVar3 = plVar6;
    }
  }
  return;
}


