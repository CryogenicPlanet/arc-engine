// ==== 0x6796e50  FUN_06796e50  (296 bytes)

void FUN_06796e50(long param_1)

{
  ulong uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  char *local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined2 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined2 local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined4 local_40;
  undefined8 local_38;
  
  local_38 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_b8 = "WebContentsImpl::OnSavePage";
    FUN_0000a280(DAT_0e4a4478,&local_38,&local_b8,1);
  }
  uVar1 = FUN_06796cb0(param_1);
  if ((uVar1 & 1) == 0) {
    uVar2 = FUN_02e7e39c(param_1);
    local_b8 = (char *)0x0;
    uStack_b0 = 0;
    local_a0 = 0;
    local_a8 = 0;
    uStack_90 = 0xffffffff00000000;
    local_98 = 0xffffffff00000000;
    uStack_80 = 0xffffffff00000000;
    local_88 = 0xffffffff00000000;
    uStack_70 = 0xffffffff00000000;
    local_78 = 0xffffffff00000000;
    uStack_60 = 0xffffffff00000000;
    local_68 = 0xffffffff00000000;
    local_58 = 0;
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 1;
    FUN_067970fc(param_1,uVar2,&local_b8,*(undefined8 *)(param_1 + 0x8d8));
    FUN_0002822c(&local_b8);
  }
  else {
    FUN_02fea8a0(param_1);
    uVar2 = FUN_00008790(0x2c8);
    uVar3 = FUN_034fca9c(param_1);
    thunk_FUN_0648c7c0(uVar2,uVar3);
    FUN_06796f78(param_1 + 0x948,uVar2);
    FUN_06490b40(*(undefined8 *)(param_1 + 0x948));
  }
  local_38 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_b8 = (char *)0x0;
    FUN_0000a280(DAT_0e4a4478,&local_38,&local_b8,2);
  }
  return;
}


