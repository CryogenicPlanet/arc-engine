// ==== 0x4376d10  FUN_04376d10  (140 bytes)

void FUN_04376d10(undefined8 param_1,undefined8 *param_2)

{
  code *pcVar1;
  long lVar2;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  local_60 = 0;
  uStack_58 = 0;
  lVar2 = param_2[3];
  uStack_48 = param_2[1];
  local_50 = *param_2;
  uStack_38 = param_2[3];
  uStack_40 = param_2[2];
  if (*(long *)(lVar2 + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  FUN_04376da0(param_1,&local_60,&local_50);
  pcVar1 = *(code **)(lVar2 + 8);
  if (pcVar1 != (code *)0x0) {
    (*pcVar1)(local_50);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


