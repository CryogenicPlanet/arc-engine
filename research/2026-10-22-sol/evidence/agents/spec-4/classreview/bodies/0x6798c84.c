// ==== 0x6798c84  FUN_06798c84  (172 bytes)

void FUN_06798c84(long param_1,undefined8 param_2)

{
  undefined8 local_40;
  char *local_38;
  
  local_40 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_38 = "WebContentsImpl::ExitFullscreen";
    FUN_0000a280(DAT_0e4a4478,&local_40,&local_38,1);
  }
  FUN_028d8e38(*(undefined8 *)(*(long *)(*(long *)(param_1 + 0x8d8) + 0x110) + 0x38),3);
  FUN_06790bf4(param_1,param_2);
  local_40 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_38 = (char *)0x0;
    FUN_0000a280(DAT_0e4a4478,&local_40,&local_38,2);
  }
  return;
}


