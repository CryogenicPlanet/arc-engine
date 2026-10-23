// ==== 0xbd58aa0  FUN_0bd58aa0  (268 bytes)

void FUN_0bd58aa0(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 local_50;
  char *local_48;
  
  local_50 = 0x6f;
  if (DAT_0e4a44bf != '\0') {
    local_48 = "AutocompleteController::DeleteMatch";
    thunk_FUN_0000a280(DAT_0e4a44bf,&local_50,&local_48,1);
  }
  lVar2 = param_2[0xa1] * 0x6b0;
  if (lVar2 != 0) {
    puVar1 = (undefined8 *)param_2[0xa0];
    do {
      if (*(char *)((long)puVar1 + 0x14) != '\0') {
        (**(code **)(*(long *)*puVar1 + 0x18))((long *)*puVar1,puVar1);
      }
      puVar1 = puVar1 + 0xd6;
      lVar2 = lVar2 + -0x6b0;
    } while (lVar2 != 0);
  }
  if ((*(byte *)((long)param_2 + 0x14) & 1) != 0) {
    FUN_0bd58bac(param_2);
    (**(code **)(*(long *)*param_2 + 0x18))((long *)*param_2,param_2);
  }
  thunk_FUN_02835204(param_1,8);
  local_50 = 0x6f;
  if (DAT_0e4a44bf != '\0') {
    local_48 = (char *)0x0;
    thunk_FUN_0000a280(DAT_0e4a44bf,&local_50,&local_48,2);
  }
  return;
}


