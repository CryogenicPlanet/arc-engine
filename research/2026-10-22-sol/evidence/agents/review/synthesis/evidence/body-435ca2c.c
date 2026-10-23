// ==== 0x435ca2c  FUN_0435ca2c  (236 bytes)

void FUN_0435ca2c(long param_1)

{
  ulong local_60;
  undefined8 local_58;
  undefined8 local_50;
  ulong uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  code *local_30;
  long local_28;
  long local_18;
  
  local_18 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_38 = *(undefined8 *)(param_1 + 0x30);
  local_40 = *(undefined8 *)(param_1 + 0x28);
  local_30 = *(code **)(param_1 + 0x38);
  local_28 = *(long *)(param_1 + 0x40);
  if (*(long *)(local_28 + 8) != 0) {
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined8 **)(param_1 + 0x40) = &DAT_0c4b3500;
  }
  local_60 = *(ulong *)(param_1 + 0x48);
  if (local_60 == 1) {
    local_58 = *(undefined8 *)(param_1 + 0x50);
    uStack_48 = *(ulong *)(param_1 + 0x60);
    local_50 = *(undefined8 *)(param_1 + 0x58);
    *(undefined8 *)(param_1 + 0x58) = 0;
    *(undefined8 *)(param_1 + 0x60) = 0;
    *(undefined8 *)(param_1 + 0x50) = 0;
  }
  else {
    *(undefined8 *)(param_1 + 0x48) = 0x37;
  }
  (*local_30)(&local_40,&local_60);
  if (local_60 == 1) {
    if ((long)uStack_48 < 0) {
      FUN_00005810(local_58,uStack_48 & 0x7fffffffffffffff);
    }
  }
  else if ((local_60 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(code **)(local_28 + 8) != (code *)0x0) {
    (**(code **)(local_28 + 8))(local_40);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


