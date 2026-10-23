// ==== 0x430dbcc  FUN_0430dbcc  (96 bytes)

void FUN_0430dbcc(undefined8 *param_1,long *param_2)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  param_2 = (long *)*param_2;
  if (param_2 == (long *)0x0) {
    *(undefined1 *)param_1 = 0;
  }
  else {
    (**(code **)(*param_2 + 0xa8))(&local_38);
    param_1[1] = uStack_30;
    *param_1 = local_38;
    param_1[2] = local_28;
  }
  *(bool *)(param_1 + 3) = param_2 != (long *)0x0;
  return;
}


