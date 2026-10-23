// ==== 0x100536028  FUN_100536028  (52 bytes)

undefined8 FUN_100536028(undefined8 param_1,long param_2)

{
  if (param_2 - 2U < 9) {
    return *(undefined8 *)(&DAT_103640410 + (param_2 - 2U) * 8);
  }
  FUN_1003568ec();
  return 10;
}


