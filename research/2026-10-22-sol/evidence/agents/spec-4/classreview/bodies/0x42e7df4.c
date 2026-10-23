// ==== 0x42e7df4  FUN_042e7df4  (204 bytes)

undefined8 FUN_042e7df4(long param_1,undefined8 param_2,long param_3)

{
  undefined8 uVar1;
  long lVar2;
  undefined1 auStack_160 [16];
  undefined1 auStack_150 [288];
  
  uVar1 = 0;
  if (*(long *)(param_1 + 0x68) != 0) {
    if (param_3 == 0) {
      if (DAT_0e4a3078 < 3) {
        FUN_00f195d8(auStack_160,"../../arc/browser/browser_context/extensions_impl.cc",0x133,2);
        FUN_00040060(auStack_150,"Invalid parameter!",0x12);
        FUN_00f1950c(auStack_160);
      }
      uVar1 = 0;
    }
    else {
      uVar1 = FUN_014baf1c();
      lVar2 = FUN_022a58ec(uVar1,param_2,7);
      uVar1 = 0;
      if (lVar2 != 0) {
        FUN_0433513c(auStack_160,*(undefined8 *)(param_1 + 0x68),lVar2);
        FUN_042e7ec0(param_3,auStack_160);
        FUN_04334d2c(auStack_160);
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}


