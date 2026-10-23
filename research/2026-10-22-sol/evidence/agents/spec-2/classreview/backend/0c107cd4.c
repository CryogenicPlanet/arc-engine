// ==== 0xc107cd4  FUN_0c107cd4  (84 bytes)

void FUN_0c107cd4(long param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x104) = 4;
  FUN_0678cfe8(*(undefined8 *)(param_1 + 0xa0),0);
  *(undefined4 *)(param_1 + 0x174) = param_2;
  if (*(long *)(param_1 + 0x198) != 0) {
    FUN_0c1055c0(param_1);
    *(undefined4 *)(param_1 + 0x1a0) = 0xffffffff;
  }
  return;
}


