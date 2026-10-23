// ==== 0x42b88c8  FUN_042b88c8  (136 bytes)

void FUN_042b88c8(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  
  uVar1 = FUN_044d84a0();
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_50,*param_2,param_2[1]);
  }
  else {
    uStack_48 = param_2[1];
    local_50 = *param_2;
    local_40 = param_2[2];
  }
  FUN_044d9b00(uVar1,param_1,&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  return;
}


