// ==== 0x577ee50  FUN_0577ee50  (184 bytes)

void FUN_0577ee50(undefined8 *param_1)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  long lVar4;
  
  plVar3 = (long *)*param_1;
  lVar4 = *plVar3;
  if (lVar4 == 0) {
    return;
  }
  lVar2 = plVar3[1] * 0x30;
  lVar1 = lVar4;
  if (lVar2 != 0) {
    lVar1 = plVar3[1] * -0x30;
    lVar2 = lVar2 + lVar4;
    do {
      lVar2 = lVar2 + -0x30;
      FUN_0577ef10(lVar2);
      lVar1 = lVar1 + 0x30;
    } while (lVar1 != 0);
    lVar1 = *plVar3;
  }
  plVar3[1] = (lVar4 - lVar1 >> 4) * -0x5555555555555555;
  FUN_00005810(*(undefined8 *)*param_1,((undefined8 *)*param_1)[2] * 0x30);
  return;
}


