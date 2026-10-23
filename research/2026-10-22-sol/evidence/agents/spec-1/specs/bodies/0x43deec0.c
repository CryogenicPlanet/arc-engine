// ==== 0x43deec0  ArcHSLAMatrix::setHslaMatrix:  (36 bytes)

/* Function Stack Size: 0x60 bytes */

void ArcHSLAMatrix::setHslaMatrix_(ID param_1,SEL param_2,HSLAMatrix *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  uVar2 = *(undefined8 *)&param_3->field2_0x8;
  uVar1 = *(undefined8 *)param_3;
  uVar4 = *(undefined8 *)&param_3->field6_0x18;
  uVar3 = *(undefined8 *)&param_3->field4_0x10;
  uVar6 = *(undefined8 *)&param_3->field10_0x28;
  uVar5 = *(undefined8 *)&param_3->field8_0x20;
  uVar7 = *(undefined8 *)&param_3->field12_0x30;
  *(undefined8 *)(param_1 + 0x40) = *(undefined8 *)&param_3->field14_0x38;
  *(undefined8 *)(param_1 + 0x38) = uVar7;
  *(undefined8 *)(param_1 + 0x30) = uVar6;
  *(undefined8 *)(param_1 + 0x28) = uVar5;
  *(undefined8 *)(param_1 + 0x20) = uVar4;
  *(undefined8 *)(param_1 + 0x18) = uVar3;
  *(undefined8 *)(param_1 + 0x10) = uVar2;
  *(undefined8 *)(param_1 + 8) = uVar1;
  uVar1 = *(undefined8 *)&param_3->field16_0x40;
  *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)&param_3->field18_0x48;
  *(undefined8 *)(param_1 + 0x48) = uVar1;
  return;
}


