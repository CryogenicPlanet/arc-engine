// ==== 0x4341d34  FUN_04341d34  (264 bytes)

void FUN_04341d34(undefined8 *param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  int iVar2;
  undefined8 *local_c0;
  long lStack_b8;
  long local_b0;
  undefined8 *local_a8;
  undefined8 *puStack_a0;
  long local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined8 local_70 [9];
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*(long *)*param_2 != 0) {
    local_70[5] = 0;
    local_70[4] = 0;
    local_70[7] = 0;
    local_70[6] = 0;
    local_70[1] = 0;
    local_70[0] = 0;
    local_70[3] = 0;
    local_70[2] = 0;
    local_78 = 0;
    iVar2 = FUN_044617b4(*(long *)*param_2,local_70,&local_78,0x40,param_3,param_4);
    if (iVar2 != 0) {
      if (0x40 < local_78) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x4341e3c);
        (*pcVar1)();
      }
      local_90 = 0;
      local_88 = 0;
      uStack_80 = 0;
      local_c0 = local_70;
      lStack_b8 = (long)local_c0 + local_78;
      local_b0 = lStack_b8;
      local_a8 = local_c0;
      puStack_a0 = local_c0;
      local_98 = lStack_b8;
      FUN_054dd308(&local_90,&local_a8,&local_c0);
      local_70[5] = 0;
      local_70[4] = 0;
      local_70[7] = 0;
      local_70[6] = 0;
      local_70[1] = 0;
      local_70[0] = 0;
      local_70[3] = 0;
      local_70[2] = 0;
      *param_1 = local_90;
      param_1[2] = uStack_80;
      param_1[1] = local_88;
      *(undefined1 *)(param_1 + 3) = 1;
      goto LAB_04341da8;
    }
    local_70[5] = 0;
    local_70[4] = 0;
    local_70[7] = 0;
    local_70[6] = 0;
    local_70[1] = 0;
    local_70[0] = 0;
    local_70[3] = 0;
    local_70[2] = 0;
  }
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 3) = 0;
LAB_04341da8:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


