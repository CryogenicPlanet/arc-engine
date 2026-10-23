// ==== 0x4b37cec  FUN_04b37cec  (176 bytes)

void FUN_04b37cec(ulong *param_1)

{
  char *pcVar1;
  undefined4 local_48;
  undefined4 uStack_44;
  char *local_40;
  char *pcStack_38;
  
  local_40 = "../../third_party/abseil-cpp/absl/status/statusor.cc";
  pcStack_38 = "An OK status is not a valid constructor argument to StatusOr<T>";
  local_48 = 0x4d;
  uStack_44 = 2;
  FUN_04b37d9c(&uStack_44,&local_40,&local_48,&pcStack_38);
  local_40 = (char *)0x0;
  FUN_04b36d14(&local_40,"An OK status is not a valid constructor argument to StatusOr<T>",0x3f,0x52
               ,"../../third_party/abseil-cpp/absl/status/statusor.cc");
  pcVar1 = (char *)*param_1;
  if (local_40 != pcVar1) {
    *param_1 = (ulong)local_40;
    local_40 = (char *)0x37;
    if (((ulong)pcVar1 & 1) != 0) {
      return;
    }
    FUN_04b35898(pcVar1);
  }
  if (((ulong)local_40 & 1) == 0) {
    FUN_04b35898();
  }
  return;
}


