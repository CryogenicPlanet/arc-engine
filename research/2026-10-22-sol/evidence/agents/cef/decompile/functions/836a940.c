// ==== 0x836a940  FUN_0836a940  (248 bytes)

long * FUN_0836a940(long *param_1,ulong param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  
  *param_1 = 0;
  param_1[1] = 0;
  uVar1 = FUN_0254a844();
  uVar1 = thunk_FUN_0009278c(uVar1,param_2,1);
  uVar2 = FUN_018d0a2c(param_2,0);
  uVar3 = FUN_03287330(param_2,0);
  lVar4 = thunk_FUN_00008790(0x178);
  FUN_06c06980(lVar4,uVar1,uVar2,uVar3,0);
  lVar5 = param_1[1];
  param_1[1] = lVar4;
  if (lVar5 != 0) {
    uVar1 = FUN_06c06af8();
    thunk_FUN_00005810(uVar1,0x178);
    lVar4 = param_1[1];
  }
  lVar5 = thunk_FUN_00008790(0x80);
  FUN_078ca4c8(lVar5,lVar4,0);
  lVar4 = *param_1;
  *param_1 = lVar5;
  if (lVar4 != 0) {
    uVar1 = FUN_078ca584();
    thunk_FUN_00005810(uVar1,0x80);
  }
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    thunk_FUN_0001fee4(param_2);
  }
  return param_1;
}


