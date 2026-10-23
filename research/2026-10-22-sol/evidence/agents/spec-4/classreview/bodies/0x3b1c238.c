// ==== 0x3b1c238  FUN_03b1c238  (60 bytes)

undefined8 FUN_03b1c238(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = FUN_000805e0(param_1 + 8,&DAT_0c9bfecc);
  if (lVar1 == 0 || lVar1 == 0x40) {
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined8 *)(lVar1 + 0x20);
  }
  return uVar2;
}


