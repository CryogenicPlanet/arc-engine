// ==== 0x7cd9ea0  FUN_07cd9ea0  (132 bytes)

bool FUN_07cd9ea0(long param_1,undefined8 param_2)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 local_38;
  
  lVar3 = *(long *)(param_1 + 0xc0);
  lVar2 = *(long *)(param_1 + 200);
  lVar5 = lVar2 * 0x18;
  lVar4 = lVar3;
  if (lVar5 != 0) {
    lVar4 = lVar3 + lVar2 * 0x18;
    lVar2 = lVar3;
    local_38 = param_2;
    do {
      uVar1 = FUN_00822540(&local_38,lVar2);
      lVar3 = lVar2;
      if ((uVar1 & 1) != 0) break;
      lVar2 = lVar2 + 0x18;
      lVar5 = lVar5 + -0x18;
      lVar3 = lVar4;
    } while (lVar5 != 0);
    lVar2 = *(long *)(param_1 + 200);
    lVar4 = *(long *)(param_1 + 0xc0);
  }
  return lVar3 != lVar4 + lVar2 * 0x18;
}


