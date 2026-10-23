// ==== 0x41c8  _ChromeMain  (288 bytes)

undefined8
_ChromeMain(undefined4 param_1,ulong param_2,undefined8 param_3,undefined8 param_4,
           undefined8 param_5)

{
  undefined8 uVar1;
  undefined8 local_138;
  undefined4 local_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined4 local_60;
  ulong local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  
  uStack_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  local_98 = 0;
  local_a0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  FUN_0421b464(&local_c0,0);
  uStack_b8 = CONCAT44(uStack_b8._4_4_,param_1);
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  if ((local_b0 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  uStack_48 = local_a0;
  local_50 = uStack_a8;
  local_d0 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  local_130 = (undefined4)uStack_b8;
  uStack_108 = uStack_90;
  local_110 = local_98;
  uStack_f8 = 0;
  uStack_100 = 0;
  local_70 = 0;
  uStack_68 = local_c0;
  local_138 = 0;
  local_60 = (undefined4)uStack_b8;
  uStack_120 = 0;
  local_128 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  local_c0 = 0;
  local_a0 = 0;
  local_118 = 0;
  uStack_38 = uStack_90;
  uStack_40 = local_98;
  uStack_78 = 0;
  local_80 = 0;
  local_58 = param_2;
  uVar1 = FUN_042aa7ec(&uStack_100,&uStack_68,0,param_4,param_5,&local_80);
  FUN_02440774(&uStack_68);
  FUN_02440774(&local_138);
  FUN_042ab360(&uStack_100);
  FUN_042aa684(&uStack_100);
  FUN_02440774(&local_c0);
  return uVar1;
}


