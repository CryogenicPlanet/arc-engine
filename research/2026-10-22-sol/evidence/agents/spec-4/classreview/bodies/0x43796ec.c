// ==== 0x43796ec  FUN_043796ec  (680 bytes)

void FUN_043796ec(long *param_1,long param_2)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  long *plVar4;
  undefined8 uVar5;
  undefined8 extraout_x1;
  long lVar6;
  undefined1 uVar7;
  undefined8 local_b0;
  undefined1 uStack_a8;
  undefined7 uStack_a7;
  undefined1 uStack_a0;
  undefined8 uStack_9f;
  undefined3 local_90;
  undefined1 uStack_8d;
  undefined3 uStack_8c;
  undefined8 local_88;
  undefined1 local_80;
  undefined7 uStack_7f;
  undefined1 uStack_78;
  undefined7 uStack_77;
  undefined1 local_70;
  undefined3 local_6f;
  undefined1 uStack_6c;
  undefined3 uStack_6b;
  undefined7 local_68;
  undefined1 uStack_61;
  undefined7 uStack_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  lVar2 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),
                       *(undefined8 *)(param_2 + 0x130));
  if ((((lVar2 == 0) || (lVar2 = FUN_0031dab4(), lVar2 == 0)) || (*(long *)(lVar2 + 0x28) == 0)) ||
     ((*(char *)(*(long *)(lVar2 + 0x28) + 4) != '\0' || (*(long *)(lVar2 + 0x30) == 0)))) {
    lVar2 = 0;
  }
  else {
    lVar6 = 0;
    if (*(long *)(lVar2 + 0x28) != 0) {
      if (*(char *)(*(long *)(lVar2 + 0x28) + 4) == '\0') {
        lVar6 = *(long *)(lVar2 + 0x30);
      }
      else {
        lVar6 = 0;
      }
    }
    lVar2 = 0;
    if (lVar6 != 0) {
      lVar2 = lVar6 + -0x28;
    }
  }
  uVar3 = FUN_0678f310(*(undefined8 *)(param_2 + 0x130));
  local_88 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_80 = 0xa2;
    uStack_7f = 0xd1dba;
    FUN_0000a280(DAT_0e4a4478,&local_88,&local_80,1);
  }
  FUN_026a2e00(uVar3,0,1);
  local_88 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_80 = 0;
    uStack_7f = 0;
    FUN_0000a280(DAT_0e4a4478,&local_88,&local_80,2);
  }
  plVar4 = (long *)(**(code **)(*(long *)(lVar2 + 0x28) + 0x78))((long *)(lVar2 + 0x28));
  iVar1 = FUN_00027de0(plVar4,*(undefined8 *)(param_2 + 0x130));
  local_68 = 0;
  uStack_61 = 0;
  uStack_60 = 0;
  local_90 = 0;
  uStack_8d = 0;
  uStack_8c = 0;
  if ((iVar1 < 0) || (*(int *)(*plVar4 + 0x18) <= iVar1)) {
    uVar7 = 0;
    local_70 = 0;
  }
  else {
    lVar2 = FUN_003abe30(*plVar4,iVar1);
    uVar7 = *(undefined1 *)(lVar2 + 0x80);
    local_68 = (undefined7)*(undefined8 *)(lVar2 + 0x81);
    uStack_61 = (undefined1)*(undefined8 *)(lVar2 + 0x88);
    uStack_60 = (undefined7)((ulong)*(undefined8 *)(lVar2 + 0x88) >> 8);
    local_70 = *(undefined1 *)(lVar2 + 0x90);
    local_90 = (undefined3)*(undefined4 *)(lVar2 + 0x91);
    uStack_8d = (undefined1)*(undefined4 *)(lVar2 + 0x94);
    uStack_8c = (undefined3)((uint)*(undefined4 *)(lVar2 + 0x94) >> 8);
  }
  FUN_043713e0(uVar3);
  uVar5 = FUN_00008790(0x340);
  FUN_038271bc(uVar5,uVar3,plVar4);
  uStack_7f = local_68;
  uStack_78 = uStack_61;
  uStack_77 = uStack_60;
  _local_6f = CONCAT13(uStack_8d,local_90);
  uStack_6b = uStack_8c;
  local_80 = uVar7;
  FUN_0422a9e4(plVar4,iVar1 + 1,uVar5,8,&local_80,0,0);
  (**(code **)(**(long **)(*(long *)(param_2 + 0x130) + 0xe8) + 0x28))();
  local_80 = 0;
  uStack_7f = 0;
  uStack_78 = (undefined1)extraout_x1;
  uStack_77 = (undefined7)((ulong)extraout_x1 >> 8);
  FUN_0679fa60(uVar3,&local_80);
  *param_1 = 0;
  param_1[1] = 0;
  local_b0 = *(undefined8 *)(param_2 + 0x588);
  uStack_a8 = (undefined1)*(undefined8 *)(param_2 + 0x590);
  uStack_9f = *(undefined8 *)(param_2 + 0x599);
  uStack_a7 = (undefined7)*(undefined8 *)(param_2 + 0x591);
  uStack_a0 = (undefined1)((ulong)*(undefined8 *)(param_2 + 0x591) >> 0x38);
  FUN_04346100(param_1,uVar3,&local_b0);
  lVar2 = *param_1;
  uVar3 = *(undefined8 *)(lVar2 + 0x2f8);
  uVar5 = *(undefined8 *)(param_2 + 0x2f8);
  if (*(long *)(param_2 + 0x300) != 0) {
    plVar4 = (long *)(*(long *)(param_2 + 0x300) + 8);
    lVar2 = *plVar4;
    *plVar4 = lVar2 + 1;
  }
  FUN_042aecf8(lVar2,uVar3,uVar5);
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


