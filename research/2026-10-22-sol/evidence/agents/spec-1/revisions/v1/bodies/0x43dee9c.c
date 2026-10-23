// ==== 0x43dee9c  ArcHSLAMatrix::hslaMatrix  (36 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcHSLAMatrix::hslaMatrix(HSLAMatrix *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  uVar6 = *(undefined8 *)(param_1 + 0x50);
  uVar5 = *(undefined8 *)(param_1 + 0x48);
  uVar2 = *(undefined8 *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  uVar4 = *(undefined8 *)(param_1 + 0x20);
  uVar3 = *(undefined8 *)(param_1 + 0x18);
  __return_storage_ptr__->field2_0x8 = (float)(int)uVar2;
  __return_storage_ptr__->field3_0xc = (float)(int)((ulong)uVar2 >> 0x20);
  __return_storage_ptr__->field0_0x0 = (float)(int)uVar1;
  __return_storage_ptr__->field1_0x4 = (float)(int)((ulong)uVar1 >> 0x20);
  __return_storage_ptr__->field6_0x18 = (float)(int)uVar4;
  __return_storage_ptr__->field7_0x1c = (float)(int)((ulong)uVar4 >> 0x20);
  __return_storage_ptr__->field4_0x10 = (float)(int)uVar3;
  __return_storage_ptr__->field5_0x14 = (float)(int)((ulong)uVar3 >> 0x20);
  uVar2 = *(undefined8 *)(param_1 + 0x30);
  uVar1 = *(undefined8 *)(param_1 + 0x28);
  uVar4 = *(undefined8 *)(param_1 + 0x40);
  uVar3 = *(undefined8 *)(param_1 + 0x38);
  __return_storage_ptr__->field10_0x28 = (float)(int)uVar2;
  __return_storage_ptr__->field11_0x2c = (float)(int)((ulong)uVar2 >> 0x20);
  __return_storage_ptr__->field8_0x20 = (float)(int)uVar1;
  __return_storage_ptr__->field9_0x24 = (float)(int)((ulong)uVar1 >> 0x20);
  __return_storage_ptr__->field14_0x38 = (float)(int)uVar4;
  __return_storage_ptr__->field15_0x3c = (float)(int)((ulong)uVar4 >> 0x20);
  __return_storage_ptr__->field12_0x30 = (float)(int)uVar3;
  __return_storage_ptr__->field13_0x34 = (float)(int)((ulong)uVar3 >> 0x20);
  __return_storage_ptr__->field18_0x48 = (float)(int)uVar6;
  __return_storage_ptr__->field19_0x4c = (float)(int)((ulong)uVar6 >> 0x20);
  __return_storage_ptr__->field16_0x40 = (float)(int)uVar5;
  __return_storage_ptr__->field17_0x44 = (float)(int)((ulong)uVar5 >> 0x20);
  return;
}


