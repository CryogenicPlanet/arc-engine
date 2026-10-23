// ==== 0x43ded58  FUN_043ded58  (32 bytes)

void FUN_043ded58(undefined8 *param_1,ID param_2)

{
  if (param_2 != 0) {
    _objc_msgSend(param_2,"actionData");
    return;
  }
  param_1[4] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  param_1[3] = 0;
  param_1[2] = 0;
  return;
}


