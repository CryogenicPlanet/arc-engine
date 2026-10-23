// ==== 0x669e6e4  FUN_0669e6e4  (112 bytes)

undefined8 FUN_0669e6e4(long param_1,undefined8 param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  if (((DAT_0e495718 & 1) == 0) && (lVar1 = *(long *)(param_1 + 400), lVar1 != 0)) {
    if ((*(byte *)(param_1 + 0x71) & 1) == 0) {
      uVar2 = (*(code *)PTR_FUN_0e2f1ea8)();
      *(undefined8 *)(param_1 + 0x2e8) = uVar2;
      lVar1 = *(long *)(param_1 + 400);
    }
    *(int *)(param_1 + 0x5c8) = (int)param_2;
    *(undefined1 *)(param_1 + 0x71) = 1;
    uVar2 = FUN_063d3600(lVar1,param_2);
    return uVar2;
  }
  return 0;
}


