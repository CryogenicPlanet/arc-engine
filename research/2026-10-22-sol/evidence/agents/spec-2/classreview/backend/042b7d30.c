// ==== 0x42b7d30  FUN_042b7d30  (136 bytes)

void FUN_042b7d30(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  long lVar3;
  code *pcVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  lVar3 = FUN_043289d0();
  uVar7 = param_2[3];
  uVar6 = param_2[2];
  if (*(long *)(param_2[3] + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  uVar1 = *param_2;
  uVar2 = param_2[1];
  lVar5 = *(long *)(lVar3 + 0x250);
  *(undefined8 **)(lVar3 + 0x250) = &DAT_0c4b3500;
  *(undefined8 *)(lVar3 + 0x248) = 0;
  pcVar4 = *(code **)(lVar5 + 8);
  if (pcVar4 != (code *)0x0) {
    (*pcVar4)(*(undefined8 *)(lVar3 + 0x238));
  }
  *(undefined8 *)(lVar3 + 0x238) = uVar1;
  *(undefined8 *)(lVar3 + 0x240) = uVar2;
  *(undefined8 *)(lVar3 + 0x250) = uVar7;
  *(undefined8 *)(lVar3 + 0x248) = uVar6;
  return;
}


