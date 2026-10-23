// ==== 0x7b02940  FUN_07b02940  (32 bytes)

void FUN_07b02940(long param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = 2;
  if (param_2 == 0) {
    uVar1 = 0;
  }
  FUN_02fede14(*(undefined8 *)(param_1 + 0x38),"performance_tuning.high_efficiency_mode.state",0x2d,
               uVar1);
  return;
}


