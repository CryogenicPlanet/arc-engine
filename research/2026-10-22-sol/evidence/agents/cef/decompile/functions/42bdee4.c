// ==== 0x42bdee4  FUN_042bdee4  (164 bytes)

void FUN_042bdee4(long param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)

{
  undefined8 uVar1;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  long local_28;
  long local_18;
  
  local_18 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_38 = param_4[1];
  local_40 = *param_4;
  local_28 = param_4[3];
  uStack_30 = param_4[2];
  if (*(long *)(param_4[3] + 8) != 0) {
    param_4[2] = 0;
    param_4[3] = &DAT_0c4b3500;
  }
  uVar1 = FUN_042bdf88(param_1 + 0x348,param_2,param_3,&local_40);
  if (*(code **)(local_28 + 8) == (code *)0x0) {
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
      return;
    }
  }
  else {
    uVar1 = local_40;
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
                    /* WARNING: Could not recover jumptable at 0x042bdf5c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(local_28 + 8))();
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(uVar1);
}


