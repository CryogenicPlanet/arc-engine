// ==== 0x42b7e3c  FUN_042b7e3c  (100 bytes)

void FUN_042b7e3c(long param_1,undefined8 *param_2)

{
  long lVar1;
  code *pcVar2;
  undefined8 uVar3;
  
  lVar1 = *(long *)(param_1 + 0x188);
  *(undefined8 *)(param_1 + 0x180) = 0;
  *(undefined8 **)(param_1 + 0x188) = &DAT_0c4b3500;
  pcVar2 = *(code **)(lVar1 + 8);
  if (pcVar2 != (code *)0x0) {
    (*pcVar2)(*(undefined8 *)(param_1 + 0x170));
  }
  uVar3 = *param_2;
  *(undefined8 *)(param_1 + 0x178) = param_2[1];
  *(undefined8 *)(param_1 + 0x170) = uVar3;
  *(undefined8 *)(param_1 + 0x180) = param_2[2];
  *(undefined8 *)(param_1 + 0x188) = param_2[3];
  param_2[2] = 0;
  param_2[3] = &DAT_0c4b3500;
  return;
}


