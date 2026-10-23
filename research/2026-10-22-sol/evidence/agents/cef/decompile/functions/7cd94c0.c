// ==== 0x7cd94c0  FUN_07cd94c0  (184 bytes)

void FUN_07cd94c0(long param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 uVar1;
  long local_50;
  long local_48;
  undefined8 uStack_40;
  undefined1 *local_38;
  
  local_48 = 0;
  uStack_40 = 0;
  local_50 = 0;
  FUN_02f555f4(*(undefined8 *)(param_1 + 0x78),"extensions.pinned_extensions",&local_50);
  if ((int)param_3 == 0) {
    uVar1 = FUN_02e81294(local_50,local_50 + local_48 * 0x18,param_2);
    FUN_00a07000(&local_50,uVar1,local_50 + local_48 * 0x18);
  }
  else {
    FUN_00ad9ee8(&local_50,param_2);
  }
  thunk_FUN_06af9204(*(undefined8 *)(param_1 + 0x78),&local_50);
  FUN_0781b63c(*(undefined8 *)(param_1 + 0x88),param_2,param_3);
  local_38 = (undefined1 *)&local_50;
  FUN_01a80e40(&local_38);
  return;
}


