// ==== 0x577fc24  FUN_0577fc24  (136 bytes)

long * FUN_0577fc24(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar1 = param_1[1];
  lVar3 = param_1[2] * 0x30;
  if (lVar3 == 0) {
    lVar3 = lVar1 + param_1[2] * 0x30;
    lVar2 = lVar1;
  }
  else {
    do {
      lVar3 = lVar3 + -0x30;
      FUN_0577ef10(lVar3 + lVar1);
    } while (lVar3 != 0);
    lVar2 = param_1[1];
    lVar3 = lVar1;
  }
  param_1[2] = (lVar3 - lVar2 >> 4) * -0x5555555555555555;
  if (*param_1 != 0) {
    FUN_00005810(*param_1,param_1[3] * 0x30);
  }
  return param_1;
}


