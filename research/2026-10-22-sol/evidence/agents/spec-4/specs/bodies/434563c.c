// ==== 0x434563c  FUN_0434563c  (488 bytes)

void FUN_0434563c(long *param_1,long *param_2)

{
  long *plVar1;
  undefined1 **ppuVar2;
  char cVar3;
  ulong uVar4;
  undefined8 ****ppppuVar5;
  long lVar6;
  undefined1 *local_e0;
  long lStack_d8;
  undefined8 local_d0;
  undefined8 ***local_c0;
  ulong uStack_b8;
  undefined8 local_b0;
  undefined1 *local_a8;
  long lStack_a0;
  long local_98;
  undefined8 local_88;
  undefined7 local_78;
  char cStack_71;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_58 = 0xffffffff00000000;
  local_c0 = (undefined8 ****)0x0;
  uStack_b8 = 0;
  local_b0 = 0;
  cVar3 = *(char *)((long)param_2 + 0x17);
  plVar1 = (long *)*param_2;
  if (-1 < (long)cVar3) {
    plVar1 = param_2;
  }
  lVar6 = param_2[1];
  if (-1 < cVar3) {
    lVar6 = (long)cVar3;
  }
  FUN_01047aa0(&local_c0,plVar1,lVar6,&local_60);
  uVar4 = uStack_b8;
  if (-1 < (long)local_b0) {
    uVar4 = local_b0 >> 0x38;
  }
  if (uVar4 == 0) {
LAB_043456f8:
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      ppppuVar5 = (undefined8 ****)*param_2;
      uVar4 = param_2[1];
LAB_043456ec:
      FUN_00060a60(param_1,ppppuVar5,uVar4);
      goto LAB_04345710;
    }
    lVar6 = *param_2;
    param_1[1] = param_2[1];
    *param_1 = lVar6;
    uVar4 = param_2[2];
  }
  else {
    if (((uint)local_60 & 0xfffffffe) != 2) {
      ppppuVar5 = (undefined8 ****)local_c0;
      if (-1 < (long)local_b0) {
        ppppuVar5 = &local_c0;
      }
      uVar4 = FUN_013e0f54(ppppuVar5);
      if ((uVar4 & 1) != 0) {
        ppppuVar5 = (undefined8 ****)local_c0;
        if (-1 < (long)local_b0._7_1_) {
          ppppuVar5 = &local_c0;
        }
        uVar4 = uStack_b8;
        if (-1 < (long)local_b0) {
          uVar4 = (long)local_b0._7_1_;
        }
        FUN_00788520(&local_a8,ppppuVar5,uVar4,0,0);
        lStack_d8 = lStack_a0;
        local_e0 = local_a8;
        local_d0 = local_98;
        lStack_a0 = 0;
        local_98 = 0;
        local_a8 = (undefined1 *)0x0;
        if ((cStack_71 < '\0') &&
           (FUN_00005810(local_88,CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff), local_98 < 0))
        {
          FUN_00005810(local_a8,local_98 << 1);
        }
        ppuVar2 = (undefined1 **)local_e0;
        if (-1 < (long)local_d0._7_1_) {
          ppuVar2 = &local_e0;
        }
        lVar6 = lStack_d8;
        if (-1 < local_d0) {
          lVar6 = (long)local_d0._7_1_;
        }
        FUN_00af22f8(param_1,ppuVar2,lVar6);
        if (local_d0 < 0) {
          FUN_00005810(local_e0,local_d0 << 1);
        }
        goto LAB_04345710;
      }
      goto LAB_043456f8;
    }
    ppppuVar5 = (undefined8 ****)local_c0;
    uVar4 = uStack_b8;
    if ((long)local_b0 < 0) goto LAB_043456ec;
    param_1[1] = uStack_b8;
    *param_1 = (long)local_c0;
    uVar4 = local_b0;
  }
  param_1[2] = uVar4;
LAB_04345710:
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


