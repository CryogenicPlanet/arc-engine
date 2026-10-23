// ==== 0x43df00c  FUN_043df00c  (432 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043df00c(undefined8 *param_1,ID param_2)

{
  ID IVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  param_1[1] = 0;
  *param_1 = 0;
  param_1[3] = 0;
  param_1[2] = 0;
  param_1[7] = 0;
  param_1[6] = 0;
  puVar2 = param_1 + 4;
  param_1[5] = 0;
  *puVar2 = 0;
  puVar3 = param_1 + 1;
  param_1[2] = 0;
  *puVar3 = 0;
  *(undefined4 *)param_1 = 4;
  param_1[4] = 0;
  param_1[3] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  *(undefined4 *)(param_1 + 7) = 0;
  IVar1 = _objc_msgSend(param_2,"eventType");
  if (IVar1 < 5) {
    *(int *)param_1 = (int)IVar1;
  }
  IVar1 = _objc_msgSend(param_2,"domCode");
  if (IVar1 == 0) {
    *puVar3 = 0;
    param_1[2] = 0;
    param_1[3] = 0;
  }
  else {
    IVar1 = _objc_msgSend(param_2,"domCode");
    if (IVar1 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,IVar1);
    }
    param_1[2] = uStack_50;
    *puVar3 = local_58;
    param_1[3] = local_48;
  }
  IVar1 = _objc_msgSend(param_2,"key");
  if (IVar1 == 0) {
    *puVar2 = 0;
    param_1[5] = 0;
    param_1[6] = 0;
  }
  else {
    IVar1 = _objc_msgSend(param_2,"key");
    if (IVar1 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,IVar1);
    }
    param_1[5] = uStack_50;
    *puVar2 = local_58;
    param_1[6] = local_48;
  }
  IVar1 = _objc_msgSend(param_2,"windowsVirtualKeyCode");
  *(int *)(param_1 + 7) = (int)IVar1;
  return 0;
}


