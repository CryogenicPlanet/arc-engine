// ==== 0x6afdb60  FUN_06afdb60  (120 bytes)

void FUN_06afdb60(undefined8 param_1,undefined8 *param_2)

{
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_40,*param_2,param_2[1]);
  }
  else {
    uStack_38 = param_2[1];
    local_40 = *param_2;
    local_30 = param_2[2];
  }
  FUN_06afdbd8(param_1,&local_40,1);
  if ((long)local_30 < 0) {
    FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
  }
  return;
}


