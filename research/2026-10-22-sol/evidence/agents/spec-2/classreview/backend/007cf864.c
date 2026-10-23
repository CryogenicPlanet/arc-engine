// ==== 0x7cf864  FUN_007cf864  (124 bytes)

void FUN_007cf864(long param_1)

{
  undefined8 uVar1;
  long lVar2;
  undefined1 auStack_38 [32];
  undefined1 auStack_18 [8];
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
    lVar2 = (long)*(char *)(param_1 + 0x17);
    if (lVar2 < 0) {
      lVar2 = *(long *)(param_1 + 8);
    }
    if (lVar2 != 0) {
      FUN_0837a430(auStack_38,"../../url/gurl.cc",0xa9);
      FUN_08376274(auStack_18,auStack_38);
      uVar1 = FUN_07454274(auStack_18);
      FUN_00040060(uVar1,"Trying to get the spec of an invalid URL!",0x29);
      FUN_083762f0(auStack_18);
      FUN_007be074();
    }
  }
  return;
}


