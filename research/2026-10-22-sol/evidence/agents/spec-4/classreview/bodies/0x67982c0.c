// ==== 0x67982c0  FUN_067982c0  (176 bytes)

void FUN_067982c0(undefined8 param_1,long param_2)

{
  undefined8 local_50;
  undefined4 local_44;
  char *local_40;
  char *local_38;
  
  local_50 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_40 = "scale_factor";
    local_38 = "WebContentsImpl::SetPageScale";
    local_44 = (undefined4)param_1;
    FUN_067a7ba8(DAT_0e4a4478,&local_50,&local_38,1,&local_40,&local_44);
  }
  FUN_041520a8(*(undefined8 *)(param_2 + 0x8d8));
  FUN_0573dc28(param_1);
  local_40 = (char *)0x28;
  if (DAT_0e4a4478 != '\0') {
    local_38 = (char *)0x0;
    FUN_0000a280(DAT_0e4a4478,&local_40,&local_38,2);
  }
  return;
}


