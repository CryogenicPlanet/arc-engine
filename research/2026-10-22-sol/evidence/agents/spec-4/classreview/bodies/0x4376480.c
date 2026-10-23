// ==== 0x4376480  FUN_04376480  (76 bytes)

undefined4 FUN_04376480(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  
  if (*(long *)(param_1 + 0x130) != 0) {
    lVar2 = FUN_000805e0(*(long *)(param_1 + 0x130) + 8,&DAT_0cd0f780);
    if (lVar2 == 0 || lVar2 == 0x20) {
      uVar1 = 0xffffffff;
    }
    else {
      uVar1 = *(undefined4 *)(lVar2 + 0x50);
    }
    return uVar1;
  }
  return 0xffffffff;
}


