// ==== 0x6f26c80  FUN_06f26c80  (92 bytes)

void FUN_06f26c80(undefined8 param_1,long param_2,undefined8 param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 uVar3;
  
  iVar2 = FUN_0419fd48();
  if (iVar2 != 0) {
    FUN_0366cd58(param_2,1);
  }
  uVar1 = *(undefined8 *)(param_2 + 0x18);
  uVar3 = FUN_0190f6b8(*(undefined8 *)(param_2 + 0x10));
  FUN_031c2dc4(param_1,uVar1,uVar3,param_3,0);
  return;
}


