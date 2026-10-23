// ==== 0x8b0218c  FUN_08b0218c  (144 bytes)

void FUN_08b0218c(undefined1 *param_1,undefined8 param_2,undefined8 param_3)

{
  ulong uVar1;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  
  local_40 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  FUN_0376f240(&uStack_50,0,0);
  uVar1 = FUN_04053be0(param_2,&local_60);
  if ((uVar1 & 1) == 0) {
    *param_1 = 0;
    param_1[0x18] = 0;
  }
  else {
    FUN_08b02038(param_1,&local_60,param_3,0,0);
  }
  thunk_FUN_005f0018(&uStack_50);
  return;
}


