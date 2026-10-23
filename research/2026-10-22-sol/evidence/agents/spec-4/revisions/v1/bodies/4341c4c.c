// ==== 0x4341c4c  FUN_04341c4c  (224 bytes)

void FUN_04341c4c(undefined8 *param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  int iVar2;
  undefined8 *local_a0;
  long lStack_98;
  long local_90;
  undefined8 *local_88;
  undefined8 *puStack_80;
  long local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  ulong local_58;
  undefined8 local_50 [5];
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*(long *)*param_2 != 0) {
    local_50[1] = 0;
    local_50[0] = 0;
    local_50[3] = 0;
    local_50[2] = 0;
    local_58 = 0;
    iVar2 = FUN_044614ac(*(long *)*param_2,local_50,&local_58,0x20,param_3,param_4);
    if (iVar2 != 0) {
      if (0x20 < local_58) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x4341d2c);
        (*pcVar1)();
      }
      local_a0 = local_50;
      lStack_98 = (long)local_a0 + local_58;
      local_90 = lStack_98;
      local_88 = local_a0;
      puStack_80 = local_a0;
      local_78 = lStack_98;
      FUN_054dd308(&local_70,&local_88,&local_a0);
      *param_1 = local_70;
      param_1[2] = uStack_60;
      param_1[1] = local_68;
      *(undefined1 *)(param_1 + 3) = 1;
      goto LAB_04341cac;
    }
  }
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 3) = 0;
LAB_04341cac:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


