// ==== 0x4377750  FUN_04377750  (192 bytes)

void FUN_04377750(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 long param_5)

{
  long *plVar1;
  long lVar2;
  undefined1 auVar3 [16];
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined8 local_40;
  char *local_38;
  
  lVar2 = *(long *)(param_5 + 0x130);
  if (lVar2 != 0) {
    local_50 = param_1;
    uStack_4c = param_2;
    local_48 = param_3;
    uStack_44 = param_4;
    auVar3 = FUN_04484338(&local_50);
    local_40 = 0x28;
    if (DAT_0e4a4478 != '\0') {
      local_38 = "WebContentsImpl::SetDisplayCutoutSafeArea";
      FUN_0000a280(DAT_0e4a4478,&local_40,&local_38,1);
    }
    plVar1 = *(long **)((long)&dylib_command_00001030.dylib.current_version + lVar2);
    if (plVar1 != (long *)0x0) {
      (**(code **)(*plVar1 + 0x48))(plVar1,auVar3._0_8_,auVar3._8_8_);
    }
    local_40 = 0x28;
    if (DAT_0e4a4478 != '\0') {
      local_38 = (char *)0x0;
      FUN_0000a280(DAT_0e4a4478,&local_40,&local_38,2);
    }
  }
  return;
}


