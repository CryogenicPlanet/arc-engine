// ==== 0x2122528  FUN_02122528  (36 bytes)

undefined8 FUN_02122528(int param_1)

{
  if (0x6b < param_1 - 4U) {
    return 0;
  }
  return *(undefined8 *)(&DAT_0c778070 + (ulong)(param_1 - 4U) * 8);
}


