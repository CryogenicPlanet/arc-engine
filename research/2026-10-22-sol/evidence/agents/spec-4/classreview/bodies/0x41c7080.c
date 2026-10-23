// ==== 0x41c7080  FUN_041c7080  (76 bytes)

undefined8 * FUN_041c7080(void)

{
  int iVar1;
  
  if (((DAT_0e499b90 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e499b90), iVar1 != 0)) {
    DAT_0e499b88 = 0;
    thunk_FUN_0c44b760(&DAT_0e499b90);
  }
  return &DAT_0e499b88;
}


