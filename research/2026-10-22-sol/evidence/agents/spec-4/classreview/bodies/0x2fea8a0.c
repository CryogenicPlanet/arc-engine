// ==== 0x2fea8a0  FUN_02fea8a0  (176 bytes)

void FUN_02fea8a0(long param_1)

{
  undefined8 local_30;
  char *local_28;
  
  local_30 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_28 = "WebContentsImpl::Stop";
    FUN_0000a280(DAT_0e4a4478,&local_30,&local_28,1);
  }
  FUN_026a3e48(param_1,&local_28,&LAB_02e3f4f8);
  FUN_02fe9904(*(undefined8 *)(s__System_Library_Frameworks_CoreT_000010e8 + param_1 + 0x38),0x18);
  FUN_01dc0dd8(param_1 + 0x158,0x1a0,1);
  local_30 = 0x28;
  if (DAT_0e4a4478 != '\0') {
    local_28 = (char *)0x0;
    FUN_0000a280(DAT_0e4a4478,&local_30,&local_28,2);
  }
  return;
}


