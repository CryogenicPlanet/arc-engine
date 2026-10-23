// ==== 0x6bd82ac  FUN_06bd82ac  (152 bytes)

void FUN_06bd82ac(undefined8 *param_1,undefined8 *param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  undefined8 local_38 [2];
  undefined7 local_28;
  char cStack_21;
  
  if (*(char *)(param_2 + 5) != '\0') {
    if (*(char *)(param_2 + 5) == '\x01') {
      FUN_06bd815c(local_38);
      FUN_06bd9710(param_1,local_38);
      if (cStack_21 < '\0') {
        FUN_06bd9108(local_38,local_38[0],CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
      }
      return;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x6bd82ec);
    (*pcVar1)();
  }
  if (-1 < *(char *)((long)param_2 + 0x17)) {
    uVar2 = *param_2;
    param_1[1] = param_2[1];
    *param_1 = uVar2;
    param_1[2] = param_2[2];
    return;
  }
  FUN_00054420(param_1,*param_2,param_2[1]);
  return;
}


