// ==== 0x430e358  FUN_0430e358  (176 bytes)

void FUN_0430e358(undefined8 *param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  lVar1 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
  if (lVar1 == 0) {
    if (*(char *)(param_2 + 0x38) == '\0') {
      *param_1 = 0;
      param_1[1] = 0;
      param_1[2] = 0;
    }
    else {
      if (*(char *)(param_2 + 0x37) < '\0') {
        uVar2 = *(undefined8 *)(param_2 + 0x20);
        uVar3 = *(undefined8 *)(param_2 + 0x28);
        goto LAB_0430e3f8;
      }
      uVar2 = *(undefined8 *)(param_2 + 0x20);
      param_1[1] = *(undefined8 *)(param_2 + 0x28);
      *param_1 = uVar2;
      param_1[2] = *(undefined8 *)(param_2 + 0x30);
    }
  }
  else {
    lVar1 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
    if (*(char *)(lVar1 + 0x2af) < '\0') {
      uVar2 = *(undefined8 *)(lVar1 + 0x298);
      uVar3 = *(undefined8 *)(lVar1 + 0x2a0);
LAB_0430e3f8:
      FUN_00060a60(param_1,uVar2,uVar3);
      return;
    }
    uVar3 = *(undefined8 *)(lVar1 + 0x2a0);
    uVar2 = *(undefined8 *)(lVar1 + 0x298);
    param_1[2] = *(undefined8 *)(lVar1 + 0x2a8);
    param_1[1] = uVar3;
    *param_1 = uVar2;
  }
  return;
}


